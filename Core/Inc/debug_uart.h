#ifndef DEBUG_UART_H
#define DEBUG_UART_H

#include <stddef.h>
#include <stdint.h>

void DebugUart_Init(void);
void DebugUart_Write(const uint8_t *buffer, size_t length);

#endif