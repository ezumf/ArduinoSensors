
void inicializaEEPROM(){
  int e; //variable con el valor de la EEPROM
  EEPROM.get( eeAddress, e );//leemos de la EEPROM
  if(e<0){//si es menor que 0, inicializamos a 0
    e=0;
    EEPROM.put( eeAddress, e );
  }else{//inicializamos contador
    EEPROM.get( eeAddress, contador );//leemos de la EEPROM
  }
}

void incrementaContador(){
  //distancia=distacia+escala;
  contador=contador+escala;
 Serial.print("contador: ");
 Serial.println(contador);
  EEPROM.put( eeAddress, contador );
}

void decrementaContador(){
  if(contador>0){
    //distancia=distacia-escala;
    contador=contador-escala;
    Serial.print("contador: ");
    Serial.println(contador);
    EEPROM.put( eeAddress, contador );
  }
  
}
void resetContador(){
  if(contador>0){
    //distancia=distacia-escala;
    contador=0;
    Serial.print("contador: ");
    Serial.println(contador);
    EEPROM.put( eeAddress, contador );
  }
  
}

void incremetaContadorDouble(double incremento){
  if(distancia>0){
    contador=contador+(int)(incremento*1000);
    EEPROM.put( eeAddress, contador );
  }
  
}
