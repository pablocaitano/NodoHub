/** @file main.c
 * 
 * Programa principal
 * 
 * @version 1.0
 * @author Juan Martin Chiale <jmchiale05@hotmail.com>
 * @author Mariana Siniscalchi <msiniscalchi@fing.edu.uy>
 * @date junio 2017
 */

#include "msp430F5438.h"
#include <stdint.h>
#include "mcu.h"
#include "uart.h"
#include "spi.h"
#include "radio.h"
#include "sx1276.h"
#include "sx1276regs-fsk.h"
#include "sx1276regs-lora.h"

//uint8_t buffer[BUFFER_SIZE];

char buffer_recibido[BUFFER_SIZE];
int state = 0;

/** Función main
 * Encargada de inicializar todo el sistema, utilizando las funciones de inicialización de los módulos
 * radio, uart y mcu. Ejecuta un loop infinito vacío. La dinámica de la aplicación viene dada por las
 * interrupciones al recibir un nuevo paquete. De ser así, se trasmite un reconocimiento, se vuelve a
 * modo de recepción y se imprime por UART el contenido del paquete recibido. Esto se hace utilizando
 * las funciones send_data y set_rx del módulo radio y la función uart_write del módulo uart.
 */
   
  
void main(void) {
  
 
  
  mcu_init();

  //P1DIR |= BIT0; // puerto 1 va a ser de I/O

  uart_init();
  uart_write("\n\n");
  spi_init();
  rf_init_lora();
  
  uart_write("NODO HUB");
  uart_writec('\r');
  uart_write("Radio inicializada...");
  uart_writec('\r'); 
  uart_write("SX1276 version:");
  //static int hola;
 // hola=1;
 // hola=sx1276_read(REG_VERSION);
    uart_printhex8(sx1276_read(REG_VERSION));
  uart_writec('\r');
 
  while(1) {
    
   sx1276_set_rx(0);
  }

}
