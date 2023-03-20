#pragma once

#include <iostream>

#define Value int

class BinaryTree {
public:
    BinaryTree() = default;

    BinaryTree(const BinaryTree &other);

    BinaryTree(BinaryTree &&other) noexcept;

    BinaryTree &operator=(const BinaryTree &other);

    BinaryTree &operator=(BinaryTree &&other) noexcept;

    ~BinaryTree();

    void insert(const Value &value, bool balance = true);

    void balance();

    void remove(const Value &value);

    bool contains(const Value &value) const;

    size_t size() const;

    bool empty() const;

    void clear();

    void print() const;

private:
    struct Node {
        Value value;
        Node *left = nullptr;
        Node *right = nullptr;
        int factor = 0;
    };

    Node *root = nullptr;
    size_t _size = 0;

    void smallLeftRotation(Node *&node);

    void smallRightRotation(Node *&node);

    void bigLeftRotation(Node *&node);

    void bigRightRotation(Node *&node);


};