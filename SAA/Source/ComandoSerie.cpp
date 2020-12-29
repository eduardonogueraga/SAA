/*
 * ComandoSerie.cpp
 *
 *  Created on: 27 jun. 2020
 *      Author: isrev
 */
#include "Arduino.h"
#include "ComandoSerie.h"
#include <SoftwareSerial.h>
#include "RegistroDatos.h"
#include <IRremote.h>
#include "Mensajes.h"

#define SENSOR_IR 7 //Ajusta el numero de pin


#define BOTON_1 0xE0E020DF
#define BOTON_2 0xE0E0A05F
#define BOTON_3 0xE0E0609F


SoftwareSerial bluetooh(11, 10);  // pin 11 RX, pin 10 TX (Se invierten en placa)

IRrecv irrecv(SENSOR_IR);   //Pin 8 para infrarojo
decode_results codigo;


ComandoSerie::ComandoSerie() {
	// TODO Auto-generated constructor stub

}

ComandoSerie::~ComandoSerie() {
	// TODO Auto-generated destructor stub
}


void ComandoSerie::iniciarBlueetooh(){

	  Serial.println("Bluetooh READY");
	  bluetooh.begin(38400);

	  irrecv.enableIRIn();   //Inicia Infrarojo

}

void ComandoSerie::controInfraRojo(){
	if(irrecv.decode(&codigo)){
		Serial.println(codigo.value, HEX);


		if(codigo.value == BOTON_1){
			//Activar
			lcd.clear();
			rcEstado= false;
			activar(rcEstado);
		}

		if(codigo.value == BOTON_2){

			//Desactivar
			rcEstado= false;
			desactivar(rcEstado);

		}

		if(codigo.value == BOTON_3){

			//Modo
			cambiarModo();

		}

		irrecv.resume(); //Resetea la entrada hexadecimal
	}
}

void ComandoSerie::demonioSerie(){

	extern RegistroDatos registro;
	extern Mensajes mensaje;

	if (Serial.available() > 0 || bluetooh.available() > 0){

		if(Serial.available() > 0)
			data = Serial.readStringUntil('\n');

		if(bluetooh.available() > 0)
			data = bluetooh.readStringUntil('\n');



		if(data.indexOf("set on")>=0)
		{
			//Activar
			lcd.clear();
			rcEstado= false;
			activar(rcEstado);

		}
		if (data.indexOf("set off")>=0){

			//Desactivar
			rcEstado= false;
			desactivar(rcEstado);

		}

		if (data.indexOf("bocina test")>=0){

			testBocina();

		}


		if (data.indexOf("set mode")>=0){

			//Modo
			cambiarModo();
		}


		if (data.indexOf("set menu")>=0){

			//Modo
			setMenu();
		}

		if (data.indexOf("pir1")>=0){

			//Modo
			pir1.pingSensor();
		}

		if (data.indexOf("pir2")>=0){

			//Modo
			pir2.pingSensor();
		}

		if (data.indexOf("pir3")>=0){

			//Modo
			pir3.pingSensor();
		}


		if (data.indexOf("show reg")>=0){

			//Modo
			registro.mostrarRegistro("REGISTRO");
		}

		if (data.indexOf("show sensor")>=0){

			//Modo
			registro.mostrarRegistro("SENSORES");

		}

		if (data.indexOf("show prop")>=0){

			//Modo
			registro.mostrarRegistro("PROP");

		}


		if (data.indexOf("set call")>=0){

			//Modo
			mensaje.realizarLlamada(true, 1); //No testado
			//mensaje.setTiempoLlamada(millis()+65000);
		}


	}

}

void ComandoSerie::testBocina(){

	pantalla.lcdBocinaTest(lcd);

	bocinaTiempo = millis() + 10000;
    bocina.iniciarBocina(1000, bocinaTiempo);

}


void ComandoSerie::cambiarModo(){

	extern bool controlUtv;

	if(estadoalarma==false){

		if (controlUtv==true){

			pantalla.lcdModoPrueba(lcd);

			controlUtv =false;
			controlUtvAcceso =1;
			entradapass = "";

		} else{
			pantalla.lcdModoDefault(lcd);

			controlUtv =true;
			controlUtvAcceso =1;
			entradapass = "";
		}

	}

}


void ComandoSerie::setMenu(){

	if(estadoalarma==false){

		estadoMenu = true;
		entradapass = "";
		key = NO_KEY;
		menu.setEstadoMenu(true);

	}

}
