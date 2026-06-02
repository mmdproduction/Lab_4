#include "TestRunner.hpp"


static GTestManager* g_ActiveManager = nullptr;

GTestManager::ResultPrinter::ResultPrinter() {}

void GTestManager::ResultPrinter::OnTestEnd(const ::testing::TestInfo& test_info) {

    if (!g_ActiveManager) return;

    TestResultData data;
    
    std::string fullName = std::string(test_info.test_suite_name()) + "." + test_info.name();
    std::strncpy(data.name, fullName.c_str(), sizeof(data.name) - 1);

    const auto* result = test_info.result();
    data.passed = result->Passed();
    data.time_ms = static_cast<double>(result->elapsed_time());

    if (!data.passed) {
        for (int i = 0; i < result->total_part_count(); ++i) {
            const auto& part = result->GetTestPartResult(i);
            if (part.failed()) {
                std::strncpy(data.message, part.message(), sizeof(data.message) - 1);
                break;
            }
        }
    }

    if (data.passed) g_ActiveManager->passedCount++;
    else             g_ActiveManager->failedCount++;
    
    g_ActiveManager->results.append(data);
    g_ActiveManager->totalTests++;
    g_ActiveManager->totalTimeMs += data.time_ms;
    
    if (g_ActiveManager->onTestFinished)
        g_ActiveManager->onTestFinished(data);
}

GTestManager::GTestManager() 
    : totalTests(0), passedCount(0), failedCount(0), 
      totalTimeMs(0.0), isRunning(false), printer(nullptr) {}

bool GTestManager::run(const char* filter) {
    if (isRunning) return false;

    g_ActiveManager = this;

    results.clear();
    totalTests = 0;
    passedCount = 0;
    failedCount = 0;
    totalTimeMs = 0.0;
    isRunning = true;

    static bool gtestInit = false;
    if (!gtestInit) {
        int argc = 1;
        char* argv[] = {(char*)"Lab2_Tests"};
        ::testing::InitGoogleTest(&argc, argv);
        gtestInit = true;
    }

    printer = new ResultPrinter();
    ::testing::UnitTest::GetInstance()->listeners().Append(printer);

    if (filter && std::strlen(filter) > 0) {
        ::testing::GTEST_FLAG(filter) = filter;
    } else {
        ::testing::GTEST_FLAG(filter) = "*";
    }

    int res = RUN_ALL_TESTS();
    (void)res;

    ::testing::UnitTest::GetInstance()->listeners().Release(printer);
    delete printer;
    printer = nullptr;


    g_ActiveManager = nullptr;
    
    isRunning = false;
    return failedCount == 0;
}

float GTestManager::getProgress() const {
    if (totalTests == 0) return 0.0f;
    return static_cast<float>(passedCount + failedCount) / static_cast<float>(totalTests);
}