#pragma once

template <typename T>
class UnqPtr {
private:
    T* ptr;

public:
    UnqPtr(T* p = nullptr) : ptr(p) {}

    ~UnqPtr() {
        delete ptr;
    }

    UnqPtr(const UnqPtr& other) = delete;
    UnqPtr& operator=(const UnqPtr& other) = delete;

    UnqPtr(UnqPtr&& other) : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UnqPtr& operator=(UnqPtr&& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    template <typename U>
    UnqPtr(UnqPtr<U>&& other) : ptr(static_cast<T*>(other.Release())) {}

    template <typename U>
    UnqPtr& operator=(UnqPtr<U>&& other) {
        if (reinterpret_cast<void*>(this) != reinterpret_cast<void*>(&other)) {
            delete ptr;
            ptr = static_cast<T*>(other.Release());
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    T* Get() const { return ptr; }

    T* Release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void Reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }

    operator bool() const { return ptr != nullptr; }
};