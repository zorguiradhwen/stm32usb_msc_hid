/*
 * printf_override.h
 *
 *  Created on: 27 déc. 2020
 *      Author: radhwen.zorgui
 */

#ifndef PRINTF_OVERRIDE_H_
#define PRINTF_OVERRIDE_H_

#include "usart.h"
#ifndef UART_HANDLER_TYPE
#define UART_HANDLER_TYPE UART_HandleTypeDef
#define UART1_HANDLER huart1
extern UART_HANDLER_TYPE UART1_HANDLER;
#endif


#ifndef Uart_send
#define Uart_send(uarthandler, string, len) \
	HAL_UART_Transmit(uarthandler, (uint8_t *)string, len, 0xffff)
#endif


typedef void* ComHandlerType;
typedef enum ComType_e
{
	COM_UART, COM_USB
}ComType;

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE;


_Bool printf_init();
_Bool printf_changeCom(ComType _comtype, ComHandlerType _comHandler);


#endif /* PRINTF_OVERRIDE_H_ */
