/*
 * Bocina.h
 *
 *  Created on: 26 jun. 2020
 *      Author: isrev
 */

#ifndef PROYECTO_BOCINA_H_
#define PROYECTO_BOCINA_H_

#include "Arduino.h"

class Bocina {

private:
	byte bocinaEstado = 0;
	long bocinaTiempoFuncionamiento;
	int bocinaIntervalo = 5000;

	unsigned long bocinaTiempoAntEncendido = 0;
	unsigned long bocinaTiempoAntApagado = 0;

public:
	Bocina();
	virtual ~Bocina();

	void iniciarBocina(int bocinaIntervalo, long bocinaTiempoFuncionamiento);
	void comprobarBocina();
	void detenerBocina();

};

#endif /* PROYECTO_BOCINA_H_ */
