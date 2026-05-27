#include "Optional.hpp"


class Cardinal{
    private:

    Optional<size_t> value_;

    public:
    static Cardinal finite(size_t size){
        Cardinal c;
        c.value_ = Optional::some(size);
        return c;
    }

    static Cardinal infinite(){
        Cardinal c;
        c.value_ = Optional::none();
        return c;
    }

    bool is_infinite(){ return !value_.has_value(); }
    size_t get_value() {return value_.value(); }

    bool operator==(const Cardinal& other) const {
        if (is_infinite() && other.is_infinite()) return true;
        if (is_infinite() || other.is_infinite()) return false;
        return length.value() == other.length.value();
    }

}