#pragma once
#include <cstddef>
#include <stdexcept>
#include<new>
#include"Exception.hpp"



template<typename T>
class Optional{
    private:
    alignas(T) std::byte storage_[sizeof(T)];
    bool has_value_;

    T* ptr(){ return reinterpret_cast<T*>(storage_); }
    const T* ptr() const { return reinterpret_cast<const T*>(storage_); }

     void reset() {
        if (has_value_) {
            ptr()->~T();
            has_value_ = false;
        }
    }

    public:

    static Optional some(const T& value) {
        Optional optional;
        new (optional.storage_) T(value);
        optional.has_value_ = true;
        return optional;
    }
 
    static Optional none() {
        return Optional();
    }
 
    Optional() : has_value_(false) {}
 
    Optional(const Optional& other) : has_value_(false) {
        if (other.has_value_) {
            new (storage_) T(*other.ptr());
            has_value_ = true;
        }
    }

    bool has_value() const {return has_value_; }

    const T& value() const {
        if (!has_value_)
            throw OptionalHasNotValue();
        return *ptr();
    }
 
    T& value() {
        if (!has_value_)
            throw OptionalHasNotValue();
        return *ptr();
    }

    Optional& operator=(const Optional& other) {
        if (this == &other)
            return *this;
        reset();
        if (other.has_value_) {
            new (storage_) T(*other.ptr());
            has_value_ = true;
        }
        return *this;
    }
 
    ~Optional() { reset(); }

};

