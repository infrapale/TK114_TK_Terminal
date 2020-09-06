
volatile char relay_group;
volatile char relay_indx; 

byte r_nbr = 1;
byte r_grp = 1;

void relay_debug(void) {
  Serial.print("light_status = ");
  Serial.print(light_status[1],BIN);
  Serial.print("--");
  Serial.print(light_status[2],BIN);
  Serial.print("--");
  Serial.print(light_status[3],BIN);
  Serial.print("--");
  Serial.println(light_status[4],BIN);  
}

void tk_autom(void){
    hx = hour; 
    //hx = second /2;
    //Serial.println(hx);
    if (hx > 23 ) hx= 23;
    if (++r_nbr> 8) {
       r_nbr = 1;
       if(++r_grp > 1) r_grp = 1;
    }  
    if (enable_auto_light && light_meas_state == 0) {
       light_status[r_grp] = RELAY_TIME_CTRL [hx][r_grp-1];          
       wr_all_lights();
    } 
}

void wr_all_lights( void){
    boolean ron=false;
    byte light_level;
    ron=false;  
    light_level = 0;  //dark
    if (light_meas <= 200 ) light_level = 3;
    if (light_meas > 200 && light_meas <= 500 ) light_level = 2;
    if (light_meas > 500 && light_meas <= 800 ) light_level = 1;

          
    if ( light_status[r_grp] & LIGHT_MEAS_CTRL[light_level][r_grp-1] & BIT_MASK8 [r_nbr] ) ron=true;
    send_relay_msg( r_grp,r_nbr, ron);
   
 
    /*
    for (r_grp = 1; r_grp < 1; r_grp++)  {
      //for (r_nbr= 1; r_nbr < 9; r_nbr++) {
      ron=false;        
      if ( light_status[r_grp-1] & BIT_MASK8 [r_nbr] ) ron=true;
        send_relay_msg( r_grp+'0',r_nbr+'0', ron);
      
    }
    */
}




void send_relay_msg( byte r_group, byte r_indx, boolean r_on){
  //"IO1R8S")
  char on_off = 'C';
  byte s1;
  boolean local= true;
  
  if (r_group >2) local = false;
  if (r_on) on_off = 'S';
  sprintf(buffer,"%c%c%c%c%c%c",UNIT_ADDR_1,UNIT_ADDR_2, r_group+'0','R',r_indx+'0',on_off);
  //delay(10);
  if (local)  SendSoftCom(buffer); else Serial.println(buffer);
  s1 = light_status[r_group];
  if (r_on)
     s1 |= BIT_MASK8[r_indx];
  else
     s1 &= ~BIT_MASK8[r_indx];
  light_status[r_group] = s1;
  
}
boolean is_on(byte gr_indx, byte r_indx){
  return((light_status[gr_indx] & BIT_MASK8[r_indx]) >0);
}



boolean update_relay_group(byte gr_indx, byte upd_mode){
  byte i=0;
  boolean gr_on=false;
  byte r_gr;
  byte r_indx;
  
  r_gr = RELAY_CODES[gr_indx][1];
  r_indx =r_gr & 0x0F;
  r_gr = r_gr >> 4;
  switch(upd_mode){
    case 0:  
      gr_on = false; 
      break;
    case 1: 
      gr_on = true; 
      break;
    case 2:
      if ((light_status[r_gr] & BIT_MASK8[r_indx])>0) gr_on = false; else gr_on = true;
      break;   
  }
   i=1;
  do 
  {
    r_gr = RELAY_CODES[gr_indx][i];
    r_indx =r_gr & 0x0F;
    r_gr = r_gr >> 4;

    send_relay_msg( r_gr, r_indx, gr_on);
    
    if ( ++i < 7)  r_gr = RELAY_CODES[gr_indx][i];
    else r_gr = 0;
  } while (r_gr > 0);
  
  return(gr_on);

}

void all_off(void){
  byte gindx;
  for(gindx = 1;gindx<9;gindx++) update_relay_group(gindx, 0);
}

void set_relay_group(byte r_group,boolean relay_on){
  byte gindx;
  
  for(gindx = 1;gindx<9;gindx++){ 
    if (relay_on) update_relay_group(gindx, 1); 
    else update_relay_group(gindx, 0);
  }  
}


