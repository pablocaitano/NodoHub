/** @file spi.c
 * Módulo que contiene funciones para comunicación SPI
 * Editado para F5438
 */

#include "msp430F5438.h"
#include <stdint.h>
#include "spi.h"

volatile uint8_t spi_buf = 0;

#define SCLK    BIT3
#define SOMI    BIT2
#define SIMO    BIT1
#define CS      BIT0



void spi_init() {
  UCB0CTL1 = UCSWRST;

  UCB0CTL0 = UCSYNC + UCMST + UCMSB + UCCKPH;   // 3-pin, 8-bit SPI master
  UCB0CTL1 |= UCSSEL_2;                         // SMCLK
  UCB0BR0 = 0x02;                               // Frequency CPU / 2 (16Mhz / 2 = 8 Mhz SPI)       MENTIRA, QUEDA 500K
  UCB0BR1 = 0;

  P3SEL  |= SCLK | SOMI | SIMO;                 // P3.2 is MISO and P3.1 is MOSI
 // P3SEL2 |= SCLK | SOMI | SIMO;					// P3.3 is SCLK

  P3DIR |= SCLK | SIMO;							// SCLK, SIMO  output
  P3DIR &= ~SOMI;								// SOMI input

  P2DIR |= CS;// | CS2;                           // P2.0 CS (chip select)
  P2OUT |= CS;// | CS2;

  UCB0CTL1 &= ~UCSWRST;                         // Initialize USCI state machine

  UCB0IE &= ~UCRXIE; 
  UCB0IE &= ~UCTXIE; 
}

void spi_txready() {
  while (!(UCB0IFG & UCTXIFG)); // TX buffer ready?
}

void spi_rxready() {
  while (!(UCB0IFG & UCRXIFG)); // RX Received?
}

void spi_send(uint8_t data) {
  spi_txready();
  UCB0TXBUF = data;   // Send data over SPI to Slave
  //UCB0IE |= UCTXIE;  
}

void spi_recv() {
  spi_rxready();
  spi_buf = UCB0RXBUF;         // Store received data
}

void spi_transfer(uint8_t data) {
  spi_send(data);
  spi_recv();
}

void spi_chipEnable() {
  P2OUT &= ~CS;
}

void spi_chipDisable() {
   P2OUT |= CS;
}
