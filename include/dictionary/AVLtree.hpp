#ifndef AVL_TREE
#define AVL_TREE

#include <stack>
#include "../Iterator.hpp"
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

    /**
     * @brief struct que representa um nó da árvore AVL
     * 
     */
    struct Node{
        K key;
        V value;
        int height;
        Node* left;
        Node* right;

    Node(const K& key, const V& value, int h, Node* left, Node* right)
        : key(key), value(value), height(h), left(left), right(right) {}
    };

    Node* root;
    int _size = 0;
    mutable size_t count = 0;

    /**
     * @brief  Classe interna responsável por fornecer a implementação do iterador para a árvore AVL
     * 
     */
    class AVLIterator : public Iterator<K, V> {
        private:
            stack<Node*> stack_nodes;
            
            // Método responsável por empilhar os nós da esquerda
            void pushLeft(Node* node){
                while(node != nullptr){
                    stack_nodes.push(node);
                    node = node->left;
                }
            }

        public:
            AVLIterator(Node* root){
                pushLeft(root);
            }

            bool hasNext() override {
                return !stack_nodes.empty();
            }

            // Retorna um par com a chave e o valor do próximo elemento
                pair<K, V> next() override {
                Node* atual = stack_nodes.top();
                stack_nodes.pop(); // Remove do topo antes de empilhar os próximos
                pushLeft(atual->right);
                return {atual->key, atual->value};
            }
        }; // fim do Iterator



    /**
     * @brief Retorna a altura de um nó na árvore
     * 
     * @param node parametro para o nó
     * @return int altura do nó
     */
    int height(Node* node){
        return (node == nullptr) ? 0 : node->height;
    }


    /**
     * @brief Calcula o balanço de um nó
     * 
     * @param node parametro para o nó
     * @return int balanço do nó
     */
    int balance(Node* node){
        if(node == nullptr) return 0;
        return (height(node->right) - height(node->left));
    }


    /**
     * @brief Realiza uma rotação a esquerda em um nó
     * 
     * @param node parametro para o nó
     * @return Node* ponteiro para o nó filho após a rotação
     */
    Node* leftRotation(Node* node){
        Node* y = node->right;
        node->right = y->left;
        y->left = node;

        //Atualiza a altura dos nós rebalanceados
        node -> height = 1 + max ( height (node -> left ) , height (node -> right ) );
        y -> height = 1 + max ( height (y -> left ) , height (y -> right ) );

        return y; //Atualiza a raiz dessa subárvore
    }


    /**
     * @brief Realiza uma rotação a direita em um nó
     * 
     * @param node parametro para o nó
     * @return Node* ponteiro para o nó filho após a rotação
     */
    Node* rightRotation(Node* node){
        Node* y = node->left;
        node->left = y->right;
        y->right = node;

        //Atualiza a altura dos nós rebalanceados
        node -> height = 1 + max ( height (node -> left ) , height (node -> right ) );
        y -> height = 1 + max ( height (y -> left ) , height (y -> right ) );

        return y; //Atualiza a raiz dessa subárvore
    }


    /**
     * @brief Realiza o rebalanceamnto da árvore após uma inserção
     * 
     * @param node parametro do nó inserido
     * @return Node* ponteiro para o nó após o rebalanceamento
     */
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


    /**
     * @brief Método privado que adiciona um novo nó na árvore
     * 
     * @param key parametro para a chave do novo nó
     * @param value parametro para o valor do novo nó
     */
    void add(const K& key, const V& value) {
        if(this->root == nullptr) {
            this->root = new Node(key, value, 1, nullptr, nullptr);
            this->_size++;
            return;
        }

        stack<Node*> pais; // Guarda o caminho percorrido na árvore
        Node* aux = this->root;

        while(aux != nullptr) {
            count ++;

            if(key < aux->key){
                pais.push(aux);
                aux = aux->left;
            }else if(key > aux->key){
                pais.push(aux);
                aux = aux->right;
            }else{
                throw runtime_error("Repeated key.");
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
        _size++;
    }


    /**
     * @brief Busca um nó na árvore com base na sua chave, 
     * se não for retorna nullptr
     * 
     * @param key parametro para a chave
     * @return Node* ponteiro para o nó encontrado
     */
    Node* search(const K& key) const {
        Node* aux = this->root;
        
        while (aux != nullptr) {
            count++;
            if (key == aux->key) return aux;
            else if (key > aux->key) aux = aux->right;
            else aux = aux->left;
        }

        return nullptr;

    }


    /**
     * @brief Método privado que limpa a árvore
     * 
     */
    void _clear(){
        if(this->root == nullptr) return;

        stack<Node*> stack_aux;
        stack_aux.push(this->root);

        while(!stack_aux.empty()){
            Node* aux = stack_aux.top();
            stack_aux.pop();

            if(aux->left != nullptr) stack_aux.push(aux->left);
            if(aux->right != nullptr) stack_aux.push(aux->right);

            delete aux;
        }

        this->root = nullptr;
        this->_size = 0;
    }


    /**
     * @brief Método privado que realiza a atualização de uma valor com base na sua chave
     * 
     * @param key parametro para a chave
     * @param value parametro para o novo valor
     */
    void _update(const K& key, const V& value) {
        Node* modificado = search(key);  
        if (modificado != nullptr) {
            modificado->value = value; // atualiza o valor se a chave existir
        } else {
                // lanca um erro se tentar atualizar uma chave inexistente
            throw runtime_error("Key not found in tree");
        }
    }


    /**
     * @brief Método privado que remove um nó da árvore, segue os seguintes passos:
     *          (1) Se o nó não existir lança uma exceção
     *          (2) Se existir, remove o nó e rebalanceia a árvore
     * 
     * @param key parametro para a chave do nó
     */
    void _remove(const K& key) {
        if (this->root == nullptr) return; // Árvore vazia, nada a fazer

        stack<Node*> pais;
        Node* aux = this->root;
        Node* pai = nullptr;

            //  Busca o nó a ser removido
        while (aux != nullptr && aux->key != key) {
            pai = aux;
            pais.push(pai); // Guarda o caminho
                
            if (key < aux->key) aux = aux->left;
                else aux = aux->right;
            }

            // Se o laço terminou e aux é nulo, a chave não existe
            if (aux == nullptr) {
                throw runtime_error("Key not found in tree");
            }

            Node* noRemover = aux;

            
            // O nó tem 2 filhos
            if (noRemover->left != nullptr && noRemover->right != nullptr) {
                Node* sucessorPai = noRemover;
                pais.push(sucessorPai); // O nó a ser removido continua na árvore
                
                // Vamos achar o Sucessor (o menor nó da subárvore direita)
                Node* sucessor = noRemover->right;
                while (sucessor->left != nullptr) {
                    sucessorPai = sucessor;
                    pais.push(sucessorPai);
                    sucessor = sucessor->left;
                }

                noRemover->key = sucessor->key;
                noRemover->value = sucessor->value;

                aux = sucessor;
                pai = sucessorPai;
            }

            // O nó que vai ser removido agora tem 0 ou 1 filho.
            Node* filho = (aux->left != nullptr) ? aux->left : aux->right;

            if (pai == nullptr) {
                this->root = filho;
            } 
            else if (pai->left == aux) {
                pai->left = filho;
            } 
            else {
                pai->right = filho;
            }
            delete aux;
            this->_size--;

            while (!pais.empty()) {
                Node* atual = pais.top();
                pais.pop();

                atual->height = 1 + max(height(atual->left), height(atual->right));

                Node* novaRaiz = fixUpNode(atual);

                if (novaRaiz != atual) {
                    if (pais.empty()) {
                        this->root = novaRaiz;
                    } else {
                        Node* avo = pais.top();
                    if (novaRaiz->key < avo->key) {
                            avo->left = novaRaiz;
                    }else {
                            avo->right = novaRaiz;
                    }
                }
            }
        }
    }

public:


    /**
     * @brief Construtor da classe AVLtree
     * 
     */
    AVLtree(){
        this->root = nullptr;
    }


    /**
     * @brief Destrutor da classe AVLtree
     * 
     */
    ~AVLtree() {
        _clear();
    }


    /**
     * @brief Método público que insere um novo nó na árvore
     * 
     * @param key parametro para a chave do novo nó
     * @param value parametro para o valor do novo nó
     */
    void insert(const K& key, const V& value) override {
        add( key, value);
    }


    /**
     * @brief Método público que atualiza o valor de um nó com base na sua chave
     * 
     * @param key parametro para chave
     * @param value paraetro para valor
     */
    void update(const K& key, const V& value) override {
        _update(key, value);
    }


    /**
     * @brief Método público que remove um nó da árvore
     * 
     * @param key chave do nó a ser removido
     */
    void remove(const K& key) override {
        _remove(key);
    }


    /**
     * @brief Método privado que limpa a árvore
     * 
     */
    void clear() override {
        _clear();
    }

    
    /**
     * @brief retorna o valor relacionado a uma determinada chave na árvore,
     * caso não pertença a árvore lança uma exceção.
     * 
     * @param key parametro para chave
     * @return V valor relacionado a chave
     */
    V get (const K& key) const override { 
        Node* result = search(key);
        if (result != nullptr) return result->value;
        throw out_of_range("Key not found in tree");
    }

    /**
     * @brief retorna a quantidade de nós na árvore
     * 
     * @return size_t 
     */
    size_t size() const override {
        return _size;
    }

    
    /**
     * @brief Verifica se uma chave existe na árvore
     * 
     * @param key parametro para a chave
     * @return true Se a chave pertence a árvore
     * @return false Se a chave não pertence a árvore
     */
    bool contains (const K& key) const override {
        return search(key) != nullptr;
    }

    
    /**
     * @brief retorna o iterador da árvore AVL
     * 
     * @return Iterator<K, V>* 
     */
    Iterator<K, V>* getIterator() const override {
        return new AVLIterator(this->root);
    }


    /**
     * @brief Retorna o número de comparações ao adicionar, procurar ou atualizar um nó
     * 
     * @return size_t variável count
     */
    size_t getComparison_Counter() const override{
        return count;
    }


};


#endif