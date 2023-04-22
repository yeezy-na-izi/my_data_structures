#include "Stack.h"
#include "LStack.h"
#include "VStack.h"

#include <iostream>

Stack::Stack(StackContainer container) {
    _containerType = container;
    switch (_containerType) {
        case StackContainer::Vector:
            _pimpl = new VectorStack();
            break;
        case StackContainer::List:
            _pimpl = new ListStack();
            break;
    }
}

Stack::Stack(const ValueType *valueArray, const size_t arraySize, StackContainer container) {
    _containerType = container;
    switch (_containerType) {
        case StackContainer::Vector:
            _pimpl = new VectorStack(valueArray, arraySize);
            break;
        case StackContainer::List:
            _pimpl = new ListStack(valueArray, arraySize);
            break;
    }

}


Stack::Stack(const Stack &copyStack) {
    _containerType = copyStack._containerType;
    switch (_containerType) {
        case StackContainer::Vector: {
            _pimpl = new VectorStack(*dynamic_cast<VectorStack *>(copyStack._pimpl));
            break;
        }
        case StackContainer::List: {
            _pimpl = new ListStack(*dynamic_cast<ListStack *>(copyStack._pimpl));
            break;
        }
    }
}


Stack &Stack::operator=(const Stack &copyStack) {
    if (this == &copyStack) {
        return *this;
    }
    delete _pimpl;

    _containerType = copyStack._containerType;
    switch (_containerType) {
        case StackContainer::Vector: {
            _pimpl = new VectorStack(*dynamic_cast<VectorStack *>(copyStack._pimpl));
            break;
        }
        case StackContainer::List: {
            _pimpl = new ListStack(*dynamic_cast<ListStack *>(copyStack._pimpl));
            break;
        }
    }
    return *this;
}


Stack::Stack(Stack &&moveStack) noexcept {
    _containerType = moveStack._containerType;
    _pimpl = moveStack._pimpl;
    moveStack._pimpl = nullptr;
}


Stack &Stack::operator=(Stack &&moveStack) noexcept {
    if (this == &moveStack) {
        return *this;
    }
    delete _pimpl;

    _containerType = moveStack._containerType;
    _pimpl = moveStack._pimpl;
    moveStack._pimpl = nullptr;
    return *this;
}


Stack::~Stack() {
    delete _pimpl;
}

void Stack::push(const ValueType &value) {
    _pimpl->push(value);
}


void Stack::pop() {
    _pimpl->pop();
}


const ValueType &Stack::top() const {
    return _pimpl->top();
}


bool Stack::isEmpty() const {
    return _pimpl->isEmpty();
}


size_t Stack::size() const {
    return _pimpl->size();
}
