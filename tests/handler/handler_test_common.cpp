#include "handler_test_common.h"

namespace handler_test {

// テスト用のISP転送関数（モック）
uint8_t mockIspTransfer(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3, uint8_t cmd4) {
    // テスト用の実装（必要に応じて拡張）
    return 0x00;
}

// テスト用のレスポンス書き込み関数（モック）
std::vector<uint8_t> capturedResponse;
void mockResponseWriter(const uint8_t* response, size_t length) {
    capturedResponse.clear();
    capturedResponse.assign(response, response + length);
}

// ハンドラテスト用の基底クラス
void HandlerTestBase::SetUp() {
    // パーサコンテキストを初期化
    initParserContext(&parserCtx);
    
    // ハンドラコンテキストを初期化
    initHandlerContext(&handlerCtx, mockIspTransfer, mockResponseWriter);
    
    // キャプチャされたレスポンスをクリア
    capturedResponse.clear();
}

}  // namespace handler_test