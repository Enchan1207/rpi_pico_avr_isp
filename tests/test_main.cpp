#include <gtest/gtest.h>
#include <stdio.h>
#include "core/logger.h"

static void testLogOutput(const char* message) {
    printf("%s", message);
}

class GlobalTestSetup : public ::testing::Environment {
public:
    void SetUp() override {
        initLogger(testLogOutput);
    }
    
    void TearDown() override {
    }
};

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GlobalTestSetup);
    return RUN_ALL_TESTS();
}