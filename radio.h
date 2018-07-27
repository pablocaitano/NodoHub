/** @file radio.h
 * Modulo radio
 * Este módulo inicializa la radio y permite transmitir daots a través del enlace
 * 
 * @version 1.0
 * @author  Mariana Siniscalchi <msiniscalchi@fing.edu.uy>
 * @author Juan Martin Chiale <jmchiale05@hotmail.com>
 * @date junio 2017
 */

#define RF_FREQUENCY   868000000 // Hz

#define FSK_FDEV                          25e3      // Hz
#define FSK_DATARATE                      50e3      // bps
#define FSK_BANDWIDTH                     50e3      // Hz
#define FSK_AFC_BANDWIDTH                 83.333e3  // Hz
#define FSK_PREAMBLE_LENGTH               5         // Same for Tx and Rx
#define FSK_FIX_LENGTH_PAYLOAD_ON         false

#define LORA_BANDWIDTH                              0         // [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved]
#define LORA_SPREADING_FACTOR                       12        // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         3         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#define RX_TIMEOUT_VALUE                  1000
#define TX_OUTPUT_POWER                   14        // dBm
#define BUFFER_SIZE                       32 // Define the payload size here
#define LED1 (0x0001)


/** Funcion de inicialización de la radio usando LoRa
 */
void rf_init_lora();

/** Funcion de transmisión de un paquete */
void Send_data();