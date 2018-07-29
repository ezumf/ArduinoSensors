//Author: Jesus MF
//Arduino uno R3 y Arduino Nano
//5 DIGIT 7SEG LCD

/*---Segment Display Screen----
  --5--
  1---6
  --2--
  3---7
  --4--
  8 decimal point
  ----------------------*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#include <EEPROM.h>
#include <Wire.h>

#include <SoftwareSerial.h>
#include <TinyGPS.h>

// constants won't change. They're used here to
// set pin numbers:
const int buttonPinMas = 12;     // the number of the pushbutton pin +
const int buttonPinMenos = 11;   // the number of the pushbutton pin -
const int buttonPinSelect = 10;  // the number of the pushbutton pin select
const int ledPin =  13;      // the number of the LED pin

int contador=0;
// variables will change:
int buttonStateMas = 0;         // variable for reading the pushbutton status
int buttonStateMenos = 0;         // variable for reading the pushbutton status
int buttonStateSelect = 0;         // variable for reading the pushbutton status

TinyGPS gps;
SoftwareSerial softSerial(9, 8);//pines de comunicacion del GPS


float sensorValue;
int eeAddress = 10; //dirección de memoria con los datos.

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


/**
 * 
 * Inicio Pantalla 1
 * 
 * 
 */
//Defined HT1621's command
#define  ComMode    0x52  //4COM,1/3bias  1000    010 1001  0  
#define  RCosc      0x30  //on-chip RC oscillator(Power-on default)1000 0011 0000 
#define  LCD_on     0x06  //Turn on LCD 
#define  LCD_off    0x04  //Turn off LCD 
#define  Sys_en     0x02  //Turn on system oscillator 1000   0000 0010 
#define  CTRl_cmd   0x80  //Write control cmd 
#define  Data_cmd   0xa0  //Write data cmd   

// //Define port    HT1621 data port
#define CS   2  //Pin 2 as chip selection output
#define WR   3  //Pin 3 as read clock  output
#define DATA 4  //Pin 4 as Serial data output

#define CS1    digitalWrite(CS, HIGH)
#define CS0    digitalWrite(CS, LOW)
#define WR1    digitalWrite(WR, HIGH)
#define WR0    digitalWrite(WR, LOW)
#define DATA1  digitalWrite(DATA, HIGH)
#define DATA0  digitalWrite(DATA, LOW)





 /**
  * Fin Pantalla 1
  * 
  */

/**
 * Inicio Pantalla a
 */

#define sbi(x, y)  (x |= (1 << y))   /*set Register x of y*/
#define cbi(x, y)  (x &= ~(1 <<y ))  /*Clear Register x of y*/
#define uchar   unsigned char
#define uint   unsigned int
//Defined HT1621's command
#define  ComMode    0x52  //4COM,1/3bias  1000    010 1001  0  
#define  RCosc      0x30  //on-chip RC oscillator(Power-on default)1000 0011 0000 
#define  LCD_on     0x06  //Turn on LCD 
#define  LCD_off    0x04  //Turn off LCD 
#define  Sys_en     0x02  //Turn on system oscillator 1000   0000 0010 
#define  CTRl_cmd   0x80  //Write control cmd 
#define  Data_cmd   0xa0  //Write data cmd   

// //Define port    HT1621 data port
#define CSa   5  //Pin 2 as chip selection output
#define WRa   6  //Pin 3 as read clock  output
#define DATAa 7  //Pin 4 as Serial data output

#define CSa1    digitalWrite(CSa, HIGH)
#define CSa0    digitalWrite(CSa, LOW)
#define WRa1    digitalWrite(WRa, HIGH)
#define WRa0    digitalWrite(WRa, LOW)
#define DATAa1  digitalWrite(DATAa, HIGH)
#define DATAa0  digitalWrite(DATAa, LOW)

/**
 * Fin Pantalla a
 */



 
//Funcion que simula la lectura de un sensor
float ReadSensor()
{
  return 10.0f;
}




void setup()
{
  //Pantalla 1 para contador
pinMode(CS, OUTPUT); //Pin 2
  pinMode(WR, OUTPUT); //Pin 3
  pinMode(DATA, OUTPUT); //Pin 4

  //pantalla 2 para brújula
  pinMode(CSa, OUTPUT); //Pin 5
  pinMode(WRa, OUTPUT); //Pin 6
  pinMode(DATAa, OUTPUT); //Pin 7

  softSerial.begin(9600);//comunicacion con el GPS
  
  Serial.println("Test EEPROM Button"); Serial.println("");
  
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPinMas, INPUT);
  pinMode(buttonPinMenos, INPUT);
  pinMode(buttonPinSelect, INPUT);

 
  delay(50) ;
  Init_1621() ;
  Init_1621a() ;
  //Parpadeo, todos ON y Todos OFF
  HT1621_all_on(16) ;
  HT1621_all_ona(16) ;
  delay(300) ;
  HT1621_all_off(16);
  HT1621_all_offa(16);
  delay(300) ;
  HT1621_all_on(16) ;
  HT1621_all_ona(16) ;
  delay(300) ;
  HT1621_all_off(16);
  HT1621_all_offa(16);
 // delay(300) ;

  uchar i, j;
 

  inicializaEEPROM();
  //Texto de inicio
  Serial.println("BotonesEEPROM");
  Serial.print("contador: ");
  Serial.println(contador);
  //displayInt(contador);
  //displayFloat(251.36);


  
  Serial.begin(9600);
  Serial.println("Test Brújula digital"); Serial.println("");

  /* Initialise the sensor */
  if (!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, Brújula no detectada, comprueba los cables!");
    while (1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetailsa();
}


void loop()
{

//inicio GPS
  float flat, flon; //se necesitan en float, por compatibilidad con la libreria TinyGPS
  double dlat,dlon,dlatIni,dlonIni; // Estos son los valores que usaremos
  double distancia=0.0; // distancia inicial, 
  bool datosCorrectos=false;
 
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  int seg=5; //numero de segundo entre medidas.
  double minDist=0.003; //distancia minima para que se actualice.
  int contSeg=0; //contador de segundos.
   int satelites=0;
//Fin GPS
  
  long int t = 0;
 

  //una vez que arranca, para que no pase más por el loop, ejecuta un loop infinito con este while
  while (1)
  {

//inicio busqueda GPS
contSeg++;
  // Intentar recibir secuencia durante un segundo
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (softSerial.available())
    {
      char c = softSerial.read();
      if (gps.encode(c)) // Nueva secuencia recibida
        newData = true;
    }
  }
   if (newData)
  {
    satelites=gps.satellites();
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);

    //convertimos las coordenadas a doble y radianes, para poder trabajar con las funciones matemticas.
    dlat=deg2rad((double)flat);
    dlon=deg2rad((double)flon);
    if(!datosCorrectos){
         dlatIni=dlat;
         dlonIni=dlon;
         datosCorrectos=true;
      }else{
        if(contSeg>seg){
          contSeg=0;
          double incremento=caculaDistancia(dlatIni,dlonIni,dlat,dlon);
          Serial.println(incremento,6);
          //Si la distancia supera el minimo, actualizamos el total, y las coordenadas
          if(incremento>minDist){
              distancia+=incremento;
              dlatIni=dlat;
              dlonIni=dlon;
            }
        }
      }
      
  }else{
     HT1621_all_off(16);
    delay(150) ;
    
    }
    
displayFloat(distancia);


Serial.print("Distancia: ");
    Serial.println(distancia,6);
    Serial.print("Satelites: ");
    Serial.println(satelites);
    Serial.println(" ");
    
    //inicio contador
 // read the state of the pushbutton value:
  buttonStateMas = digitalRead(buttonPinMas);
  buttonStateMenos = digitalRead(buttonPinMenos);
  buttonStateSelect = digitalRead(buttonPinSelect);

  
  
       // check if the pushbutton is pressed.
        // if it is, the buttonState is HIGH:
        if (buttonStateMas == HIGH) {
          // contador++;
          incrementaContador();
          displayInt(contador);
          Serial.print("contador: ");
          Serial.println(contador);
        } else {
          // nothing;
        }
        if (buttonStateMenos == HIGH) {
          // contador--:
          decrementaContador();
          displayInt(contador);
          Serial.print("contador: ");
          Serial.println(contador);
          
        } else {
          // nothing:
        }
      
        if (buttonStateSelect == HIGH) {
          // SELECTED
          // Serial.println("SELECT");
          
        } else {
          
        }
      
        
        delay(150);
    
    
    //fin contador

    
    //calculo del rumbo
    int grados = getHeading();
    //mandamos los grados como entero, a nuestra función para pintar grados (3 digitos y º)
    displayGradosa(grados);
    Serial.print("Rumbo: "); Serial.print(grados);Serial.println("º ");

    delay(150);
  }
}


