#ifndef STK500_PARAMETERS_H
#define STK500_PARAMETERS_H

#ifdef __cplusplus
extern "C" {
#endif

/// STK500v1パラメータ番号定義
typedef enum {
    /// スターターキットハードウェアのバージョン（読み取り専用）
    PARM_STK_HW_VER = 0x80,

    /// スターターキットMCUソフトウェアのメジャーバージョン（読み取り専用）
    PARM_STK_SW_MAJOR = 0x81,

    /// スターターキットMCUソフトウェアのマイナーバージョン（読み取り専用）
    PARM_STK_SW_MINOR = 0x82,

    /// スターターキットのステータスLED制御
    PARM_STK_LEDS = 0x83,

    /// ターゲット電圧（VTARGET）
    PARM_STK_VTARGET = 0x84,

    /// 調整可能電圧（AREF）
    PARM_STK_VADJUST = 0x85,

    /// 調整可能オシレータのタイマープリスケーラ値
    PARM_STK_OSC_PSCALE = 0x86,

    /// 調整可能オシレータのタイマーコンペアマッチ値
    PARM_STK_OSC_CMATCH = 0x87,

    /// ISP SCKクロックの半周期時間
    PARM_STK_SCK_DURATION = 0x89,

    /// スターターキットMCU通信バッファサイズ下位バイト（読み取り専用）
    PARM_STK_BUFSIZEL = 0x90,

    /// スターターキットMCU通信バッファサイズ上位バイト（読み取り専用）
    PARM_STK_BUFSIZEH = 0x91,

    /// STK500接続トップカード検出（読み取り専用）
    PARAM_STK500_TOPCARD_DETECT = 0x98
} Stk500Parameter;

/// プロジェクト固有のバージョン定義
#define STK500_HW_VERSION 0x02
#define STK500_SW_MAJOR 0x01
#define STK500_SW_MINOR 0x00

#ifdef __cplusplus
}
#endif

#endif  // STK500_PARAMETERS_H
