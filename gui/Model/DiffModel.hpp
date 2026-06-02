#pragma once
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

        ArraySequence<double> seed_copy;
        for (size_t i = 0; i < seed.getLength(); ++i)
            seed_copy.append(seed.get(i));

        ReadOnlyStream<double> input(&seed_copy);
        WriteOnlyStream<double> output([](const double& x) { return std::to_string(x); });
        predict(input, output, predict_count);

        const std::string& buf = output.get_buffer();
        std::string token;
        for (char c : buf) {
            if (c == '\n') {
                if (!token.empty()) {
                    predictions.append(std::stod(token));
                    token.clear();
                }
            } else {
                token += c;
            }
        }
    }
};