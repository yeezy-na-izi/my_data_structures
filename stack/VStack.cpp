#include "VStack.h"

VectorStack::VectorStack(const ValueType *values, const size_t size) {
    vector_.insert(vector_.begin(), values, values + size);
}

void VectorStack::push(const ValueType &value) {
    vector_.push_back(value);
}

void VectorStack::pop() {
    vector_.pop_back();
}

size_t VectorStack::size() const {
    return vector_.size();
}


const ValueType &VectorStack::top() const {
    return vector_[vector_.size() - 1];
}

bool VectorStack::isEmpty() const {
    return vector_.size() == 0;
}

