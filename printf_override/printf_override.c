/*
 * printf_override.c
 *
 *  Created on: 27 déc. 2020
 *      Author: radhwen.zorgui
 */

//#include "Hal.h"
#include "printf_override.h"



static ComHandlerType* chandler = NULL;
static ComType comType = COM_UART;

/*
 * printf overload
 */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
	if (comType == COM_UART)
	{
		UART_HANDLER_TYPE *uarthandler = (UART_HANDLER_TYPE*) chandler;
		Uart_send(uarthandler, (uint8_t *)&ch, 1);
	}

  return ch;
}


_Bool printf_changeCom(ComType _comtype, ComHandlerType _comHandler)
{
	chandler = _comHandler;
	comType = _comtype;
	return 1u;
}
_Bool printf_init()
{
	printf_changeCom(COM_UART, &UART1_HANDLER);
	return 1u;
}


