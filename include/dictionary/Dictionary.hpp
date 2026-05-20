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
    virtual V& acess(K& key) = 0;
    virtual void remove(K& key) = 0;
    virtual bool contains(K& key) = 0;
    virtual void size() = 0;
    virtual clear() = 0;
};

#endif