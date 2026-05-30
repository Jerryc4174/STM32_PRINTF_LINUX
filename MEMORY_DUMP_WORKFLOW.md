# Memory Dump Workflow

This project uses `STM32_Programmer_CLI` to dump RAM or flash to a file.

The method below is the one that worked reliably on Linux when the debugger console would not accept raw GDB dump commands.

## When To Use This

Use this workflow when you want to capture:

- a task stack
- a FreeRTOS heap region
- a crash dump from RAM
- a flash image fragment

## Important Notes

- Stop the VS Code debug session before dumping memory.
- `STM32_Programmer_CLI` cannot connect if `ST-LINK_gdbserver` or `arm-none-eabi-gdb` is still holding the probe.
- Some GUI file pickers hide files with unusual extensions. If needed, save as `.bin` first and rename later.

## Example: Dump 4 KB From RAM

Example source address:

- `0x2001333C`

Example size:

- `0x1000` bytes = 4 KB

Command:

```bash
pkill -f ST-LINK_gdbserver || true
pkill -f arm-none-eabi-gdb || true

/home/jerry/st/stm32cubeclt_1.21.0/STM32CubeProgrammer/bin/STM32_Programmer_CLI \
  -c port=SWD mode=HOTPLUG \
  -u 0x2001333C 0x1000 /home/jerry/Downloads/memdump.bin
```

If `mode=HOTPLUG` does not connect, retry with `mode=UR`:

```bash
/home/jerry/st/stm32cubeclt_1.21.0/STM32CubeProgrammer/bin/STM32_Programmer_CLI \
  -c port=SWD mode=UR \
  -u 0x2001333C 0x1000 /home/jerry/Downloads/memdump.bin
```

## Renaming For Other Tools

If a tool wants a different extension, make a copy after the dump:

```bash
cp /home/jerry/Downloads/memdump.bin /home/jerry/Downloads/memdump.svdat
```

For this course, the `.bin` version was the easiest one to open in the SystemView file picker.

## Verify The Dump

Check the file size:

```bash
ls -l /home/jerry/Downloads/memdump.bin
```

For a 4 KB dump, the file should be `4096` bytes.

## Useful Address Math

If you know the start address and size, the end address for a raw memory range is:

```text
end = start + size
```

Example:

- start: `0x2001333C`
- size: `0x1000`
- end: `0x2001433C`

## What To Do If The File Does Not Appear

If Nemo or a file picker does not show the dump:

1. Open the exact folder path with `Ctrl+L` in Nemo.
2. Check that the file was written to the folder you expect.
3. Prefer `/home/jerry/Downloads` if a dialog is hiding files in the project root.

## Notes From This Project

- The debug console commands `dump binary memory ...` and `-exec dump binary memory ...` were unreliable in this setup.
- The CLI method above worked consistently once ST-LINK was released from the active debug session.
