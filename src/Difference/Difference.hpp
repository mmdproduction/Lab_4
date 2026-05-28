#pragma once
#include "LazySequence.hpp"
#include "Stream.hpp"


LazySequence<double> predict(ReadOnlyStream<double>& input,
                             WriteOnlyStream<double>& output,
                             size_t predict_count) {
    ArraySequence<double> seed;
    while (!input.is_end_of_stream()) {
        double value;
        input >> value;
        seed.append(value);
    }
 
    if (seed.getLength() == 0)
        throw EmptyStream();
 
    auto* gen = new FiniteDifferenceGenerator<double>(seed);
    LazySequence<double> series(seed, gen);

    size_t offset = seed.getLength();
    for (size_t i = 0; i < predict_count; ++i)
        output << series.get(offset + i);
 
    return series;
}

