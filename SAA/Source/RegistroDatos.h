/*
 * RegistroDatos.h
 *
 *  Created on: 1 jul. 2020
 *      Author: isrev
 */

#include <SPI.h>
#include <SD.h>
#include "Fecha.h"

#ifndef PROYECTO_REGISTRODATOS_H_
#define PROYECTO_REGISTRODATOS_H_

extern Fecha tiempo;

class RegistroDatos {

private:
	File fichero;
	String sdErrorMessage = "Error en apertura del archivo SD";

public:
	RegistroDatos();
	virtual ~RegistroDatos();
	void iniciarSd();
	void generarFichero();
	void registrarEventoBD(String sentenciaSQL);
	void registrarEvento(String descripcion);
	void registrarSensor(String descripcion);
	void inicioAlarmaBD();
	void bateriaEmergenciaInfoBD(String estado);
	void intentosRecuperadosInfoBD();
	void intentosRealizadosInfoBD(String intentos);
	void intentosAcabadosInfoBD();
	void activarAlarmaBD(String modo, String intentos);
	void desactivarAlarmaBD(String modo, String intentos);
	void modoAlarmaInfoBD(String modo);
	void sensorInfoBD(String tipo, String estado, String modo);
	void saltoInfoBD(String restaurado);
	void mensajeInfoBD(String tipo, String asunto, String cuerpo);
	void updateSaltoInfoBD(); //Actualiza el ultimo salto como intrusismo
	void updateEntradaInfoBD();
	void mostrarRegistro(String nom);
	void leerPropiedades();
	String getPropiedades();
	int getValorPropiedad(String nombreVar);
	String getValue(String data, char separator, int index);

};

#endif /* PROYECTO_REGISTRODATOS_H_ */
