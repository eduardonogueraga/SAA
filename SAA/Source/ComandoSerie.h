/*
 * ComandoSerie.h
 *
 *  Created on: 27 jun. 2020
 *      Author: isrev
 *
 *      Clase con funciones para el debugeo
 */

#ifndef PROYECTO_COMANDOSERIE_H_
#define PROYECTO_COMANDOSERIE_H_

#include "Arduino.h"
#include "InterStrike.h"
#include "Menus.h"
#include "Pantalla.h"
#include <Keypad.h>
#include "Bocina.h"
#include <LiquidCrystal_I2C.h>



extern char key;   //Revisa las variables del main
extern String entradapass;
extern bool estadoalarma;
extern bool estadoMenu;
extern long bocinaTiempo;
extern byte controlUtvAcceso;
extern Menus menu;
extern Pantalla pantalla;
extern Bocina bocina;
extern LiquidCrystal_I2C lcd;
extern InterStrike pir1;
extern InterStrike pir2;
extern InterStrike pir3;
extern bool rcEstado;
extern void desactivar(bool rcEstado);
extern void activar(bool rcEstado);

class ComandoSerie {
private:
	String data ="";
	int tiempoPing = 0;

public:
	ComandoSerie();
	virtual ~ComandoSerie();

	void demonioSerie();
	void controInfraRojo();
	void iniciarBlueetooh();
	void setMenu();
	void cambiarModo();
	void testBocina();
};

#endif /* PROYECTO_COMANDOSERIE_H_ */
