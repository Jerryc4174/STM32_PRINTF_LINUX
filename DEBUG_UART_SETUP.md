# UART Trace Setup

This project supports one trace mode at a time:

- `PRINTF`: normal `printf()` output on the ST-LINK virtual COM port
- `SYSVIEW`: continuous SEGGER SystemView recording on the same port

## Switch Modes

The active mode is selected with the `APP_TRACE_MODE` CMake cache variable.

- Edit [CMakePresets.json](CMakePresets.json) and set `APP_TRACE_MODE` to `PRINTF` or `SYSVIEW`
- Reconfigure and rebuild after changing the preset

The current defaults in the repo are `SYSVIEW` for both `Debug` and `Release` presets.

## Hardware And Port

Both modes use the NUCLEO-F429ZI ST-LINK VCP on `USART3`:

- `PD8` -> `USART3_TX` -> `STLK_RX`
- `PD9` -> `USART3_RX` -> `STLK_TX`

On Linux, the device usually appears as `/dev/ttyACM0` or a path under `/dev/serial/by-id`.

If the device changes, check with:

```bash
ls -l /dev/ttyACM* /dev/serial/by-id 2>/dev/null
```

## How To Use It

### `PRINTF` mode

1. Build with `APP_TRACE_MODE=PRINTF`.
2. Open the serial port at `115200`.
3. Start the debugger and continue execution.

```bash
stty -F /dev/ttyACM0 115200 cs8 -cstopb -parenb -icanon -echo
cat /dev/ttyACM0
```

### `SYSVIEW` mode

1. Build with `APP_TRACE_MODE=SYSVIEW`.
2. In SEGGER SystemView Recorder, choose `UART` and select the same ST-LINK VCP device.
3. Set the recorder speed to `115200`.
4. Start recording after the target is running.

## VS Code Notes

The debug launch config is still in [/.vscode/launch.json](.vscode/launch.json) and uses ST-LINK GDB server on Linux. The UART trace mode is controlled by the preset, not the launch config.

The UART retarget and SystemView setup live in `main.c`, `syscalls.c`, `debug_uart.c`, `sysview_uart.c`, and the SEGGER config files. If STM32CubeMX regenerates `syscalls.c`, you may need to reapply the retarget block.