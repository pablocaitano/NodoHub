/** @file mcu.h
 * Modulo mcu
 * 
 * @see referencia a código de prueba disponible en github
 */

#include <stdint.h>

#ifndef MCU_H
#define MCU_H



void mcu_init();
void mcu_delayms(uint32_t ms);
void mcu_delayus(uint32_t us);
void mcu_memcpy1(uint8_t *dst, const uint8_t *src, uint16_t size);

#endif
