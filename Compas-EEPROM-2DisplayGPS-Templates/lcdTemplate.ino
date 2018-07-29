
char dispnum[5] = {0x00, 0x00, 0x00, 0x00, 0x00,};

/*0,1,2,3,4,5,6,7,8,9,A,b,C,c,d,E,F,G,H,L,P,S,º,*/    
const char num[]={0xEB,0x60,0xC7,0xE5,0x6C,0xAD,0xAF,0xE0,0xEF,0xED,0xEE,0x2F,0x8B,0x07,0x67,0x8F,0x8E,0xAB,0x6E,0x0B,0xCE,0xAD,0xCC,}; 

/************************************************************************************************************
 * 
 * 
 * Inicio Programación Pantalla 1
 * 
 * 
 ************************************************************************************************************/

void SendBit_1621(uchar sdata, uchar cnt) //High bit first
{
  uchar i;
  for (i = 0; i < cnt; i++)
  {
    WR0;
    delayMicroseconds(20);
    if (sdata & 0x80) DATA1;
    else DATA0;
    delayMicroseconds(20);
    WR1;
    delayMicroseconds(20);
    sdata <<= 1;
  }
  delayMicroseconds(20);
}
/**-------------------------------------------------------------------------

                  Name: SendCmd(send cmd)
                  //Write MODE“100” AND 9 bits command
  ---------------------------------------------------------------------------*/

void SendCmd_1621(uchar command)
{
  CS0;
  SendBit_1621(0x80, 4);
  SendBit_1621(command, 8);
  CS1;
}
/**-------------------------------------------------------------------------

                  Name: Write_1621send data and cmd)
  ---------------------------------------------------------------------------*/

void Write_1621(uchar addr, uchar sdata)
{
  addr <<= 2;
  CS0;
  SendBit_1621(0xa0, 3);    //Write MODE“101”
  SendBit_1621(addr, 6);    //Write addr high 6 bits
  SendBit_1621(sdata, 8);   //Write data  8 bits
  CS1;
}
/**-------------------------------------------------------------------------

                          Name: all_off(Clear Display)
  ---------------------------------------------------------------------------*/

void HT1621_all_off(uchar num)
{ uchar i;
  uchar addr = 0;
  for (i = 0; i < num; i++)
  {
    Write_1621(addr, 0x00);
    addr += 2;
  }
}
/****************************************************************************

                          Name: all_on(All lit)
****************************************************************************/

void HT1621_all_on(uchar num)
{ uchar i, j;
  uchar addr = 0;
  for (i = 0; i < num; i++)
  { Write_1621(addr, 0xff);
    addr += 2;
  }
}     

/****************************************************************************

                          Name: Init_1621(initialize 1621)
*****************************************************************************/

void Init_1621(void)
{
  SendCmd_1621(Sys_en);
  SendCmd_1621(RCosc);
  SendCmd_1621(ComMode);
  SendCmd_1621(LCD_on);
}


/****************************************************************************

  Name: displaydata(unsigned long int t,int p,char bat1,char bat2,char bat3) Display  data
  t : Data to be displayed
  p : Display decimal
  s1,s2 : Left side of the Triangle

*****************************************************************************/

void displayInt(long int t)
{ uchar i;
  int j;
  boolean noCero=false;
  dispnum[4] = num[(t / 10000) % 10];
  dispnum[3] = num[(t / 1000) % 10];
  dispnum[2] = num[(t / 100) % 10];
  dispnum[1] = num[(t / 10) % 10];
  dispnum[0] = num[t % 10];

  //númeo de digitos a mostra
  if (t>9999){
    j=4;
  }else if (t>999){
    j=3;
  }else if (t>99){
    j=2;
  }else if (t>9){
    j=1;
  }else{
    j=0;
  }

  //excribimos los digitos acutalizados
  for (i = 0; i <= j; i++){
    Write_1621(i * 2, dispnum[i]);
  }
  //borramos los que no tienen que tener nada
  for (i = j+1; i < 4; i++){
    Write_1621(i * 2, 0x00);
  }
}

void displayFloat(float fdata){
  //convertimos el dato flotante a entero de dos decimales
  int t=(int)(fdata*100);
  
  uchar i;
  int j;
  boolean noCero=false;
  dispnum[4] = num[(t / 10000) % 10];
  dispnum[3] = num[(t / 1000) % 10];
  dispnum[2] = num[(t / 100) % 10];
  dispnum[1] = num[(t / 10) % 10];
  dispnum[0] = num[t % 10];

  //númeo de digitos a mostra
  /*
  if (t>9999){
    j=4;
  }else if (t>999){
    j=3;
  }else if (t>99){
    j=2;
  }else if (t>9){
    j=1;
  }else{
    j=0;
  }*/
  if (t>9999){
    j=4;
  }else if (t>999){
    j=3;
  }else {
    j=2;
  }
  

//Escribimos el punto de los decimales.
  
sbi(dispnum[1], 4);
  //excribimos los digitos acutalizados
  for (i = 0; i <= j; i++){
    Write_1621(i * 2, dispnum[i]);
  }
  //borramos los que no tienen que tener nada
  for (i = j+1; i < 4; i++){
    Write_1621(i * 2, 0x00);
  }
}
  
  



/************************************************************************************************************
 * 
 * 
 * Fin Programación Pantalla A
 * 
 * 
 ************************************************************************************************************/


/************************************************************************************************************
 * 
 * 
 * Inicio Programación Pantalla A
 * 
 * 
 ************************************************************************************************************/

/**------------------------------ Name: SendBit_1621(send data)------------------------------------------------------------*/
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
/**------------- Name: SendCmd(send cmd) //Write MODE“100” AND 9 bits command  ------------------------------------------*/

void SendCmd_1621a(uchar command)
{
  CSa0;
  SendBit_1621a(0x80, 4);
  SendBit_1621a(command, 8);
  CSa1;
}
/**------------------------------ Name: Write_1621send data and cmd)------------------------------------------*/

void Write_1621a(uchar addr, uchar sdata)
{
  addr <<= 2;
  CSa0;
  SendBit_1621a(0xa0, 3);    //Write MODE“101”
  SendBit_1621a(addr, 6);    //Write addr high 6 bits
  SendBit_1621a(sdata, 8);   //Write data  8 bits
  CSa1;
}
/**--------------------------------- Name: all_off(Clear Display)-------------------------------------------------*/
void HT1621_all_offa(uchar num)
{ uchar i;
  uchar addr = 0;
  for (i = 0; i < num; i++)
  {
    Write_1621a(addr, 0x00);
    addr += 2;
  }
}
/****************************************** Name: all_on(All lit)*************************************************/
void HT1621_all_ona(uchar num)
{ uchar i, j;
  uchar addr = 0;
  for (i = 0; i < num; i++)
  { Write_1621a(addr, 0xff);
    addr += 2;
  }
}     
/****************************** Name: Init_1621(initialize 1621)**********************************************/
void Init_1621a(void)
{
  SendCmd_1621a(Sys_en);
  SendCmd_1621a(RCosc);
  SendCmd_1621a(ComMode);
  SendCmd_1621a(LCD_on);
}
/****************************************************************************

  Name: displayGradosa(unsigned long int t,int p,char bat1,char bat2,char bat3) Display  data
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

/********************************************************************************************************************
 * 
 * 
 * 
 * Fin programacion Pantalla A
 * 
 * 
 *********************************************************************************************************************/

