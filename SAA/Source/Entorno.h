/*
 * Entorno.h
 *
 * Created on: 27 oct. 2020
 *      Author: isrev
 */

#include "Arduino.h"

#ifndef PROYECTO_ENTORNO_H_
#define PROYECTO_ENTORNO_H_

class Entorno
{
private:

   static String password;

   static String tlf_principal;
   static String tlf_numero_2;
   static String tlf_numero_3;

public:
    static String getPassword();
    static String getTelefonoMensaje();
    static String getTelefonoLlamada(int numero);
};


#endif /* PROYECTO_ENTORNO_H_ */
