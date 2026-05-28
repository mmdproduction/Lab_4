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
    
    public:

    LazySequence() : is_infinite_(false){}

    LazySequence(std::initializer_list list): cache_(list), is_infinite_(false){}

    LazySequence(T* items, size_t count): cache_(items, count), is_infinite_(false){}

    LazySequence(const Sequence<T>& other) : cache_(other), is_infinite_(false){}

    LazySequence(std::function<T(const Sequence<T>&)> rule, const Sequence<T>& seed): is_infinite_(true){
        //TODO reccurent infinite LazySequence
        }
};