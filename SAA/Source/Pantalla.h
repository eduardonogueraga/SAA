/*
 * Pantalla.h
 *
 *  Created on: 24 jun. 2020
 *      Author: isrev
 */

#ifndef PROYECTO_PANTALLA_H_
#define PROYECTO_PANTALLA_H_

#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


class Pantalla {
private:

public:
	Pantalla();
	virtual ~Pantalla();
	void lcdAlarmaDesactivada(LiquidCrystal_I2C &lcd);
	void lcdAlarmaActivada(LiquidCrystal_I2C &lcd);
	void lcdClave(char key, LiquidCrystal_I2C &lcd);
	void lcdInicio(LiquidCrystal_I2C &lcd, String version, String fecha);
	void lcdFalloClaveDesactivar(LiquidCrystal_I2C &lcd);
	void lcdFalloClaveActivar(LiquidCrystal_I2C &lcd);
	void lcdFalloClaveNoValida(LiquidCrystal_I2C &lcd);
	void lcdModoPrueba(LiquidCrystal_I2C &lcd);
	void lcdModoDefault(LiquidCrystal_I2C &lcd);
	void lcdIntrusismo(LiquidCrystal_I2C &lcd, int num);
	void lcdAvisoEnviado(LiquidCrystal_I2C &lcd);
	void lcdBocinaTest(LiquidCrystal_I2C &lcd);
	void lcdAvisoBateriaDesactivada(LiquidCrystal_I2C &lcd);
	void lcdAvisoResetAutomatico(LiquidCrystal_I2C &lcd);

	void menuHome0(LiquidCrystal_I2C &lcd);
	void menuHome1(LiquidCrystal_I2C &lcd);
	void menuConfigSensor(LiquidCrystal_I2C &lcd);
	void menuConfigModos(LiquidCrystal_I2C &lcd, bool controlUtv);
	void menuResetAutomatico(LiquidCrystal_I2C &lcd);
	void menuConfigBluetooh(LiquidCrystal_I2C &lcd, byte sleepmode_bluetooth);
	void menuSelectInfoMenu(LiquidCrystal_I2C &lcd);
	void menuInfoVersion(LiquidCrystal_I2C &lcd, String version_software, String fecha);
	void menuInfoSaltoSubmenu(LiquidCrystal_I2C &lcd);
	void menuInfoSalto(LiquidCrystal_I2C &lcd, int saltoEnviado, int saltoFalso);
	void menuInfoSaltoHistorico(LiquidCrystal_I2C &lcd, byte smsDireccion);
	void menuInfoTime(LiquidCrystal_I2C &lcd, String tiempo, double time_on);
	void menuModSensor1(LiquidCrystal_I2C &lcd);
	void menuModSensor2(LiquidCrystal_I2C &lcd);
	void menuModSensor2_2(LiquidCrystal_I2C &lcd);
	void menuModSensor3(LiquidCrystal_I2C &lcd, byte PIR_AUX);
	void menuModSensor4(LiquidCrystal_I2C &lcd);


};

#endif /* PROYECTO_PANTALLA_H_ */
