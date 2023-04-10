#include "LStack.h"


ListStack::ListStack(const ValueType *values, const size_t size) {
    m_list.insert(m_list.begin(), values, values + size);
}

void ListStack::push(const ValueType &value) {
    m_list.push_back(value);
}

void ListStack::pop() {
    m_list.pop_back();
}

size_t ListStack::size() const {
    return m_list.size();
}

const ValueType &ListStack::top() const {
    return m_list.back();
}

bool ListStack::isEmpty() const {
    return m_list.empty();
}