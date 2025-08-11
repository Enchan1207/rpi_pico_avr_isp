#ifndef COMMANDS_H
#define COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief STK500 v1プロトコルのコマンド定義
 */
typedef enum {
    /** @brief 書込み器との同期を取る */
    CMD_GET_SYNC = 0x30,
    /** @brief 書込み器が接続されているかを確認する */
    CMD_GET_SIGNON = 0x31,

    /** @brief 書込み器内のパラメータを設定する */
    CMD_SET_PARAMETER = 0x40,
    /** @brief 書込み器内のパラメータの値を取得する */
    CMD_GET_PARAMETER = 0x41,

    /** @brief 接続されているデバイスのプログラミングパラメータを設定する */
    CMD_SET_DEVICE = 0x42,
    /** @brief 接続されているデバイスの拡張プログラミングパラメータを設定する */
    CMD_SET_DEVICE_EXT = 0x45,

    /** @brief プログラミングモードに入る */
    CMD_ENTER_PROGMODE = 0x50,
    /** @brief プログラミングモードから抜ける */
    CMD_LEAVE_PROGMODE = 0x51,

    /** @brief 接続されているデバイスのチップをクリア(内容を消去)する */
    CMD_CHIP_ERASE = 0x52,
    /** @brief 書込み器が読み書きするアドレスを自動インクリメントするかどうかを返す */
    CMD_CHECK_AUTOINC = 0x53,
    /** @brief 書込み器が読み書きするアドレスを設定する */
    CMD_LOAD_ADDRESS = 0x55,

    /** @brief 接続されているデバイスに対して単一のコマンドを発行する */
    CMD_UNIVERSAL = 0x56,
    /** @brief 接続されているデバイスに対して複数のコマンドを発行する */
    CMD_UNIVERSAL_MULTI = 0x57,

    /** @brief フラッシュメモリの指定アドレスに1ワード(2バイト)書き込む */
    CMD_PROG_FLASH = 0x60,
    /** @brief EEPROMの指定アドレスに1バイト書き込む */
    CMD_PROG_DATA = 0x61,
    /** @brief 接続されているデバイスのヒューズバイトを書き込む */
    CMD_PROG_FUSE = 0x62,
    /** @brief 接続されているデバイスのロックビットを書き込む */
    CMD_PROG_LOCK = 0x63,
    /** @brief フラッシュメモリまたはEEPROMに複数バイトを書き込む */
    CMD_PROG_PAGE = 0x64,
    /** @brief ヒューズバイトと拡張ヒューズバイトを書き込む */
    CMD_PROG_FUSE_EXT = 0x65,

    /** @brief フラッシュメモリから1ワード(2バイト)読み込む */
    CMD_READ_FLASH = 0x70,
    /** @brief EEPROMから1バイト読み込む */
    CMD_READ_DATA = 0x71,
    /** @brief ヒューズバイトを読み込む */
    CMD_READ_FUSE = 0x72,
    /** @brief ロックビットを読み込む */
    CMD_READ_LOCK = 0x73,
    /** @brief フラッシュメモリまたはEEPROMから複数バイトを読み込む */
    CMD_READ_PAGE = 0x74,
    /** @brief デバイスのシグネチャを読み込む */
    CMD_READ_SIGN = 0x75,
    /** @brief 発振器キャリブレーションバイトを読み込む */
    CMD_READ_OSC_CAL = 0x76,
    /** @brief ヒューズバイトと拡張ヒューズバイトを読み込む */
    CMD_READ_FUSE_EXT = 0x77,
    /** @brief 発振器キャリブレーションバイトを特定アドレスから読み込む */
    CMD_READ_OSC_CAL_EXT = 0x78
} stk_command;

/**
 * @brief STK500 v1プロトコルのレスポンスコード定義
 */
typedef enum {
    /** @brief コマンドは受理され、正常に処理された */
    RESP_STK_OK = 0x10,
    /** @brief コマンドの実行に失敗した */
    RESP_STK_FAILED = 0x11,
    /** @brief 不明なコマンドを受信した */
    RESP_STK_UNKNOWN = 0x12,
    /** @brief デバイスが接続されていない */
    RESP_STK_NODEVICE = 0x13,
    /** @brief コマンドのeopを受信した（同期が取れている） */
    RESP_STK_INSYNC = 0x14,
    /** @brief コマンドのeopを受信しなかった（同期が取れていない） */
    RESP_STK_NOSYNC = 0x15
} stk_response;

/**
 * @brief STK500 v1プロトコルの制御文字
 */
typedef enum {
    /** @brief コマンド終端文字（End Of Packet） */
    STK_EOP = 0x20
} stk_control;

/**
 * @brief メモリタイプ指定子
 */
typedef enum {
    /** @brief フラッシュメモリ */
    MEMTYPE_FLASH = 'F',
    /** @brief EEPROM */
    MEMTYPE_EEPROM = 'E'
} stk_mem_type;

#ifdef __cplusplus
}
#endif

#endif /* COMMANDS_H */
