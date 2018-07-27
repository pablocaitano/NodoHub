//******************************************************************************
//   MSP430F543xA Demo - USCI_A0, SPI 3-Wire Master Incremented Data
//
//   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
//   data is sent by the master starting at 0x01. Received data is expected to
//   be same as the previous transmission.  USCI RX ISR is used to handle
//   communication with the CPU, normally in LPM0. If high, P1.0 indicates
//   valid data reception.  Because all execution after LPM0 is in ISRs,
//   initialization waits for DCO to stabilize against ACLK.
//   ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz.  BRCLK = SMCLK/2
//
//   Use with SPI Slave Data Echo code example.  If slave is in debug mode, P1.1
//   slave reset signal conflicts with slave's JTAG; to work around, use IAR's
//   "Release JTAG on Go" on slave device.  If breakpoints are set in
//   slave RX ISR, master must stopped also to avoid overrunning slave
//   RXBUF.
//
//                   MSP430F5438A
//                 -----------------
//             /|\|                 |
//              | |                 |
//              --|RST          P1.0|-> LED
//                |                 |
//                |             P3.4|-> Data Out (UCA0SIMO)
//                |                 |
//                |             P3.5|<- Data In (UCA0SOMI)
//                |                 |
//  Slave reset <-|P1.1         P3.0|-> Serial Clock Out (UCA0CLK)
//
//
//   M. Morales
//   Texas Instruments Inc.
//   June 2009
//   Built with CCE Version: 3.2.2 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************

#include "msp430F5438.h"
#include "spi.h"

#define SS_DAC P5OUT


volatile uint8_t spi_buf = 0;

#define SCLK    BIT5
#define SDI     BIT6
#define SDO     BIT7
#define CS      BIT0
/*
// table of discrete values for triangular wave
unsigned int triangular_wave[24] = {0,0x50,0xA0,0xF0,0x140,0x190,0x1E0,0x230,0x280,0x2D0,0x320,0x370,
                            0x3C0,0x370,0x320,0x2D0,0x280,0x230,0x1E0,0x190,0x140,0xF0,0xA0,0x50};
volatile int index;

void main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  index = 0;
  P5DIR |= 0x01;                            // /CS
  P5OUT |= 0x01;                            // set chip select high

  P1OUT |= 0x00;                            // Set P1.0 for LED
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  init_timer();
  init_spi();

  // UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  // P1OUT &= ~0x02;                           // Now with SPI signals initialized,
  // P1OUT |= 0x02;                            // reset slave

  // __delay_cycles(100);                      // Wait for slave to initialize


 // while (!(UCA0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  // UCA0TXBUF = MST_Data;                     // Transmit first character
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  __no_operation();                         // For debugger

}
*/


void spi_txready() {
  while (!(UCB0IFG & UCTXIFG)); // TX buffer ready?        //IFG2
}

void spi_rxready() {
  while (!(IFG2 & UCRXIFG)); // RX Received?
}

void spi_send(uint8_t data) {
  spi_txready();
  UCB0TXBUF = data;            // Send data over SPI to Slave
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



void spi_init( void ) // se configura la inicializacion de SPI
{
  
  
  UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**. Sigue abajo la config de CTL1
  
  UCB0CTL0 |= UCMST+UCSYNC+UCMSB;           // pagina 978 manual de familia. Se configura el micro en SPI master, se enviara informacion 
                                            //de modo tal que el primer bit enviado  es el más significativo, 3-pin SPI (no hay STE), 8-bit van a tener los datos
  
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK (NO SE XQ ELEGIRIAMOS ESTE)
                                            
                                            // Frequency CPU / 2 (16Mhz / 2 = 8 Mhz SPI)
  UCB0BR0 = 0x02;                           // /2 ~ 500 kHz
  UCB0BR1 = 0;                              // (la frecuencia del reloj se dividira solo entre 2)
  //UCB0MCTL = 0;                             // No modulation
  P3SEL |= 0x0F;                            // P3.3,2,1,0 clk,somi,simo,ste
  
  /* si falla algo considerar que mariana tmb puso esto
P1SEL  |= SCLK | SDI | SDO;                   // P1.6 is MISO and P1.7 is MOSI
  P1SEL2 |= SCLK | SDI | SDO;

  P1DIR |= SCLK | SDO;
  P1DIR &= ~SDI;

  P2DIR |= CS;// | CS2;                           // P2.0 CS (chip select)
  P2OUT |= CS;// | CS2;

*/
  
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCB0IE |= UCRXIE;                         // Enable USCI_B0 RX interrupt
}
