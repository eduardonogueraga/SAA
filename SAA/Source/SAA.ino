/**
 * SISTEMA DE ALARMA DE LA ALBERQUILLA
 * Version:
 * Leyenda:  UTV, ACCESO A GSM, MDBUG
 * Por hacer: Escribir en variables SD, Desactivar la bocina?, Investigar sobre refrescos i2c por tiempo, Sistema de deteccion fotosensible, Sistema de sabotaje
 *
 * Atencion:
 *
 * -Cambiar MDBUG Especial atencion a la interrupcion de bateria
 *
 */

//Version-info
String version_software = "VE19R1-01 ";
String fecha = "02/01/21";


#include "Arduino.h"
#include <SoftwareSerial.h> //Libreria Software Serial
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Keypad.h>
#include "Fecha.h"

#include "InterStrike.h"
#include "Datos.h"
#include "Mensajes.h"
#include "Pantalla.h"
#include "Menus.h"
#include "Bocina.h"
#include "ComandoSerie.h"
#include "RegistroDatos.h"
#include "Entorno.h"


//Variables de teclado

char key; //Variable de tecla

const byte ROWS = 4; // 4 filas
const byte COLS = 3; // 3 columnas
char keys[ROWS][COLS] = {

		{'1','2', '3'},
		{'4','5', '6'},
		{'7','8', '9'},
		{'*','0', '#'}
};
	byte rowPins[ROWS] = {36, 34, 32, 30}; 	// Salidas en placa para las filas
	byte colPins[COLS] = {38, 40, 42}; 		// Entrada en placa para las columnas


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define GSM_TX 8
#define GSM_RX 9

SoftwareSerial SIM800L(GSM_TX,GSM_RX); 		 //SIM800L parametros pines TX y RX
LiquidCrystal_I2C lcd (0x3F, 16, 2); 		 //El codec cambia en la pantalla nueva  0x3F 0x27


//Instancias de interstrike
InterStrike pir1;
InterStrike pir2;
InterStrike pir3;
InterStrike mg;

//Instancias varias
Datos datos;
Datos datosPhantom;

Mensajes mensaje;
Pantalla pantalla;
Menus menu;
Bocina bocina;
Fecha tiempo;
RegistroDatos registro;

//Instancia de serie MDBUG
ComandoSerie demon;



//Varibles EEPROM

#define ALARMA_ACTIVADA 0 //Guarda el estado en el que se encontraba la alarma
#define ALARMA_SALTADA 1 //Guarda el salto de la alarma
#define SENSOR_SALTADO 2 //Guarda el numero del sensor detonante
#define CONTROL_INTERRUPCION 3 //Solo permite la ejecucion de la interrupcion una vez

byte estadoAnterior = 0;
byte smsDireccion = 25; // eeprom historico sms direcccion


//Variables de password
String entradapass = "";
String entrada = "";         		// Contiene la entrada de teclado
boolean entradacompleta = false;    // Contiene si la entrada esta entera o no


//Variables sensores interStrike
bool disparadorPir = false;
bool estadoSensorPir;

#define PIR_MOTION_SENSOR_1 43
#define PIR_MOTION_SENSOR_2 45
#define PIR_MOTION_SENSOR_3 47
#define PIR_MOTION_SENSOR_4 49
#define MG_SENSOR 48

bool sensor0 = false;
bool sensor1;
bool sensor2;
bool sensor3;

enum valiza {PUERTA, COCHERA, PORCHE, ALMACEN};
String zona[4] {"PUERTA COCHERA","COCHERA","PORCHE","ALMACEN"};
int valiza;


byte strike1 = 0;
byte strike2 = 0;
byte strike3 = 0;
byte strike4 = 0;

byte PIR_VALOR_1 = 1;  //Estados de activacion de cada sensor
byte PIR_VALOR_ANT_1 =0;
byte PIR_VALOR_2 = 1;
byte PIR_VALOR_ANT_2 =0;
byte PIR_VALOR_3 = 1;
byte PIR_VALOR_ANT_3 =0;
byte PIR_VALOR_4 = 1;
byte PIR_VALOR_ANT_4 =0;

byte PIR_AUX;


//Variables estado de Alarma
boolean estadoalarma = false;
boolean ladrones = false;
byte avisointrusismo = 0;
byte avisoalarma = 0;

long tiempoOff = 0;  //Tiempo para desactivar
long tiempoOn = 0;   //Tiempo despues de activar


//Variables de reset
#define RESETEAR 46

//Variables ledCochera
#define LED_COCHERA 44

//Variables bocina
byte bocinaPin = 23;
long bocinaTiempo = 0;  //Tiempo de funcionamiento


//Variables sleepMode
byte sleepmode_gsm = 0; //Control de alimentacion GSM
byte sleepmode_bluetooth = 2;	//Control de alientacion bluetooth

byte sleep_t1 = 12;   // bluetooth
byte sleep_t2 = 28;   // gsm
long sleep_tiempo = 0;


//Variables modoSensible
boolean modo_sensible = false;
long modo_sensible_tiempo = 0;

//Variables comprobarModo
 bool controlUtv =true; //true para versiones definitivas MDBUG
 byte controlUtvAcceso =1;


//Variables autoReactivacion
 bool rcEstado= false; //Modos de funcion activar/desactivar

 byte auto_rc_intentos = 0;
 boolean auto_rc = false;
 long auto_rc_time = 0; //Tiempo hasta que se reactiva
 long auto_rc_time_delay = 0; //Tiempo de acoplacion entre desactivacion y activacion rc


//Variables Menu
 bool estadoMenu = false;

//Variables datosSms
 int saltoFalso = 0; //Si salta pero se desactiva
 int saltoEnviado = 0; //Veces que se ha enviado un SMS
 int saltoEnviadoHistorico = 0;


 //Watchdog
 #define WATCHDOG 22

 //Control bateria
 #define SENSOR_BATERIA 2 //Monitoriza el flujo de tension en el booster
 #define FALLO_BATERIA 19 //Interrupcion por fallo en la bateria

 bool sensorBateriaAnterior; //Compara el estado de la bateria

 //Cupo de mensajes diarios maximos
 #define MENSAJES_ENVIADOS 5
 const unsigned int LIMITE_MAXIMO = 15; //MDBUG Cupo de SMS q se pueden mandar en un dia


//Variables de tiempo UTV

 long   utv_tiempo_off = 0;         //TIEMPO HASTA QUE SE ENVIA EL MENSAJE  1M 20s         //3 M       180000 / 20000
 long   utv_tiempo_off_beta = 0;   	//TIEMPO HASTA QUE SE ENVIA EL MENSAJE MODO SENSIBLE   10 S 10000 /  5000

 long   utv_tiempo_on = 0;             //TIEMPO DE MARGEN DESPUES DE ACTIVAR   10M       600000 / 8000
 long   utv_tiempo_on_remoto = 0;      //TIEMPO DE MARGEN DE ACTIVACION REMOTA 1 M       60000  / 15000
 long   utv_tiempo_reactivacion = 0;   //TIEMPO HASTA QUE SE AUTO-REACTIVA     4 M       240000 / 20000
 long	utv_tiempo_sensible = 3600000; //TIEMPO DURACION EN MODO SENSIBLE	     1 H      3600000‬ /  60000
 long   utv_tiempo_bocina = 0;         //TIEMPO DURACION DE LA BOCINA         10 M       600000 / 20000
 long   utv_tiempo_espera_gsm = 0;     //TIEMPO DE MARGEN PARA APAGAR  GSM    20 M      1200000 / 60000


 //Temporal
 long lcd_tiempo_clave = 0;  //	TIEMPO QUE APARECE LA PANTALLA CLAVE

 //Fecha SMS Historico
 char cadena[11]; //Formato DD/MM/YYYY

//Variable global para codigos de error e interrupciones
 volatile byte codigoError = 0;


void setup() {

//Inicializamos la primera comunicacion Serial.

	Serial.begin(9600);


//Declaracion sensores
  pinMode(MG_SENSOR, INPUT_PULLUP);
  pinMode(PIR_MOTION_SENSOR_1, INPUT); //pull down externo
  pinMode(PIR_MOTION_SENSOR_2, INPUT);
  pinMode(PIR_MOTION_SENSOR_3, INPUT);

//Declaracion bocina
  pinMode(bocinaPin, OUTPUT);  //Bocina
  digitalWrite(bocinaPin, false);

//Declaracion ledCochera
  pinMode(LED_COCHERA, OUTPUT);
  digitalWrite(LED_COCHERA, LOW);

//Declaracion reset
  pinMode(RESETEAR,OUTPUT);

//Declaracion sleepMode
  pinMode(sleep_t1, OUTPUT);
  pinMode(sleep_t2, OUTPUT);
  digitalWrite(sleep_t1, LOW);
  digitalWrite(sleep_t2, HIGH);

//Control watchdog
pinMode(WATCHDOG, OUTPUT);

//Inicializacion RTC
tiempo.iniciarRTC();

//Inicializar Blueetooh y InfraRojo

demon.iniciarBlueetooh();

//Inicializacion I2C y Pantalla
pantalla.lcdInicio(lcd,version_software, fecha);


//Inicializacion Tarjeta SD
registro.iniciarSd();

//Inicializacion SIM800L
mensaje.inicioSIM800(SIM800L);

//Control bateria
pinMode(SENSOR_BATERIA, INPUT_PULLUP);

//Interrupciones del sistema MDBUG Deshabilitar solo durante testeo
attachInterrupt(digitalPinToInterrupt(FALLO_BATERIA), interrupcionFalloAlimentacion, FALLING); //Flanco de bajada

//Comprobar estado previo de la alarma

if ((EEPROM.read(ALARMA_ACTIVADA) == 1) || (EEPROM.read(ALARMA_SALTADA) == 1)){
	estadoAnterior = 1; //Si alguna de las variables estaba activa la restaura al estado anterior
	}


//Info de inicio

Serial.print("Version SAA: ");
Serial.print(version_software);
Serial.println(fecha);
Serial.println("UTV, GSM, MDBUG");
Serial.println("set on, set off, set mode");


//Perido de reset automatico

tiempo.establecerFechaReset(10); //Dentro de 10 dias
Serial.println();
Serial.println("La alarma se reiniciara en la fecha: ");
Serial.println(tiempo.imprimeFechaSimple(tiempo.getFechaReset()));
Serial.println("En la hora prevista 16:30");

}

void loop() {

//WATCHDOG
 watchDog();

 demon.demonioSerie(); //Comandos serie MDBUG
 demon.controInfraRojo();

 //Control de bateria de emergencia

 if(digitalRead(SENSOR_BATERIA) != sensorBateriaAnterior){
	 if(digitalRead(SENSOR_BATERIA) == HIGH){
		 registro.registrarEvento("ACTIVADA BATERIA DE EMERGENCIA");
	 } else{
		 registro.registrarEvento("DESACTIVADA BATERIA DE EMERGENCIA");
	 }

 }

 sensorBateriaAnterior=digitalRead(SENSOR_BATERIA);


 //CUPO DE MENSAJES DIARIOS
 if(digitalRead(FALLO_BATERIA) == HIGH){ //Si no hay fallo con el RTC prosigue

	 if(tiempo.comprobarHora(0, 0)){ //A las 12 cada dia comprueba si se han gastado mensajes
		 if(EEPROM.read(MENSAJES_ENVIADOS) != 0){
			 EEPROM.write(MENSAJES_ENVIADOS,0);
			 registro.registrarEvento("INTENTOS SMS DIARIOS RECUPERADOS");
			 Serial.println("Intentos diarios recuperados");
		 }
	 }

 }


//PANTALLAS REPETITIVAS

if(millis()> 8000){ //Segundos para mostrar la pantalla bienvenida

	if ((ladrones == false) && (estadoalarma == true)){

		if(millis()>lcd_tiempo_clave){
			pantalla.lcdAlarmaActivada(lcd);
		}
	}else if ((estadoMenu == false)&&(estadoalarma == false)){

		if(millis()>lcd_tiempo_clave){
			pantalla.lcdAlarmaDesactivada(lcd);  //Para ver la hora
		}

	}

}

//LED COCHERA

	if ((digitalRead(MG_SENSOR) == HIGH) && (millis() < tiempoOn) && (estadoalarma == true) && (PIR_VALOR_4==1)){

		digitalWrite(LED_COCHERA, HIGH);

	} else if((digitalRead(MG_SENSOR) == HIGH) && (millis() < tiempoOn) && (estadoalarma == true) && (PIR_VALOR_4==0)){


		ledParpadeo();

	} else {
		digitalWrite(LED_COCHERA, LOW);
	}


////////SENSORES MG / PIR//////////////////////////////////////////////////

    sensor0=digitalRead(MG_SENSOR);

    sensor1 = digitalRead(PIR_MOTION_SENSOR_1);
    sensor2 = digitalRead(PIR_MOTION_SENSOR_2);
    sensor3 = digitalRead(PIR_MOTION_SENSOR_3);





//CLASE INTERSTRIKE//

    if (estadoalarma == true && disparadorPir == false){

    	pir1.setSensor(sensor1); // Define el sensor
    	//pir1.setTiempo(15000, 30000);
    	pir1.setTiempo(5000, 60000);
    	pir1.compruebaEstado(1, PIR_VALOR_1); //Recibe el numero de pir  y lo monitorea

    	pir2.setSensor(sensor2);
    	//pir2.setTiempo(20000, 20000);
    	pir2.setTiempo(7000, 20000);
    	pir2.compruebaEstado(2, PIR_VALOR_2); // @suppress("Ambiguous problem")

    	pir3.setSensor(sensor3);
    	//pir3.setTiempo(15000, 21000);
    	pir3.setTiempo(5000, 21000);
    	pir3.compruebaEstado(3, PIR_VALOR_3); // @suppress("Ambiguous problem")

    	mg.setSensor(sensor0);
    	mg.compruebaEstado(PIR_VALOR_4);

    	if(millis() > tiempoOn){

    		strike1=pir1.getStrike(); //Asigna el numero de strikes
    		strike2=pir2.getStrike();
    		strike3=pir3.getStrike();

    		strike4=mg.getStrike();


    		if(pir1.disparador(strike1, 3, 1, datos)){
    			Serial.println("\nPIR1");
    			estadoSensorPir = true;
    			valiza = COCHERA;
    		}

    		if(pir2.disparador(strike2, 2, 2, datos)){
    			Serial.println("\nPIR2");
    			estadoSensorPir = true;
    			valiza = PORCHE;
    		}

    		if(pir3.disparador(strike3, 2, 3, datos)){
    			Serial.println("\nPIR3");
    			estadoSensorPir = true;
    			valiza = ALMACEN;
    		}

    		if(mg.disparador(strike4, 1, 0, datos)){ //InterStrike como MG
    			Serial.println("\nMG");
    			estadoSensorPir = true;
    			valiza = PUERTA;

    		}

    		if (estadoSensorPir == true){  //Si algun sensor se dispara borra el buffer de todos los sensores
    			pir1.setStart();
    			pir2.setStart();
    			pir3.setStart();
    			mg.setStart();
    		}



    	} else {
    		pir1.setStart(); //Elimina los strikes acumulados
    		pir2.setStart(); //Chapuza hasta que arregle los pir
    		pir3.setStart();
    		mg.setStart();
    	}
    } else {
    	estadoSensorPir = false;  //Para que vuelva al estado anterior
    	strike1=0;
    	strike2=0;
    	strike3=0;
    	strike4=0;
    }



    if(auto_rc_time > millis()){   //Modo phantom

    	mg.setSensor(sensor0);
    	pir1.setSensor(sensor1);
    	pir2.setSensor(sensor2);
    	pir3.setSensor(sensor3);

    	pir1.compruebaPhantom(1, PIR_VALOR_1, datosPhantom); // @suppress("Ambiguous problem")
    	pir2.compruebaPhantom(2, PIR_VALOR_2, datosPhantom); // @suppress("Ambiguous problem")
    	pir3.compruebaPhantom(3, PIR_VALOR_3, datosPhantom); // @suppress("Ambiguous problem")
    	mg.compruebaPhantom(0, PIR_VALOR_4, datosPhantom);   // @suppress("Ambiguous problem")
    }

/*
 *
 *  Esta seccion se encarga de leer el teclado matriz y activar o desactivar la alarma
 *	mientras que la entrada sea <4  entrdapass se va construyendo a base de char key
 *	si la key es algun caracter especial pasa esto
 *  si la password coincide pero el estado no se corresponde al caracter (clave no valida para x)
 *	si la password no coincide diractamente clave no valida
 *	si todito coincide ejecuta la funcion correspondiente
 *
 *
 */


  key = keypad.getKey(); //Tecla de confirmacion
  if (key != NO_KEY){ //Si la tecla (key) es distinta de no puslsar ninguna tecla (no key) entonces ...
	  Serial.print(key);
	  lcd_tiempo_clave = millis() + 5000; //Tiempo de paron para clave
	  pantalla.lcdClave(key, lcd);
	  if (entradapass.length() < 4) {
		  entradapass = entradapass + key;
	  } else if (key == '*') {
		  if (entradapass == Entorno::getPassword()) {
			  if (estadoalarma == true) {

				  pantalla.lcdFalloClaveDesactivar(lcd);

			  } else {
				  if(estadoMenu==false){ //Comprueba si se esta intentando acceder desde el menu
					  rcEstado= false;
					  activar(rcEstado);
				  }
			  }
		  } else {

			  pantalla.lcdFalloClaveNoValida(lcd);
			  entradapass = "";
		  }
	  } else if (key == '#') {     //almohadilla
		  if (entradapass == Entorno::getPassword()) {
			  if (estadoalarma == false) {

				  pantalla.lcdFalloClaveActivar(lcd);

			  } else {
				  if(estadoMenu==false){
					  rcEstado= false;
					  desactivar(rcEstado);
				  }
			  }
		  } else {

			  pantalla.lcdFalloClaveNoValida(lcd);

			  entradapass = "";
		  }
	  } else if (key == '0') {     //Entrada teclado reset
		  if (entradapass == Entorno::getPassword()) {
			  if (estadoalarma == true) {

			  } else {
				  resetear();
			  }
		  } else {
			  pantalla.lcdFalloClaveNoValida(lcd);


			  entradapass = "";
		  }
	  } else if (key == '2') {   //Entrada teclado
		  if (entradapass == Entorno::getPassword()) {
			  if (estadoalarma == true) {

			  } else {

				  if(estadoalarma==false){
					  estadoMenu = true;
					  entradapass = "";
					  key = NO_KEY;
					  menu.setEstadoMenu(true);
				  }
			  }
		  } else {
			  pantalla.lcdFalloClaveNoValida(lcd);
			  entradapass = "";
		  }
	  }
  }


    // Si la alarma esta activada analizara la entrada, en caso de que no lo este, no hara nada.
if (millis() > tiempoOn) {
	if (ladrones == false) {
		if (estadoalarma == true){
			if (estadoSensorPir == true) { //Estado sensor

				Serial.println("\nConjunto activo");
				registro.registrarEvento("AVISO INTRUSISMO EN "+zona[valiza]);

				ladrones = true;
				disparadorPir=true;  //Cierre de la clase interstrike


				if((modo_sensible == true) && (modo_sensible_tiempo > millis())){  //Si tiempo sensible se activa cambia tiempoOff

				tiempoOff = millis() + utv_tiempo_off_beta;
				} else {
				tiempoOff = millis() + utv_tiempo_off;
				}

				avisointrusismo = 1;
				avisoalarma = 1;

				if(controlUtv==true){  //false para debug  Evita que las entradas se guarden en modo prueba  MDBUG
					saltoFalso++;
				}
			}
		}
	}
}


// Si salta la alarma mantiene la alerta hasta que se desactiva.
if (ladrones){
	if (avisointrusismo == 1) {

		//REV SLEEPMODE//
		sleepmode_gsm = 2; //Control del switch
		pantalla.lcdIntrusismo(lcd, valiza); //Envia la pantalla correspondiente
		avisointrusismo = 0;

		EEPROM.write(ALARMA_SALTADA,1); //Guarda el estado de salto
		EEPROM.write(SENSOR_SALTADO, valiza); //Guarda la valiza de procedencia
	}
	if (millis() >= tiempoOff) {

		if (avisoalarma == 1){

			Serial.println("\nTiempo off acabado \nAVISO ENVIADO");
			registro.registrarEvento("TIEMPO ACABADO AVISO ENVIADO");
			pantalla.lcdAvisoEnviado(lcd);


			if(controlUtv==true){  //false para debug   //ACCESO A GSM MDBUG

				mensaje.setAsuntoPir(datos);   // Mensaje PIR
				mensaje.enviarMensaje(datos);  //Registro principal

				mensaje.setTiempoLlamada(millis()+50000); //Abre los tiempos para llamar
				saltoEnviado++;
				saltoFalso--;


			}

			if ((auto_rc_intentos <= 3) && (valiza != PUERTA)){  //Si la entrada es distinta de puerta entrara
				auto_rc = true;
				auto_rc_intentos++;
				auto_rc_time = millis() + utv_tiempo_reactivacion;  //UTV utv_tiempo_reactivacion 240000 20000 TIEMPO HASTA QUE SE AUTO REACTIVA
			}

			bocinaTiempo = millis() + utv_tiempo_bocina;
			bocina.iniciarBocina(5000, bocinaTiempo); //Invocion principal de bocina


			avisoalarma = 0;
		}
	}
}

//RESET AUTOMATICO
 resetAutomatico();

//SLEEP MODE
 sleepMode(); //Gestiona la desconexion del GSM

//COMPROBAR MODOS
 comprobarModos();

//EEPROM ESTADO ANTERIOR ALARMA
 resetearEstadoPrevio();

//GESTION DE ERRORES
 mensajeError(codigoError);

 //GESTIONAR LLAMADAS
 gestionarLlamadas(); //Tiempos de espera entre los distintos llamacuelga

 //BOCINA LOOP
 bocina.comprobarBocina(); //Comprueba si alguen la esta invocando

 //ACTIVACION AUTOMATICA
 activacionAutomatica(); //Si se dan las condiciones


 //ACCESO A MENU
 if(menu.getEstadoMenu()){  //Si es true se llama al menuManager
	 menu.menuManager(lcd, pantalla, bocina, tiempo);
	 estadoMenu = true;
 } else{
	 estadoMenu = false; //Cierra el estado
 }



}


void activar(bool estado_rc){
	estadoalarma = true;

	if(estado_rc){  //TODO ajustar con tiempos utv

		Serial.println("\nAlarma activada automaticamente");
		registro.registrarEvento("ALARMA ACTIVADA AUTOMATICAMENTE");

		if(controlUtv==true){ 	//false para debug unicamente MDBUG
			bocinaTiempo = millis() + 480000;	//Reduccion del tiempo de bocina durante reactivaciones
		} else{
			bocinaTiempo = millis() + 10000;
		}

		bocina.iniciarBocina(5000, bocinaTiempo); //Invoca a bocina
		auto_rc = false;

		} else{
			Serial.println("\nAlarma activada manualmente");
			registro.registrarEvento("ALARMA ACTIVADA MANUALMENTE");
			EEPROM.write(CONTROL_INTERRUPCION, 1); //Una interrupcion por cada activacion manual
		}

	entradapass = "";

	if(estado_rc){
		tiempoOn = millis() + utv_tiempo_on_remoto;  // UTV utv_tiempo_on_remoto TIEMPO DE MARGEN DE ACTIVACION REMOTA 1 MINUTO  60000// 15000
		} else {
		tiempoOn = millis() + utv_tiempo_on;  // UTV utv_tiempo_on TIEMPO DE MARGEN DESPUES DE ACTIVAR 10M 600000 8000
		}

	pantalla.lcdAlarmaActivada(lcd); //Breve muestreo de lcd

	if(estado_rc){
		sleep_tiempo = millis() + utv_tiempo_espera_gsm;   // UTV utv_tiempo_espera_gsm Tiempo de margen para apagar GSM  20 MIN 1200000 //60000
		sleepmode_gsm = 4;
	} else{
		sleep_tiempo = millis() + 6000;
		sleepmode_gsm = 1;
	}


	EEPROM.write(ALARMA_ACTIVADA, 1); //Guarda el estado de activacion

}

void desactivar(bool estado_rc){


	bocina.detenerBocina(); //Detiene los procesos abiertos de bocina
	estadoalarma = false;
	disparadorPir = false; //Vuelve a abrir el acceso a la parrilla

	if(estado_rc){
		Serial.println("\nAlarma desactivada automaticamente");
		registro.registrarEvento("ALARMA DESACTIVADA AUTOMATICAMENTE");
	} else{
		Serial.println("\nAlarma desactivada manualmente");
		registro.registrarEvento("ALARMA DESACTIVADA MANUALMENTE");
	}

    modo_sensible = false;//Apaga el modo sensible

	if (ladrones == true) {

		ladrones = false;
		avisointrusismo = 0;
		avisoalarma = 0;
	}

	if(estado_rc){
		sleepmode_gsm = 3;
	} else{
		sleep_tiempo = millis() + 6000;
		sleepmode_gsm = 1;
	}


/*
	if(tiempo.comprobarRangoHorario(14, 16, 20,2)){

		Serial.println("\nRango correcto");

	}else {
		Serial.println("\nFuera de rango");
	}
*/

	entradapass = "";

	lcd.clear();
	pantalla.lcdAlarmaDesactivada(lcd);

	EEPROM.write(ALARMA_ACTIVADA, 0); //Estado de activacion desactivado
	EEPROM.write(ALARMA_SALTADA, 0); //Estado de salto sin salto



	if(estado_rc){
		Serial.print("Intentos restantes:  ");
		Serial.println(3-auto_rc_intentos);
		registro.registrarEvento("INTENTOS DE REACTIVACION RESTANTES: "+(String)(3-auto_rc_intentos));
	} else{
		auto_rc_intentos = 0; //Limite de veces que puede rearmarse de manera automatica
	}

	Serial.print("Falsa alarma:  ");
	Serial.println(saltoFalso);
	Serial.print("SMS enviados:  ");
	Serial.println(saltoEnviado);
	Serial.print("SMS Historico:  ");
	Serial.println(EEPROM.read(smsDireccion));


	if(digitalRead(SENSOR_BATERIA) == LOW){
		Serial.print("Aviso bateria interna desactivada");
		lcd_tiempo_clave = millis()+ 5000;
		pantalla.lcdAvisoBateriaDesactivada(lcd);
		delay(2000);

	}

	if(tiempo.comprobarFecha(tiempo.getFechaReset())){
		lcd_tiempo_clave = millis()+ 5000;
		pantalla.lcdAvisoResetAutomatico(lcd);
	}


}

void interrupcionFalloAlimentacion(){
	//Serial.println("\nInterrupcion por fallo en la alimentacion");
	codigoError = 1; //Codigo de error 1 para fallo de alimentacion

}

void mensajeError(volatile byte &Error){


		//Switch para mensajes de errores varios
		switch (Error) {
		case 1:
			if(EEPROM.read(CONTROL_INTERRUPCION) == 1){

				registro.registrarEvento("INTERRUPCION POR FALLO EN ALIMENTACION PRINCIPAL");

				desactivar(false);
				EEPROM.write(ALARMA_ACTIVADA, 0); //Estado de activacion desactivado
				EEPROM.write(ALARMA_SALTADA, 0); //Estado de salto sin salto
				valiza = -1; //Valor nulo

				digitalWrite(sleep_t2, HIGH); //Enciende el GSM
				sleepmode_gsm = 2; //Lo mantiene encendido
				delay(7000); //Delay que espere al encendido del la alimentacion


					if(datos.comprobarDatos()){
						mensaje.setAsunto("MOVIMIENTO DETECTADO Y SABOTAJE EN LA ALIMENTACION");

					}else{
						mensaje.setAsunto("FALLO EN LA ALIMENTACION PRINCIPAL");

					}
				mensaje.enviarMensaje(datos);
				mensaje.setTiempoLlamada(millis()+65000);


				EEPROM.write(CONTROL_INTERRUPCION, 0);
			}else{
				Serial.println("\nNo se enviaran mas avisos hasta reactivar la alarma manualmente");
			}
			Error = 0;
			break;
		case 2:
			// Mensaje de error
			break;

		}

}



void resetear(){
	Serial.println("\nRESETEANDO");
	registro.registrarEvento("ALARMA RESETEADA");
	entradapass = "";
	EEPROM.write(ALARMA_ACTIVADA, 0); //Estado de activacion desactivado
	EEPROM.write(ALARMA_SALTADA, 0); //Estado de salto sin salto
	digitalWrite(RESETEAR, HIGH);

}

void resetAutomatico(){

	if(tiempo.comprobarFecha(tiempo.getFechaReset())){
		if(tiempo.comprobarHora(16, 30)){
			Serial.println("\nRESET PROGRAMADO DEL SISTEMA");
			registro.registrarEvento("RESET PROGRAMADO CON FECHA ["+tiempo.imprimeFechaSimple(tiempo.getFechaReset())+"]");
			entradapass = "";
			digitalWrite(RESETEAR, HIGH);
		}
	}


}


void watchDog(){

	digitalWrite(WATCHDOG, !digitalRead(WATCHDOG));
}

void resetearEstadoPrevio(){

	if(estadoAnterior == 1){

		if (EEPROM.read(ALARMA_ACTIVADA) == 1) {
			registro.registrarEvento("ALARMA ACTIVADA (RESTAURADO ESTADO ANTERIOR)");
			lcd.clear();
			rcEstado= false;  //Peligro bucles
			activar(rcEstado);
			tiempoOn = millis() + 5000;

		}

		if (EEPROM.read(ALARMA_SALTADA) == 1) {
			Serial.println("\nSalto desconocido");
			registro.registrarEvento("INTRUSISMO ALARMA (RESTAURADO ESTADO ANTERIOR)");

			delay(9000); //Tiempo de espera para la inicializacion del GSM
			tiempoOn = millis() + 5000;
			mensaje.setAsunto("AVISO ALARMA PRESENCIA DESCONOCIDA");
			mensaje.enviarMensaje("La alarma ha saltado en sensor "+(String)EEPROM.read(SENSOR_SALTADO)+" pero se ha reiniciado durante proceso deteccion. "
					+((EEPROM.read(SENSOR_SALTADO)==0)?("Puerta abierta, alarma apagada por seguridad"):("Alarma reactivada")));
			bocinaTiempo = millis() + utv_tiempo_bocina;
			bocina.iniciarBocina(5000, bocinaTiempo);

			mensaje.setTiempoLlamada(millis()+65000); //Abre llamada

			if(EEPROM.read(SENSOR_SALTADO) == 0){ //Si es la puerta la que esta abierta apagara la alarma
				desactivar(false);
				EEPROM.write(ALARMA_ACTIVADA, 0); //Estado de activacion desactivado
				EEPROM.write(ALARMA_SALTADA, 0); //Estado de salto sin salto
			}

			EEPROM.write(ALARMA_SALTADA, 0); //Estado de salto sin salto
		}

		estadoAnterior = 0;
	}

}


void gestionarLlamadas(){

	if(mensaje.getTiempoLlamada() >= millis()){

		static byte control = 0;


		if(mensaje.getTiempoLlamada() == millis()){
			control = 0;
			return;
		}


		if(mensaje.getTiempoLlamada() <= millis()+40000  && control == 0){
			mensaje.realizarLlamada(true, 1);
			control = 1;
		}

		if(mensaje.getTiempoLlamada()  <= millis()+25000  && control == 1){
			mensaje.realizarLlamada(false, 1);
			control = 2;
		}


		if(mensaje.getTiempoLlamada() <= millis()+15000  && control == 2){
			mensaje.realizarLlamada(true, 2);
			control = 3;
		}

		if(mensaje.getTiempoLlamada()  <= millis()+1000  && control == 3){
			mensaje.realizarLlamada(false, 2);
			control = 4;
		}


	}

}

void ledParpadeo(){

	static long tiempoLedOn;
	static long tiempoLedOff;
	static bool estado;
	const int  intervalo = 1000;

	if((millis()-tiempoLedOn>intervalo)&& estado==true){
		estado = false;
		digitalWrite(LED_COCHERA, HIGH);
		tiempoLedOff=millis();
	}

	if((millis()-tiempoLedOff>intervalo)&& estado==false){
		estado = true;
		digitalWrite(LED_COCHERA, LOW);
		tiempoLedOn=millis();

	}
}


void sleepMode(){

	switch(sleepmode_gsm){

	case 1: //  (Activacion/Desactivacion)  manual

		if (millis() > sleep_tiempo) {
			digitalWrite(sleep_t2, LOW);
		} else {
			digitalWrite(sleep_t2, LOW);
		}


		break;

	case 2: //Salto estado de alarma

		digitalWrite(sleep_t2, HIGH);


		break;

	case 3: //Desactivacion remota

		digitalWrite(sleep_t2, HIGH);

		break;

	case 4:  //Activacion remota

		if (millis() > sleep_tiempo) {
			digitalWrite(sleep_t2, LOW);
		}else{
			digitalWrite(sleep_t2, HIGH);
		}
		break;
	}


	switch(sleepmode_bluetooth){ //Control bluetooth

	case 1:

		digitalWrite(sleep_t1, HIGH);
		break;

	case 2:

		digitalWrite(sleep_t1, LOW);
		break;


	}

}


void activacionAutomatica(){

	if ((auto_rc == true) && (millis() > auto_rc_time) && (auto_rc_intentos <= 3) && (auto_rc_intentos != 0) && (valiza != PUERTA) ){


		if (estadoalarma == true) {
			rcEstado=true;
			desactivar(rcEstado);
			auto_rc_time_delay = millis() + 5000;	//Tiempo de espera entre la desactivacion y la activacion rc

			modo_sensible = true;
			modo_sensible_tiempo =  millis() + utv_tiempo_sensible; //UTV utv_tiempo_sensible Tiempo duracion del modo sensible una hora 3600000‬ /  60000

		}
		if ((estadoalarma == false) && (millis()>auto_rc_time_delay)) { //Si esta desactivada y han pasado los 5 segundos

			lcd.clear();
			rcEstado=true;
			activar(rcEstado);
			entradapass = "";

			if(controlUtv==true){   //false para debug unicamente MDBUG //ACCESO A GSM

				mensaje.setAsunto("ALARMA REACTIVDA CON EXITO");
				mensaje.enviarMensaje(datosPhantom); //Esta vez con los datos del phantom
				datosPhantom.borraDatos(); //Elimina el registro previo

			}
		}
	}

}


void comprobarModos(){

	if(controlUtvAcceso==1){
		if(controlUtv==true){

				 Serial.println("UTV establecida en modo default");
				 registro.registrarEvento("ALARMA ESTABLECIDA EN MODO DEFAULT");
				 utv_tiempo_off = 80000;
				 utv_tiempo_off_beta = 10000;

				 utv_tiempo_on = 600000;
				 utv_tiempo_on_remoto = 60000;
				 utv_tiempo_reactivacion = 240000;
				 utv_tiempo_sensible = 3600000;
				 utv_tiempo_bocina = 600000;
				 utv_tiempo_espera_gsm = 1200000;


			}else {

					 Serial.println("UTV establecida en modo prueba");
					 registro.registrarEvento("ALARMA ESTABLECIDA EN MODO PRUEBA");
					 utv_tiempo_off = 20000;
					 utv_tiempo_off_beta = 5000;

					 utv_tiempo_on = 10000;
					 utv_tiempo_on_remoto = 5000;
					 utv_tiempo_reactivacion = 20000;
					 utv_tiempo_sensible = 60000;
					 utv_tiempo_bocina = 20000;
					 utv_tiempo_espera_gsm = 60000;
			}

		controlUtvAcceso=0;
	}

}
