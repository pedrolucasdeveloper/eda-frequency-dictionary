EDA Frequency Dictionary - Instruções para Linux

Projeto final da disciplina de Estrutura de Dados Avançada.

Este programa lê um arquivo de texto no formato .txt, conta a frequência das palavras e gera um arquivo .csv com as palavras em ordem alfabética e suas respectivas frequências.

O programa permite executar a contagem usando quatro estruturas de dados diferentes:

1. Árvore AVL
2. Árvore Rubro-Negra
3. Tabela Hash com Encadeamento Exterior
4. Tabela Hash com Endereçamento Aberto

---

1. Requisitos

---

O programa foi desenvolvido em C++ e utiliza apenas bibliotecas padrão da linguagem.

Não é necessário instalar bibliotecas externas.

Em sistemas Linux, é necessário ter o compilador g++ instalado.

No LinuxMint/Ubuntu, o compilador pode ser instalado com:

sudo apt update
sudo apt install build-essential

Para verificar se o g++ está instalado:

g++ --version

---

2. Estrutura do projeto

---

A estrutura principal do projeto é:

eda-frequency-dictionary/
├── src/
│   ├── AVLTree.hpp
│   ├── Dictionary.hpp
│   ├── HashChaining.hpp
│   ├── HashOpenAddressing.hpp
│   ├── RedBlackTree.hpp
│   └── main.cpp
├── livros/
│   ├── dom-casmurro.txt
│   ├── sherlock_holmes.txt
│   └── a_riqueza_das_nacoes_eng...
├── results/
├── README.md
├── readme.txt
└── LICENSE

A pasta src/ contém o código-fonte.
A pasta livros/ contém arquivos de texto usados nos testes.
A pasta results/ é usada para armazenar os arquivos CSV gerados.

---

3. Como compilar

---

Na raiz do projeto, execute:

g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp -o freq

Esse comando gera o executável chamado freq.

---

4. Como executar

---

A sintaxe geral do programa é:

./freq dictionary estrutura arquivo_entrada.txt arquivo_saida.csv

As estruturas disponíveis são:

avl
rb
red-black
hash-chaining
hash-open

---

5. Exemplos de execução

---

Usando Árvore AVL:

./freq dictionary avl livros/dom-casmurro.txt results/dom_avl.csv

Usando Árvore Rubro-Negra:

./freq dictionary rb livros/dom-casmurro.txt results/dom_rb.csv

Usando Hash com Encadeamento Exterior:

./freq dictionary hash-chaining livros/dom-casmurro.txt results/dom_hash_chaining.csv

Usando Hash com Endereçamento Aberto:

./freq dictionary hash-open livros/dom-casmurro.txt results/dom_hash_open.csv

---

6. Ajuda

---

Para exibir a ajuda do programa:

./freq --help

ou:

./freq -h

---

7. Saída gerada

---

O programa gera um arquivo CSV com duas colunas:

palavra,frequencia

Exemplo:

palavra,frequencia
casa,3
de,1
diferente,1
mais,1
quatro,4

As palavras são ordenadas alfabeticamente antes da geração do arquivo CSV.

---

8. Métricas exibidas no terminal

---

Ao final da execução, o programa exibe informações como:

Estrutura utilizada
Arquivo de entrada
Arquivo de saída
Quantidade de palavras distintas
Tempo de montagem
Comparações de chave
Rotações
Colisões
Probes/sondagens
Rehashes
Fator de carga

Nem todas as métricas se aplicam a todas as estruturas. Por exemplo, rotações são usadas nas árvores AVL e Rubro-Negra, enquanto colisões, probes e rehashes são métricas das tabelas hash.

---

9. Tratamento das palavras

---

Durante a leitura do texto, o programa:

* ignora espaços em branco;
* ignora sinais de pontuação;
* converte letras maiúsculas ASCII para minúsculas;
* mantém caracteres acentuados;
* mantém hífen quando aparece dentro de uma palavra;
* remove hífen quando aparece no começo ou no fim de uma palavra.

Exemplo:

"Quatro, QUATRO!" -> "quatro", "quatro"
"Mostra-lo"       -> "mostra-lo"

Observação: caracteres acentuados são preservados. A conversão completa de letras acentuadas maiúsculas para minúsculas pode depender de tratamento Unicode mais específico.

---

10. Observações finais

---

O programa é executado por linha de comando, sem menu interativo.

Todas as estruturas implementam uma interface comum de dicionário definida em Dictionary.hpp.

O objetivo do projeto é comparar o desempenho das estruturas na tarefa de contagem de frequência de palavras.