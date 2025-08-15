#ifndef STK500_H
#define STK500_H

/// STK500v1プロトコルのコマンド定義
typedef enum {
    /// 書込み器との同期を取る
    STK500_CMD_GET_SYNC = 0x30,

    /// 書込み器が接続されているかを確認する
    STK500_CMD_GET_SIGN_ON = 0x31,

    /// 書込み器内のパラメータを設定する
    STK500_CMD_SET_PARAMETER = 0x40,

    /// 書込み器内のパラメータの値を取得する
    STK500_CMD_GET_PARAMETER = 0x41,

    /// 現在接続されているデバイスのプログラミングパラメータを設定する
    STK500_CMD_SET_DEVICE = 0x42,

    /// 現在接続されているデバイスの拡張プログラミングパラメータを設定する
    STK500_CMD_SET_DEVICE_EXT = 0x45,

    /// プログラミングモードに入る
    STK500_CMD_ENTER_PROG_MODE = 0x50,

    /// プログラミングモードから抜ける
    STK500_CMD_LEAVE_PROG_MODE = 0x51,

    /// 現在接続されているデバイスのチップをクリア(内容を消去)する
    STK500_CMD_CHIP_ERASE = 0x52,

    /// 書込み器が読み書きするアドレスを自動インクリメントするかどうかを返す
    STK500_CMD_CHECK_AUTOINC = 0x53,

    /// 書込み器が読み書きするアドレスを設定する
    STK500_CMD_LOAD_ADDRESS = 0x55,

    /// 現在接続されているデバイスに対して単一のコマンドを発行する
    STK500_CMD_UNIVERSAL = 0x56,

    /// 現在接続されているデバイスに対して複数のコマンドを発行する
    STK500_CMD_UNIVERSAL_MULTI = 0x57,

    /// フラッシュメモリの指定アドレスに1ワード(2バイト)書き込む
    STK500_CMD_PROG_FLASH = 0x60,

    /// EEPROMの指定アドレスに1バイト書き込む
    STK500_CMD_PROG_DATA = 0x61,

    /// 現在接続されているデバイスのヒューズバイトを書き込む
    STK500_CMD_PROG_FUSE = 0x62,

    /// 現在接続されているデバイスのロックビットを書き込む
    STK500_CMD_PROG_LOCK = 0x63,

    /// フラッシュメモリまたはEEPROMに複数バイトを書き込む
    STK500_CMD_PROG_PAGE = 0x64,

    /// ヒューズバイトと拡張ヒューズバイトを書き込む
    STK500_CMD_PROG_FUSE_EXT = 0x65,

    /// フラッシュメモリから1ワード(2バイト)読み込む
    STK500_CMD_READ_FLASH = 0x70,

    /// EEPROMから1バイト読み込む
    STK500_CMD_READ_DATA = 0x71,

    /// ヒューズバイトを読み込む
    STK500_CMD_READ_FUSE = 0x72,

    /// ロックビットを読み込む
    STK500_CMD_READ_LOCK = 0x73,

    /// フラッシュメモリまたはEEPROMから複数バイトを読み込む
    STK500_CMD_READ_PAGE = 0x74,

    /// デバイスのシグネチャを読み込む
    STK500_CMD_READ_SIGN = 0x75,

    /// 発振器キャリブレーションバイトを読み込む
    STK500_CMD_READ_OSC_CAL = 0x76,

    /// ヒューズバイトと拡張ヒューズバイトを読み込む
    STK500_CMD_READ_FUSE_EXT = 0x77,

    /// 発振器キャリブレーションバイトを特定アドレスから読み込む
    STK500_CMD_READ_OSC_CAL_EXT = 0x78
} Stk500Command;

/// STK500v1プロトコルのレスポンス定義
typedef enum {
    /// コマンドは受理され、正常に処理された
    STK500_RESP_OK = 0x10,

    /// コマンドの実行に失敗した
    STK500_RESP_FAILED = 0x11,

    /// 不明なコマンドを受信した
    STK500_RESP_UNKNOWN = 0x12,

    /// デバイスが接続されていない
    STK500_RESP_NO_DEVICE = 0x13,

    /// コマンドのeopを受信した。PCと書込み器とは同期が取れている
    STK500_RESP_IN_SYNC = 0x14,

    /// コマンドのeopを受信しなかった。PCと書込み器との同期が取れていない
    STK500_RESP_NO_SYNC = 0x15
} Stk500Response;

#endif  // STK500_H
