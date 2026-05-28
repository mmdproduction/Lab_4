#include "Generator.hpp"
#include "Exception.hpp"
#include "Cardinal.hpp"
#include "Sequence.hpp"
#include<initializer_list>
#include<functional>



template <typename T>
class LazySequence{
    private:

    IGenerator<T>* generator_;
    ArraySequence<T> cache_;

    bool is_infinite_;

    bool has_generator() const { return generator_ != nullptr; }

    void materialize_up_to(size_t index) {
        while (cache_.getLength() <= index) {
            if (!has_generator())
                throw IndexOutOfRange(index, cache_.getLength());
            cache_.append(generator_->get_next());
        }
    }

    void ensure_generator() {
        if (generator_ == nullptr)
            generator_ = new FiniteGenerator<T>(cache_);
    }
    
    public:

    LazySequence() : is_infinite_(false){}

    LazySequence(std::initializer_list list): cache_(list), is_infinite_(false){}

    LazySequence(T* items, size_t count): cache_(items, count), is_infinite_(false){}

    LazySequence(const Sequence<T>& other) : cache_(other), is_infinite_(false){}

    LazySequence(std::function<T(const Sequence<T>&)> rule, const Sequence<T>& seed, size_t k): is_infinite_(true), cache_(seed){
        generator_ = new ReccurentGenerator<T>(rule, seed, k);
        }

    ~LazySequence() { delete generator_; }


    LazySequence(const LazySequence<T>& other)
        : is_infinite_(other.is_infinite_),
          generator_(other.generator_ ? other.generator_->clone() : nullptr) {
            cache_ = ArraySequence<T>(other.cache_);
    }

    LazySequence<T>& operator=(const LazySequence<T>& other) {
        if (this == &other)
            return *this;
        delete generator_;
        cache_ = ArraySequence<T>(other.cache_);
        generator_ = other.generator_ ? other.generator_->clone() : nullptr;
        is_infinite_ = other.is_infinite_;
        return *this;
    }




    T get(size_t index) {
        materialize_up_to(index);
        return cache_.get(index);
    }
 
    T get_first() {
        if (cache_.getLength() == 0 && !has_generator())
            throw EmptySequence();
        return get(0);
    }
 
    T get_last() {
        if (cache_.getLength() == 0 && !has_generator())
            throw EmptySequence();
        return cache_.getLast();
    }
 
    size_t get_materialized_count() const { return cache_.getLength(); }
    bool is_infinite() const { return is_infinite_; }
 
    void materialize(size_t count) { materialize_up_to(count - 1); }

    
};