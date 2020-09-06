// convert bin to BCD
byte dec2bcd(byte val){
  return ((val/0xA*0x10)+(val%0xA));
}

// convert BCD to binary
byte bcd2dec(byte val){
  return ((val/0x10*0xA)+(val%0x10));
}

// convert two ASCII numbers from the input array to a single byte
byte digitToByte(int x,int y){
  return (byte) ((buffer[x]&0x0F)*10+(buffer[y]&0x0F));
}

