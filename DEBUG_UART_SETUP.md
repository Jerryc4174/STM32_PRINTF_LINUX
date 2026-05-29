# UART Debug Output Setup

This project now routes `printf()` output to the Nucleo board's ST-LINK virtual COM port instead of ITM or semihosting.

## What Changed

- `printf()` is retargeted in `Core/Src/syscalls.c` to call `DebugUart_Write()`.
- A minimal USART3 debug UART driver was added in `Core/Src/debug_uart.c` with declarations in `Core/Inc/debug_uart.h`.
- `DebugUart_Init()` is called from the `USER CODE BEGIN Init` block in `Core/Src/main.c`.
- The VS Code launch configuration was renamed to `STM32Cube: ST-Link (UART printf)` and no longer depends on semihosting.

## Why USART3

On the NUCLEO-F429ZI board, the onboard ST-LINK exposes a USB virtual COM port wired to:

- `PD8` -> `USART3_TX` -> `STLK_RX`
- `PD9` -> `USART3_RX` -> `STLK_TX`

That means `printf()` output travels through the same USB cable already connected to the board.

## How To Use It

1. Build and launch the `STM32Cube: ST-Link (UART printf)` debug configuration.
2. Let the debugger stop at `main`.
3. In a Linux terminal, open the ST-LINK virtual COM port:

```bash
stty -F /dev/ttyACM0 115200 cs8 -cstopb -parenb -icanon -echo
cat /dev/ttyACM0
```

4. In the debugger, press Continue.
5. The task messages printed by `printf()` should appear in that terminal.

## Finding The Serial Device

On this system the ST-LINK VCP appears as:

- `/dev/ttyACM0`
- `/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_066DFF565087534867133614-if02`

If `/dev/ttyACM0` changes, check again with:

```bash
ls -l /dev/ttyACM* /dev/serial/by-id 2>/dev/null
```

## Linux Permissions

If opening `/dev/ttyACM0` fails with a permission error, add your user to the `dialout` group:

```bash
sudo usermod -aG dialout "$USER"
```

Then log out and back in.

## VS Code Configuration Requirements

### launch.json

The active debug configuration is stored in `.vscode/launch.json` and should contain these key values:

- `name`: `STM32Cube: ST-Link (UART printf)`
- `type`: `stlinkgdbtarget`
- `request`: `launch`
- `program`: `${workspaceFolder}/build/Debug/001Tasks.elf`
- `deviceName`: `STM32F429ZIT7`
- `runEntry`: `main`
- `serverExe`: `ST-LINK_gdbserver`
- `serverCwd`: `/home/jerry/st/stm32cubeclt_1.21.0/STLink-gdb-server/bin`
- `serverCubeProgPath`: `/home/jerry/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin`
- `serverInterface`: `SWD`
- `serverPort`: `61234`

Notes:

- `deviceName` must be the exact MCU name expected by the STM32 VS Code debugger. The wildcard form `STM32F429ZITx` did not validate in this setup.
- `serverExe` must be only the executable name. The containing folder belongs in `serverCwd`.
- `runEntry: main` is what causes the debugger to stop at the start of `main()`.
- `serverSemihosting` is no longer required because `printf()` now uses UART, not semihosting.
- `openGdbConsole` should stay unset on Linux because the STM32 adapter path for that option failed on this platform.

### settings.json

The workspace settings in `.vscode/settings.json` are mainly for STM32/CMake integration and code indexing, not for UART output itself.

Current relevant entries:

- `cmake.cmakePath`: `cube-cmake`
- `cmake.configureArgs`: passes `-DCMAKE_COMMAND=cube-cmake`
- `cmake.preferredGenerators`: uses `Ninja`
- `stm32cube-ide-clangd.path`: `cube`
- `stm32cube-ide-clangd.arguments`: points clangd at the STM32 GNU toolchain drivers

Notes:

- These settings help the STM32 VS Code extensions configure, build, and index the project correctly.
- They are not what makes UART `printf()` work. UART output is controlled by the firmware changes in `main.c`, `syscalls.c`, and `debug_uart.c`.
- The existing `cortex-debug.liveWatchRefreshRate` setting is left in place, but it is not required for the STM32-only UART debug flow.

## About STM32CubeMX Regeneration

The custom code in `main.c` is inside an existing STM32 user code block and should survive regeneration.

The `syscalls.c` changes are marked with `USER CODE BEGIN/END` comments to make them easy to find, but survival across regeneration depends on how STM32CubeMX rewrites that file. If `syscalls.c` is regenerated from scratch, you may need to re-apply the UART retarget portion.