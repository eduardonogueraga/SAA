/*
 * RegistroDatos.h
 *
 *  Created on: 1 jul. 2020
 *      Author: isrev
 */

#include <SPI.h>
#include <SD.h>

#ifndef PROYECTO_REGISTRODATOS_H_
#define PROYECTO_REGISTRODATOS_H_

class RegistroDatos {

private:
	File fichero;


public:
	RegistroDatos();
	virtual ~RegistroDatos();
	void iniciarSd();
	void generarFichero();
	void registrarEvento(String descripcion);
	void registrarSensor(String descripcion);
	void mostrarRegistro(String nom);
	void leerPropiedades();
	String getPropiedades();
	int getValorPropiedad(String nombreVar);
	String getValue(String data, char separator, int index);

};

#endif /* PROYECTO_REGISTRODATOS_H_ */
