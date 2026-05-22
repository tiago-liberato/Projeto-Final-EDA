#ifndef AVL_TREE
#define AVL_TREE

#include <stack>

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
        this->height = h;
        this->left = left;
        this->right = right;
    }
    
}

Node* root;

void add(Node* node, K key, V value){

    stack<Node*> pais; // Guarda o caminho percorrido na árvore
    Node* aux = this->root;

    while(aux != nullptr){
        if(key == aux->key){
            throw runtime_error("Chave Repetida")
        }
        if(value < aux->value){
            pais.push(aux);
            aux = aux->left;
        }else if(value > aux->value){
            pais.push(aux);
            aux = aux->right;
        }else{
            throw runtime_error("")
        }
    }

    aux = new Node(key, value, 1, nullptr, nullptr )
   

}


Node* leftRotation(Node* node){
    Node* y = node->right;
    node->right = y->left;
    y->left = node;

    //Atualiza a altura dos nós rebalanceados
    node -> height = 1 + max ( height (node -> left ) , height (node -> right ) );
    y -> height = 1 + max ( height (y -> left ) , height (y -> right ) );

    return y; //Atualiza a raiz dessa subárvore
}


Node* rightRotation(Node* node){
    Node* y = node->left;
    node->left = y->right;
    y->right = node;

    //Atualiza a altura dos nós rebalanceados
    node -> height = 1 + max ( height (node -> left ) , height (node -> right ) );
    y -> height = 1 + max ( height (y -> left ) , height (y -> right ) );

    return y; //Atualiza a raiz dessa subárvore
}


Node* fixUpInsertion(Node* node, V value){ // MUDAR O NOME value POR VALUE
    int balance = balance(node);
    
    //Caso 1(a): Configuração esq-esq - rotação a direita
    if(balance < -1 && value < node->left->value)
        return rightRotation(node);

    //Caso 1(b): Configuração esq dir - rotação dupla a direita
    else if(balance < -1 && value > node->left->value){
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }

    //Caso 2(b): Configuração dir-dir - rotação a esquerda
    if(balance > 1 && value > node->right->value)
        return leftRotation(node);

    else if(balance > 1 && value < node->right->value){
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }

    node -> height = 1 + max ( height (node -> left ) , height (node -> right ) );



}

int balance(Node* node){
    if(node == nullptr) return 0;
    return (height(node->right) - height(node->left));
}

int height(Node* node){
    return node->height;
}



public:

void insert(K key, V value){
    
}












};

#endif