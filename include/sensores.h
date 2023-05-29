/**
 * Proporciona las funciones necesarias para realizar las medidas de sensores
 * pertinentes.
 * PIN_ACELE y PIN_FRENO deben estar definidos.
 * TODO: Falta documentar algunas funciones. Las constantes deberían estar
 * claramente explicadas, y a poder ser diferenciadas en definiciones.
 * @file sensores.h
*/

#ifndef __SENSORES_H
#define __SENSORES_H

#if !defined(PIN_ACELE) || !defined(PIN_FRENO)
#error "No se han definido los pines de aceleración (PIN_ACELE) y freno (PIN_ACELE)"
#endif

#include <Arduino.h>

///// TIPOS DE DATOS //////

union {
  float f[5];
  int c[10];
} datos;


///// VARIABLES /////

float porAceleracion;
float porFrenada;
unsigned long time;
float deltatime;
float rpm;
float marcha;
float velocidad;
float metros;
float v0 = 90;
float S0 = 2036;
char csv[40];


///// CONFIGURACIÓN ///////

/** Resolución de los sensores */
#define ADC_RES 10

/** El valor máximo de los sensores. Debe ser (2^ADC_RES)-1 */
#define ADC_MAX 1023


///// FUNCIONES ///////

float acelera(float Vant, float porA, float tMuestra);
float frenada(float Vant, float porF, float tMuestra);
float revolucion(float V);
float transmision (float V);
float GPS (float Vant, float V,float Sant, float tMuestra);

/**
 * Configura algunos parámetros de los sensores:
 * - Resolución del ADC. Coge el valor de ADC_RES
*/
void sensores_init() {
#ifdef analogReadResolution
    analogReadResolution(ADC_RES);
#endif
}

/**
 * Realiza la lectura de todos los sensores. Actualiza los datos en datos.
*/
void sensores_leer() {
    time = millis();
    porAceleracion = analogRead(PIN_ACELE);  // Potenciometro 1
    porFrenada = analogRead(PIN_FRENO);      // Potencionmetro 2
    //Los potenciometros se conectan: los extremos a 5v y gnd, la del medio al pin correspondiente

    /////Cálculo de valores con métodos/////
    deltatime = (time - timebefore)*0.001;
    timebefore = time;
    porAceleracion = porAceleracion / ADC_MAX;
    porFrenada = porFrenada / ADC_MAX;
    velocidad = v0 + acelera(v0, porAceleracion, deltatime) + frenada(v0, porFrenada, deltatime);
    marcha = transmision(velocidad);
    rpm = revolucion(velocidad);
    metros = GPS(v0, velocidad, S0, deltatime);

    /////Montado de la trama/////
    datos.f[0] = metros;
    datos.f[1] = velocidad;
    datos.f[2] = porFrenada;
    datos.f[3] = rpm;
    datos.f[4] = marcha;

    /////Guardado de valores anteriores/////
    v0 = velocidad;
    S0 = metros;
}

float acelera(float Vant, float porA, float tMuestra) { 
    Vant = constrain(Vant,0,208);
    float inst = sqrt(360.058258278995/(104*104/(Vant*0.5*Vant*0.5) - 1));
    inst += tMuestra;
    return porA*(208*inst/sqrt(inst*inst + 360.058258278995)-Vant);
}

float frenada(float Vant, float porF, float tMuestra) {
    return porF*(130 + 26.82458782989084*((Vant - 130 + 26.82458782989084 * 4.8463) / 26.82458782989084 - tMuestra - 4.8463) - Vant);
}

float revolucion(float V) {
    if(V <= 50.4)
        return 7000 + 111.11*(V - 63);
    else if((V > 50.4) && (V <= 80.03)) 
        return 7000 + 70*(V - 100);
    else if((V > 80.03) && (V <= 113.8))
        return 7000 + 47.9452*(V - 143);
    else if((V > 113.8) && (V <= 153.6))
        return 7000 + 36.458*(V - 192);
    else if((V > 153.6) && (V <= 242))
        return 7000 + 28.9256*(V - 242);
    else return 0;
}

float transmision (float V) {
    if(V <= 50.4)
        return 1;
    else if((V > 50.4) && (V <= 80.03))
        return 2;
    else if((V > 80.03) && (V <= 113.8))
        return 3;
    else if((V > 113.8) && (V <= 153.6))
        return 4;
    else if((V > 153.6) && (V <= 242))
        return 5;
    else
        return 0;
}

float GPS (float Vant, float V,float Sant, float tMuestra) {
    return Sant + Vant*tMuestra + (V*0.277777778 - Vant*0.277777778)*0.5;
}

#endif
