#ifndef RED_BLACK_TREE
#define RED_BLACK_TREE

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

    struct Node{
        K key;
        V value;
        Color color;
        Node* left;
        Node* right;

        Node() : color(BLACK), left(nullptr), right(nullptr) {}          // Construtor nó NIL
        Node(const K& key, const V& value) : key(key), value(value) {}  
        ~Node() = default;
        
    };

    Node* root;
    Node* NIL;
    int _size;



    Node* leftRotation(Node* node){
        Node* y = node->right;
        node->right = y->left;
        y->left = node;

        return y; 
    }


    Node* rightRotation(Node* node){
        Node* y = node->left;
        node->left = y->right;
        y->right = node;

        return y;
    }

    
    void add(K& key, V& value){
        //TODO
    }
    
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

    RedBlackTree(){
        NIL = new Node();
        root = NIL;
    }

    ~RedBlackTree(){
        _clear();
        delete NIL;
    }



    void insert(K& key, V& value) override{
        add(key, value);
    }


    void clear() override{
        _clear();
    }

    
};

#endif