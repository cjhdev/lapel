Lapel
=====

Lapel is a CLI shell suitable for porting to a microcontroller or embedding within a general purpose application. It can be made to work over any reliable serial interface.

## Features

- multi-line editing
- line completions (tab)
- line history (up/down)
- argument tokenizer supporting quotes and escape characters
- getopt argument parser with support for long and short options
- users are optional
    - optional out-of-band login (e.g. if transport layer provides authentication)
    - optional username and pasword prompt
    - role based access control
- multiple shell instances are possible
- command handlers may yield to wait for input without blocking (and can be 'terminated' by ctrl-c)

## Limitations / Caveats

- requires a C++ compiler (C++11 and up)
- no redirection, pipes, scripts, and other sophisticated features you might expect in a real shell
- no concept of file system (for now)
- ascii only
- line buffers are allocated from heap

## Builtin Commands

- `help`
- `clear`: clear screen
- `resize`: specify the size or ask shell to discover using DSR control code
- `logout`: logout user
- `echo`: echo back the arguments

## Examples

- [raw terminal](example/terminal_example.cpp)
- [Arduino](wrapper/arduino)

## Serial Terminal Compatibility

Lapel has been confirmed to work with the following serial terminals:

- GNU Screen
- Putty

## Building

- create `lapel_config.h` somewhere in search path (lapel source includes this file)
- add `include` to search path
- compile sources in `src`

There are some preprocessor options which can be defined in `lapel_config.h`:

### `#define LAPEL_DEBUG(...)`

Print debug messages.

### `#define LAPELERROR(...)`

Print error messages.

### `#define LAPEL_ASSERT(X)`

Implement assert.

## License

See LICENSE and NOTICE files.
