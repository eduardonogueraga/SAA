/*
 * Mensajes.cpp
 *
 *  Created on: 23 jun. 2020
 *      Author: isrev
 *
 */

#include "Mensajes.h"
#include "Datos.h"
#include <SoftwareSerial.h>
#include "Fecha.h"
#include "RegistroDatos.h"
#include <EEPROM.h>
#include "Entorno.h"


	Mensajes::Mensajes()
	{}

	Mensajes::~Mensajes()
	{}

	void Mensajes::inicioSIM800(SoftwareSerial &_SIM800L){
		_SIM800L.begin(9600); //Inicializamos la segunda comunicacion Serial.
		_SIM800L.println("AT+CMGF=1"); //Vamos utilizar los SMS.
		delay(100); //Delay de 0.1 sec
		_SIM800L.println("AT+CNMI=1,2,0,0,0"); //Configurar el SIM800L p/ que muestre msm por com. serie.
	 }

	String Mensajes::getAsunto(){
		return asuntoMensaje;
	}

	void Mensajes::setAsunto(String _asuntoMensaje){
		asuntoMensaje = _asuntoMensaje;
	}


	void Mensajes::setTiempoLlamada(unsigned long tiempoLlamada){
		this->tiempoLlamada = tiempoLlamada;
		}

	unsigned long Mensajes::getTiempoLlamada(){
			return tiempoLlamada;
		}

	void Mensajes::setAsuntoPir(Datos &_datos){  //Selecciona el asunto pir  del mapeo de sensores


		int* mapSensor= _datos.getDatos();
		cont=0; //Reinicia el contador


		for (int i = 0; i < 4; i++) { //TODO enlaza con tam el tamaño de los array

			if(mapSensor[i] == MAX_SALTO[i]) {
				//Salto principal
				asuntoMensaje = "AVISO ALARMA";
				if(zonas[i] == ("PUERTA")) {
					asuntoMensaje += " PUERTA DE LA COCHERA ABIERTA";
				}else {
					asuntoMensaje += " MOVIMIENTO DETECTADO EN "+zonas[i];
				}


				for (int j= 0; j < 4; j++) {
					if((mapSensor[j] != 0)&&(mapSensor[j] != MAX_SALTO[j])) {
						//Saltos secundarios

						if(cont == 0) {
							asuntoMensaje += " JUNTO CON MOVIMIENTO EN "+zonas[j];
							cont++;

						}else {
							asuntoMensaje += " Y "+zonas[j];
						}

					}
				}
			}
		}


	}


	void  Mensajes::enviarMensaje(Datos &_datos){

		extern byte auto_rc_intentos;
		extern bool rcEstado;
		extern int valiza;
		extern SoftwareSerial SIM800L;
		extern Fecha tiempo;
		extern RegistroDatos registro;

		extern int saltoEnviadoHistorico;
		extern byte smsDireccion;

		if(EEPROM.read(5) < LIMITE_MAXIMO ){ //MENSAJES_ENVIADOS

			saltoEnviadoHistorico =  EEPROM.read(smsDireccion);
			saltoEnviadoHistorico++;
			EEPROM.update(smsDireccion, saltoEnviadoHistorico);

			//Mensaje
			String cuerpoSMS = "";
			SIM800L.println("AT+CMGF=1");
			delay(200);
			SIM800L.println("AT+CMGS=\"+34"+Entorno::getTelefonoMensaje()+"\"");
			delay(200);
			SIM800L.print(Mensajes::getAsunto()+"\n");
			Serial.print(Mensajes::getAsunto()+"\n");
			SIM800L.println(_datos.imprimeDatos());
			//cuerpoSMS.concat(_datos.imprimeDatos()); //Falta desarrollar el cuerpo
			//cuerpoSMS.replace("\n", "-");
			//Serial.println(cuerpoSMS);
			Serial.println(_datos.imprimeDatos()); //Visor para debug

				if(valiza > 0){ //Bug en el mensaje de Fallo bateria
					if(rcEstado == true){
						SIM800L.println("Intentos restantes: "+ (String)(3-auto_rc_intentos)); //SOLO MODO REACTIVACION
						//Serial.println("Intentos restantes: "+ (String)(3-auto_rc_intentos));
					}
				}else if(valiza == 0){
					SIM800L.println("La alarma no se reactivara con la puerta abierta");
					//Serial.println("La alarma no se reactivara con la puerta abierta");
				} else{
					SIM800L.println("Salto indeterminado");
				}

				if(digitalRead(2) == LOW){ //SENSOR_BATERIA_EMERGENCIA
					SIM800L.println("Bateria de emergencia desactivada");
				}

				if(digitalRead(19) == HIGH){ //SENSOR_BATERIA_PRINCIPAL
					SIM800L.println(tiempo.imprimeFecha()); //RTC no disponible con bateria de emergencia
					//Serial.println(tiempo.imprimeFecha());
				}
			Serial.println("\nOK");
			delay(200);
			SIM800L.print((char)26);
			delay(200);
			SIM800L.println("");
			delay(200);

			EEPROM.write(5,(EEPROM.read(5))+1); //Suma uno al contador

			registro.registrarEvento("["+Mensajes::getAsunto()+"] MENSAJE ENVIADO");
			registro.registrarEvento("INTENTOS SMS REALIZADOS: "+(String)EEPROM.read(5));
			registro.intentosRealizadosInfoBD((String)EEPROM.read(5));

				if(Mensajes::getAsunto().indexOf("AVISO") >=0){
					//Mensaje de aviso intrusismo
					registro.mensajeInfoBD("salto", Mensajes::getAsunto(), "");
					registro.updateSaltoInfoBD();
				}else{
					//Mensaje de reactivacion
					registro.mensajeInfoBD("info", Mensajes::getAsunto(), "");
					registro.updateEntradaInfoBD();
				}

		}else{
			Serial.println("Intentos diarios acabados");
			registro.registrarEvento("INTENTOS SMS DIARIOS ACABADOS");
			registro.intentosAcabadosInfoBD();
		}

	}


	void  Mensajes::enviarMensaje(String mensaje){

		extern SoftwareSerial SIM800L;
		extern Fecha tiempo;
		extern RegistroDatos registro;

		extern int saltoEnviadoHistorico;
		extern byte smsDireccion;


		if(EEPROM.read(5) < LIMITE_MAXIMO){

			saltoEnviadoHistorico =  EEPROM.read(smsDireccion);
			saltoEnviadoHistorico++;
			EEPROM.update(smsDireccion, saltoEnviadoHistorico);

			//Mensaje
			SIM800L.println("AT+CMGF=1");
			delay(200);
			SIM800L.println("AT+CMGS=\"+34"+Entorno::getTelefonoMensaje()+"\"");
			delay(200);
			SIM800L.print(Mensajes::getAsunto()+"\n");
			Serial.print(Mensajes::getAsunto()+"\n");
			SIM800L.println(mensaje);
			Serial.println(mensaje); //Visor para debug

			if(digitalRead(2) == LOW){ //SENSOR_BATERIA_EMERGENCIA
				SIM800L.println("Bateria de emergencia desactivada");
			}
			SIM800L.println(tiempo.imprimeFecha());
			//Serial.println(tiempo.imprimeFecha());
			Serial.println("\nOK");
			delay(200);
			SIM800L.print((char)26);
			delay(200);
			SIM800L.println("");
			delay(200);

			EEPROM.write(5,(EEPROM.read(5))+1); //Suma uno al contador
			registro.registrarEvento("["+Mensajes::getAsunto()+"] MENSAJE ENVIADO");
			registro.registrarEvento("INTENTOS SMS REALIZADOS: "+(String)EEPROM.read(5));
			registro.intentosRealizadosInfoBD((String)EEPROM.read(5));

			registro.mensajeInfoBD("info", Mensajes::getAsunto(), mensaje);
			registro.updateEntradaInfoBD();
		}else{
			Serial.println("Intentos diarios acabados");
			registro.registrarEvento("INTENTOS SMS DIARIOS ACABADOS");
			registro.intentosAcabadosInfoBD();
		}



	}


	void Mensajes::realizarLlamada(bool estado, byte telefono){

		extern SoftwareSerial SIM800L;
		extern RegistroDatos registro;

		if(estado){
			Serial.println("Llamando");
			SIM800L.println("AT");
			delay(200);

			if(telefono ==1){
				SIM800L.println("ATD+ +34"+Entorno::getTelefonoLlamada(1)+";");
				registro.llamadaInfoBD("Mi movil");
			}
			if(telefono ==2){
				SIM800L.println("ATD+ +34"+Entorno::getTelefonoLlamada(2)+";");
				registro.llamadaInfoBD("Movil 2");
			}

			delay(200);
		}else{
			Serial.println("Llamada finalizada");
			SIM800L.println("ATH");
		}




	}
