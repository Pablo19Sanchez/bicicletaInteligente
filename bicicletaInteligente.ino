/* BICICLETA INTELIGENTE. Pablo Sánchez Carmona */
#include "sensor.h"
#include "pendiente.h"
#include "bluetooth.h"

// Definición de pines 
#define PIN_ACELERADOR 4 // Señal PWM
// Ponderación (para el cálculo de la aceleración)
#define A1 0.5
#define A2 0.2
#define A3 0.1
#define TASA 0.5
// Definición de variables para controlar el algoritmo
static unsigned long velocidad_minima = 5;
static unsigned long velocidad_paseo = 15;
static unsigned long velocidad_crucero = 25;
// Definición de variables para la aceleración
byte acelerador = 0;
byte acelerador_deseable = 0;
byte incremento_acelerador = 0;
byte acelerador_final = 0;

//----------------------------------------

void setup() {
  analogWrite(PIN_ACELERADOR, 0);
  setupSensores();
  establecerMPU();
  configurarBT();
}

void loop() {

  // Funciones de la librería "Sensor" y "Pendiente"
  lecturaSensores();
  lecturaDatos();
  obtenerVelocidad();
  obtenerAceleracion();
  // Modo de funcionamiento
  switch(modo){
    case 0:
      analogWrite(PIN_ACELERADOR, 0);
      break;
    case 1:
      // Funciones del algoritmo de funcionamiento
      decisionAcelerador();
      obtenerIncremento();
      acelerador_final = acelerador + TASA*incremento_acelerador;
      analogWrite(PIN_ACELERADOR, acelerador_final);
      break;
    case 2:
      if(sensor_freno_der || sensor_freno_izq || !sensor_pas || anguloX < 0){
        acelerador = 0;
      }
      else{
        acelerador = 120;
      }
      analogWrite(PIN_ACELERADOR, acelerador);
      break;
    default:
      analogWrite(PIN_ACELERADOR, 0);
      break;
  }
  // Función de la librería "Bluetooth"
  envioDatos();
}

//----------------------------------------

void decisionAcelerador(){  
  if(sensor_freno_der || sensor_freno_izq || !sensor_pas || anguloX < 0){
    acelerador = 0;
  }
  else {
    if(velocidad < velocidad_minima){
      acelerador = 255;
    }
    else {
      if (velocidad < velocidad_paseo){
        acelerador_deseable = 120;
      }
      else if (velocidad < velocidad_crucero){
        acelerador_deseable = 120*((velocidad_crucero - velocidad)/(velocidad_crucero - velocidad_paseo));
      }
      else {
        acelerador_deseable = 0;
      } 
    }
  }
}

void obtenerIncremento(){
  
  if(aceleracion > 0) {
    incremento_acelerador = incremento_acelerador + A1*(acelerador_deseable - acelerador)*255;
  }
  if(anguloX > 0){
    incremento_acelerador = incremento_acelerador + A2*anguloX*255;
  }
  if(velocidad < velocidad_crucero){
    incremento_acelerador = incremento_acelerador + A3*(velocidad_crucero - velocidad)*255;
  }
  
}







