#include <gtest/gtest.h>

#include "core/calc.h"

TEST(CalcTest, AddFunction) {
    uint8_t x = 5;
    uint8_t y = 8;
    uint8_t expected = 13;

    uint8_t result = add(x, y);

    EXPECT_EQ(result, expected);
}
