//<#IO41x>, x=0|1
#include <SoftwareSerial.h>
#define rxPin 10
#define txPin 9
#define SCOM_BUFF_LEN 16

boolean inp[4];

SoftwareSerial softCom(rxPin, txPin); // RX, TX
char c;
byte softComState;
char softComBuff[SCOM_BUFF_LEN];
short int softComBuffIndx =0;
short int i;


boolean SoftComFlag(void){
  return softComFlag;
}

void InitSoftCom (void){
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  softCom.begin(9600);
  softComState = 0;
  softComBuffIndx =0;
  for (i=0;i<SCOM_BUFF_LEN;i++){
    softComBuff[i]= '0'+i;
  }
  softCom.listen();
  softComFlag = false;
}

void SendSoftCom(char *b){
  //delay(1000);
  softCom.print('<');
  softCom.print(b);
  softCom.print(">\r");
  //Serial.println(b);
 
}

 void DebugSoftCom(void){
 
 if (softCom.available()>0){
    digitalWrite(LED_PIN, HIGH); 
    c = softCom.read();
    Serial.print(c);
  }
 }

void SoftComMonitor(void) {
  
  //softCom.print("X");
  softCom.listen();
  

  if (softCom.available()>0){
     digitalWrite(LED_PIN, HIGH); 
     c = softCom.read();
     switch(softComState){
     case 0:
       if (c=='<') {
         softComState = 1;
       }
       break;
     case 1:
       if (c=='>') {
         softComState = 2;
         //Serial.println("msg <...> ok");
       } 
       else {
         softComBuff[softComBuffIndx] = c;
         softComBuffIndx++;
         if (softComBuffIndx >= SCOM_BUFF_LEN) {
            InitSoftCom;
         }
         
       }     
       break;
     case 2:
       if ((c=='\r')||(c=='\n')) {
          //Serial.println(softComBuff);
          switch( softComBuff[0] ){
          case '#': 
             if ((softComBuff[1] == UNIT_ADDR_1 ) && (softComBuff[2] == UNIT_ADDR_2 ) && 
                 (softComBuff[3] == UNIT_ADDR_3 ) && (softComBuff[4] == 'I' )) {
                if ((softComBuff[5] >='1') && (softComBuff[5] <='4'))  {
                   if ((softComBuff[6] =='0') || (softComBuff[6] =='1'))  {
                      i = softComBuff[5]-'1' ; 
                      if ( softComBuff[6] =='1') inp[i]=HIGH; else inp[i]=LOW; 
                      softComFlag = true;
                      
                   }
                }   
             }
             break;
          case MY_ADDR_1:    // <SCQ1S>
             if (softComBuff[1] == MY_ADDR_2 ){
                switch(softComBuff[2]){
                case 'Q':
                  if (((softComBuff[4]=='S')||(softComBuff[4]=='C')) &&
                       (softComBuff[3]>='0')&&(softComBuff[3]<='9')){
                    quick_buff[0]= softComBuff[4];
                    quick_buff[1]= softComBuff[3];
                    softComFlag = true;
                    //Serial.print("quick_buff = "); Serial.print(quick_buff[0]); Serial.println(quick_buff[1]);
                    
                  }
                  break;
                }
               
             }
             break; 
          }  
       } 
       softComState = 0;
       softComBuffIndx =0;
       for (i=0;i<SCOM_BUFF_LEN;i++) softComBuff[i]= '0';
       break;
    }  
  }         
}

