/** @file uart.c
 * Módulo que agrupa funciones para comunicación por uart
 */
#include "msp430F5438.h"
//#include <msp430.h>
#include "uart.h"
#include "mcu.h"

void uart_init() {
 UCA1CTL1 |= UCSWRST;  // paso 1 tal cual dice el manual SET UCSWRST
  
  UCA1CTL1 = UCSSEL_1 | UCSWRST;   // 0x41: Elegimos ACLK (32768 Hz), en reset para poder configurar los registros
  UCA1CTL0 = UCMODE_0; // Estamos configurando la palabra de control de la USCI.
  // 0x00: Elegimos sin paridad, LSB primero, 8 bits de data, 1 bit de stop, modo UART y asincrono.   		

  // Configuracion de baud rate y modulacion 
  UCA1BR0  = 0x03 ;                 //32Khz / 9600  Ver tabla 36-4 pag 952 slau208q manual
  UCA1BR1 = 0x00 ;
  UCA1MCTL = UCBRS_3;             // 0x06 (00000110): Elegimos UCBRF0 = 0, UCBRS0 = 3 y UCOS16 = 0 
 
  // Elijo los puertos 5.6 y 5.7 (11000000) USCI_A1 TXD RXD
  P5SEL = 0xC0 ;

  
  UCA1CTL1 &= ~UCSWRST; // Saco el micro de modo reset
   
  UCA1IE = UCRXIE;  // Habilita interrupciones del receptor y deshabilita las de transmisor
}

void uart_write(char* str) {
  for(int i = 0; str[i] != '\0'; i++) {
    while (!(IFG2 & UCA0TXIFG));    // TX buffer ready?
    UCA0TXBUF = str[i];
  }
}

void uart_writen(char* data, int n) {
  while(n--) {
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = *data++;
  }
}

void uart_writec(char data) {
  while (!(IFG2 & UCA0TXIFG));
  UCA0TXBUF = data;
}

void uart_printhex8(uint8_t n) {
  char buf[2 + 1];
  char *str = &buf[3 - 1];

  *str = '\0';

  uint8_t base = 16;

  do {
    uint8_t m = n;
    n /= base;
    char c = m - base * n;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  uart_write(str);
}

void uart_printhex32(uint32_t n) {
  char buf[8 + 1];
  char *str = &buf[9 - 1];

  *str = '\0';

  uint32_t base = 16;

  do {
    uint32_t m = n;
    n /= base;
    char c = m - base * n;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  uart_write(str);
}
