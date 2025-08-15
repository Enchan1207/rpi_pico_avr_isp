# STK500 v1 を用いたPCと書込み器との通信

## 基本

STK500v1は、PCと書込み器との通信を行うためのプロトコルです。
このプロトコルに沿って書込み器を実装することにより、CLIツール _avrdude_ を用いてプログラムを書き込めるようになります。

## コマンドフォーマット

コマンドは以下のフォーマットに従います。

```text
<cmd> <args...> <eop>
```

- `cmd` は1byteの数値です。詳細は [コマンドリファレンス](#コマンドリファレンス)から確認できます。
- `args` はコマンドの引数です。引数は0~259byteで、ほとんどの命令は固定長ですが、いくつかの命令は可変長です。
- `eop` はコマンドの終端であり、値は `0x20` です。 **この値で終わらないデータは受理されません。**

## レスポンス

レスポンスは以下のフォーマットに従います。

```text
<insync> <data...> <resp>
```

- `insync` はコマンドを受理したことを意味し、値は `0x14` です。受理されたコマンドのレスポンスは常にこの値から始まります。
- `data` はレスポンスのデータです。長さは規定されていませんが、256byteを超えることはありません。
- `resp` は1byteの数値です。詳細は [レスポンスリファレンス](#レスポンスリファレンス)から確認できます。

## 通信の流れ

通信は基本的に「コマンド受信 -> コマンド解釈・処理 -> レスポンス」を繰り返す形で行われます。

- コマンド受信
    PCから与えられたコマンドを、引数を含めて受信します。
    通信がタイムアウトするか、コマンド終端(`eop`)が来るまで続きます。
- コマンド解釈・処理
    受信したコマンドを解釈し、処理を実行します。
- レスポンス
    コマンドに対する処理結果を返します。

## コマンドリファレンス

定義されているコマンドは以下の通りです。

- GetSync (0x30)
- GetSignOn (0x31)
- SetParameter (0x40)
- GetParameter (0x41)
- SetDevice (0x42)
- SetDeviceExt (0x45)
- EnterProgMode (0x50)
- LeaveProgMode (0x51)
- ChipErase (0x52)
- CheckAutoinc (0x53)
- LoadAddress (0x55)
- Universal (0x56)
- UniversalMulti (0x57)
- ProgFlash (0x60)
- ProgData (0x61)
- ProgFuse (0x62)
- ProgLock (0x63)
- ProgPage (0x64)

### GetSync (0x30)

書込み器との同期を取る。

- 引数: なし
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### GetSignOn (0x31)

書込み器が接続されているかを確認する。

- 引数: なし
- レスポンス:
  - `data`:
    - `sign_on_message`: ASCII文字列 `AVR STK`
  - `resp`: `Ok`

### SetParameter (0x40)

書込み器内のパラメータを設定する。

- 引数:
  - `parameter`: パラメータ番号
  - `value`: 設定値
- レスポンス:
  - 正しいパラメータ番号が設定された場合
    - `data`: なし
    - `resp`: `Ok`
  - 不正なパラメータ番号が設定された場合
    - `data`:
      - `parameter`: 与えられたパラメータ番号
    - `resp`: `Failed`

### GetParameter (0x41)

書込み器内のパラメータの値を取得する。

- 引数:
  - `parameter`: パラメータ番号
- レスポンス
  - 正しいパラメータ番号が設定された場合
    - `data`:
      - `value`: 番号に対応するパラメータの設定値
    - `resp`: `Ok`
  - 不正なパラメータ番号が設定された場合
    - `data`:
      - `parameter`: 与えられたパラメータ番号
    - `resp`: `Failed`

### SetDevice (0x42)

現在接続されているデバイスのプログラミングパラメータを設定する。

- 引数: (20bytes)
  - `devicecode`: デバイスコード
  - `revision`: 省略 (本プロジェクトでは未使用, 定数 `0`)
  - `progtype`: 省略 (本プロジェクトでは未使用)
  - `parmode`: 省略 (本プロジェクトでは未使用)
  - `polling`: 省略 (本プロジェクトでは未使用)
  - `selftimed`: 省略 (本プロジェクトでは未使用)
  - `lockbytes`: 施錠バイトの長さ
  - `fusebytes`: ヒューズバイトの長さ
  - `flashpollval1`: 省略 (本プロジェクトでは未使用)
  - `flashpollval2`: 省略 (本プロジェクトでは未使用)
  - `eeprompollval1`: 省略 (本プロジェクトでは未使用)
  - `eeprompollval2`: 省略 (本プロジェクトでは未使用)
  - `pagesizehigh`: フラッシュメモリ ページサイズ上位
  - `pagesizelow`: フラッシュメモリ ページサイズ下位
  - `eepromsizehigh`: EEPROM サイズ上位
  - `eepromsizelow`: EEPROM サイズ下位
  - `flashsize4`: フラッシュメモリ サイズ上位
  - `flashsize3`: フラッシュメモリ サイズ
  - `flashsize2`: フラッシュメモリ サイズ
  - `flashsize1`: フラッシュメモリ サイズ下位
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### SetDeviceExt (0x45)

現在接続されているデバイスの拡張プログラミングパラメータを設定する。

- 引数:
  - `commandsize`
  - `eeprompagesize`: EEPROM ページサイズ
  - `signalpagel`: 省略 (本プロジェクトでは未使用)
  - `signalbs2`: 省略 (本プロジェクトでは未使用)
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### EnterProgMode (0x50)

プログラミングモードに入る。

- 引数: なし
- レスポンス:
  - プログラミングモードに遷移できた場合
    - `data`: なし
    - `resp`: `Ok`
  - プログラミングモードに遷移できなかった場合
    - `data`: なし
    - `resp`: `NoDevice`

### LeaveProgMode (0x51)

プログラミングモードから抜ける。

- 引数: なし
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### ChipErase (0x52)

現在接続されているデバイスのチップをクリア(内容を消去)する。

- 引数: なし
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### CheckAutoinc (0x53)

書込み器が読み書きするアドレスを自動インクリメントするかどうかを返す。
STK500においては常に成功する。

- 引数: なし
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### LoadAddress (0x55)

書込み器が読み書きするアドレスを設定する。

- 引数:
  - `addr_low`: アドレス下位
  - `addr_high`: アドレス上位
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### Universal (0x56)

現在接続されているデバイスに対して単一のコマンドを発行する。

- 引数:
  - `byte1`: 送信データ1
  - `byte2`: 送信データ2
  - `byte3`: 送信データ3
  - `byte4`: 送信データ4
- レスポンス:
  - `byte4_out`: 4バイト目を送出した際の応答

### UniversalMulti (0x57)

現在接続されているデバイスに対して複数のコマンドを発行する。

- 引数:
  - `number_of_bytes`: `data` の長さ
  - `data` デバイスに送信するコマンド群
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### ProgFlash (0x60)

フラッシュメモリの指定アドレスに1ワード(2バイト)書き込む。

- 引数:
  - `flash_low`: プログラムする値の下位バイト
  - `flash_high`: プログラムする値の上位バイト
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### ProgData (0x61)

EEPROMの指定アドレスに1バイト書き込む。

- 引数:
  - `datum`: 書き込む値
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### ProgFuse (0x62)

現在接続されているデバイスのヒューズバイトを書き込む。

- 引数:
  - `fuse_low`: ヒューズ下位バイトの値
  - `fuse_high`: ヒューズ上位バイトの値
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### ProgLock (0x63)

現在接続されているデバイスのロックビットを書き込む。

- 引数:
  - `lock`: ロックビットの値
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### ProgPage (0x64)

フラッシュメモリまたはEEPROMに複数バイトを書き込む。

- 引数:
  - `bytes_high`: データ長上位
  - `bytes_low`: データ長下位
  - `memtype`: メモリ種別(`F`: フラッシュ, `E`: EEPROM)
  - `data` 書き込むデータ
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### ProgFuseExt (0x65)

ヒューズバイトと拡張ヒューズバイトを書き込む。

- 引数:
  - `fuse_low`: ヒューズ下位バイトの値
  - `fuse_high`: ヒューズ上位バイトの値
  - `fuse_ext`: 拡張ヒューズバイトの値
- レスポンス:
  - `data`: なし
  - `resp`: `Ok`

### ReadFlash (0x70)

フラッシュメモリから1ワード(2バイト)読み込む。
アドレスは `LoadAddress` コマンドで指定された値に依存する。

- 引数: なし
- レスポンス:
  - `data`:
    - `flash_low`: フラッシュ下位バイト
    - `flash_high`: フラッシュ上位バイト
  - `resp`: `Ok`

### ReadData (0x71)

EEPROMから1バイト読み込む。
アドレスは `LoadAddress` コマンドで指定された値に依存する。

- 引数: なし
- レスポンス:
  - `data`:
    - `datum`: EEPROMの値
  - `resp`: `Ok`

### ReadFuse (0x72)

ヒューズバイトを読み込む。

- 引数: なし
- レスポンス:
  - `data`
    - `fuse_low`: ヒューズ下位バイトの値
    - `fuse_high`: ヒューズ上位バイトの値
  - `resp`: `Ok`

### ReadLock (0x73)

ロックビットを読み込む。

- 引数: なし
- レスポンス:
  - `data`:
    - `lock`: ロックビットの値
  - `resp`: `Ok`

### ReadPage (0x74)

フラッシュメモリまたはEEPROMから複数バイトを読み込む。

- 引数:
  - `bytes_high`: 読み込むバイト数上位
  - `bytes_low`: 読み込むバイト数下位
  - `memtype`: メモリ種別(`F`: フラッシュ, `E`: EEPROM)
- レスポンス:
  - `data`:
    - `data`: 読み込んだデータ
  - `resp`: `Ok`

### ReadSign (0x75)

デバイスのシグネチャを読み込む。

- 引数: なし
- レスポンス:
  - `data`:
    - `sign_high`: シグネチャ上位
    - `sign_middle`: シグネチャ
    - `sign_low`: シグネチャ下位
  - `resp`: `Ok`

### ReadOscCal (0x76)

発振器キャリブレーションバイトを読み込む。

- 引数: なし
- レスポンス:
  - `data`:
    - `osc_cal_byte`: 発振器キャリブレーションバイト
  - `resp`: `Ok`

### ReadFuseExt (0x77)

ヒューズバイトと拡張ヒューズバイトを読み込む。

- 引数: なし
- レスポンス:
  - `data`:
    - `fuse_low`: ヒューズ下位バイトの値
    - `fuse_high`: ヒューズ上位バイトの値
    - `fuse_ext`: 拡張ヒューズバイトの値
  - `resp`: `Ok`

### ReadOscCalExt (0x78)

発振器キャリブレーションバイトを特定アドレスから読み込む。

- 引数:
  - `address`: キャリブレーションバイトのアドレス
- レスポンス:
  - `data`:
    - `osc_cal_byte`: 発振器キャリブレーションバイト
  - `resp`: `Ok`

## レスポンスリファレンス

定義されているレスポンスは以下の通りです。

- Ok: `0x10`
  - コマンドは受理され、正常に処理された。
- Failed: `0x11`
  - コマンドの実行に失敗した。
- Unknown: `0x12`
  - 不明なコマンドを受信した。
- NoDevice: `0x13`
  - デバイスが接続されていない。
- InSync: `0x14`
  - コマンドのeopを受信した。PCと書込み器とは同期が取れている。
- NoSync: `0x15`
  - コマンドのeopを受信しなかった。PCと書込み器との動機が取れていない。
