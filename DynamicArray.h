#pragma once

template <typename T>
class DynamicArray {
private:
    T* data;
    size_t size_;
    size_t capacity_;

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = static_cast<T&&>(data[i]);
        }
        delete[] data;
        data = new_data;
        capacity_ = new_capacity;
    }

public:
    DynamicArray() : data(nullptr), size_(0), capacity_(0) {}

    DynamicArray(size_t capacity) : size_(0), capacity_(capacity) {
        data = new T[capacity_];
    }

    ~DynamicArray() {
        delete[] data;
    }

    DynamicArray(const DynamicArray& other) : size_(other.size_), capacity_(other.capacity_) {
        data = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    DynamicArray(DynamicArray&& other) : data(other.data), size_(other.size_), capacity_(other.capacity_) {
        other.data = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    DynamicArray& operator=(DynamicArray&& other) {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    void PushBack(const T& value) {
        if (size_ == capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data[size_++] = value;
    }

    void PushBack(T&& value) {
        if (size_ == capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data[size_++] = static_cast<T&&>(value);
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t Size() const { return size_; }
    size_t Capacity() const { return capacity_; }
};