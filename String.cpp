// Copyright (c) 2024 pieteraerens.eu
// All rights reserved.
// The file String.cpp is a part of PythonDataType.
// Last modified: 6/5/24 11:52 AM

#include "String.h"

namespace Max {

    String::String() : data(nullptr), size_(0) { }
    String::String(const char *str) {
        size_ = strlen(str);
        data = new char[size_ + 1];
        strcpy(data, str);
    }
    String::String(const String &other) {
        size_ = other.size_;
        data = new char[size_ + 1];
        strcpy(data, other.data);
    }


    String::~String() {
        delete[] data;
    }


    String &String::operator=(const String &other) {
        if (this != &other) {
            delete[] data;
            size_ = other.size_;
            data = new char[size_ + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    String String::operator+(const String &other) const {
        String result;
        result.size_ = size_ + other.size_;

        result.data = new char[result.size_ + 1];

        strcpy(result.data, data);
        strcat(result.data, other.data);

        return result;
    }

    String &String::operator+=(const String &other) {
        const size_t new_size = size_ + other.size_;

        char* new_data = new char[size_ + 1];

        std::memcpy(new_data, data, size_);
        std::memcpy(new_data + size_, other.data, other.size_ + 1);

        delete[] data;
        data = new_data;
        size_ = new_size;
        return *this;
    }


    bool String::operator==(const String & other) const {
        if (size_ != other.size_) {
            return false;
        }

        return strcmp(data, other.data) == 0;
    }

    const char &String::operator[](const size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        return data[index];
    }

    std::ostream& operator<<(std::ostream& os, const String& str) {
        os << "\"";
        os.write(str.data, static_cast<std::streamsize>(str.size_));
        os << "\"";
        return os;
    }


    size_t String::length() const {
        return size_;
    }

    String String::upper() const {
        String result(*this);

        for (int i = 0; i < result.size_; ++i) {
            result.data[i] = static_cast<char>(std::toupper(result.data[i]));
        }

        return result;
    }

    String String::lower() const {
        String result(*this);

        for (int i = 0; i < result.size_; ++i) {
            result.data[i] = static_cast<char>(std::tolower(result.data[i]));
        }

        return result;
    }

    int String::find(const String &substr, const int start) const {
        if (start >= size_) {
            throw std::out_of_range("Index out of range");
        }

        for (int i = start; i <= size_ - substr.size_; ++i) {
            bool found = true;

            for (int j = 0; j < substr.size_; ++j) {
                if (data[i + j] != substr[j]) {
                    found = false;
                    break;
                }
            }

            if (found) {
                return i;
            }
        }

        return -1;
    }

    String String::substr(const size_t start, const size_t length) const {
        if (start >= size_ || length >= size_) {
            throw std::out_of_range("Invalid substring parameters");
        }

        const size_t actualLength = std::min(length, size_ - start);

        String result;
        result.size_ = actualLength;
        result.data = new char[result.size_ + 1];
        strncpy(result.data, data + start, actualLength);
        result.data[actualLength] = '\0';

        return result;
    }

    String String::replace(const String &oldStr, const String &newStr) const {
        String result;
        int lastIndex = 0;

        while (true) {
            if (lastIndex >= size_) {
                break;
            }

            const int index = find(oldStr, lastIndex);

            if (index == -1) {
                break;
            }

            result += substr(lastIndex, index - lastIndex);
            result += newStr;
            lastIndex = index + static_cast<int>(oldStr.size_);
        }

        if (lastIndex < size_) {
            result += substr(lastIndex, size_ - 1);
        }
        return result;
    }

} // Max