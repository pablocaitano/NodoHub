#include "uart.h"    
#include "msp430F5438.h"
#include "mcu.h"


/** VARIABLES VIEJAS
static char data_rx[32];
static int indice_rx = 0;


static char data_tx[32]; 
static int indice_tx = 0;
*/

static int length_tx = 0;

void uart_init(){
    
  UCA1CTL1 |= UCSWRST;  // paso 1 tal cual dice el manual SET UCSWRST
  
  UCA1CTL1 |= UCSSEL_2 ;   // 0x41: Elegimos SMCLK (1MHZ se supone), en reset para poder configurar los registros
  UCA1CTL0 = UCMODE_0; // Estamos configurando la palabra de control de la USCI.
  // 0x00: Elegimos sin paridad, LSB primero, 8 bits de data, 1 bit de stop, modo UART y asincrono.   		

  // Configuracion de baud rate y modulacion 
  UCA1BR0  = 65 ;                 //8Mhz - 9600 baudios (UCA1BR0 + UCA1BR1*256 = 833) Ver tabla 36-4 pag 952 slau208q manual
  UCA1BR1 = 0x03 ;
  UCA1MCTL = 0x04 ;             // 0x04 (00000010): Elegimos UCBRF0 = 0, UCBRS0 = 2 y UCOS16 = 0 
 
  // Elijo los puertos 5.6 y 5.7 (11000000) USCI_A1 TXD RXD
  P5SEL = 0xC0 ;

  
  UCA1CTL1 &= ~UCSWRST; // Saco el micro de modo reset
   
  //UCA1IE = UCRXIE + UCTXIE;  // Habilita interrupciones del receptor y del transmisor
  }


/** ***************************************************************************************************
*                               Funciones que manejan la transmision de datos 
***************************************************************************************************** */


void uart_write(char* data_out){
                                // for (i=0;!data_out[i];i++){length_tx++} para contar el largo de la cadena sin strlen  
  length_tx = strlen(data_out);
  for (int i=0;i<length_tx;i++){
    while(!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = data_out[i];
  }
  
}  
void uart_writen(char* data, int n) {
  while(n--) {
    while(!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = *data++;
  }
}

void uart_writec(char data) {
	while(!(UCA1IFG & UCTXIFG));
  UCA1TXBUF = data;
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





