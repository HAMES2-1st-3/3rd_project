/*
 * Driver_USB.c
 *
 *  Created on: 2024. 5. 12.
 *      Author: jio
 */




/***********************************************************************/
/*Include*/
/***********************************************************************/
#include <DeviceDriver/Driver_USB.h>
/***********************************************************************/
/*Define*/
/***********************************************************************/
#define USB_UART_MAX_PRINT_SIZE (255)
/***********************************************************************/
/*Typedef*/
/***********************************************************************/

/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/

/***********************************************************************/
/*Variable*/
/***********************************************************************/

/***********************************************************************/
/*Function*/
/***********************************************************************/


void usb_printf(pchar format,...)
{
    char      message[USB_UART_MAX_PRINT_SIZE + 1];
    Ifx_SizeT count;
    va_list   args;
    va_start(args, format);
    vsprintf((char *)message, format, args);
    va_end(args);
    count = (Ifx_SizeT)strlen(message);
    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, count < STDIF_DPIPE_MAX_PRINT_SIZE);
    usb_print_string(message, USB_UART_MAX_PRINT_SIZE);
    //return
}



void usb_print_string(char *str, uint32 max_len) {
    uint32 count = 0;

    char ch = *str;
    while(ch != 0 && count < max_len) {
        if(ch == '\n') {
            write_asclin3_uart('\r');
            write_asclin3_uart('\n');
        } else {
            write_asclin3_uart(ch);
        }
        str++;
        max_len++;
        ch = *str;
    }
}

void usb_init() {
    init_asclin3();
}




