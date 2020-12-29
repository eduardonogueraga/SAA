/*
 * Menus.h
 *
 *  Created on: 24 jun. 2020
 *      Author: isrev
 */

#ifndef PROYECTO_MENUS_H_
#define PROYECTO_MENUS_H_

#include "Arduino.h"
#include "Pantalla.h"
#include "Bocina.h"
#include "Fecha.h"

class Menus {
private:
	bool estadoMenu;
	int SENSOR_NEXT;
	int HOME_NEXT =1;     //Nivel por defecto
	int CONFIG_NEXT;
	int INFO_NEXT;
	int time_change = 3; //4
	int bluetooh_estado = 2; //Apagado y encendido
	String tiempo ="";



public:
	Menus();
	virtual ~Menus();
	bool getEstadoMenu();
	void setEstadoMenu(bool estadoMenu);
	void menuManager(LiquidCrystal_I2C &lcd, Pantalla &pantalla, Bocina &bocina, Fecha &_fecha);


};





#endif /* PROYECTO_MENUS_H_ */
