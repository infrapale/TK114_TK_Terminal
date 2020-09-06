/*
  Software serial multple serial test
 
 Receives from the hardware serial, sends to software mySerial.
 Receives from software serial, sends to hardware mySerial.
 
 The circuit: 
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
 
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts, 
 so only the following can be used for RX: 
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 
 Not all pins on the Leonardo support change interrupts, 
 so only the following can be used for RX: 
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 
 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 
 This example code is in the public domain.
 
 */


#define UNIT_ADDR_1 'I'
#define UNIT_ADDR_2 'O'
#define UNIT_ADDR_3 '1'


boolean inp[4];

void setup()  
{

  InitSoftCom();
 
  Serial.begin(9600);


  // set the data rate for the SoftwareSerial port
  //mySerial.begin(9600);
  Serial.println("Hello, world?");
  SendSoftCom("IO1I1");
}

void loop() // run over and over
{
  
  SoftComMonitor();
  //DebugSoftCom();
  if (SoftComFlag()) {
     if (inp[0]) SendSoftCom("IO1R8S"); else SendSoftCom("IO1R8C");
     Serial.println("flag");
     InitSoftCom();
     delay(5000);
     SendSoftCom("IO1I1");
  }
  //SendSoftCom("IO1R8S");
  //delay(1000);
  //SendSoftCom("IO1R8C");
  //delay(1000);
  //softCom.print("<IO1R8C>");softCom.print('\r');
  //softCom.print("<IO1I1>");softCom.print('\r');


  
  


}
