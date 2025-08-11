# AVR Serial programming によるプログラムの書き込み

## 概要

AVRマイコンには _Serial programming_ (以降ISP)と呼ばれる方法を用いて書込むことができます。
これは、マイコンが持つフラッシュメモリやEEPROMのデータをSPI経由で読み書きするものです。

ISPは4byteのコマンドと1byteのレスポンスをターゲットMCUと書込み器とでやりとりすることによって行われます。
特に支持のない限りレスポンスはコマンドの4byteめを送出した際の戻り値として得られます (一部例外あり)。

## 書込みシーケンス

under construction

## コマンドリファレンス

以下、ISPにおいてやり取りされるコマンドの詳細を整理します。

### Programming Enable

接続されているマイコンのISPを有効化します。成功した場合は第3バイト送出時に `$53` が返ります。
このコマンドのみ、第4バイト送出時ではなく第3バイト送出時にレスポンスが返ります。

- コマンド: `$AC $53 $00 $00`
- レスポンス:
  - ISPの有効化に成功した場合: `xx $AC $53 xx`
  - ISPの有効化に失敗した場合: `xx $AC xx xx`

### Chip Erase

マイコンのフラッシュメモリとEEPROMの情報を消去します。

- コマンド: `$AC $80 $00 $00`
- レスポンス: なし

### Read Program Memory (Low)

フラッシュメモリの指定されたアドレスに格納されているデータの下位バイトを読み込みます。

- コマンド: `$20 am al xx`
  - `am`: アドレスのMSB
  - `al`: アドレスのLSB
- レスポンス: 読み出したデータ

### Read Program Memory (High)

フラッシュメモリの指定されたアドレスに格納されているデータの上位バイトを読み込みます。

- コマンド: `$28 am al xx`
  - `am`: アドレスのMSB
  - `al`: アドレスのLSB
- レスポンス: 読み出したデータ

### Load Program Memory Page (Low)

指定されたページアドレスの下位バイトにデータを書き込みます。

- コマンド: `$40 $00 aa vv`
  - `aa`: ページアドレス
  - `vv`: 書き込むデータ
- レスポンス: なし

### Load Program Memory Page (High)

指定されたページアドレスの上位バイトにデータを書き込みます。

- コマンド: `$48 $00 aa vv`
  - `aa`: ページアドレス
  - `vv`: 書き込むデータ
- レスポンス: なし

### Write Program Memory Page

指定されたアドレスのフラッシュメモリページにデータを書き込みます。

- コマンド: `$4C am al $00`
  - `am`: アドレスのMSB
  - `al`: アドレスのLSB
- レスポンス: なし

### Read EEPROM Memory

EEPROMの指定されたアドレスに格納されているデータを読み込みます。

- コマンド: `$A0 am al xx`
  - `am`: アドレスのMSB
  - `al`: アドレスのLSB
- レスポンス: 読み出したデータ

### Write EEPROM Memory

EEPROMの指定されたアドレスにデータを書き込みます。

- コマンド: `$C0 am al vv`
  - `am`: アドレスのMSB
  - `al`: アドレスのLSB
  - `vv`: 書き込むデータ
- レスポンス: なし

### Load EEPROM Memory Page

指定されたアドレスのEEPROMページにデータを書き込みます。

- コマンド: `$C1 $00 aa vv`
  - `aa`: ページアドレス
  - `vv`: 書き込むデータ
- レスポンス: なし

### Write EEPROM Memory Page

指定されたアドレスのEEPROMページにデータを書き込みます。

- コマンド: `$C2 am al $00`
  - `am`: アドレスのMSB
  - `al`: アドレスのLSB
- レスポンス: なし

### Read Lock Bits

施錠ビットの値を取得します。

- コマンド: `$58 $00 $00 xx`
- レスポンス: 施錠ビットの値

### Write Lock Bits

施錠ビットに値を書き込みます。

- コマンド: `$58 $00 $00 aa`
  - `aa`: 施錠ビットの値
- レスポンス: なし

### Read Signature Byte

デバイスのシグネチャバイトを読み込みます。

- コマンド: `$30 $00 aa xx`
  - `aa`: シグネチャバイトのアドレス(`0x00` ~ `0x02`)
- レスポンス: シグネチャバイトの値

### Write Fuse Bits

デバイスの下位ヒューズビットに値を書き込みます。

- コマンド: `$AC $A0 $00 aa`
  - `aa`: ヒューズビットの値
- レスポンス: なし

### Write Fuse High Bits

デバイスの上位ヒューズビットに値を書き込みます。

- コマンド: `$AC $A8 $00 aa`
  - `aa`: ヒューズビットの値
- レスポンス: なし

### Write Extended Fuse Bits

デバイスの拡張ヒューズビットに値を書き込みます。

- コマンド: `$AC $A4 $00 aa`
  - `aa`: ヒューズビットの値
- レスポンス: なし

### Read Fuse Bits

デバイスの下位ヒューズビットを読み込みます。

- コマンド: `$50 $00 $00 xx`
- レスポンス: ヒューズビットの値

### Read Fuse High Bits

デバイスの上位ヒューズビットを読み込みます。

- コマンド: `$58 $08 $00 xx`
- レスポンス: ヒューズビットの値

### Read Extended Fuse Bits

デバイスの拡張ヒューズビットを読み込みます。

- コマンド: `$50 $08 $00 xx`
- レスポンス: ヒューズビットの値

### Read Calibration Byte

デバイスの発振器キャリブレーションバイトを読み込みます。

- コマンド: `$38 $00 $00 xx`
- レスポンス: キャリブレーションバイトの値

### Poll RDY/BSY

デバイスのビジー状態を確認します。

- コマンド: `$F0 $00 $00 xx`
- レスポンス: `0x01` なら動作中、`0x00` なら完了

ATtiny2313

| command                    | data                                            | description                      |
| -------------------------- | ----------------------------------------------- | -------------------------------- |
| Programming Enable         | `$AC $53 $00 $00` | シリアルプログラミングを有効化           |
| Chip Erase                 | `$AC $80 $00 $00` | EEPROMとFlashを消去                  |
| Read Program Memory (low)  | `$20 am al data_out` | プログラムメモリのワードアドレスa\:bからデータoを読み出す  |
| Read Program Memory (high) | `$28 am al data_out` | プログラムメモリのワードアドレスa\:bからデータoを読み出す  |
| Load Program Memory Page (low) | `$40 $00 al data_in` | プログラムメモリページにデータを書き込む（Lowバイト先行）   |
| Load Program Memory Page (high) | `$48 $00 al data_in` | プログラムメモリページにデータを書き込む（Lowバイト先行）   |
| Write Program Memory Page | `$4C 000000aa aaaaxxxx $00` | 指定アドレスのプログラムメモリページを書き込み          |
| Read EEPROM Memory        | `$A0 am al data_out` | EEPROMからデータを読み出す                 |
| Write EEPROM Memory       | `$C0 am al data_in` | EEPROMにデータを書き込む                  |
| Load EEPROM Memory Page   | `$C1 $00 000000aa data_in` | EEPROMページバッファにデータをロード            |
| Write EEPROM Memory Page  | `$C2 000000aa aaaaaa00 $00` | EEPROMページを書き込み                   |
| Read Lock bits            | `$58 $00 $00 data_out` | ロックビットを読み出す（0=プログラム済み, 1=未プログラム） |
| Write Lock bits           | `$AC $E0 $00 data_in` | ロックビットを書き込み（0=プログラム, 1=未プログラム）   |
| Read Signature Byte       | `$30 $00 000000aa data_out` | シグネチャバイトを読み出す                    |
| Write Fuse bits           | `$AC $A0 $00 data_in` | Fuseビットを書き込み（0=プログラム, 1=未プログラム）  |
| Write Fuse High bits      | `$AC $A8 $00 data_in` | Fuse Highビットを書き込み                |
| Write Extended Fuse Bits  | `$AC $A4 $00 data_in` | Extended Fuseビットを書き込み            |
| Read Fuse bits            | `$50 $00 $00 data_out` | Fuseビットを読み出す                     |
| Read Fuse High bits       | `$58 $08 $00 data_out` | Fuse Highビットを読み出す                |
| Read Extended Fuse Bits   | `$50 $08 $00 data_out` | Extended Fuseビットを読み出す            |
| Read Calibration Byte     | `$38 $00 $00 data_out` | キャリブレーションバイトを読み出す                |
| Poll RDY/BSY              | `$F0 $00 $00 data_out` | o=1なら動作中、0なら完了                   |

ATmega328P

| command                         | data                             | arguments                     | description                    |
| ------------------------------- | -------------------------------- | ----------------------------- | ------------------------------ |
| Programming enable              | `$AC $53 $00 $00`                | –                             | シリアルプログラミングを有効化                |
| Chip erase                      | `$AC $80 $00 $00`                | –                             | プログラムメモリおよびEEPROMを消去           |
| Read program memory (low)       | `$20 am al data_out`   | adr\_MSB/adr\_LSB / data\_out | プログラムメモリLowバイトを読み出す            |
| Read program memory (high)      | `$28 am al data_out`   | adr\_MSB/adr\_LSB / data\_out | プログラムメモリHighバイトを読み出す           |
| Load program memory page (low)  | `$40 $00 al low_data_in`    | adr\_LSB / low\_data\_in      | プログラムメモリページにLowバイトを書き込む        |
| Load program memory page (high) | `$48 $00 al high_data_in`   | adr\_LSB / high\_data\_in     | プログラムメモリページにHighバイトを書き込む       |
| Write program memory page       | `$4C am al $00`        | adr\_MSB/adr\_LSB             | プログラムメモリページを書き込む               |
| Read EEPROM memory              | `$A0 000000aa aaaaaaaa data_out` | a=アドレス / data\_out            | EEPROMからデータを読み出す               |
| Write EEPROM memory             | `$C0 000000aa aaaaaaaa data_in`  | a=アドレス / data\_in             | EEPROMにデータを書き込む                |
| Load EEPROM memory page         | `$C1 $00 000000aa data_in`       | a=アドレス / data\_in             | EEPROMページバッファにデータをロード          |
| Write EEPROM memory page        | `$C2 000000aa aaaaaa00 $00`      | a=アドレス                        | EEPROMページを書き込む                 |
| Read lock bits                  | `$58 $00 $00 data_out`           | data\_out=ロックビット              | ロックビットを読み出す（0=プログラム, 1=未プログラム） |
| Write lock bits                 | `$AC $E0 $00 data_in`            | data\_in                      | ロックビットを書き込む                    |
| Read signature byte             | `$30 $00 000000aa data_out`      | a=アドレス / data\_out            | シグネチャバイトを読み出す                  |
| Write fuse bits                 | `$AC $A0 $00 data_in`            | data\_in                      | Fuseビットを書き込む                   |
| Write fuse high bits            | `$AC $A8 $00 data_in`            | data\_in                      | Fuse Highビットを書き込む              |
| Write extended fuse bits        | `$AC $A4 $00 data_in`            | data\_in                      | Extended Fuseビットを書き込む          |
| Read fuse bits                  | `$50 $00 $00 data_out`           | data\_out                     | Fuseビットを読み出す                   |
| Read fuse high bits             | `$58 $08 $00 data_out`           | data\_out                     | Fuse Highビットを読み出す              |
| Read extended fuse bits         | `$50 $08 $00 data_out`           | data\_out                     | Extended Fuseビットを読み出す          |
| Read calibration byte           | `$38 $00 $00 data_out`           | data\_out                     | キャリブレーションバイトを読み出す              |
| Poll RDY/BSY                    | `$F0 $00 $00 data_out`           | o=状態                          | ビジー状態を確認（1=動作中, 0=完了）          |
| Load extended address byte      | `$4D $00 ext_addr $00`           | ext\_addr=拡張アドレス              | 拡張アドレスバイトをロード                  |
