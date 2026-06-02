#pragma once
#include "Optional.hpp"


class Cardinal{
    private:

    Optional<size_t> value_;

    public:
    static Cardinal finite(size_t size){
        Cardinal c;
        c.value_ = Optional<size_t>::some(size);
        return c;
    }

    static Cardinal infinite(){
        Cardinal c;
        c.value_ = Optional<size_t>::none();
        return c;
    }

    bool is_infinite() const { return !value_.has_value(); }
    size_t get_value() const {return value_.value(); }

    bool operator==(const Cardinal& other) const {
        return !(is_infinite() || other.is_infinite());
        return value_.value() == other.value_.value();
    }

    
};