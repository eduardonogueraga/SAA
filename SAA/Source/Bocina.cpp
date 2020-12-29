/*
 * Bocina.cpp
 *
 *  Created on: 26 jun. 2020
 *      Author: isrev
 */

#include "Bocina.h"
#include "Arduino.h"

Bocina::Bocina()
{}

Bocina::~Bocina()
{}

	void Bocina::iniciarBocina(int bocinaIntervalo, long bocinaTiempoFuncionamiento) {
		this->bocinaIntervalo = bocinaIntervalo;
		this->bocinaTiempoFuncionamiento = bocinaTiempoFuncionamiento;
	}


	void Bocina::comprobarBocina() {

		extern byte bocinaPin;

		if (bocinaTiempoFuncionamiento > millis()){

			if((millis()-bocinaTiempoAntEncendido>=bocinaIntervalo)	&& bocinaEstado==1){
				//Serial.println("BOCINA OUT");
				bocinaEstado=0;
				digitalWrite(bocinaPin,bocinaEstado);
				bocinaTiempoAntApagado=millis();
			}
			if((millis()-bocinaTiempoAntApagado>=bocinaIntervalo) && bocinaEstado==0){
				//Serial.println("BOCINA IN");
				bocinaEstado=1;
				digitalWrite(bocinaPin,bocinaEstado);
				bocinaTiempoAntEncendido=millis();
			}

		} else {
			bocinaEstado=0;
			digitalWrite(bocinaPin,bocinaEstado);
			bocinaTiempoAntEncendido = 0;
			bocinaTiempoAntApagado = 0;
		}
	}

	void Bocina::detenerBocina() {
		Serial.println("BOCINA DETENIDA");
		iniciarBocina(Bocina::bocinaIntervalo,0); //Vuelve al inicio
	}


