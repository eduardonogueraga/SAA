/*
 * InterStrike.h
 *
 *  Created on: 23 jun. 2020
 *      Author: isrev
 */

#ifndef PROYECTO_INTERSTRIKE_H_
#define PROYECTO_INTERSTRIKE_H_
#include "Arduino.h"
#include "Datos.h"

class InterStrike {

private:

	unsigned long tiempoBase = 0;
	unsigned long tiempoBaseAux=0;
	unsigned long tiempoFondo=0;
	int strike = 0;
	int strikeFondo = 0;
	bool sensor, sensorant;
	long tiempoCeguera = 5000; //default
	long tiempoExposicion = 15000; //default
	long tiempoStrikeFondo = 80000; //defult
	byte gotoPing= 0;


public:

	InterStrike();
	virtual ~InterStrike();

	void setSensor(bool _sensor);
	void setTiempo(long tiempoCeguera, long tiempoExposicion, long tiempoStrikeFondo = 80000);
	int getStrike();
	int matarStrike();
	int matarStrikeFondo();
	void pingSensor();
	void setStart(); //Evita que se cargen strikes antes de tiempo
	void compruebaEstado(int numero, byte pirValor);
	void compruebaEstado(byte pirValor); //Sobrecarga MG
	bool disparador(int strike, int max, int numero, Datos &_datos);
	void compruebaPhantom(int numero, byte pirValor,Datos &_datos);



};

#endif /* PROYECTO_INTERSTRIKE_H_ */
