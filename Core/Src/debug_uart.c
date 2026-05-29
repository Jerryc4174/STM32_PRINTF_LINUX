#include "debug_uart.h"

#include "main.h"

static void DebugUart_WriteByte(uint8_t byte)
{
  while ((USART3->SR & USART_SR_TXE) == 0U)
  {
  }

  USART3->DR = byte;
}

void DebugUart_Init(void)
{
  uint32_t peripheral_clock_hz;

  __HAL_RCC_USART3_CLK_ENABLE();

  USART3->CR1 = 0U;
  USART3->CR2 = 0U;
  USART3->CR3 = 0U;

  peripheral_clock_hz = HAL_RCC_GetPCLK1Freq();
  USART3->BRR = (peripheral_clock_hz + (115200U / 2U)) / 115200U;
  USART3->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void DebugUart_Write(const uint8_t *buffer, size_t length)
{
  size_t index;

  for (index = 0; index < length; ++index)
  {
    if (buffer[index] == '\n')
    {
      DebugUart_WriteByte('\r');
    }

    DebugUart_WriteByte(buffer[index]);
  }

  while ((USART3->SR & USART_SR_TC) == 0U)
  {
  }
}