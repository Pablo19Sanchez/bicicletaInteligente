// Librería "sensor". Encargada comprobar el estado de los sensores:
//  - Manetas de freno derecho e izquierdo.
//  - Sensor de pedaleo.
//  - Sensor de velocidad.
#ifndef _SENSOR
#define _SENSOR
#include <arduino.h>

// Variables globales que dan la velocidad y aceleración
extern unsigned long velocidad;
extern long aceleracion;
// Variables para obtener el funcionamiento de los sensores
extern boolean sensor_freno_izq; 
extern boolean sensor_freno_der;
extern boolean sensor_pas;

// Definición de funciones
void setupSensores();
void lecturaSensores();
void obtenerVelocidad();
void obtenerAceleracion();

#endif // _SENSOR
