#pragma once

template<typename T>
class SharedPtr
{
private:

    T* _ptr;
    size_t* _count;

    void free();
    void copyFrom(const SharedPtr<T>& other);
    void moveFrom(SharedPtr<T>&& other);

public:

    SharedPtr();
    SharedPtr(const SharedPtr<T>& other);
    SharedPtr(SharedPtr<T>&& other) noexcept;
    SharedPtr<T>& operator=(const SharedPtr<T>& other);
    SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept;
    ~SharedPtr();

    explicit SharedPtr(T* ptr);

    T* get() const;

    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;

    bool operator==(const SharedPtr<T>& other) const;

};

template<typename T>
void SharedPtr<T>::free()
{
    if (_ptr == nullptr && _count == nullptr)
    {
        return;
    }
    (*_count)--;
    if (*_count == 0)
    {
        delete _ptr;
        delete _count;
    }
}

template<typename T>
void SharedPtr<T>::copyFrom(const SharedPtr<T>& other)
{
    _ptr = other._ptr;
    _count = other._count;
    (*_count)++;
}

template<typename T>
void SharedPtr<T>::moveFrom(SharedPtr<T>&& other)
{
    _ptr = other._ptr;
    _count = other._count;
    other._ptr = nullptr;
    other._count = nullptr;
}

template<typename T>
SharedPtr<T>::SharedPtr()
{
    _ptr = nullptr;
    _count = nullptr;
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
{
    copyFrom(other);
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept
{
    moveFrom(std::move(other));
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
    free();
}

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr)
{
    _ptr = ptr;
    _count = new size_t(1);
}

template<typename T>
inline T* SharedPtr<T>::get() const
{
    return _ptr;
}

template<typename T>
T& SharedPtr<T>::operator*()
{
    return *_ptr;
}

template<typename T>
const T& SharedPtr<T>::operator*() const
{
    return *_ptr;
}

template<typename T>
T* SharedPtr<T>::operator->()
{
    return _ptr;
}

template<typename T>
const T* SharedPtr<T>::operator->() const
{
    return _ptr;
}

template<typename T>
inline bool SharedPtr<T>::operator==(const SharedPtr<T>& other) const
{
    return _ptr == other._ptr && _count == other._count;
}
