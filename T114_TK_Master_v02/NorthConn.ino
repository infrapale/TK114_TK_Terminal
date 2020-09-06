#define north_conn_o     1000
#define north_conn_UNIT  100
#define north_conn_T     10
#define NORTH_CONN_BUFF_LEN     20


boolean north_msg_rdy;
byte north_conn_state =0;
byte north_conn_buff_indx;
char north_conn_buff[NORTH_CONN_BUFF_LEN];

//<IO1R8S>\r
//<
// uu      2 char unit identifier
//   i     1 char unit index
//    xxx  unit specific commands and data

//<KB1*>\r  Key '*' from keyboard 1

void north_conn_setup(void){
  north_conn_state=0;
  north_conn_buff_indx = 0;
  north_msg_rdy = false;
  
}

void rd_north_conn(void){
   char char_in;
 
   if (Serial.available() && !north_msg_rdy) { 	
      char_in = Serial.read();
      //Serial.print(char_in);Serial.print("-");Serial.print(north_conn_state);
      switch (north_conn_state){
         case 0:
            if(char_in=='<') north_conn_state=1;
            break;
          case 1:
            if (char_in=='>'){ north_conn_state=2; }
            else {
               if (north_conn_buff_indx < NORTH_CONN_BUFF_LEN) 
                  north_conn_buff[north_conn_buff_indx++] = char_in;
               else {
                  Serial.println("north_conn buffer overrun");
                  north_conn_setup();
               }   
            }
            break;  
         case 2:
            if ((char_in=='\n')||(char_in=='\r')) {
               north_msg_rdy = true;
               north_conn_state=100;
               //proc_north_conn_msg();
            }
            break;
         case 100:
            // wait for the message to be processed
            if (!north_msg_rdy)  north_conn_setup();
            break;
      }   //  north_conn switch
 
 
        //Serial.print("-");Serial.println(north_conn_state);
  }
  
} 


void proc_north_conn_msg(void){
   boolean is_ok = true;
   int data_tag =0;

   //Serial.print("proc_north_conn_msg"); Serial.println(north_conn_buff);
   if (north_conn_buff[0]=='K' && north_conn_buff[1]=='B'){
   
   }
   
   else Serial.println("Invalid msg on north_conn");  
}
