#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <memory>
#include <cctype>

#include "Dictionary.hpp"
#include "HashChaining.hpp"
#include "HashOpenAddressing.hpp"

using namespace std;

void printHelp() {
    cout << "Uso:\n";
    cout << "  ./freq dictionary hash-chaining entrada.txt saida.csv\n";
    cout << "  ./freq dictionary hash-open entrada.txt saida.csv\n\n";

    cout << "Estruturas disponiveis:\n";
    cout << "  hash-chaining  -> Tabela hash com encadeamento exterior\n";
    cout << "  hash-open      -> Tabela hash com enderecamento aberto\n\n";

    cout << "Exemplo:\n";
    cout << "  ./freq dictionary hash-chaining livro.txt resultado.csv\n";
}

bool isWordChar(unsigned char c) {
    /*
     * Letras e numeros ASCII entram como parte da palavra.
     * Caracteres >= 128 sao mantidos para preservar acentos em UTF-8.
     */
    return isalnum(c) || c >= 128;
}

void cleanTrailingHyphen(string& word) {
    while (!word.empty() && word[word.size() - 1] == '-') {
        word.erase(word.size() - 1);
    }
}

vector<string> extractWordsFromLine(const string& line) {
    vector<string> words;
    string current;

    for (size_t i = 0; i < line.size(); i++) {
        unsigned char c = static_cast<unsigned char>(line[i]);

        if (isWordChar(c)) {
            if (c < 128) {
                current.push_back(static_cast<char>(tolower(c)));
            } else {
                current.push_back(static_cast<char>(c));
            }
        } else if (c == '-' && !current.empty()) {
            /*
             * Mantem hifen quando ele aparece dentro de uma palavra.
             * Exemplo: mostra-lo
             */
            current.push_back('-');
        } else {
            cleanTrailingHyphen(current);

            if (!current.empty()) {
                words.push_back(current);
                current.clear();
            }
        }
    }

    cleanTrailingHyphen(current);

    if (!current.empty()) {
        words.push_back(current);
    }

    return words;
}

bool processTextFile(const string& inputFile, Dictionary<string, int>& dictionary) {
    ifstream file(inputFile.c_str());

    if (!file.is_open()) {
        cerr << "Erro: nao foi possivel abrir o arquivo de entrada: " << inputFile << endl;
        return false;
    }

    string line;

    while (getline(file, line)) {
        vector<string> words = extractWordsFromLine(line);

        for (size_t i = 0; i < words.size(); i++) {
            const string& word = words[i];

            int* frequency = dictionary.get(word);

            if (frequency != NULL) {
                (*frequency)++;
            } else {
                dictionary.insert(word, 1);
            }
        }
    }

    file.close();
    return true;
}

bool comparePairsAlphabetically(const pair<string, int>& a, const pair<string, int>& b) {
    return a.first < b.first;
}

bool writeCsvFile(const string& outputFile, vector<pair<string, int> >& items) {
    ofstream file(outputFile.c_str());

    if (!file.is_open()) {
        cerr << "Erro: nao foi possivel criar o arquivo de saida: " << outputFile << endl;
        return false;
    }

    file << "palavra,frequencia\n";

    for (size_t i = 0; i < items.size(); i++) {
        file << items[i].first << "," << items[i].second << "\n";
    }

    file.close();
    return true;
}

Dictionary<string, int>* createDictionary(const string& structureName) {
    if (structureName == "hash-chaining") {
        return new HashChaining<string, int>();
    }

    if (structureName == "hash-open") {
        return new HashOpenAddressing<string, int>();
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        string option = argv[1];

        if (option == "help" || option == "--help" || option == "-h") {
            printHelp();
            return 0;
        }
    }

    if (argc != 5) {
        cerr << "Erro: quantidade invalida de argumentos.\n\n";
        printHelp();
        return 1;
    }

    string command = argv[1];
    string structureName = argv[2];
    string inputFile = argv[3];
    string outputFile = argv[4];

    if (command != "dictionary") {
        cerr << "Erro: comando invalido. Use 'dictionary'.\n\n";
        printHelp();
        return 1;
    }

    Dictionary<string, int>* dictionary = createDictionary(structureName);

    if (dictionary == NULL) {
        cerr << "Erro: estrutura invalida: " << structureName << "\n\n";
        printHelp();
        return 1;
    }

    dictionary->resetMetrics();

    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

    bool success = processTextFile(inputFile, *dictionary);

    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();

    if (!success) {
        delete dictionary;
        return 1;
    }

    vector<pair<string, int> > items = dictionary->items();

    sort(items.begin(), items.end(), comparePairsAlphabetically);

    if (!writeCsvFile(outputFile, items)) {
        delete dictionary;
        return 1;
    }

    chrono::duration<double, milli> elapsed = end - start;
    DictionaryMetrics metrics = dictionary->metrics();

    cout << "Estrutura utilizada: " << dictionary->name() << endl;
    cout << "Arquivo de entrada: " << inputFile << endl;
    cout << "Arquivo de saida: " << outputFile << endl;
    cout << "Palavras distintas: " << dictionary->size() << endl;
    cout << "Tempo de montagem: " << elapsed.count() << " ms" << endl;
    cout << "Comparacoes de chave: " << metrics.keyComparisons << endl;
    cout << "Colisoes: " << metrics.collisions << endl;
    cout << "Probes: " << metrics.probes << endl;
    cout << "Rehashes: " << metrics.rehashes << endl;
    cout << "Fator de carga: " << metrics.loadFactor << endl;

    delete dictionary;

    return 0;
}