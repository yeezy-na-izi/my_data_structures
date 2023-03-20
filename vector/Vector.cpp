#include "Vector.h"
#include <iostream>


size_t Vector::size() const {
    return _size;
}

size_t Vector::capacity() const {
    return _capacity;
}

double Vector::loadFactor() const {
    return (double) _size / (double) _capacity;
}

Vector::Vector(const Value *rawArray, const size_t size, float coefficient)
        : _size(size), _capacity(size), _multiplicativeCoef(coefficient) {
    _data = new Value[_capacity];
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = rawArray[i];
    }
}

Vector::Vector(const Vector &other)
        : _size(other.size()),
          _multiplicativeCoef(other._multiplicativeCoef) {
    _capacity = std::min(other.capacity(), _size * (size_t) _multiplicativeCoef);
    _data = new Value[_capacity];
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = other._data[i];
    }
}


Vector &Vector::operator=(const Vector &other) {
    if (this == &other) {
        return *this;
    }
    _size = other.size();
    _multiplicativeCoef = other._multiplicativeCoef;
    _capacity = std::min(other.capacity(), _size * (size_t) _multiplicativeCoef);
    delete[] _data;

    _data = new Value[_capacity];
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = other._data[i];
    }

    return *this;
}


Vector::Vector(Vector &&other) noexcept {
    _data = other._data;
    _multiplicativeCoef = other._multiplicativeCoef;
    _capacity = std::min(other._capacity, other._size * (size_t) _multiplicativeCoef);
    _size = other._size;
    other._data = nullptr;
    other._capacity = 0;
    other._size = 0;
}

Vector &Vector::operator=(Vector &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    _size = other.size();
    _multiplicativeCoef = other._multiplicativeCoef;
    _capacity = std::min(other.capacity(), _size * (size_t) _multiplicativeCoef);
    delete[] _data;

    _data = other._data;
    other._data = nullptr;

    return *this;
}

Vector::~Vector() {
    _capacity = 0;
    _size = 0;
    delete[] _data;
}

void Vector::reserve(size_t capacity) {
    if (capacity <= _capacity) {
        return;
    }
    auto *newData = new Value[capacity];

    if (_data != nullptr) {
        for (size_t i = 0; i < _size; ++i) {
            newData[i] = _data[i];
        }
        delete[] _data;
    }

    _data = newData;
    _capacity = capacity;
}

void Vector::pushBack(const Value &value) {
    if (_size == _capacity) {
        reserve(std::max(_capacity * (size_t) _multiplicativeCoef, (size_t) _multiplicativeCoef));
    }

    _data[_size] = value;
    ++_size;
}

void Vector::pushFront(const Value &value) {
    if (_size == _capacity) {
        reserve(std::max(_capacity * (size_t) _multiplicativeCoef, (size_t) _multiplicativeCoef));
    }

    for (size_t i = _size; i > 0; --i) {
        _data[i] = _data[i - 1];
    }
    _data[0] = value;
    ++_size;
}


void Vector::popBack() {
    if (_size == 0) {
        throw std::out_of_range("Vector is empty");
    }
    --_size;
}

void Vector::popFront() {
    if (_size == 0) {
        throw std::out_of_range("Vector is empty");
    }
    for (size_t i = 0; i < _size - 1; ++i) {
        _data[i] = _data[i + 1];
    }
    --_size;
}


Value &Vector::operator[](size_t index) {
    return _data[index];
}

const Value &Vector::operator[](size_t index) const {
    return _data[index];
}

long long Vector::find(const Value &value) const {
    for (size_t i = 0; i < _size; ++i) {
        if (_data[i] == value) {
            return (long long) i;
        }
    }
    return -1;
}

void Vector::shrinkToFit() {
    if (_size == _capacity) {
        return;
    }

    if (_data == nullptr) {
        return;
    }

    auto *newData = new Value[_size];
    for (size_t i = 0; i < _size; ++i) {
        newData[i] = _data[i];
    }
    delete _data;

    _data = newData;
    _capacity = _size;
}


void Vector::insert(const Value &value, size_t pos) {
    if (_size == _capacity) {
        reserve(_capacity * (size_t) _multiplicativeCoef);
    }
    for (size_t i = _size; i > pos; --i) {
        _data[i] = _data[i - 1];
    }
    _data[pos] = value;
    ++_size;
}

void Vector::insert(const Vector &vector, size_t pos) {
    this->insert(vector._data, vector._size, pos);
}

void Vector::insert(const Value *values, size_t size, size_t pos) {
    for (size_t i = 0; i < size; ++i) {
        this->insert(values[i], pos + i);
    }
}

void Vector::erase(size_t pos, size_t count) {
    if (pos + count > _size) {
        count = _size - pos;
    }

    for (size_t i = pos; i < _size - count; ++i) {
        _data[i] = _data[i + count];
    }
    _size -= count;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos) {
    this->erase(beginPos, endPos - beginPos);
}

Vector::Iterator::Iterator(Value *ptr) : _ptr(ptr) {}

Value &Vector::Iterator::operator*() {
    return *_ptr;
}

const Value &Vector::Iterator::operator*() const {
    return *_ptr;
}

Value *Vector::Iterator::operator->() {
    return _ptr;
}

const Value *Vector::Iterator::operator->() const {
    return _ptr;
}

Vector::Iterator Vector::Iterator::operator++() {
    ++_ptr;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++_ptr;
    return tmp;
}

bool Vector::Iterator::operator==(const Iterator &other) const {
    return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Iterator &other) const {
    return _ptr != other._ptr;
}

Vector::Iterator Vector::begin() {
    return Iterator(_data);
}

Vector::Iterator Vector::end() {
    return Iterator(_data + _size);
}