#pragma once
#include <gtest/gtest.h>
#include "Sequence.hpp"
#include <string>
#include <cstring>


class GTestManager;

struct TestResultData {
    char name[256];
    char message[512]; 
    bool passed;
    double time_ms;
    
    TestResultData() : passed(false), time_ms(0.0) {
        name[0] = '\0';
        message[0] = '\0';
    }
};

class GTestManager {
private:
    ArraySequence<TestResultData> results;

    int totalTests;
    int passedCount;
    int failedCount;
    double totalTimeMs;
    bool isRunning;

    class ResultPrinter : public ::testing::EmptyTestEventListener {
    public:
        
        ResultPrinter(); 
        void OnTestEnd(const ::testing::TestInfo& test_info) override;
    };

    ResultPrinter* printer;

public:
    GTestManager();

    std::function<void(const TestResultData&)> onTestFinished;
    bool run(const char* filter = nullptr);
    
    ArraySequence<TestResultData>& getResults() { return results; }
    
    int getTotal() const { return totalTests; }
    int getPassed() const { return passedCount; }
    int getFailed() const { return failedCount; }
    double getTime() const { return totalTimeMs; }
    bool getIsRunning() const { return isRunning; }

    float getProgress() const;
};