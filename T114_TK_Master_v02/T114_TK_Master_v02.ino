/*
 * LDR:   GND--LDR--LDR_PIN--10k-->5V
*/
#include <Arduino.h>
#include <Wire.h> // include the I2C library
#include <EEPROM.h>

#define DS1307_I2C_ADDRESS 0x68 // DS1307 I2C address
#include "ds__1307.h"
#include <SimpleTimer.h> 
#include <LiquidCrystal_I2C.h>

//#include "gs64.h"
#  if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <akbd.h>
#include "LiCo.h"
#include "txt.h"
#define LED_PIN 13

//#define AUTO_SW_PIN 9
#define AN_NUM_KBD A2
#define LDR_PIN A3



SimpleTimer timer;
akbd quick_kbd(1);
akbd num_kbd(2);
//LiquidCrystal_I2C lcd(0x27,16,2);
LiquidCrystal_I2C lcd(0x27,20,4);




byte hx;
byte hx_prev;
char key;
byte num_key;
byte quick_indx;
byte relay_group_indx;
int goto_auto_cntr;
boolean softComFlag;

boolean enable_auto_light;
int analog_inp;

boolean new_cntr_state;
char lcd_txt_2[17];
char quick_buff[2];
int  show_cnt_down;
unsigned int light_meas;
byte light_meas_state;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 
  Wire.begin(); // initalise I2C interface
  Serial.begin(9600); // initialise serial 
  InitSoftCom();
  timer.setInterval(5000, tk_autom);
  timer.setInterval(10000, run_every_10000_ms);
  timer.setInterval(600000, run_interval_10min); // output time over serial
  timer.setInterval(1000, run_every_1000_ms);
  timer.setInterval(10, run_every_10_ms);
  //timer.setInterval(1, run_every_1_ms);
  

  InitQuickKbd();
  InitNumKbd();

  rd_param_EEPROM();
  
  //gs64_setup();
  Serial.println("T114 TK Master v02");
  Serial.println("TH 2014-12-13");
  delay(2000);
  //pinMode(AUTO_SW_PIN, INPUT);
  //enable_auto_light= true;
  light_meas_state = 0;
  light_meas = analogRead(LDR_PIN);
  
  lcd.init();lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Villa Astrid");
  lcd.setCursor(0, 1);
  lcd.print("T114 2015");
}

void loop() {
   SoftComMonitor(); //DebugSoftCom();
  timer.run();
  //if (digitalRead(AUTO_SW_PIN) == LOW) enable_auto_light= true; else enable_auto_light= false;
  if (quick_indx >0){
    Serial.print("Quick Index=");
    Serial.println(quick_indx);
    switch (quick_indx){
      case 0: break;
      case 1: //MH1
        new_cntr_state = update_relay_group(1,2);
        // 13 228 / 1606  if (new_cntr_state) quick_txt_2("MH 1: Tander"); else quick_txt_2("MH 1: Slacker");
        if (new_cntr_state) lcd_txt(2,TXT_MH1); else lcd_txt(2,TXT_MH1+1);
        break;
      case 2: //MH1
        new_cntr_state = update_relay_group(2,2);
        if (new_cntr_state) lcd_txt(2,TXT_MH2); else lcd_txt(2,TXT_MH2+1);
        break;
      case 3: //Eteinen
        new_cntr_state = update_relay_group(3,2);
        if (new_cntr_state) lcd_txt(2,TXT_ET); else lcd_txt(2,TXT_ET+1);
        break;
      case 4: //Keittio
        new_cntr_state = update_relay_group(4,2);
        if (new_cntr_state) lcd_txt(2,TXT_K); else lcd_txt(2,TXT_K+1);
        break;
      case 5: //Tupa
        new_cntr_state = update_relay_group(5,2);
        if (new_cntr_state) lcd_txt(2,TXT_TUPA); else lcd_txt(2,TXT_TUPA+1);
        break;
      case 6: //KHH +PSH+S
        new_cntr_state = update_relay_group(6,2);
        if (new_cntr_state) lcd_txt(2,TXT_KHH); else lcd_txt(2,TXT_KHH+1);
        break;
      case 7: //Parvi
        new_cntr_state = update_relay_group(7,2);
        if (new_cntr_state) lcd_txt(2,TXT_PARVI); else lcd_txt(2,TXT_PARVI+1);
        break;
      case 8: //Ulkovalot
        new_cntr_state = update_relay_group(8,2);
        if (new_cntr_state) lcd_txt(2,TXT_ULKO); else lcd_txt(2,TXT_ULKO+1);
         break;
      case 12:
        quick_txt_2("!!!All Off!!!");
        all_off();
        break;
      case 13:
        enable_auto_light= false;
        wr_param_EEPROM();
        //                              1234567890123456
        //lcd.setCursor(0, 1); lcd.print("-----Kotona-----");
        break;
      case 14:
        enable_auto_light= false;
        goto_auto_cntr = 20;
        wr_param_EEPROM();
        //                              1234567890123456        
        //lcd.setCursor(0, 1); lcd.print("(-8= Astrid =8-)");
        break;
      default: 
        
        break; 
     
    }
     quick_indx = 0;  
  }
  if (softComFlag) {
     softComFlag = false;
     if (quick_buff[0]== 'S') new_cntr_state = update_relay_group(quick_buff[1]-'0',1);
     if (quick_buff[0]== 'C') new_cntr_state = update_relay_group(quick_buff[1]-'0',0);
     quick_buff[0] = 0;
  }
  //if (relay_group_indx) update_relay_group(relay_group_indx);
  //quick_kbd.scan();
  //wrTimeSerial(); // output time over serial
  //delay(500);
} 

//void lcd_txt(byte row, const char *txt_ptr ){
void lcd_txt(byte row, byte txt_indx ){
  byte i=0;
  show_cnt_down = 5;
  
  for(i=0;i<17;i++) lcd_txt_2[i++] = ' ';
  strcpy_P(lcd_txt_2, (char*)pgm_read_word(&Texts[txt_indx]));
  //strcpy(lcd_txt_2, txt_ptr);
    
}

void quick_txt_2(char *buff ){
  byte i=0;
  show_cnt_down = 5;
  while (*buff && i<17){
     lcd_txt_2[i++] =*(buff++);
  }
  while (i<17){
     lcd_txt_2[i++] = ' ';
  }
   
}
 
  
void run_every_10000_ms(void){
  wrTimeSerial(); // output time over serial
  relay_debug();
  sprintf(buffer,"Astrid:%2d:%2d%4d",hour,minute,light_meas);
  Serial.println(buffer);
  lcd.setCursor(0, 0);
  lcd.print(buffer);
  
  
  
}
void run_every_1000_ms(void){
  getDateDS1307();   //get time
  //Serial.println(analog_inp);
  num_key = num_kbd.read(); //Serial.println(char(num_key));
  if (quick_indx == 0) quick_indx =  get_gr_indx();
  if (goto_auto_cntr >0) {
    if (--goto_auto_cntr == 0) {
       all_off();
       enable_auto_light= true;
    }
  }
  //                              1234567890123456
  lcd.setCursor(0, 1); 
  if ( enable_auto_light ) lcd.print("(-8= Astrid =8-)");
  else {
    if (goto_auto_cntr >0) {
      lcd.print("                "); 
      lcd.setCursor(0, 1); 
      lcd.print("Countdown: ");
      lcd.print(goto_auto_cntr); 
    }
    else {
      
      
      if ( show_cnt_down ) {
         lcd.print (lcd_txt_2);
         show_cnt_down--;
      }
      else {
        lcd.print("-----Kotona-----"); 
      
      }
    }
  }
  if (enable_auto_light) {
     if (light_meas_state++ == 1) send_relay_msg( 1,1, false);
     if (light_meas_state > 2 ) light_meas_state++;
     if (light_meas_state > 10) {
        light_meas_state = 0;
        light_meas = analogRead(LDR_PIN);
     }
     
  } 
  else {
     light_meas = analogRead(LDR_PIN);  
  }
}

void run_every_10_ms(void){
  quick_kbd.scan();  
  num_kbd.scan();
  //analog_inp = analogRead(AN_NUM_KBD);
}

void run_every_1_ms(void){
  SoftComMonitor();  
}

void run_interval_10min(void){
  wr_param_EEPROM();
  if (enable_auto_light) light_meas_state = 1;
}


void wr_param_EEPROM(void){
// Reserved for address range 0..255
  int addr =0;
  EEPROM.write(addr, (byte)enable_auto_light);
  addr = 0x0010;
  for (int i=0;i<4;i++) EEPROM.write(addr+i, light_status[i]);
  

}

void rd_param_EEPROM(void){
  int addr =0;
  
  enable_auto_light = (boolean)EEPROM.read(addr);
  addr = 0x0010;
  for (int i=0;i<4;i++) light_status[i] = EEPROM.read(addr+i);

}
