#pragma once

#include "StackImplementation.h"
#include <vector>

using ValueType = double;

class VectorStack : public IStackImplementation {
public:
    //конструктор
    VectorStack() = default;

    //конструктор с параметрами
    VectorStack(const ValueType *values, const size_t size);

    // добавление в хвост
    virtual void push(const ValueType &value) override;

    // удаление с хвоста
    virtual void pop() override;

    // посмотреть элемент в хвосте
    virtual const ValueType &top() const override;

    // проверка на пустоту
    virtual bool isEmpty() const override;

    // размер
    virtual size_t size() const override;

    ~VectorStack() = default;

private:
    std::vector<ValueType> vector_;
};
