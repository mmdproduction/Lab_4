#pragma once
#include "Optional.hpp"


#pragma once
#include "Optional.hpp"

class Cardinal {
    private:
    Optional<size_t> value_;
    bool defined_ = true;

    public:

    static Cardinal finite(size_t size) {
        Cardinal c;
        c.value_   = Optional<size_t>::some(size);
        c.defined_ = true;
        return c;
    }

    static Cardinal infinite() {
        Cardinal c;
        c.value_   = Optional<size_t>::none();
        c.defined_ = true;
        return c;
    }

    static Cardinal undefined() {
        Cardinal c;
        c.value_   = Optional<size_t>::none();
        c.defined_ = false;
        return c;
    }

    bool is_infinite()  const { return defined_ && !value_.has_value(); }
    bool is_defined()   const { return defined_; }
    size_t get_value()  const { return value_.value(); }

    bool operator==(const Cardinal& other) const {
        if (!defined_ || !other.defined_) return false;
        if (is_infinite() || other.is_infinite()) return false;
        return value_.value() == other.value_.value();
    }

    bool operator!=(const Cardinal& other) const { return !(*this == other); }

    bool operator<(const Cardinal& other) const {
        if (!defined_ || !other.defined_) return false;
        if (is_infinite()) return false;
        if (other.is_infinite()) return true;
        return value_.value() < other.value_.value();
    }

    bool operator<=(const Cardinal& other) const { return *this == other || *this < other; }
    bool operator> (const Cardinal& other) const { return other < *this; }
    bool operator>=(const Cardinal& other) const { return other <= *this; }

    Cardinal operator+(const Cardinal& other) const {
        if (!defined_ || !other.defined_) return undefined();
        if (is_infinite() || other.is_infinite()) return infinite();
        return finite(value_.value() + other.value_.value());
    }

    Cardinal& operator+=(const Cardinal& other) {
        *this = *this + other;
        return *this;
    }

    Cardinal operator*(const Cardinal& other) const {
        if (!defined_ || !other.defined_) return undefined();
        if (!is_infinite() && value_.value() == 0) return finite(0);
        if (!other.is_infinite() && other.value_.value() == 0) return finite(0);
        if (is_infinite() || other.is_infinite()) return infinite();
        return finite(value_.value() * other.value_.value());
    }

    Cardinal& operator*=(const Cardinal& other) {
        *this = *this * other;
        return *this;
    }

    Cardinal operator-(const Cardinal& other) const {
        if (!defined_ || !other.defined_) return undefined();
        if (is_infinite() && !other.is_infinite()) return infinite();
        if (other.is_infinite()) return undefined();
        size_t a = value_.value(), b = other.value_.value();
        if (a < b) return undefined();
        return finite(a - b);
    }

    Cardinal& operator-=(const Cardinal& other) {
        *this = *this - other;
        return *this;
    }
};