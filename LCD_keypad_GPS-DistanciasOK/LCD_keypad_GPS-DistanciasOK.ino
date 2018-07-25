#include <LiquidCrystal.h>    //Funciones del LCD
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial softSerial(3, 2);

//Indica a la librería los pines de conexión del shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);



//Varables para leer las teclas y el ADC
int lcd_key     = 0;
int adc_key_in  = 0;

//Contantes para identificar las teclas
#define btnDERECHA      0
#define btnARRIBA      1
#define btnABAJO       2
#define btnIZQUIERDA   3
#define btnSELECT      4
#define btnNADA        5

//Función que lee las teclas
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // Leer la entrada analógica

 //Las teclas se conectan a un divisor resistivo que produce distintos valores de tensión según la tecla pulsada
 
 if (adc_key_in > 1000) return btnNADA; //Si no hay ninguna tecla pulsada, sale para demorar menos tiempo

 if (adc_key_in < 50)   return btnDERECHA;  
 if (adc_key_in < 195)  return btnARRIBA;
 if (adc_key_in < 380)  return btnABAJO;
 if (adc_key_in < 555)  return btnIZQUIERDA;
 if (adc_key_in < 790)  return btnSELECT;   

 return btnNADA;  // No debería llegar aquí, pero por las dudas retorna este valor.
}

 
 
void setup() {
  // Inicializaciones
  Serial.begin(115200);
  softSerial.begin(9600);
  lcd.begin(16, 2);                 //LCD de 16 columnas y 2 filas
  lcd.clear();                      //Borrar la pantalla
  lcd.setCursor(0, 0);              //Llevar el cursor al inicio
  lcd.print("Buscando Satelites:");

}

void loop() {
  // Bucle principal del programa
//inizialización de variables
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

  //bucle infinito, para mantener las variables declaradas
  while(1){
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
 lcd.clear();  
 //si hay datos los mostramos
  if (newData)
  {
    int satelites=gps.satellites();
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    //flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat;
    //flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon;

    //convertimos las coordenadas a doble y radianes, para poder trabajar con las funciones matemticas.
    dlat=deg2rad((double)flat);
    dlon=deg2rad((double)flon);
    if(!datosCorrectos){
         dlatIni=dlat;
         dlonIni=dlon;
         //dlatIni=35.3;
         //dlonIni=-6;
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

    
    
    lcd.setCursor(0, 0);              //Llevar el cursor al inicio
    lcd.print("N Sat=");
    lcd.setCursor(7, 0);
    lcd.print(satelites);
    Serial.print("LAT=");
    Serial.print(flat,6);    
    
    
    
    lcd.setCursor(0, 1);              //Llevar el cursor al inicio
    lcd.print("Dis=");
    lcd.setCursor(5, 1);
    lcd.print(distancia,4);
    Serial.print(" LON=");
    Serial.print(flon,6);
    
    
    Serial.print(" SAT=");
    Serial.println(satelites);
    Serial.print("Distancia: ");
    Serial.println(distancia,6);
    Serial.println(" ");
   // Serial.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    
    
    
    /*
    lcd.setCursor(0,1);            // Lleva el cursor al inicio de la segunda linea
    lcd.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    */

    
  }else{
    lcd.setCursor(0, 0);              //Llevar el cursor al inicio
  lcd.print("Buscando Satelites:");
  lcd.setCursor(0, 1);              //Llevar el cursor al inicio
  lcd.print("....");
  Serial.print(".");
    }
  }
}


double caculaDistancia(double latIni,double lonIni,double lat,double lon){
  double radio = 6372.795;
  double dlat= latIni-lat;
  double dlon= lonIni-lon;
  double a=(sin(dlat/2)*sin(dlat/2))+(cos(latIni)*cos(lat)*sin(dlon/2)*sin(dlon/2));
  double c=2*atan2(sqrt(a),sqrt(1-a));
  double distancia=radio*c;
  return distancia;
  }

  double deg2rad(double deg){
    double rad= deg*PI/180;
    return rad;
  }

