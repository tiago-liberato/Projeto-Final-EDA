#ifndef RED_BLACK_TREE
#define RED_BLACK_TREE
#include <stack>
using namespace std;

enum Color{RED, BLACK};

/**
 * @brief 
 * 
 * @tparam K 
 * @tparam V 
 */

template<typename K, typename V>
class RedBlackTree : public Dictionary<K, V>{

private:

    /**
     * @brief Struct que representa o nó da árvore Rubro-Negra
     * 
     */
    struct Node{
        K key;
        V value;
        Color color;
        Node* parent;
        Node* left;
        Node* right;

        Node() : color(BLACK), left(nullptr), right(nullptr) {}          // Construtor nó NIL
        Node(const K& key, const V& value, Node* nil) : key(key), value(value), color(RED), parent(nil), left(nil), right(nil) {}  
        ~Node() = default;
        
    };

    Node* root;
    Node* NIL;
    int _size;
    mutable size_t count = 0;


    /**
     * @brief Classe que fornece a implementação do Iterador da RBtree
     * 
     */
    class RBIterator : public Iterator<K, V>{
        private:
            stack<Node*> stack_nodes;
            Node* NIL;
            
            // Método responsável por empilhar os nós da esquerda
            void pushLeft(Node* node){
                while(node != NIL){
                    stack_nodes.push(node);
                    node = node->left;
                }
            }

        public:

            RBIterator(Node* raiz, Node* nil){
                this-> NIL = nil;
                pushLeft(raiz);
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
        
    };



    /**
     * @brief Realizar uma rotação a esquerda no nó
     * 
     * @param node parametro para o nó
     */
    void leftRotation(Node* node){

        Node* y = node->right;
        node->right = y->left;

        if(y->left != NIL){
            y->left->parent = node;
        }
        
        y->parent = node->parent;

        if(node->parent == NIL){
            root = y;
        }else if(node == node->parent->left){
            node->parent->left = y;
        }else{
                node->parent->right = y;
        }

        y->left = node;
        node->parent = y;

    }



    /**
     * @brief Realizar uma rotação a direita no nó
     * 
     * @param node parametro para o nó
     */
    void rightRotation(Node* node){

        Node* y = node->left;
        node->left = y->right;

        if(y->right != NIL){
            y->right->parent = node;
        }
        
        y->parent = node->parent;
        
        if(node->parent == NIL){
            root = y;
        }else if(node == node->parent->right){
            node->parent->right = y;
        }else{
                node->parent->left = y;
        }

        y->right = node;
        node->parent = y;
    }


    /**
     * @brief Método privado que adiciona um nó na árvore
     * 
     * @param key parametro para chave
     * @param value parametro para valor
     */
    void add(const K& key, const V& value){
        Node* novo = new Node(key, value, this->NIL);
        Node* x = root; 
        Node* y = NIL; 

        while(x != NIL){ //Enquanto x não chegar a NIL, continua percorrendo a árvore
            y = x;
            count ++;

            if(novo->key < x->key){
                x = x->left;
            }else if(novo->key > x->key){
                x = x->right;
            }else{
                throw runtime_error("Repeated key.");
            }
        }


        novo->parent = y;
        
        if(y == NIL){
            novo->color = BLACK;
            root = novo;
        }else if(novo->key < y->key){
            y->left = novo;
        }else if (novo->key > y->key){y->right = novo;} 
        
        RBInsertFixUP(novo);
        _size ++;
    }


    /**
     * @brief Realiza as operações de rebelanceamento e recolorações na árvore após inserção
     * 
     * @param x parametro para o nó inserido
     */
    void RBInsertFixUP(Node* x){

            while(x->parent->color == RED){

            if(x->parent == NIL || x->parent->parent == NIL) break; // Se x for a raiz ou filho da raiz o fixInsertNode não é necessário

            if(x->parent == x->parent->parent->left){
                Node* y = x->parent->parent->right;

                if(y->color == RED){  //Caso 1
                    x->parent->color = BLACK;
                    y -> color = BLACK;
                    x->parent->parent->color = RED;
                    x = x->parent->parent;
                }else{
                    if(x == x->parent->right){ 
                        x = x->parent; //Caso 2a(esq-dir)
                        leftRotation(x); //Caso 2a(esq-dir)
                    }    
                    x->parent->color = BLACK; //Caso 3a(esq-esq)
                    x->parent->parent->color = RED; //Caso 3a(esq-esq)
                    rightRotation(x->parent->parent); //Caso 2a(esq-esq)
                    
                }
            }else{
                Node* y = x->parent->parent->left;
                if(y->color == RED){  //Caso 1
                    x->parent->color = BLACK;
                    y -> color = BLACK;
                    x->parent->parent->color = RED;
                    x = x->parent->parent;
                }else{
                    if(x == x->parent->left){ 
                        x = x->parent; //Caso 2b(dir-esq)
                        rightRotation(x); //Caso 2b(dir-esq)
                    }    
                    x->parent->color = BLACK; //Caso 2b(dir-dir)
                    x->parent->parent->color = RED; //Caso 2b(dir-dir)
                    leftRotation(x->parent->parent); //Caso 2b(dir-dir)
                    
                }
            }
        }

        root->color = BLACK;

    }


    /**
     * @brief Busca um nó na árvore 
     * 
     * @param key parametro para chave
     * @return Node* ponteiro para o nó
     */
    Node* search(const K& key) const{
        Node* aux = this->root;

        while(aux != NIL && aux->key != key){
            count ++;
            if(key < aux->key){
                aux = aux->left;
            }else if(key > aux->key){
                aux = aux->right;
            }
        }

        return aux;
    }


    /**
     * @brief Realiza a atualização do valor de um nó com base na sua chave
     * 
     * @param key parametro para chave
     * @param value parametro para o novo valor
     */
    void _update(const K& key, const V& value){
        Node* aux = search(key);

        if(aux == NIL) throw runtime_error("Key not found in tree");

        aux->value = value;
    }


    /**
     * @brief Remove um nó da árvore com base na sua chave
     * 
     * @param key parametro para chave
     */
    void RemoveNode(const K& key){
            Node* node = search(key);

            if(node == NIL) throw runtime_error("Key not found in tree");

            // Aplicar as regras de remoção do nó
            Node* y;
            Node* x;
            if(node->right == NIL || node->left == NIL){
                y = node;
            }else{//percorrer a subarvore direita de x até encontrar o sucessor
                y = findMinimum(node->right);
            }if(y->right != NIL){
                x = y->right;
            }else{
                x = y->left; // necessário para o caso de só filho esquerdo
            }
            x->parent = y->parent;

            if(y->parent == NIL){
                root = x;
            }else{
                if(y->parent->left == y ){
                    y->parent->left = x;
                }else{
                    y->parent->right = x;
                }
            }
            if(y != node){ //Se o novo nó a ser removido for diferente do inicial, cópio a chave do que será removido no inicial
                node->key = y->key;
            }if(y->color == BLACK){
                fixRemoveNode(x);
            }

            delete y;
            _size --;
            
            
                
        }


    /**
     * @brief Realiza as operações de rebalanceamento e recoloração da árvore após remoção
     * 
     * @param x parametro para o descendente do nó removido
     */
    void fixRemoveNode(Node* x){

        while(x != root && x->color == BLACK){

            if(x->parent->left == x){ //x é filho esquerdo

                Node* b = x->parent->right;

                if(b->color == RED){ //Caso 1
                    b->color = BLACK;
                    x->parent->color = RED;
                    leftRotation(x->parent);
                    b = x->parent->right;
                }else if(b->color == BLACK && b->left->color == BLACK && b->right->color == BLACK){
                    b->color = RED;
                    x = x->parent;
                }else{
                    if(b->right->color == BLACK){
                        b->color = RED;
                        b->left->color = BLACK;
                        rightRotation(b);
                        b = x->parent->right;
                    }
                    b->color = b->parent->color;
                    b->parent->color = BLACK;
                    b->right->color = BLACK;
                    leftRotation(b->parent);
                    x = root;

                }
            }else{
                Node* b = x->parent->left;
                
                if(b->color == RED){
                    b->color = BLACK;
                    x->parent->color = RED;
                    rightRotation(x->parent);
                    b = x->parent->left;
                }else if(b->color == BLACK && b->right->color == BLACK && b->left->color == BLACK){
                    b->color = RED;
                    x = x->parent;
                } else {
                    if(b->left->color == BLACK){
                        b->right->color = BLACK;
                        b->color = RED;
                        leftRotation(b);
                        b = x->parent->left;
                    }
                    b->color = x->parent->color;
                    x->parent->color = BLACK;
                    b->left->color = BLACK;
                    rightRotation(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }


    /**
     * @brief Busca o nó com valor mínimo de uma árvore
     * 
     * @param node parametro para a raiz da árvore
     * @return Node* ponteiro para o nó mínimo
     */
    Node* findMinimum(Node* node){
        while(node->left != NIL)
            node = node->left;
        return node;
    }


    /**
     * @brief Método privado que limpa a árvore
     * 
     */
    void _clear() {

        if(this->root == NIL) return;

        stack<Node*> stack_aux;
        stack_aux.push(this->root);

        while(!stack_aux.empty()){
            Node* aux = stack_aux.top();
            stack_aux.pop();

            if(aux->left != NIL) stack_aux.push(aux->left);
            if(aux->right != NIL) stack_aux.push(aux->right);

            delete aux;
        }

        this->root = NIL;
        this->_size = 0;
    }

public:

    /**
     * @brief Construtor da classe RedBlackTree
     * 
     */
    RedBlackTree(){
        NIL = new Node();
        root = NIL;
        _size = 0;
    }

    
    /**
     * @brief Destrutor da classe RedBlackTree
     * 
     */
    ~RedBlackTree(){
        _clear();
        delete NIL;
    }


    /**
     * @brief Método público que insere um nó na árvore
     * 
     * @param key parametro para chave
     * @param value parametro para valor
     */
    void insert(const K& key, const V& value) override{
        add(key, value);
    }


    /**
     * @brief Método público que atualiza o valor de um nó com base na sua chave
     * 
     * @param key parametro para chave
     * @param value parametro para valor
     */
    void update(const K& key, const V& value) override{
        _update(key, value);
    }


    /**
     * @brief Método público que remove um nó da árvore
     * 
     * @param key parametro para chave
     */
    void remove(const K& key) override{
        RemoveNode(key);
    }


    /**
     * @brief Retorna o Iterador da árvore Rubro-Negra
     * 
     * @return Iterator<K, V>* 
     */
    Iterator<K, V>* getIterator() const override{
        return new RBIterator(this->root, this->NIL);
    }


    /**
     * @brief Retorna o valor associado a determinada chave
     * 
     * @param key parametro para chave
     * @return V valor associado a chave
     */
    V get(const K& key) const override{
        Node* x = search(key);
        return (x == NIL ? throw runtime_error("Key not found in tree"): x->value);
    }


    /**
     * @brief Verifica se uma chave pertence a árvore
     * 
     * @param key parametro para chave
     * @return true Se a chave pertence a árvore
     * @return false Se a chave não pertence a árvore
     */
    bool contains(const K& key) const override{
        Node* x = search(key);
        return x != NIL;
    }

    /**
     * @brief Método público que limpa a árvore
     * 
     */
    void clear() override{
        _clear();
    }


    /**
     * @brief Retorna a quantidade de nós na árvore
     * 
     * @return size_t 
     */
    size_t size() const override{
        return _size;
    } 

    /**
     * @brief Retorna o número de comparações ao adicionar, procurar ou atualizar um nó
     * 
     * @return size_t variavel count
     */
    size_t getComparison_Counter() const override{
        return count;
    }


};

#endif