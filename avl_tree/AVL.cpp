#include "AVL.h"
#include <vector>
#include <iostream>


AVLTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right)
        : keyValuePair(key, value), parent(parent), left(left), right(right) {}


AVLTree::Node::Node(const AVLTree::Node &other) {
    keyValuePair = other.keyValuePair;
    parent = other.parent;
    m_height = other.m_height;
    if (other.left == nullptr) {
        left = nullptr;
    } else {
        left = new Node(*other.left);
    }
    if (other.right == nullptr) {
        right = nullptr;
    } else {
        right = new Node(*other.right);
    }
}

AVLTree::Node::~Node() {
    if (left != nullptr) {
        delete left;
    }
    if (right != nullptr) {
        delete right;
    }
}

void AVLTree::Node::insert(const Key &key, const Value &value) {
    if (key < keyValuePair.first) {
        if (left == nullptr) {
            left = new Node(key, value, this);
        } else {
            left->insert(key, value);
        }
    } else if (key > keyValuePair.first) {
        if (right == nullptr) {
            right = new Node(key, value, this);
        } else {
            right->insert(key, value);
        }
    } else if (value < keyValuePair.first) {
        if (left == nullptr) {
            left = new Node(key, value, this);
        } else {
            left->insert(key, value);
        }
    } else {
        if (right == nullptr) {
            right = new Node(key, value, this);
        } else {
            right->insert(key, value);
        }
    }
    m_height = 1 + std::max(
            left == nullptr ? 0 : left->m_height,
            right == nullptr ? 0 : right->m_height
    );
    balance();
}

bool AVLTree::Node::operator==(const AVLTree::Node &other) const {
    return keyValuePair == other.keyValuePair && parent == other.parent && left == other.left &&
           right == other.right;
}

void AVLTree::Node::output_node(const std::string &prefix, const AVLTree::Node *node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──");

        std::cout << node->keyValuePair.first << std::endl;

        output_node(prefix + (isLeft ? "│   " : "    "), node->left, node->right != nullptr);
        output_node(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void AVLTree::Node::small_left_rotation() {
    Node* new_root = right;
    Node* new_left = new Node(keyValuePair.first, keyValuePair.second, this, left, new_root->left);
    new_root->left = new_left;
    if (new_left->left != nullptr) {
        new_left->left->parent = new_left;
    }
    keyValuePair = new_root->keyValuePair;
    right = new_root->right;
    if (right != nullptr) {
        right->parent = this;
    }
    left = new_left;
    new_left->parent = this;
    m_height = 1 + std::max(
            left == nullptr ? 0 : left->m_height,
            right == nullptr ? 0 : right->m_height
    );
    new_root->m_height = 1 + std::max(
            new_root->left == nullptr ? 0 : new_root->left->m_height,
            new_root->right == nullptr ? 0 : new_root->right->m_height
    );
}

void AVLTree::Node::small_right_rotation() {
    Node* new_root = left;
    Node* new_right = new Node(keyValuePair.first, keyValuePair.second, this, new_root->right, right);
    new_root->right = new_right;
    if (new_right->right != nullptr) {
        new_right->right->parent = new_right;
    }
    keyValuePair = new_root->keyValuePair;
    left = new_root->left;
    if (left != nullptr) {
        left->parent = this;
    }
    right = new_right;
    new_right->parent = this;
    m_height = 1 + std::max(
            left == nullptr ? 0 : left->m_height,
            right == nullptr ? 0 : right->m_height
    );
    new_root->m_height = 1 + std::max(
            new_root->left == nullptr ? 0 : new_root->left->m_height,
            new_root->right == nullptr ? 0 : new_root->right->m_height
    );
}

void AVLTree::Node::big_left_rotation() {
    right->small_right_rotation();
    small_left_rotation();
}

void AVLTree::Node::big_right_rotation() {
    left->small_left_rotation();
    small_right_rotation();
}

void AVLTree::Node::balance() {
    int left_height = left == nullptr ? 0 : left->m_height;
    int right_height = right == nullptr ? 0 : right->m_height;
    if (left_height - right_height > 1) {
        int left_left_height = left->left == nullptr ? 0 : left->left->m_height;
        int left_right_height = left->right == nullptr ? 0 : left->right->m_height;
        if (left_left_height - left_right_height > 0) {
            small_right_rotation();
        } else {
            big_right_rotation();
        }
    } else if (right_height - left_height > 1) {
        int right_left_height = right->left == nullptr ? 0 : right->left->m_height;
        int right_right_height = right->right == nullptr ? 0 : right->right->m_height;
        if (right_right_height - right_left_height > 0) {
            small_left_rotation();
        } else {
            big_left_rotation();
        }
    }
}

AVLTree::AVLTree(
        const AVLTree &other) {
    if (other._root == nullptr) {
        _root = nullptr;
    } else {
        _root = new Node(*other._root);
    }
}


AVLTree &AVLTree::operator=(const AVLTree &other) {
    if (this == &other) {
        return *this;
    }
    this->~AVLTree();
    if (other._root == nullptr) {
        _root = nullptr;
    } else {
        _root = new Node(*other._root);
    }
    return *this;
}

AVLTree::AVLTree(AVLTree &&other)
noexcept {
    _root = other._root;
    other._root = nullptr;
}

AVLTree &AVLTree::operator=(AVLTree &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    this->~AVLTree();
    _root = other._root;
    other._root = nullptr;
    return *this;
}

AVLTree::~AVLTree() {
    delete _root;
}

AVLTree::Iterator::Iterator(AVLTree::Node *node) {
    _node = node;
}

std::pair<Key, Value> &AVLTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value> &AVLTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value> *AVLTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *AVLTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

AVLTree::Iterator AVLTree::Iterator::operator++() {
    if (_node->right != nullptr) {
        _node = _node->right;
        while (_node->left != nullptr) {
            _node = _node->left;
        }
    } else {
        while (_node->parent != nullptr && _node->parent->right == _node) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}

AVLTree::Iterator AVLTree::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

AVLTree::Iterator AVLTree::Iterator::operator--() {
    if (_node->left != nullptr) {
        _node = _node->left;
        while (_node->right != nullptr) {
            _node = _node->right;
        }
    } else {
        while (_node->parent != nullptr && _node->parent->left == _node) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}

AVLTree::Iterator AVLTree::Iterator::operator--(int) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
}

bool AVLTree::Iterator::operator==(const AVLTree::Iterator &other) const {
    return _node == other._node;
}

bool AVLTree::Iterator::operator!=(const AVLTree::Iterator &other) const {
    return !(*this == other);
}


AVLTree::ConstIterator::ConstIterator(
        const AVLTree::Node *node) {
    _node = node;
}

const std::pair<Key, Value> &AVLTree::ConstIterator::operator*() const {
    return _node->keyValuePair;
}

const std::pair<Key, Value> *AVLTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

AVLTree::ConstIterator AVLTree::ConstIterator::operator++() {
    if (_node->right != nullptr) {
        _node = _node->right;
        while (_node->left != nullptr) {
            _node = _node->left;
        }
    } else {
        while (_node->parent != nullptr && _node->parent->right == _node) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}

AVLTree::ConstIterator AVLTree::ConstIterator::operator++(int) {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
}

AVLTree::ConstIterator AVLTree::ConstIterator::operator--() {
    if (_node->left != nullptr) {
        _node = _node->left;
        while (_node->right != nullptr) {
            _node = _node->right;
        }
    } else {
        while (_node->parent != nullptr && _node->parent->left == _node) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}

AVLTree::ConstIterator AVLTree::ConstIterator::operator--(int) {
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
}

bool AVLTree::ConstIterator::operator==(const AVLTree::ConstIterator &other) const {
    return _node == other._node;
}

bool AVLTree::ConstIterator::operator!=(const AVLTree::ConstIterator &other) const {
    return !(*this == other);
}

void AVLTree::insert(const Key &key, const Value &value) {
    _size++;
    if (_root == nullptr) {
        _root = new Node(key, value);
    } else {
        _root->insert(key, value);
    }
}


void AVLTree::deleteNode(AVLTree::Node *node) {
    if (node->left == nullptr && node->right == nullptr) {
        if (node->parent == nullptr) {
            _root = nullptr;
        } else if (node->parent->left == node) {
            node->parent->left = nullptr;
        } else {
            node->parent->right = nullptr;
        }
        delete node;
    } else if (node->left == nullptr) {
        if (node->parent == nullptr) {
            _root = node->right;
            _root->parent = nullptr;
        } else if (node->parent->left == node) {
            node->parent->left = node->right;
            node->right->parent = node->parent;
        } else {
            node->parent->right = node->right;
            node->right->parent = node->parent;
        }
        delete node;
    } else if (node->right == nullptr) {
        if (node->parent == nullptr) {
            _root = node->left;
            _root->parent = nullptr;
        } else if (node->parent->left == node) {
            node->parent->left = node->left;
            node->left->parent = node->parent;
        } else {
            node->parent->right = node->left;
            node->left->parent = node->parent;
        }
        delete node;
    } else {
        Node *tmp = node->right;
        while (tmp->left != nullptr) {
            tmp = tmp->left;
        }
        node->keyValuePair = tmp->keyValuePair;
        deleteNode(tmp);
    }
    --_size;
}


void AVLTree::erase(const Key &key) {
    bool flag = true;
    while (flag) {
        flag = false;
        Node *node = _root;
        while (node != nullptr) {
            if (key < node->keyValuePair.first) {
                node = node->left;
            } else if (key > node->keyValuePair.first) {
                node = node->right;
            } else {
                flag = true;
                break;
            }
        }
        if (flag) {
            deleteNode(node);
        }
    }
}

AVLTree::Iterator AVLTree::find(const Key &key) {
    Node *node = _root;
    while (node != nullptr) {
        if (key < node->keyValuePair.first) {
            node = node->left;
        } else if (key > node->keyValuePair.first) {
            node = node->right;
        } else {
            break;
        }
    }
    return Iterator(node);
}

AVLTree::ConstIterator AVLTree::find(const Key &key) const {
    Node *node = _root;
    while (node != nullptr) {
        if (key < node->keyValuePair.first) {
            node = node->left;
        } else if (key > node->keyValuePair.first) {
            node = node->right;
        } else {
            break;
        }
    }
    return ConstIterator(node);
}

std::pair<AVLTree::Iterator, AVLTree::Iterator> AVLTree::equalRange(const Key &key) {
    Iterator it_start = find(key);
    if (it_start == end()) {
        return std::make_pair(it_start, it_start);
    }
    Iterator it_tmp = it_start;
    Iterator it_end = it_start;
    it_tmp--;
    Iterator begin_it = begin();
    while (it_tmp->first == key && it_tmp != begin_it) {
        it_start--;
        it_tmp--;
    }
    it_tmp = it_end;
    it_tmp++;
    Iterator end_it = end();
    while (it_tmp->first == key && it_tmp != end_it) {
        it_end++;
        it_tmp++;
    }
    return std::make_pair(it_start, it_end);
}

std::pair<AVLTree::ConstIterator, AVLTree::ConstIterator>
AVLTree::equalRange(const Key &key) const {
    ConstIterator it_start = find(key);
    if (it_start == cend()) {
        return std::make_pair(it_start, it_start);
    }
    ConstIterator it_tmp = it_start;
    ConstIterator it_end = it_start;
    it_tmp--;
    ConstIterator begin_it = cbegin();
    while (it_tmp->first == key && it_tmp != begin_it) {
        it_start--;
        it_tmp--;
    }
    it_tmp = it_end;
    it_tmp++;
    ConstIterator end_it = cend();
    while (it_tmp->first == key && it_tmp != end_it) {
        it_end++;
        it_tmp++;
    }
    it_tmp++;
    return std::make_pair(it_start, it_end);
}

AVLTree::ConstIterator AVLTree::min(const Key &key) const {
    ConstIterator it = find(key);
    if (it == cend()) {
        return it;
    }
    ConstIterator it_tmp = it;
    it_tmp--;
    ConstIterator begin_it = cbegin();
    while (it_tmp->first == key && it_tmp != begin_it) {
        it--;
        it_tmp--;
    }
    return it;
}

AVLTree::ConstIterator AVLTree::max(const Key &key) const {
    ConstIterator it = find(key);
    if (it == cend()) {
        return it;
    }
    ConstIterator it_tmp = it;
    it_tmp++;
    ConstIterator end_it = cend();
    while (it_tmp->first == key && it_tmp != end_it) {
        it++;
        it_tmp++;
    }
    return it;
}

AVLTree::Iterator AVLTree::begin() {
    Node *node = _root;
    if (node == nullptr) {
        return Iterator(node);
    }
    while (node->left != nullptr) {
        node = node->left;
    }
    return Iterator(node);
}

AVLTree::ConstIterator AVLTree::cbegin() const {
    Node *node = _root;
    if (node == nullptr) {
        return ConstIterator(node);
    }
    while (node->left != nullptr) {
        node = node->left;
    }
    return ConstIterator(node);
}

AVLTree::Iterator AVLTree::end() {
    Node *node = _root;
    if (node == nullptr) {
        return Iterator(node);
    }
    while (node->right != nullptr) {
        node = node->right;
    }
    return Iterator(node);
}

AVLTree::ConstIterator AVLTree::cend() const {
    Node *node = _root;
    if (node == nullptr) {
        return ConstIterator(node);
    }
    while (node->right != nullptr) {
        node = node->right;
    }
    return ConstIterator(node);
}

size_t AVLTree::size() const {
    return _size;
}

void AVLTree::output_tree() {
    _root->output_node("", _root, false);
}