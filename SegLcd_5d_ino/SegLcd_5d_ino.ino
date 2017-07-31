//Author: cantone-electonics
//More information welcome to : http://www.canton-electronics.com 
//Arduino 1.0.4
//Arduino uno R3
//5 DIGIT 7SEG LCD

/*---Segment Display Screen----
--5--
1---6
--2--
3---7
--4--
8 decimal point
----------------------*/  

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
      #define CS   2  //Pin 2 as chip selection output
      #define WR   3  //Pin 3 as read clock  output
      #define DATA 4  //Pin 4 as Serial data output
     
      #define CS1    digitalWrite(CS, HIGH) 
      #define CS0    digitalWrite(CS, LOW)
      #define WR1    digitalWrite(WR, HIGH) 
      #define WR0    digitalWrite(WR, LOW)
      #define DATA1  digitalWrite(DATA, HIGH) 
      #define DATA0  digitalWrite(DATA, LOW)
      
     

char dispnum[5]={0x00,0x00,0x00,0x00,0x00,};

/*0,1,2,3,4,5,6,7,8,9,A,b,C,c,d,E,F,*/	  
const char num[]={0xEB,0x60,0xC7,0xE5,0x6C,0xAD,0xAF,0xE0,0xEF,0xED,0xEE,0x2F,0x8B,0x07,0x67,0x8F,0x8E,}; 

       /**------------------------------------------------------------------------- 

                        Name: SendBit_1621(send data) 
      ---------------------------------------------------------------------------*/ 

      void SendBit_1621(uchar sdata,uchar cnt) //High bit first
      { 
       uchar i; 
       for(i=0;i<cnt;i++) 
             { 
              WR0;
			  delayMicroseconds(20); 
              if(sdata&0x80) DATA1; 
              else DATA0; 
			  delayMicroseconds(20);
              WR1;
			  delayMicroseconds(20);
              sdata<<=1; 
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
      SendBit_1621(0x80,4);    
      SendBit_1621(command,8);  
      CS1;                    
      } 
      /**------------------------------------------------------------------------- 

                        Name: Write_1621send data and cmd) 
      ---------------------------------------------------------------------------*/ 

      void Write_1621(uchar addr,uchar sdata) 
      { 
       addr<<=2; 
       CS0; 
       SendBit_1621(0xa0,3);     //Write MODE“101” 
       SendBit_1621(addr,6);     //Write addr high 6 bits
       SendBit_1621(sdata,8);    //Write data  8 bits
       CS1; 
      } 
      /**------------------------------------------------------------------------- 

                                Name: all_off(Clear Display) 
      ---------------------------------------------------------------------------*/ 

      void HT1621_all_off(uchar num) 
      { uchar i; 
        uchar addr=0; 
        for(i=0;i<num;i++) 
                       { 
                         Write_1621(addr,0x00); 
                     addr+=2; 
                    } 
      } 
      /**************************************************************************** 

                                Name: all_on(All lit) 
      ****************************************************************************/ 

      void HT1621_all_on(uchar num) 
      { uchar i,j; 
        uchar addr=0; 
        for(i=0;i<num;i++) 
                { Write_1621(addr,0xff); 
                addr+=2; 
                } 
      }      /**************************************************************************** 

                                Name: all_on_num(All lit,Display the same number) 
      ****************************************************************************/ 

      void HT1621_all_on_num(uchar num,uchar xx) 
      { uchar i,j; 
        uchar addr=0; 
        for(i=0;i<num;i++) 
                { Write_1621(addr,xx); 
                addr+=2; 
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

                                Name: LCDoff(off 1621) 
      *****************************************************************************/ 

      void LCDoff(void) 
      {  
       SendCmd_1621(LCD_off);  
      } 
      /**************************************************************************** 

                                Name: LCDon(on 1621) 
      *****************************************************************************/ 

      void LCDon(void) 
      {  
       SendCmd_1621(LCD_on);  
      } 
 

void displayallnum(unsigned int n)       //Display all digital characters
{uchar i,j; 
 uchar addr=0; 
 
        for(i=0;i<n;i++) 
                       { for(j=0;j<29;j++)
					     {
                         Write_1621(addr,num[j]); delay(100) ;
						 }
                         addr+=2; 
                       } 
  
}  


//By-bit display 8 characters and decimal point	
void displayall8(void)
{uchar i;
    HT1621_all_off(6);
	for(i=0;i<6;i++)
	   {
	Write_1621(2*i,0b11111111);
	delay(50) ;
	   }
}


//

 /**************************************************************************** 

   Name: displaydata(unsigned long int t,int p,char bat1,char bat2,char bat3) Display  data
   t : Data to be displayed
   p : Display decimal
   s1,s2 : Left side of the Triangle
                                
*****************************************************************************/ 

void displaydata(long int t,int p,char s1,char s2)
{uchar i;
dispnum[4]=num[t/10000];
dispnum[3]=num[(t/1000)%10];
dispnum[2]=num[(t/100)%10];
dispnum[1]=num[(t/10)%10];
dispnum[0]=num[t%10];
switch(p)
{case 1:
     sbi(dispnum[0], 4);
     break;
case 2:
     sbi(dispnum[1], 4);
     break;
case 3:
     sbi(dispnum[2], 4);
     break;
default:break;
}

if(s1==1)  sbi(dispnum[3], 4);
if(s2==1)  sbi(dispnum[4], 4);

for(i=0;i<5;i++) 
{
  Write_1621(i*2,dispnum[i]);
}
} 

void setup()
{
   pinMode(CS, OUTPUT); //Pin 2 
   pinMode(WR, OUTPUT); //Pin 3 
   pinMode(DATA, OUTPUT); //Pin 4  
  
}


void loop()
{
long int t=0;
uchar i,j;
delay(50) ;
Init_1621() ;

    HT1621_all_on(16) ;
	delay(300) ;
    HT1621_all_off(16);
	delay(300) ;
    HT1621_all_on(16) ;
	delay(300) ;
    HT1621_all_off(16);
	delay(300) ;

for(i=0;i<1;i++) 
   {
     HT1621_all_on_num(16,num[i]);
     delay(200) ;
   }
	 
    HT1621_all_off(16);
	
while(1)
    { 
   
    //display 0 to 999999
   for(t=320;t<370;t++)
   {	 
    Serial.println(t);
    int grados=t*10;
     displaydata(grados,1,0,0);
     delay(100) ;
    }
  }
}
