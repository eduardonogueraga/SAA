/*
 * InterStrike.cpp
 *
 *  Created on: 23 jun. 2020
 *      Author: isrev
 */
#include "Arduino.h"
#include "InterStrike.h"
#include "Datos.h"
#include "RegistroDatos.h"
#include "Fecha.h"

	InterStrike::InterStrike()
	{}

	InterStrike::~InterStrike()
	{}

	void InterStrike::setSensor(bool sensor){
		this->sensor = sensor;
	}

	void InterStrike::setTiempo(long tiempoCeguera, long tiempoExposicion, long tiempoStrikeFondo){
			this->tiempoCeguera = tiempoCeguera;
			this->tiempoExposicion = tiempoExposicion;
			this->tiempoStrikeFondo = tiempoStrikeFondo;
		}


	int InterStrike::getStrike(){
		return strike;
	}


	int InterStrike::matarStrike(){
		strike =0;
		return strike;
	}


	int InterStrike::matarStrikeFondo(){
		strikeFondo=0;
		return strikeFondo;
	}

	void InterStrike::pingSensor(){
		gotoPing = 1;
	}

	void InterStrike::setStart(){  //Evita que se cargen strikes antes de tiempo
		matarStrike();
		matarStrikeFondo();
	}

	void InterStrike::compruebaEstado(int numero, byte pirValor){

		extern long tiempoOn;
		extern Fecha tiempo;
		extern RegistroDatos registro;

		if(gotoPing==1){ //Control serie
			goto ping;
			gotoPing=0;
		}


		if(millis()>tiempoBase){
			if (sensor && !sensorant) {
				ping:
				gotoPing=0;
				if(pirValor ==1){
					tiempoBase = millis()+tiempoCeguera;
					if(strike==0){
						tiempoBaseAux = millis()+tiempoExposicion+tiempoCeguera;
					}
					strike++;
					Serial.print("\nSignal strike ");
					Serial.print(numero);
					Serial.print(": ");
					Serial.println(strike);
					if(millis()>tiempoOn){
						registro.registrarEvento("SEÑAL EN SENSOR "+(String)numero+": "+(String)strike); //Registran los datos en la tarjeta
						registro.registrarEvento("PIR "+(String)numero+" :"+(String)strike+" ONLINE ");
						registro.sensorInfoBD("pir "+(String)numero, "online", "estandar");
						registro.saltoInfoBD("0");
						// Modificar los inserts para que en update posterior al mensaje se cambien ademas del campo id_mensaje el campo intrusismo
					}

				} else {
					Serial.print("\nSensor ");
					Serial.print(numero);
					Serial.print(" deshabilitado");
					if(millis()>tiempoOn){
						registro.registrarEvento("SEÑAL EN SENSOR APAGADO "+(String)numero+": "+(String)strike);
						registro.registrarSensor("PIR "+(String)numero+" :"+(String)strike+" OFFLINE ");
						registro.sensorInfoBD("pir "+(String)numero, "offline", "estandar");
						registro.saltoInfoBD("0");
					}
				}
			}

			sensorant = sensor;
		}


		if((millis()>tiempoBaseAux) ){

			if(strike > 0){

				if(strikeFondo==0){
					//tiempoFondo = millis()+80000;
					tiempoFondo = millis()+tiempoStrikeFondo;

				}
				strikeFondo++;
				matarStrike();
				Serial.print("\nEliminar strike ");
				Serial.print(numero);
				Serial.print(": ");
				Serial.println(strike);
			}

		}


		if((millis()>tiempoFondo) ){
			strikeFondo= matarStrikeFondo();

		}

	}

	void InterStrike::compruebaEstado(byte pirValor){
		//Sobrecarga Sensor MG

		extern long tiempoOn;
		extern Fecha tiempo;
		extern RegistroDatos registro;

		if(gotoPing==1){ //Control serie
			goto ping;
			gotoPing=0;
		}

		if(millis()>tiempoOn){
			if (sensor == HIGH) { // @develop("Cambiado a LOW para evitar saltos en sensor MG")
				ping:
				gotoPing=0;
				if(pirValor ==1){

					if(strike==0){

						strike++;
						Serial.print("\nSignal strike MG");
						registro.registrarEvento("SEÑAL EN PUERTA COCHERA"); //Registran los datos en la tarjeta
						registro.registrarSensor("SEÑAL EN PUERTA COCHERA ONLINE ");

						registro.sensorInfoBD("mg", "online", "estandar");
						registro.saltoInfoBD("0");

					}


				}else{
					registro.registrarSensor("SEÑAL EN PUERTA COCHERA OFFLINE");
					registro.sensorInfoBD("mg", "offline", "estandar");
					registro.saltoInfoBD("0");
				}
			}
		}


	}


	bool InterStrike::disparador(int strike, int max, int numero, Datos &_datos){

		int auxStrike = strike;

		if(strike < max){

			_datos.setDatos(numero, strike);

		}

		if (strike == max) {

			_datos.setDatos(numero, strike);


			strike=matarStrike();
			strikeFondo= matarStrikeFondo();

			Serial.print("\nSignal PIR");
			Serial.println(numero);
			Serial.print("\nEliminar strike ");
			Serial.print(numero);
			Serial.print(": ");
			Serial.println(strike);

		}

		if(strikeFondo>max){ //Cambiado 2 por el maximo

			auxStrike = max;
			_datos.setDatos(numero, 1);
			strikeFondo= matarStrikeFondo();
			strike=matarStrike();
			Serial.print("\nStirke fondo");
			Serial.println(numero);
			Serial.print("\nSignal strike ");
			Serial.print(numero);
			Serial.print(": ");
			Serial.println(strike);

		}


		return(auxStrike == max)?true:false;
	}

	void InterStrike::compruebaPhantom(int numero, byte pirValor, Datos &_datos){

		extern Fecha tiempo;
		extern RegistroDatos registro;

		if(gotoPing==1){ //Control serie
			goto ping;
			gotoPing=0;
		}

		if (sensor && !sensorant) {
			ping:
			gotoPing=0;
			if(pirValor ==1){

				if(strike <= 100){ //Saltos maximos

				strike++;
				Serial.print("\nSignal phantom ");
				Serial.print(numero);
				Serial.print(": ");
				Serial.println(strike);

					_datos.setDatos(numero, strike);
					registro.registrarEvento("SEÑAL PHANTOM EN SENSOR "+(String)numero+": "+(String)strike);
					registro.registrarSensor(((numero != 0)? "PIR "+(String)numero:"PUERTA COCHERA")+" :"+(String)strike+" ONLINE/PHANTOM ");
					registro.sensorInfoBD(((numero != 0)? "pir"+(String)numero:"mg"), "online", "phantom");
					registro.saltoInfoBD("0");
				}
			} else {
				Serial.print("\nSensor ");
				Serial.print(numero);
				Serial.print(" deshabilitado");
				registro.registrarSensor(((numero != 0)? "PIR "+(String)numero:"PUERTA COCHERA")+" :"+(String)strike+" OFFLINE/PHANTOM ");
				registro.sensorInfoBD(((numero != 0)? "pir"+(String)numero:"mg"), "offline", "phantom");
				registro.saltoInfoBD("0");
			}
		}

		sensorant = sensor;



	}
