#include "uart.h"



static char data_rx[32];
static int indice_rx = 0;

static char data_tx[32]; //< Buffer de transmision
static int indice_tx = 0;
static int length_tx = 0;

void ini_uart(){
    
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




/** Funcion de atencion a interrupcion */  
#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(){ 
	
  switch(__even_in_range(UCA1IV,4)){
    
  case 0:break;                             // Vector 0 - no interrupt
  case 2:           // Vector 2 - RXIFG
    if ((data_rx[indice_rx] = UCA1RXBUF) =='\r'){
 
      UCA1IE &= ~UCRXIE;// Al recibir el ultimo dato deshabilito las interrupciones
      rx_flag = 1;  
  }else{    	
	indice_rx++;
  }
  break;
  case 4:    // Vector 4 - TXIFG
	if (indice_tx >= length_tx){
  
          if (indice_tx == length_tx){
   			UCA1TXBUF = '\0'; // manda un null
      		indice_tx++;
          }else{
   		   	UCA1TXBUF = '\r'; // manda un fin de linea
			UCA1IE &= ~UCTXIE; // deshabilitamos las interrupciones del transmisor
      		//tx_flag = 1; // Puedo enviar un nuevo dato
          }
	}else{    
    	UCA1TXBUF = data_tx[indice_tx];
    	indice_tx++;
  	}
  break;
  default: break;
  }

 }

/** ***************************************************************************************************
*                               Funcion que maneja la recepcion de datos 
***************************************************************************************************** */

void uart_rx(char* data_in){
 
  for (int i=0;i < indice_rx + 1; i++){ // o i < indice + 1
      data_in[i] = data_rx[i]; // buffer de entrada.  
      
  }
  indice_rx = 0;
  UCA1IE |= UCRXIE; // vuelvo a habilitar las interrupciones del receptor
  rx_flag = 0;
  
}  


/** ***************************************************************************************************
*                               Funcion que maneja la transmision de datos 
***************************************************************************************************** */


void uart_tx(char* data_out){
                                // for (i=0;!data_out[i];i++){length_tx++} para contar el largo de la cadena sin strlen  
  length_tx = strlen(data_out);
  
  for (int i=0;i<length_tx;i++){
    
    data_tx[i] = data_out[i];
  }
  
  UCA1TXBUF = data_tx[0];
  indice_tx = 1;
  
  UCA1IE |= UCTXIE; // habilitamos las interrupciones del transmisor
  tx_flag = 0;
}  
