
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
  if(contador>0){
    contador--;
    EEPROM.put( eeAddress, contador );
  }
  
}

