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

* `libcurl >= 7.61.1`
* `glib >= 2.56`
* `gtk >= 3.22`
* `libjson-glib >= 1.4.2`

**Disclaimer**: These are known working dependency versions. Previous versions
may work fine.

## Development Environment

This project uses `clang-format` to format files and Meson as the build system.
Project should build fine with `clang` or `gcc`.

To initalize the build environment, run

```text
CC={compiler of choice} meson build
```

## Environment Variables

* `TLLT_CP_SERVER` - URL of the server
* `TLLT_CP_TOASTER_CONFIG_FILE_PATH` - file path of the toaster config

## Driver Config File

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
        "gpio-pin": 3
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
