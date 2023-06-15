#pragma once

#include <utility> // std::move
#include "MyIterator.hpp"

template <typename T>
class MyVector
{
private:

    T* _data;
    size_t _size;
    size_t _capacity;

    void free();
    void copyFrom(const MyVector<T>& other);
    void moveFrom(MyVector<T>&& other);

public:

    MyVector();
    MyVector(const MyVector<T>& other);
    MyVector(MyVector<T>&& other) noexcept;
    MyVector<T>& operator=(const MyVector<T>& other);
    MyVector<T>& operator=(MyVector<T>&& other) noexcept;
    ~MyVector();

    void push_back(const T& value);
    void pop_back();

    void fill(size_t size, const T& value);
    void resize(size_t newCapacity);
    void clear();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    size_t size() const;
    bool empty() const;

    MyIterator<T> begin();
    const MyIterator<T> begin() const;
    MyIterator<T> end();
    const MyIterator<T> end() const;
    const MyIterator<T> cbegin() const;
    const MyIterator<T> cend() const;

};

template<typename T>
void MyVector<T>::free()
{
    delete[] _data;
    _data = nullptr;
}

template<typename T>
void MyVector<T>::copyFrom(const MyVector<T>& other)
{
    _size = other._size;
    _capacity = other._capacity;
    _data = new T[_capacity];
    for (size_t i = 0; i < _size; i++)
    {
        _data[i] = other._data[i];
    }
}

template<typename T>
void MyVector<T>::moveFrom(MyVector<T>&& other)
{
    _size = other._size;
    _capacity = other._capacity;
    _data = other._data;
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

template<typename T>
void MyVector<T>::fill(size_t size, const T& value)
{
    if (size > _capacity)
    {
        resize(size);
    }
    for (size_t i = 0; i < size; i++)
    {
        _data[i] = value;
    }
    _size = size;
}

template<typename T>
void MyVector<T>::resize(size_t newCapacity)
{
    if (newCapacity <= _capacity)
    {
        return;
    }
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < _size; i++)
    {
        newData[i] = _data[i];
    }
    delete[] _data;
    _data = newData;
    _capacity = newCapacity;
}

template<typename T>
void MyVector<T>::clear()
{
    _size = 0;
}

template<typename T>
MyVector<T>::MyVector()
{
    _size = 0;
    _capacity = 8;
    _data = new T[_capacity];
}

template<typename T>
MyVector<T>::MyVector(const MyVector<T>& other)
{
    copyFrom(other);
}

template<typename T>
MyVector<T>::MyVector(MyVector<T>&& other) noexcept
{
    moveFrom(std::move(other));
}

template<typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

template<typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

template<typename T>
MyVector<T>::~MyVector()
{
    free();
}

template<typename T>
void MyVector<T>::push_back(const T& value)
{
    if (_size == _capacity)
    {
        resize(_capacity * 2);
    }
    _data[_size++] = value;
}

template<typename T>
void MyVector<T>::pop_back()
{
    if (_size > 0)
    {
        _size--;
    }
}

template<typename T>
T& MyVector<T>::operator[](size_t index)
{
    return _data[index];
}

template<typename T>
const T& MyVector<T>::operator[](size_t index) const
{
    return _data[index];
}

template<typename T>
T& MyVector<T>::front()
{
    return _data[0];
}

template<typename T>
const T& MyVector<T>::front() const
{
    return _data[0];
}

template<typename T>
T& MyVector<T>::back()
{
    if (_size > 0)
    {
        return _data[_size - 1];
    }
    return _data[0];
}

template<typename T>
const T& MyVector<T>::back() const
{
    if (_size > 0)
    {
        return _data[_size - 1];
    }
    return _data[0];
}

template<typename T>
size_t MyVector<T>::size() const
{
    return _size;
}

template<typename T>
bool MyVector<T>::empty() const
{
    return _size == 0;
}

template<typename T>
MyIterator<T> MyVector<T>::begin()
{
    return MyIterator<T>(_data);
}

template<typename T>
const MyIterator<T> MyVector<T>::begin() const
{
    return MyIterator<T>(_data);
}

template<typename T>
MyIterator<T> MyVector<T>::end()
{
    return MyIterator<T>(_data + _size);
}

template<typename T>
const MyIterator<T> MyVector<T>::end() const
{
    return MyIterator<T>(_data + _size);
}

template<typename T>
const MyIterator<T> MyVector<T>::cbegin() const
{
    return MyIterator<T>(_data);
}

template<typename T>
const MyIterator<T> MyVector<T>::cend() const
{
    return MyIterator<T>(_data + _size);
}