# YADE (Yet Another DVD Exploit)

A PlayStation 2 DVD exploit that leverages a vulnerability in the DVD player to execute custom code.

## Overview

YADE is a proof-of-concept exploit for the PlayStation 2 that uses a malformed DVD-Video disc structure to gain code execution. The exploit targets the PS2's DVD player firmware and allows loading custom ELF binaries.

## Building

### Prerequisites

- `mipsel-none-elf-gcc` - MIPS cross-compiler for PS2 EE (Emotion Engine)
- `gcc` - Native compiler for the injector tool
- `genisoimage` - ISO image creation tool

### Build Instructions

Simply run the build script:

```bash
./compile.sh
```

This will:
1. Compile the exploit code for PS2 (`code.c`, `jump.c`)
2. Build the injector tool for your host system
3. Generate the exploit DVD ISO image at `build/exploit.iso`

## Project Structure

- `src/code/` - PS2 exploit code that runs on the target
  - `code.c` - Main exploit loader
  - `ps2*.c/h` - PS2 system call interfaces and utilities
- `src/jump/` - Secondary stage loader
- `src/injector/` - Host-side tool to inject exploit into DVD structure
  - `injector.c` - Main injector logic
  - `pgc.c/h` - DVD PGC (Program Chain) manipulation
- `src/ld/` - Linker scripts for PS2 binaries
- `fs/` - Base DVD-Video filesystem structure

## Technical Details

The exploit works by:
1. Creating a malformed DVD-Video structure with specially crafted PGC (Program Chain) data
2. Triggering a buffer overflow in the PS2 DVD player firmware
3. Redirecting execution to custom code loaded from the disc
4. Loading and executing a custom ELF binary

The exploit targets specific memory addresses in the PS2 DVD player 3.00E:
- `VM_CMD_PARSER_SWITCH_ADDR`: 0x00909208
- `JUMP_POINTER`: 0x0090ec20
- `VM_ADDR`: 0x01558e40

## License

This is a proof-of-concept exploit for research purposes. Use at your own risk.

## Disclaimer

This software is provided for educational and research purposes only. The authors are not responsible for any misuse or damage caused by this software. Only use this on hardware you own.
