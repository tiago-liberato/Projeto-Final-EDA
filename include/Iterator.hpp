#ifndef ITERATOR
#define ITERATOR

/**
 * @brief Classe Iterator responsável por pecorrer as estruturas de dados do Dicionário
 * 
 * @tparam K parametro para chave
 * @tparam V parametro para valor
 */

template<typename K, typename V>
class Iterator{
public:
    virtual bool hasNext() = 0;
    virtual std::pair<K, V> next() = 0;

};


#endif 
