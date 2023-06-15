#pragma once

template<typename T>
class MyIterator //used for range-based for loop with MyVector
{
private:
    T* _ptr;

public:
    MyIterator(T* ptr);

    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;

    MyIterator<T>& operator++();
    MyIterator<T> operator++(int);
    MyIterator<T>& operator--();
    MyIterator<T> operator--(int);

    bool operator==(const MyIterator<T>& other) const;
    bool operator!=(const MyIterator<T>& other) const;
};

template<typename T>
MyIterator<T>::MyIterator(T* ptr)
{
    _ptr = ptr;
}

template<typename T>
T& MyIterator<T>::operator*()
{
    return *_ptr;
}

template<typename T>
const T& MyIterator<T>::operator*() const
{
    return *_ptr;
}

template<typename T>
T* MyIterator<T>::operator->()
{
    return _ptr;
}

template<typename T>
const T* MyIterator<T>::operator->() const
{
    return _ptr;
}

template<typename T>
MyIterator<T>& MyIterator<T>::operator++()
{
    _ptr++;
    return *this;
}

template<typename T>
MyIterator<T> MyIterator<T>::operator++(int)
{
    MyIterator temp(*this);
    _ptr++;
    return temp;
}

template<typename T>
MyIterator<T>& MyIterator<T>::operator--()
{
    _ptr--;
    return *this;
}

template<typename T>
MyIterator<T> MyIterator<T>::operator--(int)
{
    MyIterator temp(*this);
    _ptr--;
    return temp;
}

template<typename T>
bool MyIterator<T>::operator==(const MyIterator& other) const
{
    return _ptr == other._ptr;
}

template<typename T>
bool MyIterator<T>::operator!=(const MyIterator& other) const
{
    return _ptr != other._ptr;
}
