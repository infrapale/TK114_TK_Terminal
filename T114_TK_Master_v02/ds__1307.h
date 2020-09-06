short int  second, minute, hour, dayweek, day, month, year; // create bytes for storing values read from DS1307
// SS[0-60], MM[0-60], HH[0-24], D[1-7], DD[1-31], MM[1-12], YY[00-99]

char buffer[50]; // temporary buffer to store incoming serial data
char pva[8][4]={"Non","Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
 
