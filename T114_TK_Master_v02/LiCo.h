
// Relay Group 1  ----------------------------------------------
// 1:  TK Kattovalo
// 2:  Parvi kattovalo
// 3:  Tupa Kattovalo 1
// 4:  Tupa Kattovalo 2
// 5:  KHH Loisteputki
// 6:  Jouluvalot
// 7:  Piha ulkovalot seinällä
// 8:  Piha ulkovalot seinällä
// Relay Group 2  ----------------------------------------------
// 1:  Vesi
// 2:  Sauna
// 3:  WC
// 4:  MH1&2 Alakeskukset
// 5:  Polku laiturille
// 6:  Lämpötilaohjaus
// 7:  
// 8:  
// -------------------------------------------------------------
// Relay Unit Address
#define UNIT_ADDR_1 'I'
#define UNIT_ADDR_2 'O'
#define UNIT_ADDR_3 '1'
// Relay Unit Address
#define MY_ADDR_1   'S'   //Sub
#define MY_ADDR_2   'C'   //Controller


char const BIT_MASK8 [9] = {
   0b00000000, 
   0b00000001, 
   0b00000010,  
   0b00000100,  
   0b00001000,  
   0b00010000,  
   0b00100000,  
   0b01000000,  
   0b10000000 };
char const RELAY_TIME_CTRL [24][4] = {
//  Group 1      Group 2      Group 3     Group 4 
//                            Group 3-4 are not directly controlled by  this unit
  { 0b01100011, 0b00000000, 0b00000000, 0b00000000 },  // 00:00
  { 0b01100101, 0b00000000, 0b00000000, 0b00000000 },  // 01:00
  { 0b01100011, 0b00000000, 0b00000000, 0b00000000 },  // 02:00
  { 0b01100001, 0b00000000, 0b00000000, 0b00000000 },  // 03:00
  { 0b00100001, 0b00000000, 0b00000000, 0b00000000 },  // 04:00
  { 0b01100101, 0b00000000, 0b00000000, 0b00000000 },  // 05:00
  { 0b00100111, 0b00000000, 0b00000000, 0b00000000 },  // 06:00
  { 0b00100011, 0b00000000, 0b00000000, 0b00000000 },  // 07:00
  { 0b00100001, 0b00000000, 0b00000000, 0b00000000 },  // 08:00
  { 0b00100011, 0b00000000, 0b00000000, 0b00000000 },  // 09:00
  { 0b00000010, 0b00000000, 0b00000000, 0b00000000 },  // 10:00
  { 0b00000010, 0b00000000, 0b00000000, 0b00000000 },  // 11:00
  { 0b00000010, 0b00000000, 0b00000000, 0b00000000 },  // 12:00
  { 0b00000010, 0b00000000, 0b00000000, 0b00000000 },  // 13:00
  { 0b00000010, 0b00000000, 0b00000000, 0b00000000 },  // 14:00
  { 0b00100010, 0b00000000, 0b00000000, 0b00000000 },  // 15:00
  { 0b00100011, 0b00000000, 0b00000000, 0b00000000 },  // 16:00
  { 0b00100111, 0b00000000, 0b00000000, 0b00000000 },  // 17:00
  { 0b01100111, 0b00000000, 0b00000000, 0b00000000 },  // 18:00
  { 0b01100111, 0b00000000, 0b00000000, 0b00000000 },  // 19:00
  { 0b00100111, 0b00000000, 0b00000000, 0b00000000 },  // 20:00
  { 0b00100111, 0b00000000, 0b00000000, 0b00000000 },  // 21:00
  { 0b01100011, 0b00000000, 0b00000000, 0b00000000 },  // 22:00
  { 0b01100101, 0b00000000, 0b00000000, 0b00000000 }   // 23:00  
};   

char const LIGHT_MEAS_CTRL [4][4] = {
//  Group 1      Group 2      Group 3     Group 4 
//                            Group 3-4 are not directly controlled by  this unit
  { 0b11111111, 0b00000000, 0b00000000, 0b00000000 },  // Dark
  { 0b11110111, 0b00000000, 0b00000000, 0b00000000 },  // 
  { 0b00100010, 0b00000000, 0b00000000, 0b00000000 },  // 
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000 }   // Bright
};  
  
char const KEY_NEW_STATE[17][12] = {
// '0' '1' '2' '3' '4' '5' '6' '7' '8' '9' '*' '#'
  { 0,  1,  1,  1,  1,  1,  1,  1,  1,  3,  0,  0 },  // state 0; select relay or group
  { 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0 },  // state 1; second digit or set on/off
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 2; Complete Relay on/off
  { 3,  9,  4, 10,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 3; Settings
  { 4,  5,  5,  4,  4,  4,  4,  4,  4,  4,  0,  0 },  // state 4; Time, h_:__
  { 5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  0,  0 },  // state 5; Time, _h:__
  { 6,  7,  7,  7,  7,  7,  6,  6,  6,  6,  0,  0 },  // state 6; Time, __:m_
  { 7,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0 },  // state 7; Time, __:_m
  { 8,  9,  9,  8,  8,  8,  8,  8,  8,  8,  0,  0 },  // state 8; Accept time change
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 9  set automatic light control on/off
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 10 all lights off
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 11
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 12
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 13
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 14
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 15
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }   // state 16
};

char const KEY_ACTION[17][12] = {
// '0' '1' '2' '3' '4' '5' '6' '7' '8' '9' '*' '#'
  { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0 },  // state 0;
  { 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  5,  6 },  // state 1;
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  4 },  // state 2;
  { 0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 3;
  { 6,  6,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 4;
  { 7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  0,  0 },  // state 5;
  { 8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0 },  // state 6;
  { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0 },  // state 7;
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,  0 },  // state 8
  {21, 20,  0,  0,  0,  0,  0,  0,  0,  0, 20, 21 },  // state 9
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25 },  // state 10
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 11
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 12
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 13
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 14
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // state 15
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }   // state 16
};

char const DISP_STATE[17] = {
    1,
    2,
    3,
    4,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};
//
char const RELAY_GROUP[16][4] = {
  { 0x11, 0x0,  0x0,  0x00 },  // group 1; TK
  { 0x31, 0x32, 0x0,  0x00 },  // group 2; MH1
  { 0x41, 0x42, 0x0,  0x00 },  // group 3; MH2
  { 0x43, 0x0,  0x0,  0x00 },  // group 4; ET
  { 0x34, 0x35, 0x0,  0x00 },  // group 5; K
  { 0x13, 0x14, 0x0,  0x00 },  // group 6; Tupa
  { 0x15, 0x22, 0x00, 0x00 },  // group 7; KHH+PSH+S
  { 0x12, 0x00, 0x00, 0x00 },  // group 8; Parvi 
  { 0x17, 0x00, 0x00, 0x00 },  // group 9; Pihavalot
  { 0x18, 0x25, 0x0,  0x00 },  // group 10; Rantavalot
  { 0x23, 0x00, 0x00, 0x00 },  // group 11; WC
  { 0x16, 0x00, 0x00, 0x00 },  // group 12; Joulu
  { 0x24, 0x00, 0x00, 0x00 },  // group 13; Halo
  { 0x00, 0x00, 0x00, 0x00 },  // group 14; Parvi
  { 0x00, 0x00, 0x00, 0x00 },  // group 15; Kotona
  { 0x00, 0x00, 0x00, 0x00 }   // group 16; Pois
};


char const GROUP_CODES[10][10] = {
  {  1,  2,  3,  4,  5,  6,  7,  8,  9,  0 },  // group 0;
  { 11, 12,  0,  0,  0,  0,  0,  0,  0,  0 },  // group 1; MH1
  { 21, 22,  0,  0,  0,  0,  0,  0,  0,  0 },  // group 2; MH2
  { 31, 32, 33, 34,  0,  0,  0,  0,  0,  0 },  // group 3; ET
  { 41, 42, 43, 44, 45,  0,  0,  0,  0,  0 },  // group 4; K
  { 51, 52,  0,  0,  0,  0,  0,  0,  0,  0 },  // group 5; Tupa
  { 61, 62, 63, 64, 65,  0,  0,  0,  0,  0 },  // group 6; KHH+PSH+S+Vesi
  { 71,  0,  0,  0,  0,  0,  0,  0,  0,  0 },  // group 7; 
  { 81, 82, 83, 84,  0,  0,  0,  0,  0,  0 },  // group 8; Parvi
  { 91, 92,  0,  0,  0,  0,  0,  0,  0,  0 }   // group 9; Ulko
};

char const RELAY_CODES[10][10] = {
//  '0'   '1'    '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // group 0;
  { 0x00, 0x35, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // group 1; MH1
  { 0x00, 0x41, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // group 2; MH2
  { 0x00, 0x11, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // group 3; ET, WC (, Vesi)  
  { 0x00, 0x31, 0x32, 0x33, 0x34, 0x37, 0x00, 0x00, 0x00, 0x00 },  // group 4; K
  { 0x00, 0x13, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // group 5; Tupa
  { 0x00, 0x15, 0x43, 0x44, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00 },  // group 6; KHH+PSH+S+Vesi
  { 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // group 7; Parvi
  { 0x00, 0x17, 0x18, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // group 8; Ulkovalot
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }   // group 9; varattu asetuksia varten
};

byte light_status[5];   //Light on/off data group 1..4
