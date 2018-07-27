

/** 
*******************************************************************
*       Encabezado del modulo UART 
*******************************************************************
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include <msp430.h>

#ifndef UART_H
#define UART_H
/** @brief Funcion de inicializacion de la UART */
/**
* Esta funcion inicializa los registros de configuracion de la UART. 
*/ 

void uart_init();


/** @brief Funcion de transmision de datos */
/**
* Función para escribir una cadena de caracteres por UART
* @param char* data_out: Puntero a buffer con el dato a transmitir
*/
void uart_write(char* data_out);

/** Función para escribir un caracter por UART
 * @param data: caracter
 */
void uart_writec(char data);

/** Función para escribir un valor por UART en hexa de 8 bits
 * @param n: valor
 */
void uart_printhex8(uint8_t n);

/** Función para escribir un valor por UART en hexa de 32 bits
* @param n: valor
*/
void uart_printhex32(uint32_t n);

#endif



