/* */
#include <msp430.h>

unsigned char MST_Data,SLV_Data;

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  //P1OUT |= 0x02;                            // Set P1.0 for LED
                                            // Set P1.1 for slave reset
  P3SEL |= 0x0F;                            // P3.3,2,1,0 clk,somi,simo,ste

  UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCB0CTL0 |= UCCKPL+UCMST+UCSYNC+UCMODE1;  // 8-bit SPI master, 
											//4-pin SPI UCxSTE = 1
                                            // Clock polarity high, LSB first
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 0x02;                           // /2
  UCB0BR1 = 0;                              //
  UCB0MCTL = 0;                             // No modulation
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCB0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

//no se si quiero resetear el LoRa
  //P1OUT &= ~0x02;                      // Now with SPI signals initialized,
  //P1OUT |= 0x02;                       // reset slave

  __delay_cycles(100);                      // Wait for slave to initialize

  MST_Data = 0x01;                          // Initialize data values
  SLV_Data = 0x00;                          //

  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = MST_Data;                     // Transmit first character

  __bis_SR_register(LPM0_bits + GIE);       // CPU off, enable interrupts
}

#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(__even_in_range(UCB0IV,4))
  {
    case 0: break;                          // Vector 0 - no interrupt
    case 2:                                 // Vector 2 - RXIFG
      while (!(UCB0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?

      MST_Data++;                           // Increment data
      SLV_Data++;
      UCB0TXBUF = MST_Data;                 // Send next value

      __delay_cycles(40);                   // Add time between transmissions to
                                            // make sure slave can process information
      break;
    case 4: break;                          // Vector 4 - TXIFG
    default: break;
  }
}
