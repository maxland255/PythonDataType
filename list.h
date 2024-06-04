// Copyright (c) 2024 pieteraerens.eu
// All rights reserved.
// The file list.h is a part of PythonDataType.
// Last modified: 6/4/24 10:04 PM

#ifndef LIST_H
#define LIST_H

#ifndef LIST_CPP_VERSION
#define LIST_CPP_VERSION 20
#include <optional>
#include <algorithm>
#endif

#if LIST_CPP_VERSION > __cplusplus
#error "This list implementation requires at least C++" LIST_CPP_VERSION
#endif

template<typename T> class list {
    T* data;
    size_t size_{};
    size_t capacity_{};

    // Resize table +

    void resize_table(const size_t increment_size = 0) {
        size_t new_capacity = std::max(capacity_ * 3 / 2, capacity_ + increment_size);

        if (new_capacity == 0) {
            new_capacity = 2;
        }

        T* new_data = new T[new_capacity];

        if (size_ != 0) {
            std::copy(data, data + size_, new_data);
        }

        delete[] data;

        data = new_data;
        capacity_ = new_capacity;
    }

    // Resize table -

    void resize_table_minder() {
        size_t new_capacity = capacity_ / 3 * 2;

        if (new_capacity <= size_) {
            return;
        }else if (new_capacity < 2) {
            new_capacity = 2;
        }

        T* new_data = new T[new_capacity];

        if (size_ != 0) {
            std::copy(data, data + size_, new_data);
        }

        delete[] data;

        data = new_data;
        capacity_ = new_capacity;
    }

    // Append element

    void append_element(const T new_element) {
        data[size_++] = new_element;
    }

    // Insert element

    void insert_element(const T new_element, const size_t index) {
        if constexpr (std::is_trivially_copyable_v<T>) {
            memmove(data + index + 1, data + index, (size_ - index) * sizeof(T));
        } else {
            std::move_backward(data + index, data + size_, data + size_ + 1);
        }

        data[index] = new_element;
        size_++;
    }

public:
    // Constructor
    list() : data(nullptr), size_(0), capacity_(0) {}
    list(std::initializer_list<T> init) : data(new T[init.size()]), size_(init.size()), capacity_(init.size()) {
        std::copy(init.begin(), init.end(), data);
    }

    // Deconstruct

    ~list() {
        delete[] data;
    }

    // Getter

    [[nodiscard]] const size_t& size() const {
        return size_;
    }

    [[nodiscard]] const size_t& capacity() const {
        return capacity_;
    }

    [[nodiscard]] bool isEmpty() const {
        return size_ == 0;
    }

    [[nodiscard]] bool isNotEmpty() const {
        return size_ > 0;
    }

    // Clear

    void clear() {
        if (data == nullptr) {
            size_ = 0;
            capacity_ = 0;
            return;
        }

        delete[] data;
        data = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    // Append

    void append(const T element) {
        if (size_ + 1 < capacity_) {
            append_element(element);
            return;
        }

        resize_table();
        append_element(element);
    }

    // Insert

    void insert(const T element, const size_t index) {
        if (index > size_) {
            throw std::out_of_range("Index out of range");
        }

        if (index == size_) {
            append(element);
        }

        if (size_ + 1 < capacity_) {
            insert_element(element, index);
            return;
        }

        resize_table();
        insert_element(element, index);
    }

    // Extend

    void extend(const list<T>& other) {
        if (this == &other) {
            return;
        }

        if (size_ + other.size() > capacity_) {
            resize_table(other.size());
        }

        std::copy(other.data, other.data + other.size(), data + size_);

        size_ += other.size();
    }

    // Remove item

    void remove(const T element) {
        if (size_ == 0) {
            return;
        }

        auto it = std::find(begin(), end(), element);

        if (it == end()) {
            return;
        }

        const size_t index = std::distance(begin(), it);

        pop(index);
    }

    // Count

    size_t count(const T element) {
        size_t count = 0;

        for (const T& e : *this) {
            if (e == element) {
                count++;
            }
        }

        return count;
    }

    // Reverse

    void reverse() {
        std::reverse(begin(), end());
    }

    // Pop

    std::optional<T> pop () {
        if (size_ == 0) {
            return std::nullopt;
        }

        resize_table_minder();

        size_--;
        return data[size_];
    }

    T pop(const size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        resize_table_minder();

        T element = data[index];

        std::move(data + index + 1, data + size_, data + index);

        size_--;
        return element;
    }

    // At

    const T& at(const size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        return data[index];
    }

    // Operator surcharge

    T& operator[](const size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        return data[index];
    }

    const T& operator[](const size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        return data[index];
    }

    friend std::ostream& operator<<(std::ostream& os, list<T>& elements) {
        os << "{";
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            os << *it;
            if (std::next(it) != elements.end()) {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }

    // Iterator

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit iterator(pointer ptr) : ptr_(ptr) {}

        reference operator*() const { return *ptr_; }
        pointer operator->() { return  ptr_; }

        iterator& operator++() { ++ptr_; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        iterator& operator--() { --ptr_; return *this; }
        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

        iterator operator+(difference_type n) const { return iterator(ptr_ + n); }
        iterator& operator+=(difference_type n) { ptr_ += n; return *this; }

        iterator operator-(difference_type n) const { return iterator(ptr_ - n); }
        iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }

        difference_type operator-(const iterator& other) const { return ptr_ - other.ptr_; }

        reference operator[](difference_type n) const { return ptr_[n]; }

        bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
        bool operator<(const iterator& other) const { return ptr_ < other.ptr_; }
        bool operator<=(const iterator& other) const { return ptr_ <= other.ptr_; }
        bool operator>(const iterator& other) const { return ptr_ > other.ptr_; }
        bool operator>=(const iterator& other) const { return ptr_ >= other.ptr_; }

    private:
        pointer ptr_;
    };

    class const_iterator : public iterator {
        using  iterator::iterator;

        const T& operator*() const { return *iterator::ptr_; }

        const T* operator->() const { return iterator::ptr_; }
    };

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + size_); }
    const_iterator cbegin() const { return const_iterator(data); }
    const_iterator cend() const { return const_iterator(data + size_); }
};



#endif //LIST_H
