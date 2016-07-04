// Implementación de las funciones de la librería "sensor"
#include <arduino.h>
#include "sensor.h"

//---------------------------------------
// Definición de pines 
#define PIN_FRENO_IZQ 51
#define PIN_FRENO_DER 53
#define PIN_PAS 2
#define PIN_VELOCIMETRO 3
// Interrupciones
#define INTERRUPCION_PAS 0
#define INTERRUPCION_VEL 1

//---------------------------------------
boolean sensor_freno_izq = 0;
boolean sensor_freno_der = 0;
boolean sensor_pas = 0;
boolean sensor_vel = 0;
// Valores para controlar el funcionamiento del PAS y velocímetro
unsigned int actual_millis;
byte pas_minimo = 64;
byte vel_minimo = 100;
unsigned int pas_maximo = 500;
unsigned int vel_maximo = 3000;
// Valores para obtener la velocidad y la aceleración
unsigned int sensor_dist = 0; // Distancia recorrida (pulsos)
unsigned int sensor_revoluciones = 0; // Vueltas recorridas (pulsos)
unsigned int tiempo_vuelta = 0; // Milisegundos / vuelta
unsigned int tiempo_pas = 0; // Milisegundos / pulso
unsigned long velocidad = 0;
long aceleracion = 0;
unsigned long vel_anterior = 0;
// Contadores
unsigned int count_loop = 0; // Contador total
unsigned int count_marcha = 0; // Contador bici marcha
unsigned int count_pedalea = 0; // Contador pedaleando
// Conversores
static long circunf_bici = 200; // Metros
static long metro_km = 1000; // Conversión metros -- kilómetros
static long miliseg_seg = 1000; // Conversión milisegundos -- segundos
static long seg_hora = 3600; // Conversión segundos -- horas

//---------------------------------------

// Interrupción de funcionamiento del PAS
static unsigned long tt_pas = 0;
void pasInterrupcion(){
  unsigned long t = millis();
  unsigned long d = t - tt_pas;
  if(d < pas_minimo) return;
  tiempo_pas = d < 65536 ? d : 65535;
  tt_pas = t;
  sensor_revoluciones++;
}
// Indica si se está pedaleando
boolean pasLectura(){
  return actual_millis - tt_pas < pas_maximo;
}

// Interrupción de funcionamiento del velocímetro
static unsigned long tt_vel = 0;
void velInterrupcion(){
  unsigned long t = millis();
  unsigned long d = t - tt_vel;
  if(d < vel_minimo) return;
  tiempo_vuelta = d < 65536 ? d : 65535;
  tt_vel = t;
  sensor_dist++;
}
// Indica si la bicicleta está funcionando
boolean velLectura(){
  return actual_millis - tt_vel < vel_maximo;
}

//----------------------------------------------------

void setupSensores(){
  pinMode(PIN_FRENO_IZQ, INPUT_PULLUP);
  pinMode(PIN_FRENO_DER, INPUT_PULLUP);
  pinMode(PIN_PAS, INPUT_PULLUP);
  pinMode(PIN_VELOCIMETRO, INPUT_PULLUP);
  // Interrupciones
  attachInterrupt(INTERRUPCION_VEL, velInterrupcion, FALLING);
  attachInterrupt(INTERRUPCION_PAS, pasInterrupcion, FALLING);
}

void lecturaSensores(){
  delay(60);
  delayMicroseconds(int(124) << 2);
  actual_millis = millis();
  count_loop++; 
  // Lectura del PAS y velocímetro
  if(count_pedalea < 65535) count_pedalea++;
  if(count_marcha  < 65535) count_marcha++;
  sensor_pas = pasLectura();
  sensor_vel = velLectura();
  if(!sensor_pas){
    count_pedalea = 0;
    tiempo_pas = 0;
  }
  if(!sensor_vel){
    count_marcha = 0;
    tiempo_vuelta = 0;
  }
  // Lectura de los frenos
  sensor_freno_der = digitalRead(PIN_FRENO_DER) == 0;
  sensor_freno_izq = digitalRead(PIN_FRENO_IZQ) == 0;
}

void obtenerVelocidad(){
  vel_anterior = velocidad;
  velocidad = (circunf_bici/metro_km) / (tiempo_vuelta/(miliseg_seg * seg_hora));
}

void obtenerAceleracion(){
  long dif_velocidad = velocidad - vel_anterior;
  aceleracion = (dif_velocidad*(metro_km/seg_hora)) / (tiempo_vuelta/miliseg_seg);
}




