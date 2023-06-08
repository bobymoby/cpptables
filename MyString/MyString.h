#pragma once

#include <iostream>
#include <string>

class MyString
{
private:
    char* _data;
    size_t _size;
    size_t _capacity;

    void free();
    void copyFrom(const MyString& other);
    void moveFrom(MyString&& other);

    void resize(size_t newCapacity);

    explicit MyString(size_t capacity);

public:

    static const size_t npos = -1;

    MyString(char c);
    MyString(size_t count, char c);
    MyString(const char* str);
    MyString(const std::string& str);

    MyString();
    MyString(const MyString& other);
    MyString(MyString&& other) noexcept;
    MyString& operator=(const MyString& other);
    MyString& operator=(MyString&& other) noexcept;
    ~MyString();

    void append(char c);
    void append(const MyString& other);

    void erase(size_t index, size_t count = 1);

    char& operator[](size_t index);
    const char& operator[](size_t index) const;
    char& front();
    const char& front() const;
    char& back();
    const char& back() const;

    MyString substr(size_t index, size_t count = npos) const;

    size_t find(char c, size_t startIndex = 0) const;

    const char* c_str() const;
    size_t size() const;

    bool empty() const;
    bool equals(const MyString& other) const;

    MyString operator+(const MyString& other) const;
    MyString& operator+=(const MyString& other);

    bool operator==(const MyString& other) const;
    bool operator!=(const MyString& other) const;

    bool getline(std::istream& is);

    friend std::istream& operator>>(std::istream& is, MyString& str);
};

std::ostream& operator<<(std::ostream& os, const MyString& str);