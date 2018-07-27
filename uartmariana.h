/**
 * @file uart.h
 * 
 * Modulo uart
 * Este modulo implementa las funciones para la comunicación uart.
 *
 * @see referencia a código de prueba disponible en github
 */
#include <stdint.h>

#ifndef UART_H
#define UART_H
/** Función para inicializar la uart
 */
void uart_init();

/** Función para escribir una cadena de caracteres por uart
 * 
 * @param str[]: cadena de caracteres
 */
void uart_write(char str[]);

/** Función para escribir una cadena de caracteres de largo n
 * 
 * @param str: puntero a cadena de caracteres
 * @param n: largo
 */
void uart_writen(char* str, int n);

/** Función para escribir un caracter por uart
 * @param data: caracter
 */
void uart_writec(char data);

/** Función para escribir un valor por uart en hexa de 8 bits
 * @param n: valor
 */
void uart_printhex8(uint8_t n);

/** Función para escribir un valor por uart en hexa de 32 bits
* @param n: valor
*/
void uart_printhex32(uint32_t n);

#endif
