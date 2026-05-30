# SEGGER FreeRTOS Patch Process (VS Code)

This project uses VS Code, not STM32CubeIDE/Eclipse Team tooling.

If you do not see a Team menu when right-clicking folders, that is expected in VS Code. Use terminal-based patching with Git.

## Scope

This guide describes how to apply:

- `ThirdParty/SEGGER/Patch/FreeRTOS_10_4_3.patch`

to this project layout:

- `ThirdParty/FreeRTOS/...`

## Why the raw patch does not apply directly

The SEGGER patch was generated for a different FreeRTOS folder layout, for example:

- `org/config/FreeRTOSConfig.h`
- `org/FreeRTOS/Source/include/FreeRTOS.h`

This repository uses:

- `ThirdParty/FreeRTOS/FreeRTOSConfig.h`
- `ThirdParty/FreeRTOS/include/FreeRTOS.h`

So path adaptation is required before applying.

## 1. Create a layout-adapted copy of the patch

Run from repository root:

```bash
sed \
  -e 's#org/config/FreeRTOSConfig.h#org/FreeRTOSConfig.h#g' \
  -e 's#new/config/FreeRTOSConfig.h#new/FreeRTOSConfig.h#g' \
  -e 's#org/FreeRTOS/Source/#org/#g' \
  -e 's#new/FreeRTOS/Source/#new/#g' \
  ThirdParty/SEGGER/Patch/FreeRTOS_10_4_3.patch \
  > ThirdParty/SEGGER/Patch/FreeRTOS_10_4_3_stm32_layout.patch
```

## 2. Dry-run check before modifying files

Apply only CM4F-relevant parts for STM32F429:

```bash
git apply --check -p1 --directory=ThirdParty/FreeRTOS \
  --include='*/FreeRTOSConfig.h' \
  --include='*/include/FreeRTOS.h' \
  --include='*/include/task.h' \
  --include='*/portable/GCC/ARM_CM4F/port.c' \
  --include='*/portable/GCC/ARM_CM4F/portmacro.h' \
  --include='*/tasks.c' \
  ThirdParty/SEGGER/Patch/FreeRTOS_10_4_3_stm32_layout.patch
```

If some hunks fail due local differences, continue with `--reject` in the next step.

## 3. Apply what can be applied and generate reject files

```bash
git apply --reject -p1 --directory=ThirdParty/FreeRTOS \
  --include='*/FreeRTOSConfig.h' \
  --include='*/include/FreeRTOS.h' \
  --include='*/include/task.h' \
  --include='*/portable/GCC/ARM_CM4F/port.c' \
  --include='*/portable/GCC/ARM_CM4F/portmacro.h' \
  --include='*/tasks.c' \
  ThirdParty/SEGGER/Patch/FreeRTOS_10_4_3_stm32_layout.patch
```

Expected in this repo: most hunks apply, with rejects typically in:

- `ThirdParty/FreeRTOS/FreeRTOSConfig.h.rej`
- `ThirdParty/FreeRTOS/include/task.h.rej`

## 4. Manually merge rejected hunks

Use each `.rej` file as source of truth and add missing content manually into target files.

In this project, the required manual inserts were:

- In `ThirdParty/FreeRTOS/FreeRTOSConfig.h`:
  - `#define INCLUDE_xTaskGetIdleTaskHandle 1`
  - `#define INCLUDE_pxTaskGetStackStart 1`
  - `#include "SEGGER_SYSVIEW_FreeRTOS.h"`

- In `ThirdParty/FreeRTOS/include/task.h`:
  - Declaration of:
    - `uint8_t* pxTaskGetStackStart( TaskHandle_t xTask) PRIVILEGED_FUNCTION;`

## 5. Remove temporary reject files

```bash
rm -f ThirdParty/FreeRTOS/FreeRTOSConfig.h.rej
rm -f ThirdParty/FreeRTOS/include/task.h.rej
```

## 6. Ensure SEGGER sources are in CMake

For successful link, include these files in `CMakeLists.txt` target sources:

- `ThirdParty/SEGGER/SEGGER/SEGGER_RTT.c`
- `ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S`
- `ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.c`
- `ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.c`
- `ThirdParty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c`
- `ThirdParty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.c`

And include directories:

- `ThirdParty/SEGGER/Config`
- `ThirdParty/SEGGER/OS`
- `ThirdParty/SEGGER/SEGGER`

## 7. Build and verify

Use CMake Tools build from VS Code (or equivalent build command) and confirm:

- No link errors
- ELF produced successfully

## 8. Review and commit

```bash
git status --short
git add CMakeLists.txt ThirdParty/FreeRTOS ThirdParty/SEGGER
git commit -m "Integrate SEGGER FreeRTOS SYSVIEW patch for CM4F layout"
```

## Notes

- Macro redefinition warnings from SYSVIEW vs default FreeRTOS trace macros can appear depending on integration order.
- This patch targets FreeRTOS 10.4.3 style code; newer or modified trees may require small manual merges.
