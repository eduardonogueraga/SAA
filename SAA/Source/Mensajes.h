/*
 * Mensajes.h
 *
 *  Created on: 23 jun. 2020
 *      Author: isrev
 *
 *    En este header van declarados los atributos, funcion, constructor y destructor
 *    como prototipos de la estructura de Mensajes.cpp
 */

#ifndef PROYECTO_MENSAJES_H_
#define PROYECTO_MENSAJES_H_
#include "Arduino.h"
#include "Datos.h"
#include <SoftwareSerial.h>

extern Datos datos;
extern const unsigned int LIMITE_MAXIMO; //Limite de mensajes

class Mensajes {

private:
	String asuntoMensaje ="";
	unsigned long tiempoLlamada = 0;

	int MAX_SALTO [4] = {1,3,2,2};
	String zonas [4]  = {"PUERTA","COCHERA","PORCHE","ALMACEN"};
	byte cont;

public:
	Mensajes();  //Constructor
	virtual ~Mensajes(); //Destructor
	void inicioSIM800(SoftwareSerial &_SIM800L);
	String getAsunto();
	void setAsunto(String _asuntoMensaje); //Prototipo de la funcion
	unsigned long getTiempoLlamada();
	void setTiempoLlamada(unsigned long tiempoLlamada);
	void setAsuntoPir(Datos &_datos);
	void enviarMensaje(Datos &_datos, String tipo);
	void enviarMensaje(String mensaje, String tipo); //Sobrecarga
	void realizarLlamada(bool estado, byte telfono);

};

#endif /* PROYECTO_MENSAJES_H_ */
