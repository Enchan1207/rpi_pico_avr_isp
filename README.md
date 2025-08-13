# Raspberry Pi PicoでAVR MCUをプログラムする

## Overview

Raspberry Pi PicoをAVR用のISPとして駆動し、AVRマイコンにプログラムを書き込むことができないか?

## Usage

本プロジェクトではCMakeを使用しています。また、ジェネレータにはNinjaを採用しています。

構成はホストとターゲット(Raspberry Pi Pico)とで個別に管理され、
それぞれ `host`, `target` という名称のプリセットが CMakePresets.jsonに定義されています。

### ホスト向けにビルド

構成:

```sh
cmake --preset host
```

ビルド:

```sh
cmake --build --preset host
```

ビルド結果は `/build/host` 以下に出力されます。

テスト:

```sh
ctest --preset host
```

ホスト向けにビルドする場合のみ、 `/tests` 以下のテストコードによる単体テストが可能です。

### ターゲット向けにビルド

構成:

```sh
cmake --preset target
```

ビルド:

```sh
cmake --build --preset target
```

ビルド結果は `/build/target` 以下に出力されます。
コンパイルされたバイナリは `/build/target/src/app/rpi_pico_avr_isp.uf2` にあります。

## License

This repository is published under [MIT License](LICENSE).
