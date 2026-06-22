#pragma once
#include "LazySequence.hpp"
#include "Stream.hpp"


template<typename T>
LazySequence<T> predict(IReadOnlyStream<T>& input,
                        IWriteOnlyStream<T>& output,
                        size_t predict_count) {
    ArraySequence<T> seed;
    while (!input.is_end_of_stream()) {
        T value;
        input >> value;
        seed.append(value);
    }
 
    if (seed.getLength() == 0)
        throw InvalidArgument();
 
    auto* gen = new FiniteDifferenceGenerator<T>(seed);
    LazySequence<T> series(seed, gen);

    size_t offset = seed.getLength();
    for (size_t i = 0; i < predict_count; ++i)
        output.write(series.get(offset + i));
 
    return series;
}
