#include "HashTable.h"


HashTable::HashTable(size_t size) noexcept {
    _capacity = size;
    _filled = 0;
    table.resize(_capacity);
}

HashTable::~HashTable() = default;


double HashTable::getLoadFactor() {
    return static_cast<double>(_filled) / _capacity;
}


void HashTable::insert(const KeyType &key, const ValueType &value) {
    if (getLoadFactor() > 0.75) {
        _capacity *= 2;
        std::vector<std::list<std::pair<KeyType, ValueType>>> new_table(_capacity);
        for (auto &list : table) {
            for (auto &pair : list) {
                new_table[hash_function(pair.first)].push_back(pair);
            }
        }
        table = new_table;
    }
    table[hash_function(key)].emplace_back(key, value);
    _filled++;
}

bool HashTable::find(const KeyType &key, ValueType &value) const {
    for (auto &pair : table[hash_function(key)]) {
        if (pair.first == key) {
            value = pair.second;
            return true;
        }
    }
    return false;
}

void HashTable::remove(const KeyType &key) {
    for (auto it = table[hash_function(key)].begin(); it != table[hash_function(key)].end(); it++) {
        if (it->first == key) {
            table[hash_function(key)].erase(it);
            _filled--;
            return;
        }
    }
}


ValueType &HashTable::operator[](const KeyType &key) {
    for (auto &pair : table[hash_function(key)]) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    insert(key, 0);
    return table[hash_function(key)].back().second;
}


size_t HashTable::hash_function(const KeyType &key) const {
    size_t hash = 0;
    for (auto &c : key) {
        hash = (hash * 127 + c) % _capacity;
    }
    return hash;
}