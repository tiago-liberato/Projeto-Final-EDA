#ifndef AVL_TREE
#define AVL_TREE

/**
 * @brief Classe que implementa uma árvore AVL iterativa
 * 
 * @tparam K parametro para a chave
 * @tparam V parametro para o valor
 */

template<typename K, typename V>
class AVLtree : public Dictionary<K, V>{

private:

 struct Node{
    K key;
    V value;
    int h;
    Node* left;
    Node* right;

    Node(K key, V value, int h, Node* left, Node* right){
        this->key = key;
        this->value = value;
        this->h = h;
        this->left = left;
        this->right = right;
    }
    
}

Node* root;




public:












};

#endif