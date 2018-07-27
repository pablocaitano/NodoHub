/** @file radio.c
 * Modulo radio
 * 
 * @version 1.0
 * @author  Mariana Siniscalchi <msiniscalchi@fing.edu.uy>
 * @author Juan Martin Chiale <jmchiale05@hotmail.com>
 * @date junio 2017
 */

#include "sx1276.h"
#include "sx1276regs-fsk.h"
#include "sx1276regs-lora.h"

#include "msp430F5438.h"
#include "radio.h"
// hola git
static radio_events_t radio_events;

/*
*Función que escribe por uart para avisar que hubo una transmisión
*/
void OnTxDone() {
  uart_write("$TXS\n");
}
/*
*Función que escribe por uart lo recibio y sus parametros.
*@param:payload: payload recibido
*@param:size:tamaño del payload
*@param:rssi:rssi
*@param:snr:snr
*/
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
  P1OUT |= BIT0;
  uart_write("$RXS,");

  uart_printhex32(rssi);
  uart_writec(',');

  uart_printhex8(snr);
  uart_writec(',');

  uart_printhex32(size);
  uart_writec(',');

  uart_printhex32(*payload);
  uart_writec('\n');
  P1OUT &= ~BIT0;

}
/*
*Función que escribe por uart un error
*/
void OnRxError() {
  uart_write("$RXE\n");
}
/*
*función que inicializa la radio en modo lora
*/

void rf_init_lora() {
  radio_events.TxDone = OnTxDone;
  radio_events.RxDone = OnRxDone;
  //radio_events.TxTimeout = OnTxTimeout;
  //radio_events.RxTimeout = OnRxTimeout;
  radio_events.RxError = OnRxError;

  sx1276_init(radio_events);
  sx1276_set_channel(RF_FREQUENCY);


  sx1276_set_txconfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                  LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                  LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                  true, 0, 0, LORA_IQ_INVERSION_ON, 3000);

  sx1276_set_rxconfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                  LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                  LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                  0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

}
/*
*función que envia un dato por radio
*@param: buffer: puntero al dato a enviar
*/
void Send_data(uint8_t *buffer) {
  sx1276_send(buffer, 1);
}