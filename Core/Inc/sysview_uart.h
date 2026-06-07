#ifndef SYSVIEW_UART_H
#define SYSVIEW_UART_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void SysViewUart_Init(void);
int SysViewUart_Send(const uint8_t *data, size_t length);
int SysViewUart_HasData(void);
unsigned int SysViewUart_Read(uint8_t *data, unsigned int length);

#ifdef __cplusplus
}
#endif

#endif /* SYSVIEW_UART_H */
