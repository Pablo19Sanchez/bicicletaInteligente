// Implementación de las funciones de la librería "pendiente"
#include <arduino.h>
#include <Wire.h>
#include "pendiente.h"

#define MPU 0X68 // Dirección del sensor
// Ratios de conversion
#define A_R 16384.0
#define G_R 131.0
#define RAD_A_DEG = 57.295779 // Conversion de radianes a grados

// Variables de trabajo
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ, Tmp; // Valores sin refinar
float Acc[2], Gy[2], Angle[2]; // Ángulos
static float pendiente_inicial = 6.9; // Posición inicial del sensor en la bicicleta

// Variables globales que serán las utilizadas en el algoritmo principal
float anguloX = 0;
int16_t temperatura = 0;

// Inicia la lectura de datos del MPU-6050
void establecerMPU(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

// Lee los datos en bruto del MPU y obtiene el ángulo con el eje X y la temperatura
void lecturaDatos(){
  // Lectura de los valores del sensor
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Pide el registro correspondiente al AcX
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true); // A partir del valor 0x3B se piden 7 registros
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  // Se calculan los ángulos X e Y
  Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
  Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
  // Se obtienen los ángulos del giroscopio
  Gy[0] = GyX/G_R;
  Gy[1] = GyY/G_R;
  // Se aplica el filtro complementario
  Angle[0] = 0.98*(Angle[0] + Gy[0]*0.010) + 0.02*Acc[0];
  Angle[1] = 0.98*(Angle[1] + Gy[1]*0.010) + 0.02*Acc[1];
  // Pasamos la temperatura a grados centígrados
  temperatura = Tmp/340.00 + 36.53;
  // Obtenemos el ángulo X
  anguloX = Angle[0] - pendiente_inicial;
}

