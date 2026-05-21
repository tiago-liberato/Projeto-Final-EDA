#ifndef AVL_TREE
#define AVL_TREE

using namespace std;

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
    int height;
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


Node* leftRotation(Node* node){
    Node* y = node->right;
    node->right = y->left;
    y->left = node;

    //Atualiza a altura dos nós rebalanceados
    node -> height = 1 + max ( height (u -> left ) , height (u -> right ) );
    y -> height = 1 + max ( height (p -> left ) , height (p -> right ) );

    return y; //Atualiza a raiz dessa subárvore
}


Node* rightRotation(Node* node){
    Node* y = node->left;
    node->left = y->right;
    y->right = node;

    //Atualiza a altura dos nós rebalanceados
    node -> height = 1 + max ( height (u -> left ) , height (u -> right ) );
    y -> height = 1 + max ( height (p -> left ) , height (p -> right ) );

    return y; //Atualiza a raiz dessa subárvore
}

void fixUpInsertion(Node* node){


}

int balance(Node* node){
    return (node->right->height - node->left->height);
}



public:












};

#endif