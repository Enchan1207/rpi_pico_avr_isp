#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdint.h>

#include "core/parser.h"

TEST(ParserTest, LengthForValidCommandWithNoArgs) {
    bool result = false;
    uint8_t length = 0;
    result = getCommandArgumentsLength(STK500_CMD_GET_SYNC, &length);

    EXPECT_EQ(result, true);
    EXPECT_EQ(length, 0);
}

TEST(ParserTest, LengthForValidCommandWithArgs) {
    bool result = false;
    uint8_t length = 0;
    result = getCommandArgumentsLength(STK500_CMD_SET_PARAMETER, &length);

    EXPECT_EQ(result, true);
    EXPECT_EQ(length, 2);
}

TEST(ParserTest, LengthForInvalidCommand) {
    bool result = false;
    uint8_t length = 0;
    result = getCommandArgumentsLength(static_cast<Stk500Command>(0xFF), &length);

    EXPECT_EQ(result, false);
}