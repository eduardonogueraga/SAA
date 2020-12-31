/*
 * Pantalla.cpp
 *
 *  Created on: 24 jun. 2020
 *      Author: isrev
 *
 *
 */

#include "Arduino.h"
#include "Pantalla.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "Fecha.h"

Pantalla::Pantalla()
{}

Pantalla::~Pantalla()
{}

void Pantalla::lcdAlarmaDesactivada(LiquidCrystal_I2C &lcd){

extern Fecha tiempo;


	lcd.setCursor(0,0);
	lcd.print("ALARMA <> MENU>2");
	lcd.setCursor(0,1);
	lcd.print("APAGADA");
	lcd.setCursor(7,1);
	if(tiempo.comprobarFecha(tiempo.getFechaReset())){
		lcd.print(" R>");
	}else{
		lcd.print("   ");
	}
	lcd.setCursor(10,1);
	// @develop ('Sin hora')
	lcd.print("__-__ ");
	lcd.setCursor(15,1);
	//lcd.write(1); //Candado

	if(digitalRead(2) == HIGH){ //SENSOR_BATERIA
		lcd.print(" ");
	}else{
		lcd.print("!");

	}


}

void Pantalla::lcdAlarmaActivada(LiquidCrystal_I2C &lcd)  {

	extern bool controlUtv;  //Datos del main
	extern byte strike1;
	extern byte strike2;
	extern byte strike3;
	extern byte PIR_VALOR_1;
	extern byte PIR_VALOR_2;
	extern byte PIR_VALOR_3;
	extern long tiempoOn;


	if(controlUtv==true){  //false para debug unicamente MDBUG

		lcd.setCursor(0,0);
		lcd.print("ALARMA  ACTIVADA");
	} else{

		lcd.setCursor(0,0);
		lcd.print("PRUEBA  ACTIVADA");
	}

	lcd.setCursor(0,1);
	lcd.print("C:");
	lcd.setCursor(2,1);
	if(PIR_VALOR_1){
		lcd.print(strike1); //VALOR STRIKE 1

	} else {
		lcd.print("X"); //fuera de servicio
	}

	lcd.setCursor(4,1);
	lcd.print("P:");
	lcd.setCursor(6,1);

	if(PIR_VALOR_2){
		lcd.print(strike2); //VALOR STRIKE 2

	} else {
		lcd.print("X");
	}

	lcd.setCursor(8,1);
	lcd.print("A:");
	lcd.setCursor(10,1);
	if(PIR_VALOR_3){
		lcd.print(strike3); //VALOR STRIKE 3

	} else {
		lcd.print("X");
	}

	if(millis()> tiempoOn){
		lcd.setCursor(12,1);
		lcd.print(">RDY");


	} else {
		lcd.setCursor(13,1);
		lcd.print(">OK");

	}

}


void Pantalla::lcdClave(char key, LiquidCrystal_I2C &lcd){
	//No cubre fallos de signos especiales repetidos
	extern String entradapass;

	static String clave ="";
	if(key == '*' || key == '#' || key == '2'|| key == '0'|| (clave.length()>3)){

		clave.remove(0);

		if(key !='*' && key !='#'  && key !='2'  && key !='0'){
			entradapass="";
			clave = clave+key;
		}

	}else{
		clave = clave+key;
	}
	lcd.clear();
	lcd.setCursor(5,0);
	lcd.print("Clave");
	lcd.setCursor(5,1);
	lcd.print(clave);

}

void Pantalla::lcdInicio(LiquidCrystal_I2C &lcd, String version, String fecha){

	Wire.begin();  //Inicializacion i2c
	lcd.begin(16, 2); //Pantalla
	lcd.clear();
	lcd.backlight();

	lcd.setCursor(0,0);
	lcd.print("   BIENVENIDO");

	//version
	lcd.setCursor(0,1);
	lcd.print(version);
	lcd.setCursor(version.length()+1,1);
	lcd.print(fecha);

}

void Pantalla::lcdFalloClaveDesactivar(LiquidCrystal_I2C &lcd){

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("NO ES LA CLAVE");
	lcd.setCursor(0,1);
	lcd.print("PARA DESACTIVAR");

}


void Pantalla::lcdFalloClaveActivar(LiquidCrystal_I2C &lcd){

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("NO ES LA CLAVE");
	lcd.setCursor(0,1);
	lcd.print("PARA ACTIVAR");

}


void Pantalla::lcdFalloClaveNoValida(LiquidCrystal_I2C &lcd){

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("CLAVE NO");
	lcd.setCursor(0,1);
	lcd.print("VALIDA");

}

void Pantalla::lcdModoPrueba(LiquidCrystal_I2C &lcd){

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("MODO PRUEBA");
	lcd.setCursor(0,1);
	lcd.print("");

}

void Pantalla::lcdModoDefault(LiquidCrystal_I2C &lcd){

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("MODO DEFAULT");
	lcd.setCursor(0,1);
	lcd.print("");

}


void Pantalla::lcdBocinaTest(LiquidCrystal_I2C &lcd){

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("TEST BOCINA");
	lcd.setCursor(0,1);
	lcd.print("");

}


void Pantalla::lcdIntrusismo(LiquidCrystal_I2C &lcd, int num){

	extern String zona[];

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("INTRUSISMO");
	lcd.setCursor(0,1);
	lcd.print(zona[num]);


}

void Pantalla::lcdAvisoEnviado(LiquidCrystal_I2C &lcd){

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("ALARMA CONFIRMADA");
	lcd.setCursor(0,1);
	lcd.print("AVISO ENVIADO");

}


void Pantalla::lcdAvisoBateriaDesactivada(LiquidCrystal_I2C &lcd){

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("ATENCION BATERIA");
	lcd.setCursor(0,1);
	lcd.print("  DESCONECTADA  ");

}



void Pantalla::lcdAvisoResetAutomatico(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,0);
	lcd.print("RESET AUTOMATICO");
	lcd.setCursor(0,1);
	lcd.print("HOY A LAS: 16:30");

}


					////////////////////////////////////////////////////////////////////////////////////////

void Pantalla::menuHome0(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,1);
	lcd.print("MAS>*     SALIR#");

	lcd.setCursor(0,0);
	lcd.print("MODOS>1 ");
	lcd.setCursor(8,0);
	lcd.print("CONFIG>2");

}


void Pantalla::menuHome1(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,0);
	lcd.print("INFO>1  BOCINA>2");
	lcd.setCursor(0,1);
	lcd.print("RESET>3  ATRAS>#");

}


void Pantalla::menuResetAutomatico(LiquidCrystal_I2C &lcd){

	extern Fecha tiempo;

	lcd.setCursor(0,0);
	lcd.print("FECHA:   ATRAS>#");
	lcd.setCursor(0,1);
	lcd.print((String)tiempo.imprimeFechaSimple(tiempo.getFechaReset())+" 16:30");

}

void Pantalla::menuConfigSensor(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,0);
	lcd.print("PIR>1 BLUETOOH>2");

	lcd.setCursor(0,1);
	lcd.print("         ATRAS>#");
}


void Pantalla::menuConfigModos(LiquidCrystal_I2C &lcd, bool controlUtv){

	lcd.setCursor(0,0);
	lcd.print("NORMAL>1  TEST>2");

	if(controlUtv==true){
		lcd.setCursor(0,1);
		lcd.print(">>NORMAL ");
	} else {
		lcd.setCursor(0,1);
		lcd.print(">>TEST>>");
	}

	lcd.setCursor(8,1);
	lcd.print(" ATRAS>#");
}


void Pantalla::menuConfigBluetooh(LiquidCrystal_I2C &lcd, byte sleepmode_bluetooth){

	lcd.setCursor(0,0);
	lcd.print("ACTIVAR/APAGAR>1");

	if(sleepmode_bluetooth==1){
		lcd.setCursor(0,1);
		lcd.print(">>ON>>  ");
	} else {
		lcd.setCursor(0,1);
		lcd.print(">>OFF>> ");
	}

	lcd.setCursor(8,1);
	lcd.print(" ATRAS>#");
}


void Pantalla::menuSelectInfoMenu(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,0);
	lcd.print("VERSION>1 TIME>2");

	lcd.setCursor(0,1);
	lcd.print("DATOS>3  ATRAS>#");
}

void Pantalla::menuInfoVersion(LiquidCrystal_I2C &lcd, String version_software, String fecha){

	lcd.setCursor(0,0);
	lcd.print(version_software);

	lcd.setCursor(0,1);
	lcd.print(fecha);
	lcd.setCursor(8,1);
	lcd.print(" ATRAS>#");
}

void Pantalla::menuInfoSaltoSubmenu(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,0);
	lcd.print("HISTORICO>1   ");

	lcd.setCursor(0,1);
	lcd.print("ACTUAL>2 ATRAS>#");
}

void Pantalla::menuInfoSalto(LiquidCrystal_I2C &lcd, int saltoEnviado, int saltoFalso){

	lcd.setCursor(0,0);
	lcd.print("SMS Enviados:");

	lcd.setCursor(13,0);
	lcd.print(saltoEnviado);
	lcd.setCursor(0,1);
	lcd.print("Entradas:");
	lcd.print(saltoFalso);
	lcd.setCursor(13,1);
	lcd.print("A>#");
}

void Pantalla::menuInfoSaltoHistorico(LiquidCrystal_I2C &lcd, byte smsDireccion){

	extern char cadena[11];
	extern Fecha tiempo;

	lcd.setCursor(0,0);
	lcd.print("SMS:");

	lcd.setCursor(4,0);
	lcd.print(EEPROM.read(smsDireccion));
	lcd.setCursor(7,0);
	lcd.print("  RESET>1");

	lcd.setCursor(0,1);
	//tiempo.imprimeFechaSimple().toCharArray(cadena, 11);
	lcd.print(EEPROM.get(10, cadena));
	lcd.setCursor(10,1);
	lcd.print("   A>#");
}

void Pantalla::menuInfoTime(LiquidCrystal_I2C &lcd, String tiempo, double time_on){

	lcd.setCursor(0,0);
	lcd.print(tiempo); //String
	lcd.setCursor(8,0);//Variable debug adaptar a version final
	(tiempo.indexOf("SEGUND: ")>=0)? lcd.print((unsigned long)time_on):lcd.print(time_on); //Double para todos menos segundos
	lcd.setCursor(0,1);
	lcd.print("TIME>*   ATRAS>#");
}



void Pantalla::menuModSensor1(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,0);
	lcd.print(" ESTADO  SENSOR ");

	lcd.setCursor(0,1);
	lcd.print("MODIF>* ");
	lcd.setCursor(8,1);
	lcd.print(" ATRAS>#");
}

void Pantalla::menuModSensor2(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,0);
	lcd.print(" SELECT  SENSOR ");

	lcd.setCursor(0,1);
	lcd.print(" P1>1 P2>2 MAS>#");
}

void Pantalla::menuModSensor2_2(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,0);
	lcd.print(" SELECT  SENSOR ");

	lcd.setCursor(0,1);
	lcd.print("P3>1 MG>2 BACK>#");
}

void Pantalla::menuModSensor3(LiquidCrystal_I2C &lcd, byte PIR_AUX){

	if(PIR_AUX==4){
		lcd.setCursor(0,0);
		lcd.print(" MG ");
	}else{
		lcd.setCursor(0,0);
		lcd.print("PIR");
		lcd.setCursor(3,0);
		lcd.print(PIR_AUX);
	}

	lcd.setCursor(4,0);
	lcd.print(" SELECIONADO");

	lcd.setCursor(0,1);
	lcd.print("ON>*       OFF>#");

}

void Pantalla::menuModSensor4(LiquidCrystal_I2C &lcd){

	lcd.setCursor(0,0);
	lcd.print(" APLICAR CAMBIO ");

	lcd.setCursor(0,1);
	lcd.print("Si>* Descartar>#");

}
