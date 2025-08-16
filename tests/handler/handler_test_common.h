#ifndef HANDLER_TEST_COMMON_H
#define HANDLER_TEST_COMMON_H

#include <gtest/gtest.h>
#include <vector>
#include <cstring>

#include "core/handler.h"
#include "core/parser.h"
#include "core/stk500.h"

namespace handler_test {

// テスト用のISP転送関数（モック）
uint8_t mockIspTransfer(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3, uint8_t cmd4);

// テスト用のレスポンス書き込み関数（モック）
extern std::vector<uint8_t> capturedResponse;
void mockResponseWriter(const uint8_t* response, size_t length);

// ハンドラテスト用の基底クラス
class HandlerTestBase : public ::testing::Test {
protected:
    void SetUp() override;

    parser_context_t parserCtx;
    handler_context_t handlerCtx;
};

}  // namespace handler_test

#endif  // HANDLER_TEST_COMMON_H