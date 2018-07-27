/** @file spi.h
 * M�dulo que contiene funciones para comunicaci�n SPI
 * @see referencia a c�digo de prueba disponible en github
 */

#include <stdint.h>

#ifndef SPI_H
#define SPI_H

/** Declaraci�n de buffer spi*/
extern volatile uint8_t spi_buf;

/** Funci�n que configura registros para utilizar SPI
 */
void spi_init();

/**
 * Funci�n para esperar que la transmisi�n fue realizada
 */
void spi_txready();

/**
 * Funci�n para esperar que la recepci�n fue realizada
 */
void spi_rxready();

/**Funci�n para enviar un dato
 * @param uint8_t data: dato que se quiere enviar
 */
void spi_send(uint8_t data);

/** Funci�n para almacenar un dato recibido en spi_buf
 */
void spi_recv();

/** Funci�n para trasmitir una direcci�n de un registros del
 * esclavo y guardar lo almacenado
 * @param uint8_t data: valor de direcci�n a enviar
*/
void spi_transfer(uint8_t data);

/** Funci�n para habilitar la comunicacion SPI
 */
void spi_chipEnable();
/** Funci�n para deshabilitar la comunicacion SPI
 */
void spi_chipDisable();

#endif
