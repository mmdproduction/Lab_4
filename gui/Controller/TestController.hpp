#pragma once
#include "TestModel.hpp"
#include <thread>

class TestController {
    TestModel& model;
    std::thread runnerThread;

public:
    explicit TestController(TestModel& m) : model(m) {}

    void onRunAll() {

    if (model.running) return;


    model.pendingResults.clear();
    model.running = true;
    model.progress = 0.f;
    model.passed = 0;
    model.failed = 0;

    model.manager.onTestFinished = [this](const TestResultData& data) {
    std::lock_guard lock(model.pendingMutex);
    model.pendingResults.append(data);
    };

    runnerThread = std::thread([this] {
        model.manager.run();
        model.passed   = model.manager.getPassed();
        model.failed   = model.manager.getFailed();
        model.progress = 1.f;
        model.running  = false;
    });
    runnerThread.detach();
    }

    void onClear(TestView& view) {
        std::lock_guard lock(model.pendingMutex);
        model.pendingResults.clear();
        view.clearList();
    }


    void update(TestView& view) {
 
    if (model.running)
        model.progress = model.manager.getProgress();

    std::lock_guard lock(model.pendingMutex);
    for (const auto& data : model.pendingResults) {
        std::string text = std::string(data.passed ? "[OK]   " : "[FAIL] ") + data.name;
        view.addResult(text, data.passed);
    }
    model.pendingResults.clear();

    }
};