#ifndef HASH_OPEN_ADR
#define HASH_OPEN_ADR

#include <functional>
#include <vector>
#include <stdexcept>
#include "Dictionary.hpp"

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

enum class Status {
    EMPTY,
    ACTIVE,
    DELETED
};

template<typename K, typename V>
struct Slot {
    K      key;
    V      value;
    Status status { Status::EMPTY };
};

template<typename K, typename V, typename Hash =  Hasher<K>>
class HashOpenAdr : public Dictionary<K, V> {

private:
    size_t                  m_table_size;
    size_t                  m_number_of_elements = 0;
    float                   m_max_load_factor;
    std::vector<Slot<K,V>>  m_table;
    Hash                    m_hashing;
    mutable size_t          m_comparison_count = 0;

    size_t get_next_prime(size_t x) {
        if (x <= 2) return 3;
        if (x % 2 == 0) x++;
        while (true) {
            bool prime = true;
            for (size_t i = 2; i * i <= x; ++i) {
                if (x % i == 0) { prime = false; break; }
            }
            if (prime) return x;
            x += 2;
        }
    }

    size_t hash_code(const K& k, size_t i) const {
        size_t h  = m_hashing(k);
        size_t h1 = h % m_table_size;
        size_t h2 = 1 + (h % (m_table_size - 1));
        return (h1 + i * h2) % m_table_size;
    }

    int aux_search(const K& k) const {
        for (size_t i = 0; i < m_table_size; ++i) {
            size_t           idx = hash_code(k, i);
            const Slot<K,V>& s   = m_table[idx];

            if (s.status == Status::EMPTY) {
                return -1;
            }
            if (s.status == Status::ACTIVE) {
                ++m_comparison_count;
                if (s.key == k) {
                    return static_cast<int>(idx);
                }
            }
        }
        return -1;
    }

    void insert_internal(const K& key, const V& value) {
        for (size_t i = 0; i < m_table_size; ++i) {
            size_t pos = hash_code(key, i);
            if (m_table[pos].status != Status::ACTIVE) {
                m_table[pos] = { key, value, Status::ACTIVE };
                ++m_number_of_elements;
                return;
            }
        }
        throw std::runtime_error("insert_internal: tabela sem espaço disponível.");
    }

    class HashIterator : public Iterator<K, V> {
    private:
        const std::vector<Slot<K,V>>& m_ref;
        size_t                         m_pos;

        void advance() {
            while (m_pos < m_ref.size() &&
                   m_ref[m_pos].status != Status::ACTIVE) {
                ++m_pos;
            }
        }

    public:
        explicit HashIterator(const std::vector<Slot<K,V>>& table)
            : m_ref(table), m_pos(0)
        {
            advance();
        }

        bool hasNext() override {
            return m_pos < m_ref.size();
        }

        std::pair<K, V> next() override {
            if (!hasNext()) {
                throw std::out_of_range("HashIterator: iterador esgotado.");
            }
            std::pair<K, V> result { m_ref[m_pos].key, m_ref[m_pos].value };
            ++m_pos;
            advance();
            return result;
        }
    };

public:

    explicit HashOpenAdr(size_t tbsize = 13, float loadFactor = 0.7f)
        : m_max_load_factor(loadFactor)
    {
        m_table_size = get_next_prime(tbsize);
        m_table.assign(m_table_size, Slot<K,V>());
    }

    ~HashOpenAdr() override = default;

    void insert(const K& key, const V& value) override {
        if (contains(key)) {
            throw std::invalid_argument(
                "insert: chave já existe. Use update() para alterar o valor.");
        }
        if (load_factor() >= m_max_load_factor) {
            rehash(2 * m_table_size);
        }
        insert_internal(key, value);
    }

    void update(const K& key, const V& value) override {
        int idx = aux_search(key);
        if (idx == -1) {
            throw std::out_of_range(
                "update: chave não encontrada na tabela.");
        }
        m_table[idx].value = value;
    }

    void remove(const K& key) override {
        int idx = aux_search(key);
        if (idx == -1) {
            throw std::out_of_range(
                "remove: chave não encontrada na tabela.");
        }
        m_table[idx].status = Status::DELETED;
        --m_number_of_elements;
    }

    void clear() override {
        for (auto& slot : m_table) {
            slot.status = Status::EMPTY;
        }
        m_number_of_elements = 0;
    }

    V get(const K& key) const override {
        int idx = aux_search(key);
        if (idx == -1) {
            throw std::out_of_range(
                "get: chave não encontrada na tabela.");
        }
        return m_table[idx].value;
    }

    bool contains(const K& key) const override {
        return aux_search(key) != -1;
    }

    std::size_t size() const override {
        return m_number_of_elements;
    }

    Iterator<K, V>* getIterator() const override {
        return new HashIterator(m_table);
    }

    size_t capacity() const {
        return m_table_size;
    }

    bool empty() const {
        return m_number_of_elements == 0;
    }

    float load_factor() const {
        return static_cast<float>(m_number_of_elements)
             / static_cast<float>(m_table_size);
    }

    float max_load_factor() const {
        return m_max_load_factor;
    }

    void rehash(size_t m) {
        if (m <= m_table_size) return;

        auto   old_table = m_table;
        size_t new_size  = get_next_prime(m);

        m_table_size         = new_size;
        m_table              = std::vector<Slot<K,V>>(new_size);
        m_number_of_elements = 0;

        for (const auto& slot : old_table) {
            if (slot.status == Status::ACTIVE) {
                insert_internal(slot.key, slot.value);
            }
        }
    }

    size_t getComparison_Counter() const {
        return m_comparison_count;
    }

    V& operator[](const K& k) {
        int idx = aux_search(k);
        if (idx != -1) {
            return m_table[idx].value;
        }
        if (load_factor() >= m_max_load_factor) {
            rehash(2 * m_table_size);
        }
        for (size_t i = 0; i < m_table_size; ++i) {
            size_t pos = hash_code(k, i);
            if (m_table[pos].status != Status::ACTIVE) {
                m_table[pos] = { k, V{}, Status::ACTIVE };
                ++m_number_of_elements;
                return m_table[pos].value;
            }
        }
        throw std::runtime_error("operator[]: tabela sem espaço disponível.");
    }

    const V& operator[](const K& k) const {
        int idx = aux_search(k);
        if (idx == -1) {
            throw std::out_of_range("operator[] const: chave não encontrada.");
        }
        return m_table[idx].value;
    }
};

#endif