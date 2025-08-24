# Development information

This project uses CMake, and Ninja for generator.

Configurations for host and target are managed separately,
and presets named _host_, _target_ are defined at `CMakePresets.json`.

## Build project

This section describes how to build project.

### for host (PC)

configure:

```sh
cmake --preset host
```

build:

The build result will be emit to `/build/host/*`.

```sh
cmake --build --preset host
```

test:

This project uses GoogleTest for unit testing.
When build it for host, you can test with ctest.

```sh
ctest --preset host
```

### for target (Raspberry Pi Pico)

configure:

```sh
cmake --preset target
```

build:

The build result will be emit to `/build/target/*` .
There will be compiled binary at `/build/target/src/app/rpi_pico_avr_isp.uf2`.

```sh
cmake --build --preset target
```
