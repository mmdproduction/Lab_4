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

template <typename T, typename U>
class MapGenerator : public IGenerator<U> {
    private:

    IGenerator<T>* source_;
    std::function<U(T)> mapper_;

    public:

    MapGenerator(IGenerator<T>* source, std::function<U(T)> mapper)
        : source_(source), mapper_(mapper) {}
 
    ~MapGenerator() { delete source_; }
 
    bool HasNext() const override { return source_->HasNext(); }
 
    U GetNext() override {
        if (!HasNext())
            throw InvalidNextValue();
        return mapper_(source_->GetNext());
    }
 
};


template <typename T>
class WhereGenerator : public IGenerator<T> {

    private:

    IGenerator<T>* source_;
    std::function<bool(T)> predicate_;
    Optional<T> buffer_;
    bool buffered_;

    public:

    WhereGenerator(IGenerator<T>* source, std::function<bool(T)> predicate)
        : source_(source), predicate_(predicate), buffered_(false) {}
 
    ~WhereGenerator() { delete source_; }
 
    bool HasNext() const override {
        if (buffered_)
            return true;
        while (source_->HasNext()) {
            T val = source_->GetNext();
            if (predicate_(val)) {
                buffer_ = Optional<T>::some(val);
                buffered_ = true;
                return true;
            }
        }
        return false;
    }
 
    T GetNext() override {
        if (!HasNext())
            throw InvalidNextValue();
        buffered_ = false;
        return buffer_.value();
    }
};

template <typename T>
class ConcatGenerator : public IGenerator<T> {
    private:

    IGenerator<T>* first_; 
    IGenerator<T>* second_;

    public:
    ConcatGenerator(IGenerator<T>* first, IGenerator<T>* second)
        : first_(first), second_(second) {}

    ~ConcatGenerator() { delete first_; delete second_; }
 
    bool HasNext() const override {
        return first_->HasNext() || second_->HasNext();
    }
 
    T GetNext() override {
        if (!HasNext())
            throw InvalidNextValue();
        if (first_->HasNext())
            return first_->GetNext();
        return second_->GetNext();
    }
};


