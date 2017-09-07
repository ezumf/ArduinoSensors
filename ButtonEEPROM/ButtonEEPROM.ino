#include <EEPROM.h>
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


float sensorValue;
int eeAddress = 10; //dirección de memoria con los datos.
 
//Funcion que simula la lectura de un sensor
float ReadSensor()
{
  return 10.0f;
}

void inicializaEEPROM(){
  int e; //variable con el valor de la EEPROM
  EEPROM.get( eeAddress, e );//leemos de la EEPROM
  if(e<0){//si e meno que 0, inicializamos a 0
    e=0;
    EEPROM.put( eeAddress, e );
  }else{//inicializamos contador
    EEPROM.get( eeAddress, contador );//leemos de la EEPROM
  }
}

void incrementaContador(){
  contador++;
  EEPROM.put( eeAddress, contador );
}

void decrementaContador(){
  contador--;
  EEPROM.put( eeAddress, contador );
}

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPinMas, INPUT);
  pinMode(buttonPinMenos, INPUT);
  pinMode(buttonPinSelect, INPUT);

  inicializaEEPROM();
  //Texto de inicio
  Serial.println("BotonesEEPROM");
  Serial.print("contador: ");
  Serial.println(contador);
 
}

void loop() {
  // read the state of the pushbutton value:
  buttonStateMas = digitalRead(buttonPinMas);
  buttonStateMenos = digitalRead(buttonPinMenos);
  buttonStateSelect = digitalRead(buttonPinSelect);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonStateMas == HIGH) {
    // contador++;
    incrementaContador();
    Serial.print("contador: ");
    Serial.println(contador);
  } else {
    // nothing;
  }
  if (buttonStateMenos == HIGH) {
    // contador--:
    decrementaContador();
    Serial.print("contador: ");
    Serial.println(contador);
    
  } else {
    // nothing:
  }

  if (buttonStateSelect == HIGH) {
    // SELECTED
     Serial.println("SELECT");
    
  } else {
    
  }

  
  delay(150);
}
