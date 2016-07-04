// Implementación de las funciones de la librería "bluetooth"
#include <arduino.h>
#include "bluetooth.h"
#include "pendiente.h"
#include "sensor.h"

#define INTERRUPCION_BT 4

int modo = 1; // Modo de funcionamiento del sistema
// Variables que controlan la comunicación
char c = ' ';
static char saludo = 'D';
static char fin = '%';
static char coma = ',';

//----------------------------------------
// Función de interrupción cuando llega un comando, y lo interpreta
void leeDatos(){
  while(Serial1.available() > 0){
    c = Serial1.read();
  }
  switch(c){
    case '0':
      modo = 0;
      break;
    case '1': 
      modo = 1;
      break;
    case '2': 
      modo = 2;
      break;
    case 's':
      modo = 0;
      break;
    default:
      break;
  }
}
//----------------------------------------

// Configura los parámetros de la transmisión serie
void configurarBT(){
  Serial1.begin(38400);
  attachInterrupt(INTERRUPCION_BT, leeDatos, CHANGE);
}
// Envía los datos vía bluetooth
void envioDatos(){
  Serial1.write(saludo);
  Serial1.write(int(anguloX));
  Serial1.write(coma);
  Serial1.write(temperatura);
  Serial1.write(coma);
  Serial1.write(aceleracion);
  Serial1.write(fin);
}

