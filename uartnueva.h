

/** 
*******************************************************************
*       Encabezado del modulo UART 
*******************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <msp430.h>


extern int rx_flag; /**< flag que avisa que el dato esta completo */
extern int tx_flag; /**< flag que avisa que ya se termino de transmitir una cadena */

/** @brief Funcion de inicializacion de la UART */
/**
* Esta funcion inicializa los registros de configuracion de la UART. 
*/ 

void ini_uart();


/** @brief Funcion de recepcion de datos */
/**
* Esta funcion recibe un puntero a un buffer, 
* donde guarda el dato recibido por la UART
* @param char* data_in Puntero a buffer donde guardar el dato recibido
*/
void uart_rx(char* data_in);

/** @brief Funcion de transmision de datos */
/**
* Esta funcion recibe un puntero a un buffer,
* que guarda el dato a transmitir por la UART
* @param char* data_out Puntero a buffer con el dato a transmitir
*/
void uart_tx(char* data_out);

