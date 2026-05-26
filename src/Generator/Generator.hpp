#pragma once
#include <functional>
#include <stdexcept>
#include "Optional.hpp"

template<typename T>
class IGenerator{
    public:

    virtual T getNext() = 0;
    virtual bool hasNext() const = 0;
 
    Optional<T> tryGetNext() {
        if (!hasNext())
            return Optional<T>::none();
        return Optional<T>::some(getNext());
    }
};

template<typename T>
class FiniteGenerator: IGenerator<T>{
    private:
    Sequence<T>& seq_;
    size_t pos_;

    public:

    FiniteGenerator(Sequence<T>& seq) : seq_(seq), pos(0){
    }
    bool hasNext() const override {
        return pos_ < seq_.getLength();
    }

    T getNext() override{
        if(!hasNext()){
            throw InvalidNextValue();
        }
        seq.get(pos_++);
    }
};
