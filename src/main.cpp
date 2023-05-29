#include <Arduino.h>

/** Pin de salida del Arduino que conecta con el RESET del módulo RF. Ver README.md */
#define RF_RESET 2

/** El pin al cual está conectado el potenciómetro del acelerador. */
#define PIN_ACELE A0
/** El pin al cual está conectado el potenciómetro del freno. */
#define PIN_FRENO A1

unsigned long timebefore;

// Incluímos las funciones de comunicaciones
#include "comunicaciones.h"

// Y las de sensores
#include "sensores.h"

void setup() {
  Serial.begin(57600); // Inicializamos el monitor serie
  while(!Serial);
  
  // Configuración de pines
  pinMode(LED_BUILTIN, OUTPUT);    // Configuramos la salida del LED interno
  pinMode(RF_RESET, OUTPUT);       // Pin 2 del arduino al pin de RST del módulo RF
  // * nota: los pines analógicos no es necesario configurarlos ya que son salidas
  //         por defecto.

  // Establecemos los valores por defecto de los pines
  digitalWrite(LED_BUILTIN, HIGH); // Apagamos el LED interno
  digitalWrite(RF_RESET, LOW);     // Desconectamos el RESET
  digitalWrite(RF_RESET, HIGH);    // Conectamos el RESET

  ///// Configuración del módulo RF /////
  rf_init();

  ///// Configuración de los sensores /////
  sensores_init();

  timebefore = millis();
}

void loop() {
  sensores_leer();
  
  /////Envio de la trama/////
  sprintf(csv, "%04X%04X%04X%04X%04X%04X%04X%04X%04X%04X", datos.c[0], datos.c[1], datos.c[2], datos.c[3], datos.c[4], datos.c[5], datos.c[6], datos.c[7], datos.c[8], datos.c[9]);
  rf_send(csv);
}
