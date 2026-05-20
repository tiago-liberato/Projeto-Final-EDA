#ifndef DICTIONARY
#define DICTIONARY

/**
 * @brief 
 * 
 * @tparam K parametro para a chave 
 * @tparam v parametro para o valor
 */

template<typename K, typename V>
class Dictionary{

    virtual ~Dictionary() = default; 
  
    virtual void insert(K& key, V& value) = 0;
    virtual void update(K& key, V& value) = 0;
    virtual V get(const K& key) const = 0;
    virtual void remove(K& key) = 0;
    virtual bool contains(K& key) const = 0;
    virtual std::size_t size() const = 0;
    virtual void clear() = 0;
};

#endif