#ifndef ITERATOR
#define ITERATOR

/**
 * @brief 
 * 
 * @tparam K 
 * @tparam V 
 */

template<typename K, typename V>
class Iterator{

    virtual bool hasNext() = 0;
    virtual <K&, V&> next() = 0;

};


#endif 
