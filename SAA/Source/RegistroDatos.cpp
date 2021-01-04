/*
 * RegistroDatos.cpp
 *
 *  Created on: 1 jul. 2020
 *      Author: isrev
 */

#include "RegistroDatos.h"
#include <SPI.h>
#include <SD.h>


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

	fichero = SD.open("REGISTRO.txt", FILE_WRITE); //TODO crear otro documento que atestigue el funcionamiento de la alarma


	if (fichero) {
		fichero.println("ALARMA INICIADA "+tiempo.imprimeFecha());
		fichero.close();
		Serial.println("Escritura correcta SD");
	} else {
		Serial.println("Error en apertura del archivo SD");
	}

}

void RegistroDatos::registrarEventoBD(String sentenciaSQL){


	fichero = SD.open("SQL.txt", FILE_WRITE);

	if (fichero) {
		fichero.println(sentenciaSQL);
		fichero.close();

	} else {
		Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
	}

}

void RegistroDatos::registrarEvento(String descripcion){

	fichero = SD.open("REGISTRO.txt", FILE_WRITE); //REGISTRO

	if (fichero) {
		fichero.println(descripcion+" "+" Fecha: " + tiempo.imprimeFecha());
		fichero.close();

	} else {
		Serial.println("Error en apertura del archivo SD");  //@develop("Omitir el fallo por SD cuando no esta conectado")
	}


}


void RegistroDatos::registrarSensor(String descripcion){


	fichero = SD.open("SENSORES.txt", FILE_WRITE);

	if (fichero) {
		fichero.println(descripcion + tiempo.imprimeFecha());
		fichero.close();

	} else {
		 Serial.println("Error en apertura del archivo SD"); //@develop("Omitir el fallo por SD cuando no esta conectado")
	}


}

void RegistroDatos::inicioAlarmaBD(){

	fichero = SD.open("SQL.txt", FILE_WRITE);

		if (fichero) {
			fichero.println("INSERT INTO `alarma` (`descripcion`, `fecha`) "
			"VALUES ('ALARMA INICIADA', '"+tiempo.imprimeFechaSQL()+"');");
			fichero.close();
		} else {
			Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
		}

}

void RegistroDatos::bateriaEmergenciaInfoBD(String estado){

	fichero = SD.open("SQL.txt", FILE_WRITE);
	estado.toUpperCase();

		if (fichero) {
			fichero.println("INSERT INTO `alarma` (`descripcion`, `fecha`) VALUES "
				 	 "('"+estado+" BATERIA DE EMERGENCIA', '"+tiempo.imprimeFechaSQL()+"');");
			fichero.close();
		} else {
			Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
		}
}

void RegistroDatos::intentosRecuperadosInfoBD(){

	fichero = SD.open("SQL.txt", FILE_WRITE);

		if (fichero) {
			fichero.println("INSERT INTO `alarma` (`descripcion`, `fecha`) "
					 "VALUES ('INTENTOS SMS DIARIOS RECUPERADOS', '"+tiempo.imprimeFechaSQL()+"');");
			fichero.close();
		} else {
			Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
		}

}

void RegistroDatos::intentosRealizadosInfoBD(String intentos){

	fichero = SD.open("SQL.txt", FILE_WRITE);

		if (fichero) {
			fichero.println("INSERT INTO `alarma` (`descripcion`, `fecha`) "
					 "VALUES ('INTENTOS SMS REALIZADOS: "+intentos+"', '"+tiempo.imprimeFechaSQL()+"');");
			fichero.close();
		} else {
			Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
		}

}


void RegistroDatos::intentosAcabadosInfoBD(){

	fichero = SD.open("SQL.txt", FILE_WRITE);

		if (fichero) {
			fichero.println("INSERT INTO `alarma` (`descripcion`, `fecha`) "
					 "VALUES ('INTENTOS SMS DIARIOS ACABADOS', '"+tiempo.imprimeFechaSQL()+"');");
			fichero.close();
		} else {
			Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
		}

}

void RegistroDatos::activarAlarmaBD(String modo, String intentos){

	fichero = SD.open("SQL.txt", FILE_WRITE);

		if (fichero) {
			fichero.println("INSERT INTO `entradas` (`tipo`, `modo`, `restaurado`, `intentos_reactivacion`,"
		" `fecha`) VALUES ('activacion', '"+modo+"', '0', '"+intentos+"', '"+tiempo.imprimeFechaSQL()+"');");
			fichero.close();
		} else {
			Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
		}
}


void RegistroDatos::desactivarAlarmaBD(String modo, String intentos){

	fichero = SD.open("SQL.txt", FILE_WRITE);

		if (fichero) {
			fichero.println("INSERT INTO `entradas` (`tipo`, `modo`, `restaurado`, `intentos_reactivacion`,"
		" `fecha`) VALUES ('desactivacion', '"+modo+"', '0', '"+intentos+"', '"+tiempo.imprimeFechaSQL()+"');");
			fichero.close();
		} else {
			Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
		}
}

void RegistroDatos::modoAlarmaInfoBD(String modo){

	fichero = SD.open("SQL.txt", FILE_WRITE);
	modo.toUpperCase();

			if (fichero) {
				fichero.println("INSERT INTO `alarma` (`descripcion`, `fecha`) "
				 			"VALUES ('ALARMA ESTABLECIDA EN MODO "+modo+"', '"+tiempo.imprimeFechaSQL()+"');");
				fichero.close();
			} else {
				Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
			}
}

void RegistroDatos::sensorInfoBD(String tipo, String estado, String modo){

	fichero = SD.open("SQL.txt", FILE_WRITE);

			if (fichero) {
				fichero.println("INSERT INTO `sensores` (`tipo`, `estado`, `modo`, `fecha`) "
						"VALUES ('"+tipo+"', '"+estado+"', '"+modo+"', '"+tiempo.imprimeFechaSQL()+"');");
				fichero.close();
			} else {
				Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
			}
}

void RegistroDatos::saltoInfoBD(String restaurado){

	fichero = SD.open("SQL.txt", FILE_WRITE);

			if (fichero) {
				fichero.println("INSERT INTO `saltos` (`intrusismo`, `restaurado`, `entradas_id`, `sensores_id`) "
						"VALUES ('0', '"+restaurado+"',(SELECT max(id) FROM entradas), (SELECT max(id) FROM sensores));");
				fichero.close();
			} else {
				Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
			}
}

void RegistroDatos::mensajeInfoBD(String tipo, String asunto, String cuerpo){

	fichero = SD.open("SQL.txt", FILE_WRITE);

			if (fichero) {
				fichero.println("INSERT INTO `mensajes` (`tipo`, `asunto`, `cuerpo`, `fecha_sms`) "
						"VALUES ('"+tipo+"', '"+asunto+"', '"+cuerpo+"', '"+tiempo.imprimeFechaSQL()+"');");
				fichero.close();
			} else {
				Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
			}
}

void RegistroDatos::updateSaltoInfoBD(){

	fichero = SD.open("SQL.txt", FILE_WRITE);

			if (fichero) {
				fichero.println("UPDATE `saltos` SET `intrusismo` = '1', `mensajes_id` = (SELECT max(id) FROM mensajes) "
						"WHERE (id = (SELECT * FROM(SELECT max(id) FROM saltos) as ultimo_id));");
				fichero.close();
			} else {
				Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
			}
}

void RegistroDatos::updateEntradaInfoBD(){

	fichero = SD.open("SQL.txt", FILE_WRITE);

			if (fichero) {
				fichero.println("UPDATE `entradas` SET `mensajes_id` = (SELECT max(id) FROM mensajes) "
						"WHERE (id = (SELECT * FROM(SELECT max(id) FROM entradas) as ultimo_id));");
				fichero.close();
			} else {
				Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
			}
}

void RegistroDatos::llamadaInfoBD(String nombre){

	fichero = SD.open("SQL.txt", FILE_WRITE);

			if (fichero) {
				fichero.println("INSERT INTO `alarma` (`descripcion`, `fecha`) "
						"VALUES ('LLAMANDO A: "+nombre+"', '"+tiempo.imprimeFechaSQL()+"');");
				fichero.close();
			} else {
				Serial.println(RegistroDatos::sdErrorMessage);  //@develop("Omitir el fallo por SD cuando no esta conectado")
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
