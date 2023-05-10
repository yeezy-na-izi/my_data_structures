#include "AVL.h"
#include <vector>
#include <iostream>


AVLTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right)
        : keyValuePair(key, value), parent(parent), left(left), right(right),
          m_height(1 + std::max(left == nullptr ? 0 : left->m_height, right == nullptr ? 0 : right->m_height)) {}


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
    balance();
}

void AVLTree::Node::erase(const Key &key) {
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

void AVLTree::Node::small_right_rotation() {
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

void AVLTree::Node::big_left_rotation() {
    right->small_right_rotation();
    small_left_rotation();
}

void AVLTree::Node::big_right_rotation() {
    left->small_left_rotation();
    small_right_rotation();
}

void AVLTree::Node::update_height() {
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


void AVLTree::Node::balance() {
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

AVLTree::AVLTree(AVLTree &&other) noexcept {
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


void AVLTree::erase(const Key &key) {
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
    if (node == nullptr) {
        return end();
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

//int main() {
//    AVLTree tree;
//    // make vector int of 47 27 93 18 36 104 9 20 32 38 63 75 97 109 4 11 18 21 37 42 57 65 74 79 95 98 107 111 3 5 10 14 20 36 42 48 59 65 65 73 74 77 87 98 99 107 109 111 112 5 15 55 57 65 69 72 74 75 78 106 111 112 122 54
//    std::vector<int> v = {47, 27, 93, 18, 36, 104, 9, 20, 32, 38, 63, 75, 97, 109, 4, 11, 18, 21, 37, 42, 57, 65, 74,
//                          79, 95, 98, 107, 111, 3, 5, 10, 14, 20, 36, 42, 48, 59, 65, 65, 73, 74, 77, 87, 98, 99, 107,
//                          109, 111, 112, 5, 15, 55, 57, 65, 69, 72, 74, 75, 78, 106, 111, 112, 122, 54};
//
//    std::sort(v.begin(), v.end());
//    for (int i = 0; i < v.size(); i++) {
////        std::cout << i << " " << std::endl;
//        tree.insert(v[i], 1);
//    }
//    tree.output_tree();

//    tree.erase(89);

//    tree.output_tree();
//
//    tree.erase(92);
//
//    tree.output_tree();

//    tree.insert(1, 1);
//    tree.insert(2, 1);
//    tree.insert(3, 1);
//    tree.insert(1, 1);
//    tree.insert(1, 0);
//
//    tree.output_tree();
//
//    tree.erase(1);
//
//    tree.output_tree();
//}