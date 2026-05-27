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
 
    bool hasNext() const override { return source_->hasNext(); }
 
    U getNext() override {
        if (!hasNext())
            throw InvalidNextValue();
        return mapper_(source_->getNext());
    }
 
};


template <typename T>
class WhereGenerator : public IGenerator<T> {

    private:

    IGenerator<T>* source_;
    std::function<bool(T)> predicate_;
    mutable Optional<T> buffer_;
    mutable bool buffered_;

    public:

    WhereGenerator(IGenerator<T>* source, std::function<bool(T)> predicate)
        : source_(source), predicate_(predicate), buffered_(false) {}
 
    ~WhereGenerator() { delete source_; }
 
    bool hasNext() const override {
        if (buffered_)
            return true;
        while (source_->hasNext()) {
            T val = source_->getNext();
            if (predicate_(val)) {
                buffer_ = Optional<T>::some(val);
                buffered_ = true;
                return true;
            }
        }
        return false;
    }
 
    T getNext() override {
        if (!hasNext())
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
 
    bool hasNext() const override {
        return first_->hasNext() || second_->hasNext();
    }
 
    T getNext() override {
        if (!hasNext())
            throw InvalidNextValue();
        if (first_->hasNext())
            return first_->getNext();
        return second_->getNext();
    }
};

template <typename T>
class AppendGenerator : public IGenerator<T>{
    private:
    IGenerator<T>* source_;
    T item_;
    bool itemed_;

    public:

    AppendGenerator(IGenerator<T>* source, T item): source_(source), item_(item), itemed_(false) {}

    ~AppendGenerator() { delete source_; }

    bool hasNext() const override{
        return source_->hasNext() || !itemed_ ;
    }


    T getNext() override {
        if (!hasNext())
            throw InvalidNextValue();
        if (source_->hasNext())
            return source_->getNext();
        itemed_ = true;
        return item_;
    }


};

template <typename T>
class PrependGenerator : public IGenerator<T>{
    private:
    IGenerator<T>* source_;
    T item_;
    bool itemed_;

    public:

    PrependGenerator(IGenerator<T>* source, T item): source_(source), item_(item), itemed_(false) {}

    ~PrependGenerator() { delete source_; }

    bool hasNext() const override{
        return source_->hasNext() || !itemed_ ;
    }


    T getNext() override {
        if (!hasNext())
            throw InvalidNextValue();
        if (!itemed_){
            itemed_ = true;
            return item_;
        }
        return source_->getNext();
        
    }
};

template <typename T, typename U>
class ZipGenerator : public IGenerator<std::pair<T, U>>{
    private:

    IGenerator<T>* left_;
    IGenerator<U>* right_;

    public:

    ZipGenerator(IGenerator<T>* left, IGenerator<U>* right)
        : left_(left), right_(right) {}

    bool hasNext() const override{
        return left_->hasNext() && right_->hasNext();
    }

    std::pair<T, U> getNext() override{
        if(!hasNext){
            throw InvalidNextValue();
        }

         return { left_->getNext(), right_->getNext() };
    }
}