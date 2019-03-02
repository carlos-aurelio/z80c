z80c: a Z80 emulator written in C
=================================

This is a Zilog Z80 CPU emulator in a very early state.

Quick Start:
------------

1. Clone the repository
2. Run `make`
3. Run `build/core_test <binary file>`

The `core_test` application will load the binary file directly into the
memory, starting at address 0x00, and then wait for commands in a prompt.

Currently, the following commands are accepted in the `>` prompt:

* `n`: Execute one instruction.
* `p`: Print the registers and the memory around **PC**.
* `s`: The same as `n` and then `p`.
* `q`: Quit.
* `r`: Run the program. This is intended to be used together with a
debugger, since there is no output or feedback of what's happening.

Pressing `<enter>` with no command will repeat the last command.

TODO:
-----
* Handle interrupts
* ???
