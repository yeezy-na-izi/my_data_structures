#include "AVL.h"
#include <vector>
#include <iostream>


AvlTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right)
        : keyValuePair(key, value), parent(parent), left(left), right(right),
          m_height(1 + std::max(left == nullptr ? 0 : left->m_height, right == nullptr ? 0 : right->m_height)) {}


AvlTree::Node::Node(const AvlTree::Node &other) {
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

void AvlTree::Node::insert(const Key &key, const Value &value) {
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
    balance();
}

void AvlTree::Node::erase(const Key &key) {
    if (key < keyValuePair.first) {
        if (left != nullptr) {
            left->erase(key);
        }
    } else if (key > keyValuePair.first) {
        if (right != nullptr) {
            right->erase(key);
        }
    } else {
        if (left == nullptr && right == nullptr) {
            if (parent != nullptr) {
                if (parent->left == this) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            }
            delete this;
            return;
        } else if (left == nullptr) {
            if (parent != nullptr) {
                if (parent->left == this) {
                    parent->left = right;
                } else {
                    parent->right = right;
                }
            }
            right->parent = parent;
            delete this;
            return;
        } else if (right == nullptr) {
            if (parent != nullptr) {
                if (parent->left == this) {
                    parent->left = left;
                } else {
                    parent->right = left;
                }
            }
            left->parent = parent;
            delete this;
            return;
        } else {
            Node *next = right;
            while (next->left != nullptr) {
                next = next->left;
            }
            keyValuePair = next->keyValuePair;
            next->erase(next->keyValuePair.first);
        }
    }
    balance();
}

bool AvlTree::Node::operator==(const AvlTree::Node &other) const {
    return keyValuePair == other.keyValuePair && parent == other.parent && left == other.left &&
           right == other.right;
}

void AvlTree::Node::output_node(const std::string &prefix, const AvlTree::Node *node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──");

        std::cout << node->keyValuePair.first << std::endl;

        output_node(prefix + (isLeft ? "│   " : "    "), node->left, node->right != nullptr);
        output_node(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void AvlTree::Node::small_left_rotation() {
    std::pair<Key, Value> copy = right->keyValuePair;
    right->keyValuePair = keyValuePair;
    keyValuePair = copy;
    Node *copy_right = right;
    right = right->right;
    if (right != nullptr) {
        right->parent = this;
    }
    copy_right->right = copy_right->left;
    copy_right->left = left;
    if (left != nullptr) {
        left->parent = copy_right;
    }
    left = copy_right;
    left->parent = this;

    update_height();
}

void AvlTree::Node::small_right_rotation() {
    std::pair<Key, Value> copy = left->keyValuePair;
    left->keyValuePair = keyValuePair;
    keyValuePair = copy;
    Node *copy_left = left;
    left = left->left;
    if (left != nullptr) {
        left->parent = this;
    }
    copy_left->left = copy_left->right;
    copy_left->right = right;
    if (right != nullptr) {
        right->parent = copy_left;
    }
    right = copy_left;
    right->parent = this;

    update_height();

}

void AvlTree::Node::big_left_rotation() {
    right->small_right_rotation();
    small_left_rotation();
}

void AvlTree::Node::big_right_rotation() {
    left->small_left_rotation();
    small_right_rotation();
}

void AvlTree::Node::update_height() {
    m_height = 1 + std::max(
            left == nullptr ? 0 : left->m_height,
            right == nullptr ? 0 : right->m_height
    );
    if (left != nullptr) {
        left->m_height = 1 + std::max(
                left->left == nullptr ? 0 : left->left->m_height,
                left->right == nullptr ? 0 : left->right->m_height
        );
    }
    if (right != nullptr) {
        right->m_height = 1 + std::max(
                right->left == nullptr ? 0 : right->left->m_height,
                right->right == nullptr ? 0 : right->right->m_height
        );
    }
}


void AvlTree::Node::balance() {
    update_height();

    int left_height = left == nullptr ? 0 : left->m_height;
    int right_height = right == nullptr ? 0 : right->m_height;
    if (left_height - right_height > 1) {
        left->update_height();
        int left_left_height = left->left == nullptr ? 0 : left->left->m_height;
        int left_right_height = left->right == nullptr ? 0 : left->right->m_height;
        if (left_left_height - left_right_height > 0) {
            small_right_rotation();
        } else {
            big_right_rotation();
        }
    } else if (right_height - left_height > 1) {
        right->update_height();
        int right_left_height = right->left == nullptr ? 0 : right->left->m_height;
        int right_right_height = right->right == nullptr ? 0 : right->right->m_height;
        if (right_right_height - right_left_height > 0) {
            small_left_rotation();
        } else {
            big_left_rotation();
        }
    }
}

AvlTree::AvlTree(
        const AvlTree&other) {
    if (other._root == nullptr) {
        _root = nullptr;
    } else {
        _root = new Node(*other._root);
    }
    _size = other._size;
}


AvlTree&AvlTree::operator=(const AvlTree&other) {
    if (this == &other) {
        return *this;
    }
    this->~AvlTree();
    if (other._root == nullptr) {
        _root = nullptr;
    } else {
        _root = new Node(*other._root);
    }
    _size = other._size;
    return *this;
}

AvlTree::AvlTree(AvlTree&&other) noexcept {
    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;
}

AvlTree&AvlTree::operator=(AvlTree&&other) noexcept {
    if (this == &other) {
        return *this;
    }
    this->~AvlTree();
    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;
    return *this;
}

AvlTree::~AvlTree() {
    if (_root == nullptr) {
        return;
    }
    std::vector<Node *> stack;
    stack.push_back(_root);
    while (!stack.empty()) {
        Node *node = stack.back();
        stack.pop_back();
        if (node->left != nullptr) {
            stack.push_back(node->left);
        }
        if (node->right != nullptr) {
            stack.push_back(node->right);
        }
        delete node;
    }
}

AvlTree::Iterator::Iterator(AvlTree::Node *node) {
    _node = node;
}

std::pair<Key, Value> &AvlTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value> &AvlTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value> *AvlTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *AvlTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

AvlTree::Iterator AvlTree::Iterator::operator++() {
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

AvlTree::Iterator AvlTree::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

AvlTree::Iterator AvlTree::Iterator::operator--() {
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

AvlTree::Iterator AvlTree::Iterator::operator--(int) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
}

bool AvlTree::Iterator::operator==(const AvlTree::Iterator &other) const {
    return _node == other._node;
}

bool AvlTree::Iterator::operator!=(const AvlTree::Iterator &other) const {
    return !(*this == other);
}


AvlTree::ConstIterator::ConstIterator(
        const AvlTree::Node *node) {
    _node = node;
}

const std::pair<Key, Value> &AvlTree::ConstIterator::operator*() const {
    return _node->keyValuePair;
}

const std::pair<Key, Value> *AvlTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

AvlTree::ConstIterator AvlTree::ConstIterator::operator++() {
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

AvlTree::ConstIterator AvlTree::ConstIterator::operator++(int) {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
}

AvlTree::ConstIterator AvlTree::ConstIterator::operator--() {
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

AvlTree::ConstIterator AvlTree::ConstIterator::operator--(int) {
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
}

bool AvlTree::ConstIterator::operator==(const AvlTree::ConstIterator &other) const {
    return _node == other._node;
}

bool AvlTree::ConstIterator::operator!=(const AvlTree::ConstIterator &other) const {
    return !(*this == other);
}

void AvlTree::insert(const Key &key, const Value &value) {
    _size++;
    if (_root == nullptr) {
        _root = new Node(key, value);
    } else {
        _root->insert(key, value);
    }
}


void AvlTree::erase(const Key &key) {
    Iterator it = this->find(key);
    Iterator root_it(_root);
    while (_root != nullptr && it->first == key) {
        _size--;
        if (it == root_it && (_root->left == nullptr || _root->right == nullptr)) {
            Node *to_del = _root;
            if (_root->left != nullptr) {
                _root = _root->left;
                _root->parent = nullptr;
            } else if (_root->right != nullptr) {
                _root = _root->right;
                _root->parent = nullptr;
            } else {
                _root = nullptr;
            }
            root_it = Iterator(_root);
            delete to_del;
        } else {
            _root->erase(key);
        }
        it = this->find(key);
    }
}

AvlTree::Iterator AvlTree::find(const Key &key) {
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
    if (node == nullptr) {
        return end();
    }
    return Iterator(node);
}

AvlTree::ConstIterator AvlTree::find(const Key &key) const {
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

std::pair<AvlTree::Iterator, AvlTree::Iterator> AvlTree::equalRange(const Key &key) {
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
    it_end++;
    return std::make_pair(it_start, it_end);
}

std::pair<AvlTree::ConstIterator, AvlTree::ConstIterator>
AvlTree::equalRange(const Key &key) const {
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

AvlTree::ConstIterator AvlTree::min(const Key &key) const {
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
    while (node->left != nullptr) {
        node = node->left;
    }

    return ConstIterator(node);
}

AvlTree::ConstIterator AvlTree::max(const Key &key) const {
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
    while (node->right != nullptr) {
        node = node->right;
    }

    return ConstIterator(node);
}

AvlTree::Iterator AvlTree::begin() {
    Node *node = _root;
    if (node == nullptr) {
        return Iterator(node);
    }
    while (node->left != nullptr) {
        node = node->left;
    }
    return Iterator(node);
}

AvlTree::ConstIterator AvlTree::cbegin() const {
    Node *node = _root;
    if (node == nullptr) {
        return ConstIterator(node);
    }
    while (node->left != nullptr) {
        node = node->left;
    }
    return ConstIterator(node);
}

AvlTree::Iterator AvlTree::end() {
    Node *node = _root;
    if (node == nullptr) {
        return Iterator(node);
    }
    while (node->right != nullptr) {
        node = node->right;
    }
    return Iterator(node);
}

AvlTree::ConstIterator AvlTree::cend() const {
    Node *node = _root;
    if (node == nullptr) {
        return ConstIterator(node);
    }
    while (node->right != nullptr) {
        node = node->right;
    }
    return ConstIterator(node);
}

size_t AvlTree::size() const {
    return _size;
}

void AvlTree::output_tree() {
    _root->output_node("", _root, false);
}
