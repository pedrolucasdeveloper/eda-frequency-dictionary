#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <vector>
#include <utility>
#include <string>
#include <cstddef>

/*
 * Dicionário: Estrutura para armazenar métricas das estruturas.
 */
struct DictionaryMetrics {
    long long keyComparisons = 0;
    long long collisions = 0;
    long long probes = 0;
    long long rotations = 0;
    long long rehashes = 0;
    double loadFactor = 0.0;
};

/*
 * Interface genérica de dicionário.
 *
 * Key   = tipo da chave
 * Value = tipo do valor
 */
template <typename Key, typename Value>
class Dictionary {
public:
    /*
     * Destrutor virtual.
     */
    virtual ~Dictionary() = default;

    /*
     * Insere um novo par chave-valor.
     */
    virtual bool insert(const Key& key, const Value& value) = 0;

    /*
     * Atualiza o valor de uma chave existente.
     */
    virtual bool update(const Key& key, const Value& value) = 0;

    /*
     * Acessa o valor associado a uma chave.
     */
    virtual Value* get(const Key& key) = 0;

    /*
     * Versão constante do get.
     */
    virtual const Value* get(const Key& key) const = 0;

    /*
     * Verifica se uma chave existe no dicionário.
     */
    virtual bool contains(const Key& key) = 0;

    /*
     * Remove uma chave do dicionário.
     */
    virtual bool remove(const Key& key) = 0;

    /*
     * Remove todos os elementos do dicionário.
     */
    virtual void clear() = 0;

    /*
     * Retorna a quantidade de pares chave-valor armazenados.
     */
    virtual std::size_t size() const = 0;

    /*
     * Verifica se o dicionário está vazio.
     */
    virtual bool empty() const {
        return size() == 0;
    }

    /*
     * Retorna todos os pares chave-valor do dicionário.
     */
    virtual std::vector<std::pair<Key, Value>> items() const = 0;

    /*
     * Retorna o nome da estrutura.
     */
    virtual std::string name() const = 0;

    /*
     * Retorna as métricas coletadas pela estrutura.
     */
    virtual DictionaryMetrics metrics() const = 0;

    /*
     * Zera as métricas.
     */
    virtual void resetMetrics() = 0;
};

#endif