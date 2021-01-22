#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

template<typename Key, typename T, typename Hash=std::hash<Key>>
class unorderder_map {
    public:
        class iterator;

        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<Key, T>;
        using pointer = value_type*;

        unorderder_map() = delete;
        unorderder_map(std::size_t _bucket_count) :
            table(_bucket_count)
        {
        }

        class iterator {
            public:
                iterator(
                    std::vector<std::list<value_type>>& _table, 
                    std::size_t _bucket_pos, 
                    typename std::list<value_type>::iterator _it) : 
                    table(_table),
                    bucket_pos(_bucket_pos),
                    it(_it)
                {}

                iterator(const iterator& copy) :
                    table(copy.table),
                    it(copy.it),
                    bucket_pos(copy.bucket_pos)
                {}

                iterator operator++(int) {
                    iterator copy(*this);
                    ++(*this);
                    return copy;
                }

                iterator& operator++() {
                    it++;
                    if (it == table[bucket_pos].end()) {
                        bucket_pos++;
                        if (bucket_pos != table.size()) {

                            while (table[bucket_pos].empty()) {
                                bucket_pos++;
                            }
                            if (bucket_pos != table.size()) {
                                it = table[bucket_pos].begin();
                            }
                        }
                    } else {
                        it++;
                    }
                    return *this;
                }

                value_type* operator->() {
                    return &(*it);
                }

                value_type operator*() {
                    return *it;
                }

                bool operator!=(const iterator& rhs) {
                    return !((*this) == rhs);
                }

                bool operator==(const iterator& rhs) {
                    return it == rhs.it && bucket_pos == rhs.bucket_pos;
                }
                
            private:
                std::vector<std::list<value_type>>& table;
                std::size_t bucket_pos;
                typename std::list<value_type>::iterator it;
        };
        
        // (h % b) % a != (h % a)
        // 1 == (10 % 3) % 2 != (10 % 2) == 0
        void rehash(std::size_t count) {
            count = (count < size() / max_load_factor()) ? size() / max_load_factor() : count;
    
            if (count < bucket_count()) {
                std::vector<std::list<value_type>> new_table(count);
                for (std::size_t i = 0; i < bucket_count(); i++) {
                    if (!table[i].empty()) {
                        auto val = table[i].front().first;
                        std::size_t bucket_pos = hash(val) % count;
                        new_table[bucket_pos].splice(new_table[bucket_pos].end(), table[i]);
                    }
                }
                table = std::move(new_table);
            }
            table.resize(count);
        }

        std::size_t bucket_count() const {
            return table.size();
        }

        float load_factor() const {
            return size() / bucket_count();
        }
        
        float max_load_factor() const {
            return _max_load_factor;
        }

        void max_load_factor(float ml) {
            _max_load_factor = ml;
        }

        std::size_t size() const {
            std::size_t sum = 0;
            for (std::size_t i = 0; i < bucket_count(); i++) {
                sum += table[i].size();
            }
            return sum;
        }
        
        std::pair<iterator, bool> insert(const value_type& value) {
            
            if (size() > max_load_factor() * bucket_count()) {
                rehash(rehash_factor * bucket_count());
            }

            std::size_t bucket_pos = hash(value.first) % bucket_count();
            auto it = std::find_if(table[bucket_pos].begin(), table[bucket_pos].end(), [&] (const value_type& e) { return e.first == value.first; });
            if (it == table[bucket_pos].end()) {
                table[bucket_pos].push_back(value);
                auto it = table[bucket_pos].end();
                it--;
                return std::make_pair<iterator, bool>(iterator(table, bucket_pos, it), true);
            }
            
            return std::make_pair<iterator, bool>(iterator(table, bucket_pos, it), false);
        };

        std::size_t erase(const key_type& key) {
            std::size_t bucket_pos = hash(key) % bucket_count();
            auto it = std::find_if(table[bucket_pos].begin(), table[bucket_pos].end(), [&] (const value_type& e) { return e.first == key; });
            if (it == table[bucket_pos].end()) {
                return 0;
            }
            table[bucket_pos].erase(it);
            return 1;
        }

        iterator begin() noexcept {
            return iterator(table, 0, table[0].begin());
        }

        iterator end() noexcept {
            return iterator(table, bucket_count(), table[bucket_count() - 1].end());
        } 

    private:
        const float _max_load_factor = 200;
        const std::size_t rehash_factor = 2;
        std::vector<std::list<value_type>> table;
        Hash hash;
};

int main() {
    
    unorderder_map<int, std::string> container(10);

    for (int i = 0; i < 10; i++) {
        container.insert(std::make_pair<int, std::string>(std::move(i), "hello"));
    }
    
    std::cout << "iteration:";
    for (auto it = container.begin(); it != container.end(); it++) {
        std::cout << it->first;
    }
    std::cout << std::endl;


    std::cout << "BUCKET_COUNT:" << container.bucket_count() << std::endl;
    container.rehash(2);
    std::cout << "BUCKET_COUNT:" << container.bucket_count() << std::endl;

    
    std::cout << "iteration:";
    for (auto it = container.begin(); it != container.end(); it++) {
        std::cout << it->first;
    }
    std::cout << std::endl;

    container.erase(4);
    
    std::cout << "iteration:";
    for (auto it = container.begin(); it != container.end(); it++) {
        std::cout << it->first;
    }
    std::cout << std::endl;
    return 0;
};