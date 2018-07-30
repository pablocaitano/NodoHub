/** @file mcu.c
 * Modulo mcu
 */

#include "msp430F5438.h"
//#include <msp430.h>
#include <stdint.h>
#include "mcu.h"
// seguimos editando
void mcu_init() {
  WDTCTL = WDTPW + WDTHOLD;   // Disable watchdog
  // VIEJO BCSCTL1 = CALBC1_16MHZ;     // Set range
  // VIEJO DCOCTL  = CALDCO_16MHZ;     // Set DCO step + modulation
int h;
  P1OUT &= 0x00;              // Shut down everything
  P1DIR &= 0x00;

  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
  UCSCTL2 = FLLD_1 + 244;                   // Set DCO Multiplier for 8MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (244 + 1) * 32768 = 8MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
  __delay_cycles(250000);

  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag


}

void mcu_delayms(uint32_t ms) {
  while (ms) {
    __delay_cycles(16 * 998);
  	ms--;
  }
}

void mcu_delayus(uint32_t us) {
	while (us) {
		__delay_cycles(14); //for 16MHz
		us--;
  }
}

void mcu_memcpy1(uint8_t *dst, const uint8_t *src, uint16_t size) {
    while(size--) *dst++ = *src++;
}
