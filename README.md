[![builds.sr.ht status](https://builds.sr.ht/~tristan957/tllt-cp/.build.yml.svg)](https://builds.sr.ht/~tristan957/tllt-cp/.build.yml?)

# The Learning Little Toaster Control Panel

## Organization

This project is seperated into three parts:

* GUI
* client library
* driver

The GUI is the control panel for interacting with the toaster. The client
library is utilized by the GUI to interact with the server. The driver is used
to talk to the toaster.

## Dependencies

* `libcurl >= 7.52.1`
* `glib >= 2.50.3`
* `gtk >= 3.22`
* `libjson-glib >= 1.2.6`

**Disclaimer**: These are known working dependency versions. Previous versions
may work fine.

## Development Environment

This project uses `clang-format` to format files and Meson as the build system.
Project should build fine with `clang` or `gcc`.

To initalize the build environment, run

```text
CC={compiler of choice} meson build
```

### WiringPi

By default, the driver builds without support for WiringPi. To enable support,
run `meson configure -Dwith-wiringPi=true build`. You will also need to make sure
that WiringPi is installed on your system. Instructions for manually building
it, can be found on the GitHub mirror.

## Environment Variables

* `TLLT_CP_SERVER` - URL of the server
* `TLLT_TOASTER_CONFIG_FILE_PATH` - file path of the toaster config

## Toaster Config File

JSON file like the following

```json
{
    "top-heating-element": {
        "gpio-pin": 1
    },
    "bottom-heating-element": {
        "gpio-pin": 2
    },
    "thermistor": {
        "spi-chan": 0,
        "base-pin": 100,
        "number-of-pins": 8
    }
}
```

## Building

```text
ninja -C build
```

## Testing

```text
ninja test -C build
```

## Running

### GUI

```text
./build/src/tllt-cp
```

### Tests

```text
./build/tests/{path to test executable}
