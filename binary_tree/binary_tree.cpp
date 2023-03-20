#include "binary_tree.h"
#include <algorithm>


BinaryTree::BinaryTree(const BinaryTree &other) {
    if (other.root) {
        root = new Node;
        root->value = other.root->value;
        root->factor = other.root->factor;
        root->left = other.root->left;
        root->right = other.root->right;
    }
}

BinaryTree::BinaryTree(BinaryTree &&other) noexcept {
    if (other.root) {
        root = new Node;
        root->value = other.root->value;
        root->factor = other.root->factor;
        root->left = other.root->left;
        root->right = other.root->right;
    }
}

BinaryTree &BinaryTree::operator=(const BinaryTree &other) {
    if (other.root) {
        root = new Node;
        root->value = other.root->value;
        root->factor = other.root->factor;
        root->left = other.root->left;
        root->right = other.root->right;
    }
    return *this;
}

BinaryTree &BinaryTree::operator=(BinaryTree &&other) noexcept {
    if (other.root) {
        root = new Node;
        root->value = other.root->value;
        root->factor = other.root->factor;
        root->left = other.root->left;
        root->right = other.root->right;
    }
    return *this;
}


BinaryTree::~BinaryTree() {
    clear();
}

void BinaryTree::insert(const Value &value, bool balance) {
    Node *node = root;
    Node *parent = nullptr;
    while (node) {
        parent = node;
        if (value < node->value) {
            node = node->left;
        } else if (value > node->value) {
            node = node->right;
        } else {
            return;
        }
    }
    node = new Node;
    node->value = value;
    if (parent) {
        if (value < parent->value) {
            parent->left = node;
        } else {
            parent->right = node;
        }
    } else {
        root = node;
    }
    if (balance) {
        this->balance();
    }
    _size++;
}