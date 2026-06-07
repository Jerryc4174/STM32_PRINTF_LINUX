#include "sysview_uart.h"

#include "main.h"

//#define SYSVIEW_UART_BAUDRATE 115200U
#define SYSVIEW_UART_BAUDRATE 500000U
void SysViewUart_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  uint32_t peripheral_clock_hz;

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_USART3_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  USART3->CR1 = 0U;
  USART3->CR2 = 0U;
  USART3->CR3 = 0U;

  peripheral_clock_hz = HAL_RCC_GetPCLK1Freq();
  USART3->BRR = (peripheral_clock_hz + (SYSVIEW_UART_BAUDRATE / 2U)) / SYSVIEW_UART_BAUDRATE;
  USART3->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

int SysViewUart_Send(const uint8_t *data, size_t length)
{
  size_t index;

  for (index = 0; index < length; ++index)
  {
    while ((USART3->SR & USART_SR_TXE) == 0U)
    {
    }

    USART3->DR = data[index];
  }

  return (int)length;
}

int SysViewUart_HasData(void)
{
  return ((USART3->SR & USART_SR_RXNE) != 0U) ? 1 : 0;
}

unsigned int SysViewUart_Read(uint8_t *data, unsigned int length)
{
  unsigned int count;

  count = 0U;
  while (count < length)
  {
    if ((USART3->SR & USART_SR_RXNE) == 0U)
    {
      break;
    }

    data[count] = (uint8_t)(USART3->DR & 0xFFU);
    count++;
  }

  return count;
}
