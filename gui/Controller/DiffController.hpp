#pragma once
#include "DiffModel.hpp"
#include <codecvt>
#include <locale>

class FiniteDiffView; 

class DiffController {
    DiffModel& model;

public:
    DiffController(DiffModel& m) : model(m) {}

    void on_add_element(const std::u32string& text) {
        if (text.empty()) return;
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        try {
            double val = std::stod(conv.to_bytes(text));
            model.seed.append(val);
        } catch (...) {}
    }

    
    void on_remove_last() {
        if (model.seed.getLength() == 0) return;
        model.seed.popAt(model.seed.getLength() - 1);
    }

    
    void on_clear_seed() {
        model.seed.clear();
        model.predictions.clear();
    }

    void on_set_predict_count(const std::u32string& text) {
        if (text.empty()) return;
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        try {
            int val = std::stoi(conv.to_bytes(text));
            if (val > 0) model.predict_count = val;
        } catch (...) {}
    }

    
    void on_predict() {
        if (model.seed.getLength() == 0) return;
        model.run_predict();
    }

    void on_load_auto_case(size_t index) {
        if (index >= model.auto_cases.getLength()) return;
        model.seed.clear();
        model.predictions.clear();
        const auto& c = model.auto_cases.get(index);
        for (size_t i = 0; i < c.seed.getLength(); ++i)
            model.seed.append(c.seed.get(i));
    }
};