# Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.20)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Project definition
project(001Tasks)

# Enable CMake support for ASM and C/CPP languages
enable_language(C ASM)

set(BUILD_UNIT_0_NAME 001Tasks)

# Create an executable object type
add_executable(${BUILD_UNIT_0_NAME})

# STM32CubeIDE for Visual Studio Code managed CMake file
include(cmake/vscode_generated.cmake)

# Compiler options
target_compile_options(${BUILD_UNIT_0_NAME} PRIVATE
  # User defined compiler options
)

# Linker options
target_link_options(${BUILD_UNIT_0_NAME} PRIVATE 
  # User defined linker options
)

set(APP_TRACE_MODE "PRINTF" CACHE STRING "Trace mode: PRINTF or SYSVIEW")
string(TOUPPER "${APP_TRACE_MODE}" APP_TRACE_MODE_UPPER)
if(APP_TRACE_MODE_UPPER STREQUAL "PRINTF")
  target_compile_definitions(${BUILD_UNIT_0_NAME} PRIVATE APP_TRACE_MODE_PRINTF=1)
elseif(APP_TRACE_MODE_UPPER STREQUAL "SYSVIEW")
  target_compile_definitions(${BUILD_UNIT_0_NAME} PRIVATE APP_TRACE_MODE_SYSVIEW=1)
else()
  message(FATAL_ERROR "APP_TRACE_MODE must be PRINTF or SYSVIEW")
endif()

# Add sources to executable/library
target_sources(${BUILD_UNIT_0_NAME} PRIVATE
  # User defined sources
  Core/Src/debug_uart.c
  Core/Src/sysview_uart.c
  ThirdParty/SEGGER/SEGGER/SEGGER_RTT.c
  ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S
  ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.c
  ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.c
  ThirdParty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c
  ThirdParty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.c
)

# Add include directories
target_include_directories(${BUILD_UNIT_0_NAME} PRIVATE
  # User defined include directories
  Core/Inc
  ThirdParty/SEGGER/Config
  ThirdParty/SEGGER/OS
  ThirdParty/SEGGER/SEGGER
)
