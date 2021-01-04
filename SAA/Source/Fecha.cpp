/*
 * Fecha.cpp
 *
 *  Created on: 29 jun. 2020
 *      Author: isrev
 */


#include "Arduino.h"
#include "Fecha.h"
#include <RTClib.h>

RTC_DS3231 rtc; //Instania de rtc

Fecha::Fecha() {
}

Fecha::~Fecha() {
	// TODO Auto-generated destructor stub
}



void Fecha::iniciarRTC(){
	 if (! rtc.begin()) {    // Si falla la inicializacion del modulo
		 Serial.println("Modulo RTC no encontrado !");
		 //while (1);
		 }
		 //rtc.adjust(DateTime(__DATE__, __TIME__));  //Establecer tiempo
}

String Fecha::imprimeFecha(DateTime paramFecha){

	if(paramFecha > 0){
		fecha = paramFecha;
	}else{
		fecha = rtc.now();
	}


	tiempo = (String)((fecha.day()<=9) ? ("0"+(String)fecha.day()) : (String)fecha.day())
					+"/"+(String)((fecha.month()<=9) ? ("0"+(String)fecha.month()) : (String)fecha.month())
					+"/"+(String)fecha.year()
					+" "+(String)((fecha.hour()<=9) ? ("0"+(String)fecha.hour()) : (String)fecha.hour())
					+":"+(String)((fecha.minute()<=9) ? ("0"+(String)fecha.minute()) : (String)fecha.minute())
					+":"+ (String)((fecha.second()<=9) ? ("0"+(String)fecha.second()) : (String)fecha.second());

	return tiempo;

}


String Fecha::imprimeFechaSQL(){

		fecha = rtc.now();
		tiempo = (String)fecha.year()
						+"-"+(String)((fecha.month()<=9) ? ("0"+(String)fecha.month()) : (String)fecha.month())
						+"-"+(String)((fecha.day()<=9) ? ("0"+(String)fecha.day()) : (String)fecha.day())
						+" "+(String)((fecha.hour()<=9) ? ("0"+(String)fecha.hour()) : (String)fecha.hour())
						+":"+(String)((fecha.minute()<=9) ? ("0"+(String)fecha.minute()) : (String)fecha.minute())
						+":"+(String)((fecha.second()<=9) ? ("0"+(String)fecha.second()) : (String)fecha.second());
		return tiempo;

}

String Fecha::imprimeFechaSimple(DateTime paramFecha){

	if(paramFecha > 0){
			fecha = paramFecha;
		}else{
			fecha = rtc.now();
		}

	tiempo = (String)((fecha.day()<=9) ? ("0"+(String)fecha.day()) : (String)fecha.day())
					+"/"+(String)((fecha.month()<=9) ? ("0"+(String)fecha.month()) : (String)fecha.month())
					+"/"+(String)fecha.year();

	return tiempo;

}


String Fecha::imprimeHora(){

	fecha = rtc.now();

	tiempo = (String)((fecha.hour()<=9) ? ("0"+(String)fecha.hour()) : (String)fecha.hour())
			+":"+(String)((fecha.minute()<=9) ? ("0"+(String)fecha.minute()) : (String)fecha.minute());

	return tiempo;

}

bool Fecha::comprobarHora(int horas, int minutos){ //Hora concreta

	 fecha = rtc.now();


	if((byte)horas == fecha.hour()){

		if ((byte)minutos == fecha.minute() || (byte)minutos == 0){

			return true;
		}else {
			return false;
		}

	}else {
		return false;
	}


}


bool Fecha::comprobarRangoHorario(int hora_inicio, int hora_fin,  int min_inicio, int min_fin){ //Rango de horas

	fecha = rtc.now();

	int aux=0;

			if(hora_inicio > hora_fin) { //Si hay diferencia entre dias entrara aqui

				if((int)fecha.hour() >= hora_inicio) {

					hora_fin = hora_fin +24; //Hasta las 12 contempla horas posteriores como mayores(+24 horas)

				}

				if((int)fecha.hour() <= 23){ //Cuando son menos de las 12 contempla la hora de inicio como inferior (-24 horas)

					hora_inicio = hora_inicio - 24;

				}

				aux = 24; //Ajusta el tiempo las primeras horas del ultimo dia
			}else {
				aux = 0;
			}


			if((int)fecha.hour() == hora_inicio+aux) {

				if(((int)fecha.hour() >= hora_inicio && (int)fecha.minute() >= min_inicio) && (hora_fin > (int)fecha.hour())) {

					return true;
				}else {
					return false;
				}


			}else if((int)fecha.hour() == hora_fin) {

				if(((int)fecha.hour() >= hora_inicio) && (hora_fin >= (int)fecha.hour() && (int)fecha.minute() < min_fin )) {

					return true;
				}else {
					return false;
				}

			}else if ((int)fecha.hour() != hora_inicio && (int)fecha.hour() != hora_fin){

				if(((int)fecha.hour() >= hora_inicio ) && (hora_fin > (int)fecha.hour())) {

					return true;
				}else {
					return false;
				}
			}else {
				return false;
			}

}

void Fecha::establecerFechaReset(int dia, int hora, int minuto, int segundo){

	DateTime fechaFutura (rtc.now() + TimeSpan(dia,hora,minuto,segundo));
	fechaReset = fechaFutura;

}

DateTime Fecha::getFechaReset(){
	return fechaReset;
}


bool Fecha::comprobarFecha(DateTime paramFecha){

	fecha = rtc.now();

	if(fecha >= paramFecha){
		return true;
	}else{
		return false;
	}

}
