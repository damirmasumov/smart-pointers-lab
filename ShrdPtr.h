#pragma once
#include "UnqPtr.h"

template <typename T>
class ShrdPtr {
    template <typename U>
    friend class ShrdPtr;

private:
    T* ptr;
    size_t* referenceCount;

    void release() {
        if (referenceCount) {
            (*referenceCount)--;
            if (*referenceCount == 0) {
                delete ptr;
                delete referenceCount;
            }
        }
        ptr = nullptr;
        referenceCount = nullptr;
    }

public:
    ShrdPtr() : ptr(nullptr), referenceCount(nullptr) {}

    ShrdPtr(UnqPtr<T>&& uptr) {
        ptr = uptr.Release();
        if (ptr) {
            referenceCount = new size_t(1);
        } else {
            referenceCount = nullptr;
        }
    }

    ~ShrdPtr() {
        release();
    }

    ShrdPtr(const ShrdPtr& other) : ptr(other.ptr), referenceCount(other.referenceCount) {
        if (referenceCount) {
            (*referenceCount)++;
        }
    }

    ShrdPtr& operator=(const ShrdPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            if (referenceCount) {
                (*referenceCount)++;
            }
        }
        return *this;
    }

    ShrdPtr(ShrdPtr&& other) : ptr(other.ptr), referenceCount(other.referenceCount) {
        other.ptr = nullptr;
        other.referenceCount = nullptr;
    }

    ShrdPtr& operator=(ShrdPtr&& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            other.ptr = nullptr;
            other.referenceCount = nullptr;
        }
        return *this;
    }

    template <typename U>
    ShrdPtr(const ShrdPtr<U>& other) : ptr(static_cast<T*>(other.ptr)), referenceCount(other.referenceCount) {
        if (referenceCount) {
            (*referenceCount)++;
        }
    }

    template <typename U>
    ShrdPtr& operator=(const ShrdPtr<U>& other) {
        if (reinterpret_cast<void*>(this) != reinterpret_cast<const void*>(&other)) {
            release();
            ptr = static_cast<T*>(other.ptr);
            referenceCount = other.referenceCount;
            if (referenceCount) {
                (*referenceCount)++;
            }
        }
        return *this;
    }

    template <typename U>
    ShrdPtr(ShrdPtr<U>&& other) : ptr(static_cast<T*>(other.ptr)), referenceCount(other.referenceCount) {
        other.ptr = nullptr;
        other.referenceCount = nullptr;
    }

    template <typename U>
    ShrdPtr& operator=(ShrdPtr<U>&& other) {
        if (reinterpret_cast<void*>(this) != reinterpret_cast<void*>(&other)) {
            release();
            ptr = static_cast<T*>(other.ptr);
            referenceCount = other.referenceCount;
            other.ptr = nullptr;
            other.referenceCount = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    T* Get() const { return ptr; }
    size_t UseCount() const { return referenceCount ? *referenceCount : 0; }

    operator bool() const { return ptr != nullptr; }
};