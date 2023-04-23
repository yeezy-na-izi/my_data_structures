#pragma once

#include <utility>
#include <cstdint>

using Key = uint32_t;
using Value = double;

/*!
    Имплементация бинарного дерева поиска
*/
class BinarySearchTree {
    struct Node {
        Node(Key key,
             Value value,
             Node *parent = nullptr,
             Node *left = nullptr,
             Node *right = nullptr);

        Node(const Node &other);

        bool operator==(const Node &other) const;

        void output_node(const std::string &prefix, const Node *node, bool isLeft);

        void insert(const Key &key, const Value &value);

        void erase(const Key &key);

        std::pair<Key, Value> keyValuePair;
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
    };

public:
    //! Конструктор по умолчанию
    BinarySearchTree() = default;

    //! Копирование
    explicit BinarySearchTree(const BinarySearchTree &other);

    BinarySearchTree &operator=(const BinarySearchTree &other);

    //! Перемещение
    explicit BinarySearchTree(BinarySearchTree &&other) noexcept;

    BinarySearchTree &operator=(BinarySearchTree &&other) noexcept;

    //! Деструктор
    ~BinarySearchTree();

    /*!
        Итератор бинарного дерева поиска

        Обходит дерево последовательно от меньшего ключа к большему 
    */
    class Iterator {
    public:
        explicit Iterator(Node *node);

        std::pair<Key, Value> &operator*();

        const std::pair<Key, Value> &operator*() const;

        std::pair<Key, Value> *operator->();

        const std::pair<Key, Value> *operator->() const;

        Iterator operator++();

        Iterator operator++(int);

        Iterator operator--();

        Iterator operator--(int);

        bool operator==(const Iterator &other) const;

        bool operator!=(const Iterator &other) const;

    private:
        Node *_node;
    };

    /*!
        Константный итератор
    */
    class ConstIterator {
    public:
        explicit ConstIterator(const Node *node);

        const std::pair<Key, Value> &operator*() const;

        const std::pair<Key, Value> *operator->() const;

        ConstIterator operator++();

        ConstIterator operator++(int);

        ConstIterator operator--();

        ConstIterator operator--(int);

        bool operator==(const ConstIterator &other) const;

        bool operator!=(const ConstIterator &other) const;

    private:
        const Node *_node;
    };

    // вставить элемент с ключем key и значением value
    void insert(const Key &key, const Value &value);

    // удалить все элементы с ключем key
    void erase(const Key &key);

    // найти первый элемент в дереве, равный ключу key
    ConstIterator find(const Key &key) const;

    Iterator find(const Key &key);

    // найти все элементы, у которых ключ равен key
    // первый итератор пары - первый элемент в дереве, равный key
    // второй итератор пары - первый элемент в дереве больший, чем key
    // [pair.first, pair.second) - полуинтервал, содержащий все элементы с ключем key
    std::pair<Iterator, Iterator> equalRange(const Key &key);

    std::pair<ConstIterator, ConstIterator> equalRange(const Key &key) const;

    // получить итератор на минимальное значение в дереве
    ConstIterator min(const Key &key) const;

    // получить итератор на максимальное значение в дереве
    ConstIterator max(const Key &key) const;

    Iterator begin();

    Iterator end();

    ConstIterator cbegin() const;

    ConstIterator cend() const;

    size_t size() const;

    void output_tree();

private:
    size_t _size = 0;
    Node *_root = nullptr; //!< корневой узел дерева
};
