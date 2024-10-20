// Copyright (c) 2024 pieteraerens.eu
// All rights reserved.
// The file String.h is a part of PythonDataType.
// Last modified: 6/5/24 11:37 AM

#ifndef STRING_H
#define STRING_H
#include <ostream>

namespace Max {

class String {
public:
    String();
    explicit String(const char* str);
    String(const String& other);

    ~String();

    String& operator=(const String& other);
    String operator+(const String& other) const;
    String& operator+=(const String& other);
    bool operator==(const String & other) const;
    const char& operator[](size_t index) const;
    friend std::ostream& operator<<(std::ostream& os, const String& str);

    [[nodiscard]] size_t length() const;
    [[nodiscard]] String upper() const;
    [[nodiscard]] String lower() const;
    [[nodiscard]] int find(const String& substr, int start = 0) const;
    [[nodiscard]] String replace(const String& oldStr, const String& newStr) const;
    [[nodiscard]] String substr(size_t start, size_t length) const;

private:
    char* data;
    size_t size_;
};

} // Max

#endif //STRING_H
