#ifndef HASH_CHANING
#define HASH_CHANING
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <cmath>
#include "Iterator.hpp"
using namespace std;


/**
 * @brief Struct Hasher genérica que sobrecarrega o operador()
 * 
 * @tparam K parametro para chave
 */
template<typename K>

struct Hasher {
    size_t operator()(const K& key) const {
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&key);
        size_t hash = 14695981039346656037ULL;
        for (size_t i = 0; i < sizeof(K); i++) {
            hash ^= bytes[i];
            hash *= 1099511628211ULL;
        }
        return hash;
    }
};


/**
 * @brief Struct Hasher para String que sobrecarrega o operador()
 * 
 * @tparam  
 */
template<>
struct Hasher<string> {
    size_t operator()(const string& key) const {
        size_t hash = 14695981039346656037ULL; // FNV offset basis
        for (unsigned char c : key) {
            hash ^= c;
            hash *= 1099511628211ULL; // FNV prime
        }
        return hash;
    }
};


/**
 * @brief Classe que implementa uma tabela Hash com tratamento de colisão por encadeamento exterior
 * 
 * @tparam K parametro para chave
 * @tparam V parametro para valor
 * @tparam H parametro para a estrutura de hashing 
 * @author
 */

template<typename K, typename V, typename H = Hasher<K>>
class HashChaining : public Dictionary<K, V>{

private:

    vector<list<pair<K, V>>> table;
    size_t number_of_elements;
    size_t table_size;
    float max_load_factor;
    H hashing;
    mutable size_t count = 0;



    /**
     * @brief Classe interna responsável por fornecer a implementação do iterador para a tabela
     * 
     */
    class HCIterator : public Iterator<K, V>{

        private:
        queue<pair<K, V>> st;
        const vector<list<pair<K, V>>>& table;
        size_t number_elements;

        public:

        HCIterator(const vector<list<pair<K, V>>>& table, size_t number_elements) : table(table), number_elements(number_elements) {
            for (const list<pair<K, V>>& bucket : table){
                for (const pair<K, V>& pair : bucket)
                    st.push(pair);
            }
        }

        bool hasNext() override{
            return !st.empty();
        }

        pair<K, V> next() override{
            pair<K, V> aux = st.front();
            st.pop();
            return aux;
        }
    };


    /**
     * @brief Calcula o próximo número primo após x
     * 
     * @param x : número natural
     * @return size_t : numero primo
     */
    size_t get_next_prime(size_t x) {
        if(x <= 2) return 3;
        x = (x % 2 == 0) ? x + 1 : x;
        bool not_prime = true;
        while(not_prime) {
            not_prime = false;
            for(int i = 2; i <= sqrt(x); ++i) {
                if(x % i == 0) {
                    not_prime = true;
                    break;
                }
            }
            x += 2;
        }
        return x - 2;
    }


    /**
     * @brief Retorna o código hash de uma chave.
     * 
     * @param key : Chave genérica
     * @return size_t : Número positivo no intervalo de [0...table_size-1]
     */
    
    size_t hash_code(const K& key) const{
        return hashing(key) % table_size;
    }
    

    /**
     * @brief Responsável por rearranjar a tabela atual em uma maior, 
     * chamada quando o fator de carga chega ao máximo.
     * 
     */
    void rehash() {

        size_t new_size = get_next_prime(table_size * 2);
        vector<list<pair<K, V>>> new_table(new_size);
        
        for (auto& bucket : table)
            for (auto& [key, value] : bucket)
            new_table[hashing(key) % new_size].push_back({key, value});
        
            table.swap(new_table);
        table_size = new_size;
        
    }

    
    /**
     * @brief Método const que recebe uma chave e buscar o par chave-valor na tabela
     * 
     * @param key parametro para a chave
     * @return pair<K, V>& : par chave-valor correspondente a chave
     */
    pair<K, V> search(const K& key) const{
        size_t hashcode = hash_code(key);

        for(const pair<K, V>& x: table[hashcode]){
            count ++;
            if(key == x.first) return {x.first, x.second};
        }

        throw out_of_range("Key not found in hash table");
    }
    
    /**
     * @brief Recebe uma chave e buscar o par chave-valor na tabela e retorna uma referência 
     * @param key parametro para a chave
     * @return pair<K, V>& : par chave-valor correspondente a chave
     */
    pair<K, V>& find(const K& key){
        size_t hashcode = hash_code(key);

        for(pair<K, V>& x: table[hashcode]){
            count++;
            if(key == x.first) return x;
        }

        throw out_of_range("Key not found in hash table");
    }
    
    
    /**
     * @brief Método privado que adiciona um par chave-valor na tabela
     * 
     * @param key parametro para chave
     * @param value parametro para valor
     * @throws runtime_error Se a chave já estiver presente na tabela
     */
    void add(const K& key, const V& value){
        
        if(contains(key)){
            throw runtime_error("Repeated key.");
        }
        
        if((float)number_of_elements / table_size > max_load_factor){
            rehash();
        }
        
        size_t hashcode = hash_code(key);
        table[hashcode].push_back({key, value});
        number_of_elements++;
    }
    
    
    /**
     * @brief Método privado que remove um par chave-valor na tabela
     * 
     * @param key parametro para chave
     * @param value parametro para valor
     */
    void remove_pair(const K& key){
        if(!contains(key)){
            throw runtime_error("Key not found in hash table");
        }
    
        size_t hashcode = hash_code(key);
        table[hashcode].remove_if([&key](const pair<K, V>& p){
            return p.first == key;
        });
    
        number_of_elements --;
    }

    /**
     * @brief Método privado que limpa a tabela
     * 
     */
    void _clear(){
        for(list<pair<K, V>>& x: table){
            x.clear();
        }

        number_of_elements = 0;
    }
    
    


public:


    /**
     * @brief Construtor da Classe HashChaining
     * 
     * @param tableSize : Valor inicial para o tamanho da tabela
     * @param maxLoadFactor :  Fator de carga máximo
     */
    HashChaining(size_t tableSize = 19, float maxLoadFactor = 0.8f){
        this->table_size = tableSize;
        table.resize(table_size);
        this->max_load_factor = maxLoadFactor;
        this->number_of_elements = 0;
    }


    /**
     * @brief Método público que insere um par chave-valor na tabela
     * 
     * @param key parametro para chave
     * @param value parametro para valor
     */
    void insert(const K& key, const V& value) override{
        add(key, value);
    }


    /**
     * @brief Método que faz o update de um valor com base na sua chave
     * 
     * @param key parametro para chave
     * @param value parametro para valor
     */
    void update(const K& key, const V& value) override{
        pair<K, V>& aux = find(key);
        aux.second = value;
    }


    /**
     * @brief Método público que remove um par chave-valor da tabela
     * 
     * @param key parametro para chave
     */
    void remove(const K& key) override{
        remove_pair(key);
    }


    /**
     * @brief Método público que limpa a tabela;
     * 
     */
    void clear() override{
        _clear();
    }


    /**
     * @brief Método responsável por retornar o Iterador da Tabela
     * 
     * @return iterator<K, V> 
     */
    Iterator<K, V>* getIterator() const override{
        return new HCIterator(table, number_of_elements);
    }


    /**
     * @brief Método resposável por retornar um valor V atrelado a uma chave
     * 
     * @param key parametro para chave
     * @return V valor atrelado a chave
     */
    V get(const K& key) const override{
        pair<K, V> aux = search(key);
        return aux.second;
    };


    /**
     * @brief Verifica se uma chave está contida na tabela
     * 
     * @param key parametro para chave
     * @return true : se está contida
     * @return false : se não está contida
     */
    bool contains(const K& key) const override{
        try{
            search(key);
            return true;
        }catch(out_of_range&){
            return false;
        }
    }

    
    /**
     * @brief Retorna o número de elementos na tabela
     * 
     * @return size_t 
     */
    size_t size()const override{
        return number_of_elements;
    }

    /**
     * @brief retorna o numero de comparações feitas ao adicionar, procurar ou atualizar um par chave-valor na tabela 
     * 
     * @return size_t 
     */
    size_t getComparison_Counter() const override{
        return count;
    }

 
};

#endif
