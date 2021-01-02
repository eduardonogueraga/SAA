/*
 * Fecha.h
 *
 *  Created on: 29 jun. 2020
 *      Author: isrev
 */

#include "Arduino.h"
#include <RTClib.h>

#ifndef PROYECTO_FECHA_H_
#define PROYECTO_FECHA_H_

class Fecha {
private:
	String tiempo;
	DateTime fecha;
	DateTime fechaReset;

public:

	Fecha();
	void iniciarRTC();
	String imprimeFecha(DateTime paramFecha = 0);
	String imprimeFechaSQL();
	String imprimeFechaSimple(DateTime paramFecha = 0);
	String imprimeHora();
	bool comprobarHora(int horas, int minutos = 0);
	bool comprobarRangoHorario(int hora_inicio, int hora_fin, int min_inicio = 0, int min_fin = 0);
	void establecerFechaReset(int dia, int hora = 0, int minuto = 0, int segundo = 0);
	DateTime getFechaReset();
	bool comprobarFecha(DateTime paramFecha);
	virtual ~Fecha();
};

#endif /* PROYECTO_FECHA_H_ */
