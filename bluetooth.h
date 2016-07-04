// Librería "bluetooth". 
// Encargada de las comunicaciones entre el Arduino y un dispositivo móvil
#ifndef _BT
#define _BT
#include <arduino.h>

// Indica el modo de funcionamiento del sistema
extern int modo;
// Definición de funciones
void configurarBT();
void envioDatos();

#endif // _BT
