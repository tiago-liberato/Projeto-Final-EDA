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
};

Node* root;



// botei esses métodos aqui pq eles são auxiliares em outras funções, então eles precisam vir antes


int height(Node* node){
    return (node == nullptr) ? 0 : node->height;
}

int balance(Node* node){
    if(node == nullptr) return 0;
    return (height(node->right) - height(node->left));
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


Node* fixUpNode(Node* node){ // MUDAR O NOME value POR VALUE
    int b = balance(node);
    
    //Caso 1(a): Configuração esq-esq - rotação a direita
    if(b == -2 && balance(node->left) < 0)
        return rightRotation(node);

    //Caso 1(b): Configuração esq dir - rotação dupla a direita
    if(b == -2 && balance(node->left) > 0) {
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }

    //Caso 2(a): Configuração dir-dir - rotação a esquerda
    if(b == 2 && balance(node->right) > 0)
        return leftRotation(node);

    //Casi 2(b): 
    if(b == 2 && balance(node->right) < 0){
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }

    node -> height = 1 + max ( height (node -> left ) , height (node -> right ) );
    return node;
}

void add(const K& key, const V& value) {
    if(this->root == nullptr) {
        this->root = new Node(key, value, 1, nullptr, nullptr);
        return;
    }

    stack<Node*> pais; // Guarda o caminho percorrido na árvore
    Node* aux = this->root;



    while(aux != nullptr) {
        if(key == aux->key){
            throw runtime_error("Chave Repetida")
        }
        if(key < aux->key){
            pais.push(aux);
            aux = aux->left;
        }else if(key > aux->key){
            pais.push(aux);
            aux = aux->right;
        }else{
            throw runtime_error("")
        }
    }
    aux = new Node(key, value, 1, nullptr, nullptr )
}


public:

AVLtree(){
    this->root = nullptr;
}

void insert(const K& key, const V& value) override {
    add( key, value);
}


void update(const K& key, const V& value) override {
    //TODO
}

void remove(const K& key) override {
    //TODO
}

void clear() override {}

V get (const K& key) const override {}

size_t sise() const override {}

bool contains (const K& key) const override {}

Iterator<K, V> getIterator() override {}

};


#endif