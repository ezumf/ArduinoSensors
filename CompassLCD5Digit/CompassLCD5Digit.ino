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

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

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



char dispnum[5] = {0x00, 0x00, 0x00, 0x00, 0x00,};

/*0,1,2,3,4,5,6,7,8,9,A,b,C,c,d,E,F,G,H,L,P,S,º,*/    
const char num[]={0xEB,0x60,0xC7,0xE5,0x6C,0xAD,0xAF,0xE0,0xEF,0xED,0xEE,0x2F,0x8B,0x07,0x67,0x8F,0x8E,0xAB,0x6E,0x0B,0xCE,0xAD,0xCC,}; 

/**-------------------------------------------------------------------------

                 Name: SendBit_1621(send data)
  ---------------------------------------------------------------------------*/

void SendBit_1621a(uchar sdata, uchar cnt) //High bit first
{
  uchar i;
  for (i = 0; i < cnt; i++)
  {
    WRa0;
    delayMicroseconds(20);
    if (sdata & 0x80) DATAa1;
    else DATAa0;
    delayMicroseconds(20);
    WRa1;
    delayMicroseconds(20);
    sdata <<= 1;
  }
  delayMicroseconds(20);
}
/**-------------------------------------------------------------------------

                  Name: SendCmd(send cmd)
                  //Write MODE“100” AND 9 bits command
  ---------------------------------------------------------------------------*/

void SendCmd_1621a(uchar command)
{
  CSa0;
  SendBit_1621a(0x80, 4);
  SendBit_1621a(command, 8);
  CSa1;
}
/**-------------------------------------------------------------------------

                  Name: Write_1621send data and cmd)
  ---------------------------------------------------------------------------*/

void Write_1621a(uchar addr, uchar sdata)
{
  addr <<= 2;
  CSa0;
  SendBit_1621a(0xa0, 3);    //Write MODE“101”
  SendBit_1621a(addr, 6);    //Write addr high 6 bits
  SendBit_1621a(sdata, 8);   //Write data  8 bits
  CSa1;
}
/**-------------------------------------------------------------------------

                          Name: all_off(Clear Display)
  ---------------------------------------------------------------------------*/

void HT1621_all_offa(uchar num)
{ uchar i;
  uchar addr = 0;
  for (i = 0; i < num; i++)
  {
    Write_1621a(addr, 0x00);
    addr += 2;
  }
}
/****************************************************************************

                          Name: all_on(All lit)
****************************************************************************/

void HT1621_all_ona(uchar num)
{ uchar i, j;
  uchar addr = 0;
  for (i = 0; i < num; i++)
  { Write_1621a(addr, 0xff);
    addr += 2;
  }
}     

/****************************************************************************

                          Name: Init_1621(initialize 1621)
*****************************************************************************/

void Init_1621a(void)
{
  SendCmd_1621a(Sys_en);
  SendCmd_1621a(RCosc);
  SendCmd_1621a(ComMode);
  SendCmd_1621a(LCD_on);
}


/****************************************************************************

  Name: displaydata(unsigned long int t,int p,char bat1,char bat2,char bat3) Display  data
  t : Data to be displayed
  p : Display decimal
  s1,s2 : Left side of the Triangle

*****************************************************************************/

void displayGradosa(long int t)
{ uchar i;
  int j;
  boolean noCero=false;
  dispnum[3] = num[(t / 100) % 10];
  dispnum[2] = num[(t / 10) % 10];
  dispnum[1] = num[t % 10];
  dispnum[0] = num[22]; //ultimo digito º

  //númeo de digitos a mostra
  if (t>99){
    j=3;
  }else if (t>9){
    j=2;
  }else{
    j=1;
  }

  //excribimos los digitos acutalizados
  for (i = 0; i <= j; i++){
    Write_1621a(i * 2, dispnum[i]);
  }
  //borramos los que no tienen que tener nada
  for (i = j+1; i < 4; i++){
    Write_1621a(i * 2, 0x00);
  }
}

void displaySensorDetailsa(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.println("------------------------------------");
 
  Serial.println("");
  delay(500);
}

void setup()
{
  pinMode(CSa, OUTPUT); //Pin 5
  pinMode(WRa, OUTPUT); //Pin 6
  pinMode(DATAa, OUTPUT); //Pin 7
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
  long int t = 0;
  uchar i, j;
  delay(50) ;
  Init_1621a() ;
  //Parpadeo, todos ON y Todos OFF
  HT1621_all_ona(16) ;
  delay(300) ;
  HT1621_all_offa(16);
  delay(300) ;
  HT1621_all_ona(16) ;
  delay(300) ;
  HT1621_all_offa(16);
  delay(300) ;

  //una vez que arranca, para que no pase más por el loop, ejecuta un loop infinito con este while
  while (1)
  {
    /* Get a new sensor event */
    sensors_event_t event;
    mag.getEvent(&event);


    // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
    // Calculate heading when the magnetometer is level, then correct for signs of axis.
    float heading = atan2(event.magnetic.x, event.magnetic.y);

    // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
    // Find yours here: http://www.magnetic-declination.com/
    // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
    // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
    //declinación electromagnetica en sevilla
    float declinationAngle = 0.22;
    heading += declinationAngle;
    
    // Correct for when signs are reversed.
    if (heading < 0)
      heading += 2 * PI;

    // Check for wrap due to addition of declination.
    if (heading > 2 * PI)
      heading -= 2 * PI;

    // Convert radians to degrees for readability.
    float headingDegrees = heading * 180 / M_PI;
    int grados = headingDegrees;
    //mandamos los grados como entero, a nuestra función para pintar grados (3 digitos y º)
    displayGradosa(grados);
    Serial.print("Rumbo: "); Serial.print(headingDegrees);Serial.println("º ");

    delay(500);
  }
}
