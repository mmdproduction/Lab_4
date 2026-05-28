#pragma once
#include "TestRunner.hpp"
#include "Sequence.hpp"
#include <atomic>
#include <mutex>

struct TestModel {
    GTestManager manager;

    std::atomic<float>   progress{0.f};
    std::atomic<bool>    running{false};
    std::atomic<int>     passed{0};
    std::atomic<int>     failed{0};

    std::mutex pendingMutex;
    ArraySequence<TestResultData> pendingResults;

    std::function<void(const TestResultData&)> onTestFinished;
};