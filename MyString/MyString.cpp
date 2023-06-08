#include "MyString.h"
#include "../Utils.hpp"


void MyString::free()
{
    delete[] _data;
}

void MyString::copyFrom(const MyString& other)
{
    _data = new char[other._capacity];
    Utils::strcpy(_data, other._data);
    _size = other._size;
    _capacity = other._capacity;
}

void MyString::moveFrom(MyString&& other)
{
    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;

    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

void MyString::resize(size_t newCapacity)
{
    if (newCapacity <= _capacity)
    {
        return;
    }
    char* newData = new char[newCapacity];
    Utils::strcpy(newData, _data);
    delete[] _data;
    _data = newData;
    _capacity = newCapacity;
}

MyString::MyString(size_t capacity)
{
    _size = 0;
    _capacity = capacity;
    _data = new char[_capacity];
    _data[0] = '\0';
}

MyString::MyString(char c)
{
    _size = 1;
    _capacity = _size + 1;
    _data = new char[_capacity];
    _data[0] = c;
    _data[1] = '\0';
}

MyString::MyString(size_t count, char c)
{
    _size = count;
    _capacity = _size + 1;
    _data = new char[_capacity];
    for (size_t i = 0; i < _size; i++)
    {
        _data[i] = c;
    }
    _data[_size] = '\0';
}

MyString::MyString(const char* str)
{
    _size = Utils::strlen(str);
    _capacity = _size + 1;
    _data = new char[_capacity];
    Utils::strcpy(_data, str);
}

MyString::MyString(const std::string& str)
{
    _size = str.size();
    _capacity = _size + 1;
    _data = new char[_capacity];
    Utils::strcpy(_data, str.c_str());
}

MyString::MyString()
{
    _size = 0;
    _capacity = 8;
    _data = new char[_capacity];
    _data[0] = '\0';
}

MyString::MyString(const MyString& other)
{
    copyFrom(other);
}

MyString::MyString(MyString&& other) noexcept
{
    moveFrom(std::move(other));
}

MyString& MyString::operator=(const MyString& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

MyString& MyString::operator=(MyString&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

MyString::~MyString()
{
    free();
}

void MyString::append(char c)
{
    if (_size + 1 == _capacity)
    {
        resize(_capacity * 2);
    }
    _data[_size] = c;
    _data[_size + 1] = '\0';
    _size++;
}

void MyString::append(const MyString& other)
{
    size_t strSize = Utils::strlen(other._data);
    size_t newSize = _size + strSize;
    if (newSize + 1 > _capacity)
    {
        resize(newSize + 1);
    }
    Utils::strcat(_data, other._data);
    _size = newSize;
}

void MyString::erase(size_t index, size_t count)
{
    if (index >= _size)
    {
        return;
    }
    if (index + count > _size)
    {
        count = _size - index;
    }
    for (size_t i = index; i < _size - count; i++)
    {
        _data[i] = _data[i + count];
    }
    _data[_size - count] = '\0';
    _size -= count;
}

char& MyString::operator[](size_t index)
{
    return _data[index];
}

const char& MyString::operator[](size_t index) const
{
    return _data[index];
}

char& MyString::front()
{
    return _data[0];
}

const char& MyString::front() const
{
    return _data[0];
}

char& MyString::back()
{
    return _data[_size - 1];
}

const char& MyString::back() const
{
    return _data[_size - 1];
}

MyString MyString::substr(size_t index, size_t count) const
{
    if (index >= _size)
    {
        return MyString();
    }
    if (count > _size - index)
    {
        count = _size - index;
    }
    MyString result(count + 1);
    for (size_t i = 0; i < count; i++)
    {
        result._data[i] = _data[index + i];
    }
    result._data[count] = '\0';
    result._size = count;
    return result;
}

size_t MyString::find(char c, size_t startIndex) const
{
    for (size_t i = startIndex; i < _size; i++)
    {
        if (_data[i] == c)
        {
            return i;
        }
    }
    return npos;
}

const char* MyString::c_str() const
{
    return _data;
}

size_t MyString::size() const
{
    return _size;
}

bool MyString::empty() const
{
    return _size == 0;
}

bool MyString::equals(const MyString& other) const
{
    if (_size != other._size)
    {
        return false;
    }
    for (size_t i = 0; i < _size; i++)
    {
        if (_data[i] != other._data[i])
        {
            return false;
        }
    }
    return true;
}

MyString MyString::operator+(const MyString& other) const
{
    MyString result(*this);
    result.append(other);
    return result;
}

MyString& MyString::operator+=(const MyString& other)
{
    append(other);
    return *this;
}

bool MyString::operator==(const MyString& other) const
{
    return equals(other);
}

bool MyString::operator!=(const MyString& other) const
{
    return !equals(other);
}

bool MyString::getline(std::istream& is)
{
    if (is.eof())
    {
        return false;
    }
    char buffer[1024];
    is.getline(buffer, 1024);
    delete[] _data;
    _size = Utils::strlen(buffer);
    _capacity = _size + 1;
    _data = new char[_capacity];
    Utils::strcpy(_data, buffer);
    return true;
}

std::istream& operator>>(std::istream& is, MyString& str)
{
    char buffer[1024];
    is.getline(buffer, 1024);
    delete[] str._data;
    str._size = Utils::strlen(buffer);
    str._capacity = str._size + 1;
    str._data = new char[str._capacity];
    Utils::strcpy(str._data, buffer);
    return is;
}

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
    os << str.c_str();
    return os;
}
