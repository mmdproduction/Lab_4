#pragma once
#include"Stream.hpp"
#include "LazySequence.hpp"
#include "Difference.hpp"
#include <string>

struct AutoCase {
    std::string name;
    ArraySequence<double>* seed;
    AutoCase(): seed(new ArraySequence<double>()){}
};

struct DiffModel {
    ArraySequence<double> seed;
    ArraySequence<double> predictions;
    size_t predict_count = 5;

    
    ArraySequence<AutoCase> auto_cases;

    DiffModel() { init_auto_cases(); }

    void init_auto_cases() {
        
        AutoCase squares;
        squares.name = "Квадраты";
        for (int i = 1; i <= 5; ++i) squares.seed->append(i * i);
        auto_cases.append(squares);

        
        AutoCase cubes;
        cubes.name = "Кубы";
        for (int i = 1; i <= 5; ++i) cubes.seed->append(i * i * i);
        auto_cases.append(cubes);

        
        AutoCase arith;
        arith.name = "Арифм.";
        for (int i = 0; i < 5; ++i) arith.seed->append(2 + 3 * i);
        auto_cases.append(arith);
    }

    void run_predict() {
        predictions.clear();
        if (seed.getLength() == 0) return;

        SequenceReadOnlyStream<double> input(&seed);
        SequenceWriteOnlyStream<double> output(&predictions);
        
        predict<double>(input, output, predict_count);
};