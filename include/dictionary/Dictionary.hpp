#ifndef DICTIONARY
#define DICTIONARY
#include "../Iterator.hpp"
/**
 * @brief  Interface para a estrutura de dados do dicionário
 * 
 * @tparam K parametro para a chave 
 * @tparam v parametro para o valor
 */

template<typename K, typename V>
class Dictionary{
public:
    // Destrutor Virtual
    virtual ~Dictionary() = default; 

  
    // Métodos que alteram a estrutura
    virtual void insert(const K& key, const V& value) = 0;
    virtual void update(const K& key, const V& value) = 0;
    virtual void remove(const K& key) = 0;
    virtual void clear() = 0;


    // Métodos de leitura, que não alteram a estrutura 
    virtual Iterator<K, V>* getIterator() const = 0;
    virtual V get(const K& key) const = 0;
    virtual bool contains(const K& key) const = 0;
    virtual std::size_t size() const = 0;
};

#endif