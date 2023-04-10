#pragma once

#include "StackImplementation.h"
#include <list>


class ListStack : public IStackImplementation {
public:
    ListStack() = default;

    ListStack(const ValueType *values, const size_t size);

    void push(const ValueType &value) override;

    void pop() override;

    const ValueType &top() const override;

    bool isEmpty() const override;

    size_t size() const override;

private:
    std::list<ValueType> m_list;
};