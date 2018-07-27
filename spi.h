/** @file spi.h
 * Módulo que contiene funciones para comunicación SPI
 * @see referencia a código de prueba disponible en github
 */

#include <stdint.h>

#ifndef SPI_H
#define SPI_H

/** Declaración de buffer spi*/
extern volatile uint8_t spi_buf;

/** Función que configura registros para utilizar SPI
 */
void spi_init();

/**
 * Función para esperar que la transmisión fue realizada
 */
void spi_txready();

/**
 * Función para esperar que la recepción fue realizada
 */
void spi_rxready();

/**Función para enviar un dato
 * @param uint8_t data: dato que se quiere enviar
 */
void spi_send(uint8_t data);

/** Función para almacenar un dato recibido en spi_buf
 */
void spi_recv();

/** Función para trasmitir una dirección de un registros del
 * esclavo y guardar lo almacenado
 * @param uint8_t data: valor de dirección a enviar
*/
void spi_transfer(uint8_t data);

/** Función para habilitar la comunicacion SPI
 */
void spi_chipEnable();
/** Función para deshabilitar la comunicacion SPI
 */
void spi_chipDisable();

#endif
