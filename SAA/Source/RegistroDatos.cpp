/*
 * RegistroDatos.cpp
 *
 *  Created on: 1 jul. 2020
 *      Author: isrev
 */

#include "RegistroDatos.h"
#include <SPI.h>
#include <SD.h>

#include "Fecha.h"

#define SSpin 53 //Pin esclavo del SD

RegistroDatos::RegistroDatos() {

}

RegistroDatos::~RegistroDatos() {
	// TODO Auto-generated destructor stub
}

void RegistroDatos::iniciarSd(){


	  if (!SD.begin(SSpin)) {
	    Serial.println("FALLO EN SD");
	    return;
	  }

	  Serial.println("SD READY");

	  generarFichero();

}

void RegistroDatos::generarFichero(){

	extern Fecha tiempo;

	fichero = SD.open("REGISTRO.txt", FILE_WRITE); //TODO crear otro documento que atestigue el funcionamiento de la alarma


	if (fichero) {
		fichero.println("ALARMA INICIADA "+tiempo.imprimeFecha());
		fichero.close();
		Serial.println("Escritura correcta SD");
	} else {
		Serial.println("Error en apertura del archivo SD");
	}

}

void RegistroDatos::registrarEvento(String descripcion){

	extern Fecha tiempo;

	fichero = SD.open("REGISTRO.txt", FILE_WRITE); //REGISTRO

	if (fichero) {
		fichero.println(descripcion+" "+" Fecha: " + tiempo.imprimeFecha());
		fichero.close();

	} else {
		//Serial.println("Error en apertura del archivo SD");  //@develop("Omitir el fallo por SD cuando no esta conectado")
	}


}


void RegistroDatos::registrarSensor(String descripcion){

	extern Fecha tiempo;

	fichero = SD.open("SENSORES.txt", FILE_WRITE);

	if (fichero) {
		fichero.println(descripcion + tiempo.imprimeFecha());
		fichero.close();

	} else {
		// Serial.println("Error en apertura del archivo SD"); //@develop("Omitir el fallo por SD cuando no esta conectado")
	}


}

void RegistroDatos::mostrarRegistro(String nom){


	fichero = SD.open(nom+".txt");
	if (fichero) {

		while (fichero.available()) {

			Serial.write(fichero.read());

		}
		fichero.close();

	} else {
		Serial.println("Error en apertura del archivo SD");
	}

}



int RegistroDatos::getValorPropiedad(String nombreVar){

	String VALOR_IN = "<v>";
	String VALOR_OUT = "</v>";
	int puntero_propiedad = 0;
	String valorPropiedead="";
	int valor = 0;

	String instrucciones[4]; //Tamaño estatico
	String archivoPropiedades = "";

	fichero = SD.open("PROP.txt"); //Lee el fichero y lo almacena en archivoPropiedades
	if (fichero) {

		while (fichero.available()) {

			archivoPropiedades += (char)fichero.read();
		}
		fichero.close();

	} else {
		Serial.println("Error en apertura del archivo SD");
	}

	for (unsigned int i = 0; i < archivoPropiedades.length(); i++){ //Divide por instrucciones
		instrucciones [i] = getValue(archivoPropiedades, ';', i);
	}

	for(unsigned int i = 0; i < sizeof(instrucciones); i++){ //Busca la instruccion deseada

		if(instrucciones[i].indexOf(nombreVar) >=0) {
			puntero_propiedad = i;
			break;
		}else{
			puntero_propiedad = -1; //Si no encuentra nada lo declara negativo
		}
	}

	if(puntero_propiedad >=0){
		valorPropiedead = instrucciones[puntero_propiedad].substring(instrucciones[puntero_propiedad].indexOf(VALOR_IN)+VALOR_IN.length(), instrucciones[puntero_propiedad].indexOf(VALOR_OUT));

		valor = valorPropiedead.toInt();
	}else{
		Serial.println("ERROR"); //Devuelve 0
	}


	return valor;
}



String  RegistroDatos::getValue(String data, char separator, int index) //Metodo de split adaptado a c++
{
	int found = 0;
	int strIndex[] = {0, -1};
	int maxIndex = data.length()-1;

	for(int i=0; i<=maxIndex && found<=index; i++){
		if(data.charAt(i)==separator || i==maxIndex){
			found++;
			strIndex[0] = strIndex[1]+1;
			strIndex[1] = (i == maxIndex) ? i+1 : i;
		}
	}

	return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
