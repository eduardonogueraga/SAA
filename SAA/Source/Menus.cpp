/*
 * Menus.cpp
 *
 *  Created on: 24 jun. 2020
 *      Author: isrev
 */

#include "Menus.h"
#include "Pantalla.h"
#include <Keypad.h>
#include "Bocina.h"
#include "Fecha.h"
#include <EEPROM.h>

Menus::Menus()
{
}

Menus::~Menus()
{}


void Menus::setEstadoMenu(bool estadoMenu){

	this->estadoMenu = estadoMenu;
}

bool Menus::getEstadoMenu(){

	return estadoMenu;
}


void Menus::menuManager(LiquidCrystal_I2C &lcd, Pantalla &pantalla, Bocina &bocina, Fecha &_fecha){



	extern char key;   //Revisa las variables del main
	extern String entradapass;
	extern bool controlUtv;
	extern byte controlUtvAcceso;
	extern String version_software;
	extern String fecha;
	extern int saltoEnviado;
	extern int saltoFalso;
	extern byte smsDireccion;
	extern byte PIR_AUX;
	extern byte PIR_VALOR_1;
	extern byte PIR_VALOR_2;
	extern byte PIR_VALOR_3;
	extern byte PIR_VALOR_4;
	extern byte PIR_VALOR_ANT_1;
	extern byte PIR_VALOR_ANT_2;
	extern byte PIR_VALOR_ANT_3;
	extern byte PIR_VALOR_ANT_4;
	extern byte sleepmode_bluetooth;



	switch (HOME_NEXT) {

	case 1:
		//SUBMENU PRINCIPAL 1
		pantalla.menuHome0(lcd);

		if(key == '#') {setEstadoMenu(false); entradapass = ""; pantalla.lcdAlarmaDesactivada(lcd);}  //SALIR
		if(key == '1'){ CONFIG_NEXT=2;  HOME_NEXT =0; key = NO_KEY;}
		if(key == '2'){ CONFIG_NEXT=1;  HOME_NEXT =0; key = NO_KEY;}
		if(key == '*')  HOME_NEXT =2;

		break;


	case 2:
		//SUBMENU PRINCIPAL 2
		pantalla.menuHome1(lcd);

		if(key == '1') {INFO_NEXT=1;HOME_NEXT =0; key = NO_KEY;}  //SALIR
		if(key == '2'){   Serial.println("");
		Serial.println("Prueba de bocina");
		bocina.iniciarBocina(2000, (8000+ millis()));
		entradapass = "";
		}
		if(key == '3') {INFO_NEXT=7;HOME_NEXT =0; key = NO_KEY;}
		if(key == '#') HOME_NEXT =1;

		break;


	}
	//////////CONFIG/////////////

	switch (CONFIG_NEXT) {

	case 1:
		//SUBMENU CONFIGURACION DE SENSORES TIPO PIR
		pantalla.menuConfigSensor(lcd);

		if(key == '1')  {SENSOR_NEXT=1; CONFIG_NEXT =0;}  //SALIR
		if(key == '2')  {CONFIG_NEXT =3;}  //Bluetooh
		if(key == '#') {CONFIG_NEXT=0;HOME_NEXT = 1;}

		break;


	case 2:
		//SUBMENU CONFIGURACION DE MODALIDADES
		pantalla.menuConfigModos(lcd, controlUtv);

		if(key == '1')  {controlUtv = true;
		controlUtvAcceso =1;
		entradapass = "";}

		if(key == '2')  {controlUtv =false;
		controlUtvAcceso =1;
		entradapass = "";}


		if(key == '#') {CONFIG_NEXT=0;HOME_NEXT = 1;}

		break;


	case 3:
		//SUBMENU CONTROL BLUETOOH

		pantalla.menuConfigBluetooh(lcd, sleepmode_bluetooth);

		if(key == '1')  {bluetooh_estado++;}


		if(bluetooh_estado >2){
			bluetooh_estado =1;
		}

		switch (bluetooh_estado) {

		case 1:

			sleepmode_bluetooth = 1;
			entradapass = "";

			break;

		case 2:
			sleepmode_bluetooth = 2;
			entradapass = "";
			break;
		}

		if(key == '#') {CONFIG_NEXT=1;}

		break;


	}

	/////////INFO//////////////
	switch (INFO_NEXT) {

	case 1: {
		pantalla.menuSelectInfoMenu(lcd);

		if(key == '1')  {INFO_NEXT=2; lcd.setCursor(0,0); lcd.clear();}
		if(key == '2')  {INFO_NEXT=3; lcd.setCursor(0,0); lcd.clear();}
		if(key == '3')  {INFO_NEXT=4; lcd.setCursor(0,0); lcd.clear();}
		if(key == '#') {INFO_NEXT=0;HOME_NEXT = 2;}
	}
	break;

	case 2:{
		//SUBMENU INFORMACION DE VERSION
		pantalla.menuInfoVersion(lcd, version_software, fecha);

		if(key == '#') {INFO_NEXT=1;}
	}
	break;

	case 3:
	{
		//SUBMENU INFORMACION TIEMPO DE FUNCIONAMIENTO

		double time_seg = (millis()/1000);
		double  time_on = 0;
		if(time_change >4){ //5
			time_change =1;
		}

		switch (time_change) {
/*
		case 1:
			tiempo ="SEGUND: ";
			time_on = time_seg;


			break;
*/
		case 1:
			tiempo ="MINUTO: ";
			time_on = (time_seg/60);
			break;

		case 2:
			tiempo ="HORAS: ";
			time_on = ((time_seg/60)/60);
			break;



		case 3:
			tiempo ="DIAS: ";
			time_on = (((time_seg/60)/60)/24);
			break;


		case 4:
			tiempo ="SEMANAS:";
			time_on = ((((time_seg/60)/60)/24)/7);

			break;

		}

		pantalla.menuInfoTime(lcd, tiempo, time_on);


		if(key == '#') {INFO_NEXT=1; time_change =3;} //Para que lo primero que tengas al salir sea dias
		if(key == '*') {lcd.clear(); time_change++;}
	}
	break;

	case 4:{

		//SUBMENU INFORMACION DE SALTOS DE LA ALARMA
		pantalla.menuInfoSaltoSubmenu(lcd);

		if(key == '2')  {INFO_NEXT=5; lcd.setCursor(0,0); lcd.clear();}
		if(key == '1')  {INFO_NEXT=6; lcd.setCursor(0,0); lcd.clear();}
		if(key == '#') {INFO_NEXT=1;}
	}
	break;

	case 5:{

		//SUBMENU INFORMACION DE FALSA ALARMA Y ENTRADAS LOCALES

		pantalla.menuInfoSalto(lcd, saltoEnviado, saltoFalso);

		if(key == '#') {INFO_NEXT=4; lcd.setCursor(0,0); lcd.clear();}
	}
	break;

	case 6:{
		//SUBMENU INFORMACION DE HISTORICO DE SMS ENVIADOS

		extern char cadena[11];

		pantalla.menuInfoSaltoHistorico(lcd, smsDireccion);


		if(key == '#') {INFO_NEXT=4; lcd.setCursor(0,0); lcd.clear();}
		if(key == '1') {

			EEPROM.update(smsDireccion, 0);
			_fecha.imprimeFechaSimple().toCharArray(cadena, 11); //Parsea Strig a cadena
			EEPROM.put(10, cadena);
			lcd.setCursor(0,0);
			lcd.clear();}
	}
	break;


	case 7:{
		//SUBMENU INFORMACION DE AUTORESET
		pantalla.menuResetAutomatico(lcd);
		if(key == '#') {INFO_NEXT=0;HOME_NEXT = 2;}

	}
	break;

	}

	////////SENSOR///////////////
	switch (SENSOR_NEXT) {
	case 1:
		//SUBMENUS DE CONFIGURACION DE SENSORES
		pantalla.menuModSensor1(lcd);

		if(key == '#'){ CONFIG_NEXT=1; SENSOR_NEXT=0;}  //SALIR
		if(key == '*') SENSOR_NEXT=2;
		break;
	case 2:

		pantalla.menuModSensor2(lcd);

		if(key == '1') { PIR_AUX=1;  SENSOR_NEXT=4;}
		if(key == '2') { PIR_AUX=2;  SENSOR_NEXT=4;}
		if(key == '#') { SENSOR_NEXT=3;}

		break;
	case 3:
		pantalla.menuModSensor2_2(lcd);

		if(key == '1') { PIR_AUX=3;  SENSOR_NEXT=4;}
		if(key == '2') { PIR_AUX=4;  SENSOR_NEXT=4;}
		if(key == '#') { SENSOR_NEXT=2; key = NO_KEY; }
		break;
	case 4:

		pantalla.menuModSensor3(lcd, PIR_AUX);


		if(key == '#'){

			if(PIR_AUX==1){

				PIR_VALOR_ANT_1=PIR_VALOR_1;
				PIR_VALOR_1=0;
			}
			if (PIR_AUX==2){

				PIR_VALOR_ANT_2=PIR_VALOR_2;
				PIR_VALOR_2=0;
			}
			if (PIR_AUX==3){

				PIR_VALOR_ANT_3=PIR_VALOR_3;
				PIR_VALOR_3=0;
			}

			if (PIR_AUX==4){

				PIR_VALOR_ANT_4=PIR_VALOR_4;
				PIR_VALOR_4=0;
			}

			SENSOR_NEXT=5;
		}

		if(key == '*'){

			if(PIR_AUX==1){

				PIR_VALOR_ANT_1=PIR_VALOR_1;
				PIR_VALOR_1=1;
			} else if (PIR_AUX==2){

				PIR_VALOR_ANT_2=PIR_VALOR_2;
				PIR_VALOR_2=1;
			} else if (PIR_AUX==3){

				PIR_VALOR_ANT_3=PIR_VALOR_3;
				PIR_VALOR_3=1;
			} else if (PIR_AUX==4){

				PIR_VALOR_ANT_4=PIR_VALOR_4;
				PIR_VALOR_4=1;
			}


			SENSOR_NEXT=5;
		}

		break;

	case 5:

		pantalla.menuModSensor4(lcd);

		if(key == '#') {

			if(PIR_AUX==1){

				PIR_VALOR_1=PIR_VALOR_ANT_1;

			} else if (PIR_AUX==2){

				PIR_VALOR_2=PIR_VALOR_ANT_2;

			} else if (PIR_AUX==3){

				PIR_VALOR_3=PIR_VALOR_ANT_3;

			} else if (PIR_AUX==4){

				PIR_VALOR_4=PIR_VALOR_ANT_4;

			}

			SENSOR_NEXT=1;
			key = NO_KEY;

		}
		if(key == '*') {
			SENSOR_NEXT=1;
			key = NO_KEY;
		}

		break;

	}

}






