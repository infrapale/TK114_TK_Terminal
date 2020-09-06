
void InitQuickKbd(void){
  quick_kbd.set_aval(0, 237);  //1
  quick_kbd.set_aval(1, 320);  //2
  quick_kbd.set_aval(2, 464);  //3
  quick_kbd.set_aval(3, 511);  //4
  quick_kbd.set_aval(4, 562);  //5
  quick_kbd.set_aval(5, 607);  //6
  quick_kbd.set_aval(6, 654);  //7
  quick_kbd.set_aval(7, 696);  //8
  quick_kbd.set_aval(8, 768);  //9
  quick_kbd.set_aval(9, 840);  //*
  quick_kbd.set_aval(10, 890); //#
  quick_kbd.set_aval(11, 932); //0
  quick_kbd.set_aval(12, 978); //=
  quick_kbd.set_aval(13, 410); //>
 
}

void InitNumKbd(void){
  num_kbd.set_aval(11, 178);   //#
  num_kbd.set_aval(10, 235);   //*
  num_kbd.set_aval(9, 319);   //9
  num_kbd.set_aval(8, 407);   //8
  num_kbd.set_aval(7, 510);   //7
  num_kbd.set_aval(6, 560);   //6
  num_kbd.set_aval(5, 605);   //5
  num_kbd.set_aval(4, 692);   //4
  num_kbd.set_aval(3, 767);   //3
  num_kbd.set_aval(2, 834);   //2
  num_kbd.set_aval(1, 884);   //1
  num_kbd.set_aval(0, 926);   //0

  
}


byte get_gr_indx(void){
  char key;
  byte gr_indx=0;
  key = quick_kbd.read();
  if(key>0) {
 
    switch (key){
      case '0': gr_indx = 1; break;
      case '1': gr_indx = 2; break;
      case '2': gr_indx = 3; break;
      case '3': gr_indx = 4; break;
      case '4': gr_indx = 5; break;
      case '5': gr_indx = 6; break;
      case '6': gr_indx = 7; break;
      case '7': gr_indx = 8; break;
      case '8': gr_indx = 9; break;
      case '9': gr_indx = 10; break;
      case '*': gr_indx = 11; break;
      case '#': gr_indx = 12; break;
      case '@': gr_indx = 13; break;
      case '=': gr_indx = 14; break;
      case '>': gr_indx = 15; break;
      case '<': gr_indx = 16; break;
    }
    Serial.print("Key=");
    Serial.print(char(key));
    Serial.print("  Group=");
    Serial.println(gr_indx);
  }  
  return (gr_indx);
}

