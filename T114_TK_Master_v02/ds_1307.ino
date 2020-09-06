// convert ASCII letters from the input array to a byte containing the day of the week
byte dayToByte(int x,int y, int z){
  int temp = buffer[x]+buffer[y]+buffer[z];
  switch(temp){
    case 0x12A: return (byte) 1; break; // Mon
    case 0x12E: return (byte) 2; break; // Tue
    case 0x120: return (byte) 3; break; // Wed
    case 0x131: return (byte) 4; break; // Thu
    case 0x121: return (byte) 5; break; // Fri
    case 0x128: return (byte) 6; break; // Sat
    case 0x136: return (byte) 7; break; // Sun
    default: return (byte) 0;
    }
}

// convert ASCII letters from the from the input array to a byte containing the month
byte monthToByte(int x,int y, int z){
  int temp = buffer[x]+buffer[y]+buffer[z];
  Serial.print(buffer[x],DEC);  Serial.print(" ");
  Serial.print(buffer[y],DEC);   Serial.print(" ");
  Serial.println(buffer[z],DEC); 
  switch(temp){
    case 0x119: return (byte) 1; break; // Jan
    case 0x10D: return (byte) 2; break; // Feb
    case 0x120: return (byte) 3; break; // Mar
    case 0x123: return (byte) 4; break; // Apr
    case 0x127: return (byte) 5; break; // May
    case 0x12D: return (byte) 6; break; // Jun
    case 0x12B: return (byte) 7; break; // Jul
    case 0x11D: return (byte) 8; break; // Aug
    case 0x128: return (byte) 9; break; // Sep
    case 0x126: return (byte) 10; break; // Oct
 //   case 0xCF: return (byte) 11; break; // Nov
    case 0x133: return (byte) 11; break; // Nov
    case 0x10C: return (byte) 12; break; // Dec
    default: return (byte) 0;
    }
}

// handle RX'd serial data
void fillBuffer(){
  delay(500); // wait for rest of transmission
  int ptr =0;
  buffer[ptr++]=Serial.read();
  while(Serial.available() > 0) { // while there is serial data, read it
    buffer[ptr++]=Serial.read(); // store it in the buffer
    if(buffer[ptr]=='/n'){ // break on newline
      break;
    }
  }
} 

// set the DS1307 using the input piped from serial in Unix date format
// Fri Sep  3 14:18:22 IST 2010
void setDateDS1307(){
   second=digitToByte(17,18);
   minute=digitToByte(14,15);
   hour=digitToByte(11,12);
   dayweek=dayToByte(0,1,2);
   day=digitToByte(8,9);
   month=monthToByte(4,5,6);
   year=digitToByte(26,27);
   
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   Wire.write((byte) 0x00);
   Wire.write(dec2bcd(second));
   Wire.write(dec2bcd(minute));
   Wire.write(dec2bcd(hour));
   Wire.write(dec2bcd(dayweek));
   Wire.write(dec2bcd(day));
   Wire.write(dec2bcd(month));
   Wire.write(dec2bcd(year));
   Wire.endTransmission();
}

// get the date and time from DS1307
void getDateDS1307(){
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write((byte) 0x00); // reset DS1307 register pointer
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  second=bcd2dec(Wire.read() & 0x7f); // control bits
  minute=bcd2dec(Wire.read());
  hour=bcd2dec(Wire.read() & 0x3f); // control bits
  dayweek=bcd2dec(Wire.read());
  day=bcd2dec(Wire.read());
  month=bcd2dec(Wire.read());
  year=bcd2dec(Wire.read());
}
// write the date and time to serial if LCD is not available
void wrTimeSerial(){
  if(day<10) Serial.print(" ");
  Serial.print(day, DEC);
  Serial.print("/");
  if(month<10) Serial.print("0");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" ");     
  if(hour<10) Serial.print("0");
  Serial.print(hour, DEC );
  Serial.print(":");     
  if(minute<10) Serial.print("0");
  Serial.print(minute, DEC);
  Serial.print(":"); 
  if(second<10) Serial.print("0");
  Serial.print(second, DEC);
  
  Serial.print("  ");
  Serial.print(dayweek,DEC);
  Serial.print("  ");
  Serial.print(pva[dayweek]);
  Serial.println();
}

/* sample input buffer
Sun Oct 25 09:30:00 IST 2015

Mon
Tue
Wed
Thu
Fri
Sat
Sun 

0: S
1: a
2: t
3:  
4: N
5: o
6: v
7:  
8: 2
9: 8
10:  
11: 2
12: 1
13: :
14: 0
15: 0
16: :
17: 0
18: 0
19:  
20: I
21: S
22: T
23:  
24: 2
25: 0
26: 1
27: 0
*/
