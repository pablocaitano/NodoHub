#include "uart.h"           // CAMBIAR UNA VEZ QUE MODIFIQUEMOS LOS ARCHIVOS
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
   
  UCA1IE = UCRXIE + UCTXIE;  // Habilita interrupciones del receptor y del transmisor
  }




/** Funcion de atencion a interrupcion    
#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(){ 
	
  switch(__even_in_range(UCA1IV,4)){
    
  case 0:		break;                             // Vector 0 - no interrupt
  case 2:           // Vector 2 - RXIFG
    if ((data_rx[indice_rx] = UCA1RXBUF) =='\r'){
 
      UCA1IE &= ~UCRXIE;// Al recibir el ultimo dato deshabilito las interrupciones
      rx_flag = 1;  
  }else{    	
	indice_rx++;
  }
  break;
  case 4:		break;
  default:	break;
  }

 }
*/

/** ***************************************************************************************************
*                               Funcion que maneja la recepcion de datos 
***************************************************************************************************** */
/*
void uart_read(char* data_in){
 
  for (int i=0;i < indice_rx + 1; i++){ // o i < indice + 1
      data_in[i] = data_rx[i]; // buffer de entrada.  
      
  }
  indice_rx = 0;
  UCA1IE |= UCRXIE; // vuelvo a habilitar las interrupciones del receptor
  rx_flag = 0;
  
}  
*/

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
    UCA0TXBUF = *data++;
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





