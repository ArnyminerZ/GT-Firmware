/**
 * Proporciona todas las funciones necesarias para comunicarse con la central
 * a través de LoRa con el módulo RN2483.
 * @file comunicaciones.h
 */

#ifndef __COMUNICACIONES_H
#define __COMUNICACIONES_H

#include <Arduino.h>

///// ENUMS ///////

enum RFError {
    /** No ha habido ningun error. */
    OK,
    /** Si ha habido algun error en el formato de los datos a mandar. */
    TX_PARAM,
    /** Si se ha intentado transmitir cuando el módulo estaba ocupado. */
    TX_BUSY,
    /** Ha habido algun error durante la transmisión (timeout). */
    TX_ERROR,
};

/** Opciones de modulación RF */
enum RFMod { LORA, FSK };


///// CONSTANTES ///////

#define MOD_LORA "lora"
#define MOD_FSK "fsk"


///// VARIABLES ///////

String rf_data;


///// CONFIGURACIÓN ///////

/**
 * El puerto serie a utilitzar.
 * 
 * Esta definición permite cambiar de puerto con
 * facilidad. Por ejemplo, si se desea usar SoftwareSerial, sería suficiente
 * con establecer el valor a la variable que referencia la instance de la
 * librería.
*/
#define COMM_SERIAL Serial


///// FUNCIONES ///////

/**
 * Inicializa el módulo RF. Los parámetros por defecto son:
 * - Stack LoRaWAN: Desconectado
 * - Modulación: FSK
 * - Potencia:   14
 * - Bitrate:    19000
 * TODO: Se debería comprobar la respuesta del módulo para cada comando.
 * @param mod La modulación que usar. FSK o LORA
 * @param potencia La potencia a usar, va de 1 a 14.
 * @param bitrate El bitrate de la comunicación, revisar documentación.
 */
void rf_init(RFMod mod = FSK, uint8_t potencia = 14, uint16_t bitrate = 19000) {
    // Esperamos al mensaje de bienvenida
    while (COMM_SERIAL.available() == 0);
    rf_data = COMM_SERIAL.readStringUntil('\n');

    ///// LORAWAN /////
    COMM_SERIAL.println("mac pause");               // Pausamos el stack LoRaWAN
    while (COMM_SERIAL.available() == 0);
    rf_data = COMM_SERIAL.readStringUntil('\n');

    ///// MODULACIÓN /////
    COMM_SERIAL.print("radio set mod ");       // Modulación: LoRa o FSK
    if (mod == LORA)
        COMM_SERIAL.println(MOD_LORA);
    else
        COMM_SERIAL.println(MOD_FSK);
    rf_data = COMM_SERIAL.readStringUntil('\n');

    ///// POTENCIA /////
    COMM_SERIAL.print("radio set pwr ");        // Poténcia: de 1 a 14,
    COMM_SERIAL.println(constrain(potencia, 1, 14));
    while (COMM_SERIAL.available() == 0);
    rf_data = COMM_SERIAL.readStringUntil('\n');

    ///// BITRATE /////
    COMM_SERIAL.print("radio set bitrate "); // Bit rate: (consultar datasheet)
    COMM_SERIAL.println(bitrate);
    while (COMM_SERIAL.available() == 0);
    rf_data = COMM_SERIAL.readStringUntil('\n');
}

/**
 * Envia los datos por el puerto serie. Espera a una respuesta, y la guarda en `rf_data`.
 * El LED integrado estará encendido mientras se estén mandando los datos.
 * @param dato El mensaje a mandar.
 * @return OK si no ha habido ningun error. O un error en caso contrario. Errores
 * posibles: TX_PARAM, TX_BUSY, TX_ERROR
*/
RFError rf_send(char dato[]) {
    digitalWrite(LED_BUILTIN, LOW);

    // Mandamos el mensaje con el comando "radio tx"
    COMM_SERIAL.print("radio tx ");
    COMM_SERIAL.println(dato);

    // Leemos la primera respuesta, esta indica si el comando es válido. Las respuestas
    // pueden ser:
    // - ok:            El comando es correcto, se mandará
    // - invalid_param: El parámetro (dato) no es válido
    // - busy:          El módulo está ocupado ahora mismo
    while (COMM_SERIAL.available() == 0);
    rf_data = COMM_SERIAL.readStringUntil('\n');

    // Si ha habido algun error, lo devolvemos
    if (rf_data == "invalid_param")
        return TX_PARAM;
    else if (rf_data == "busy")
        return TX_BUSY;

    // La segunda respuesta nos informa del resultado de la transmisión. Las respuestas
    // pueden ser:
    // - radio_tx_ok: El mensaje se ha mandado correctamente
    // - radio_err:   Ha habido un error durante la transmisión (devuelto por el Watchdog)
    while (COMM_SERIAL.available() == 0);
    rf_data = COMM_SERIAL.readStringUntil('\n');

    // Si ha habido algun error, lo devolvemos
    if (rf_data == "radio_err")
        return TX_ERROR;

    digitalWrite(LED_BUILTIN, HIGH);

    return OK;
}

#endif