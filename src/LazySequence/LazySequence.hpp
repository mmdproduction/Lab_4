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

    LazySequence() : generator_(nullptr), is_infinite_(false){}

    LazySequence(std::initializer_list<T> list): generator_(nullptr), cache_(list), is_infinite_(false){}

    LazySequence(T* items, size_t count): generator_(nullptr),  cache_(items, count), is_infinite_(false){}

    LazySequence(Sequence<T>& seq) : generator_(nullptr), is_infinite_(false) {
        for (size_t i = 0; i < seq.getLength(); ++i)
            cache_.append(seq.get(i));
    }

    LazySequence(std::function<T(const Sequence<T>&)> rule, const Sequence<T>& seed, size_t k): is_infinite_(true) {
        for (const auto& elem : seed)
            cache_.append(elem);
        generator_ = new ReccurentGenerator<T>(rule, seed, k);
        }

    ~LazySequence() { delete generator_; }


    LazySequence(const LazySequence<T>& other)
        : is_infinite_(other.is_infinite_),
          generator_(other.generator_ ? other.generator_->clone() : nullptr) {
        for (const auto& elem : other.cache_)
            cache_.append(elem);
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

    Cardinal get_length() const {
    if (is_infinite_)
        return Cardinal::infinite();
    return Cardinal::finite(cache_.getLength());
    
    }
 
    size_t get_materialized_count() const { return cache_.getLength(); }
    bool is_infinite() const { return is_infinite_; }
 
    void materialize(size_t count) { materialize_up_to(count - 1); }

    LazySequence<T>& append(const T& item) {
        ensure_generator();
        generator_ = new AppendGenerator<T>(generator_, item);
        is_infinite_ = false;
        return *this;
    }
 
    LazySequence<T>& prepend(const T& item) {
        ensure_generator();
        generator_ = new PrependGenerator<T>(item, generator_);
        return *this;
    }
 
    LazySequence<T>& concat(LazySequence<T>& other) {
        ensure_generator();
        IGenerator<T>* other_gen = other.has_generator()
            ? other.generator_->clone()
            : new FiniteGenerator<T>(other.cache_);
        generator_ = new ConcatGenerator<T>(generator_, other_gen);
        is_infinite_ = is_infinite_ || other.is_infinite_;
        return *this;
    }
 
    LazySequence<T>& where(std::function<bool(T)> predicate) {
        ensure_generator();
        generator_ = new WhereGenerator<T>(generator_, predicate);
        return *this;
    }


    template<typename U>
    LazySequence<U> map(std::function<U(T)> mapper) {
        ensure_generator();
        LazySequence<U> result;
        result.generator_ = new MapGenerator<T, U>(generator_->clone(), mapper);
        result.is_infinite_ = is_infinite_;
        return result;
    }
 
    template<typename U>
    LazySequence<std::pair<T, U>> zip(LazySequence<U>& other) {
        ensure_generator();
        IGenerator<T>* left  = generator_->clone();
        IGenerator<U>* right = other.has_generator()
            ? other.generator_->clone()
            : new FiniteGenerator<U>(other.cache_);
        LazySequence<std::pair<T, U>> result;
        result.generator_ = new ZipGenerator<T, U>(left, right);
        result.is_infinite_ = is_infinite_ && other.is_infinite_;
        return result;
    }


};