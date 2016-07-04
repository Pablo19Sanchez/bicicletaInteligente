// Librería "pendiente". 
// Encargada de realizar las lecturas del MPU-6050.
#ifndef _PENDIENTE
#define _PENDIENTE
#include <arduino.h>
#include <Wire.h> // Comunicación I2C

// Variables globales
extern float anguloX;
extern int16_t temperatura;

// Definición de funciones
void establecerMPU();
void lecturaDatos();

#endif // _PENDIENTE


