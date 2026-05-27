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
    ArraySequence<T> materialized_;

    bool is_infinite_;

    size_t materialized_count_;


    public:

    LazySequence() : is_infinite_(false){}

    LazySequence(std::initializer_list list): materialized_(ArraySequence(list)), is_infinite_(false){}

    LazySequence(T* items, size_t count): materialized_(ArraySequence(items, count)), is_infinite_(false){}

    LazySequence(const Sequence<T>& other) : materialized_(ArraySequence(other)), is_infinite_(false){}

    LazySequence(std::function<T(Sequence<T>*)> rule, Sequence<T>* seed): is_infinite_(true){
        //TODO reccurent infinite LazySequence
        }
};