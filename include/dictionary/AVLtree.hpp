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

class AVLIterator: public iterator{
private:
    stack<Node*> pilha;

public:

    AVLIterator(){
        pushLeft(this->root);
    }

    //Método responsável por empilhar os nós da esquerda
    void pushLeft(Node* node){
        while(node != nullptr){
            pilha.push(node);
            node = node->left;
        }
    }

    //Método responsárvel por retornar se ainda existem elementos a serem percorridos
    bool hasNext() override{
        return !pilha.empty();
    }

    // Retorna um par com a chave e o valor do próximo elemento
    pair<K, V> next(){
        Node* atual = pilha.top();
        pushLeft(atual->right);
        pilha.pop();
        return {atual->key, atual->value};
    }
}



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


Node* fixUpNode(Node* node){ 
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

    //Caso 2(b): 
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
        if(key < aux->key){
            pais.push(aux);
            aux = aux->left;
        }else if(key > aux->key){
            pais.push(aux);
            aux = aux->right;
        }else{
            throw runtime_error("Chave Repetida")
        }
    }

    //Cria o novo nó
    aux = new Node(key, value, 1, nullptr, nullptr );

    //Liga o novo nó ao pai
    if(!pais.empty()){
        Node* pai = pais.top();
        if(aux->key > pai->key){
            pai->right = aux;
        }else{
            pai->left = aux;
        }
    }

    //Rebalanceia a árvore
    while(!pais.empty()){
        Node* atual = pais.top();
        atual = fixUpNode(atual);
        pais.pop();

        if(!pais.empty()){
            Node* pai = pais.top();
            
            if(atual->key > pai->key){ //Liga a subárvore rebalanceada ao pai
                pai->right = atual;
            }else{
                pai->left = atual;
            }
        }else{
            this->root = atual;
        }


    }
}

Node* search(const K& key) const {
    Node* aux = this->root;
    
    while (aux != nullptr) {
        if (key == aux->key) return aux;
        else if (key > aux->key) aux = aux->right;
        else aux = aux->left;
    }
   return nullptr;
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

V get (const K& key) const override { 
    Node* result = search(key);
    if (result != nullptr) return result->value;
    throw runtime_error("Chave não encontrada na arvore");
}

size_t size() const override {}

bool contains (const K& key) const override {
    return search(key) != nullptr;
}

//Retorna uma instância da classe AVLIterator
Iterator<K, V> getIterator() override {
    return new AVLIterator();
}

};


#endif