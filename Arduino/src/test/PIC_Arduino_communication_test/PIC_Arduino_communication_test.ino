#include <SoftwareSerial.h>

//Read-only (Arduino-PIC Comm)
const byte rxPin = 1;
const byte txPin = 0;

//Arduino-PIC Comm
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

void setup() {
  // Configure pin modes for tx and rx
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  // Open software serial port with baud rate = 9600
  //mySerial.begin(9600);
  Serial.begin(9600);
}

volatile char stat = '\0';
  
void loop() {

  //bit_logic_test();
  //rotary_encoder_test();

  int curr_pos = 500;
  char curr_pos_char[6];
  dtostrf(curr_pos, 5, 0, curr_pos_char);
  Serial.write('P');
  for (int i=0; i<5; i++) {
    if (curr_pos_char[i] == ' ') {
     Serial.write('0');
      //wait();
      continue;
    }
    Serial.write(curr_pos_char[i]);
    //wait();
  }

  while(1);
  
}

void rotary_encoder_test() {
  
  // Need to keep numbers under 2^15 = 32,768
  
  int RE_right = -1;
  mySerial.write('R');
  while(RE_right == -1) {
    if (mySerial.available() > 0) {
      RE_right = mySerial.read();
    }
  }
  
  int RE_left = -1;
  mySerial.write('L');
  while(RE_left == -1) {
    if (mySerial.available() > 0) {
      RE_left = mySerial.read();
    }
  }
  
  int RE = ((RE_left << 8) & 0xff00) | (RE_right & 0x00ff);
  if (RE == 100) {
    mySerial.write('A');
  }

  while(1);
}

void bit_logic_test() {
  
  int RE = 100;
  int RE_left = RE >> 8;
  int RE_right = RE & 0x00ff;
  Serial.print(RE, BIN);
  Serial.print('\n');
  Serial.print(RE_left, BIN);
  Serial.print('\n');
  Serial.print(RE_right, BIN);
  Serial.print('\n');

  int RE_together = ((RE_left << 8) & 0xff00) | (RE_right & 0x00ff);
  Serial.print(RE_together, BIN);
  Serial.print('\n');

  while(1);
}

void sending_end_data_test() {
  
  float cracks[] = {3.2, 11.1, 200.1, 300.1, 400.1};
  int crack_n = 5;
  float holes[] = {100.2, 200.2, 300.2, 400.2};
  int hole_n = 4;

  int number_of_cones_deployed_int = 15;
  char crack_num_int = 10;
  char hole_num_int = 3;
  
  // Sending metadata
  char number_of_cones_deployed[3];
  dtostrf(number_of_cones_deployed_int, 2, 0, number_of_cones_deployed);
  mySerial.write('^');
  for (int i=0; i<3; i++) {
    if (number_of_cones_deployed[i] == ' ') {
     mySerial.write('0');
      wait();
      continue;
    }
    mySerial.write(number_of_cones_deployed[i]);
    wait();
  }

  char hole_num[3];
  dtostrf(hole_num_int, 2, 0, hole_num);
  mySerial.write('*');
  for (int i=0; i<3; i++) {
    if (hole_num[i] == ' ') {
      mySerial.write('0');
      wait();
      continue;
    }
    mySerial.write(hole_num[i]);
    wait();
  }

  char crack_num[3];
  dtostrf(crack_num_int, 2, 0, crack_num);
  mySerial.write('_');
  for (int i=0; i<3; i++) {
    if (crack_num[i] == ' ') {
      mySerial.write('0');
      wait();
      continue;
    }
    mySerial.write(crack_num[i]);
    wait();
  }

  // Sending crack data
  mySerial.write('c');
  for (int i=0; i<crack_n; i++) {
    char buf[6];
    dtostrf(cracks[i], crack_n, 1, buf);
    for (int j=0; j<6; j++) {
      if (buf[j] == ' ') {
        mySerial.write('0');
        wait();
        continue;
      }
      mySerial.write(buf[j]);
      wait();
    }
  }
  
  // Sending hole data
  mySerial.write('h');
  for (int i=0; i<hole_n; i++) {
    char buf[6];
    dtostrf(holes[i], hole_n, 1, buf);
    for (int j=0; j<6; j++) {
      if (buf[j] == ' ') {
        mySerial.write('0');
        wait();
        continue;
      }
      mySerial.write(buf[j]);
      wait();
    }
  }

  while(1);
  
}

void wait() {
  while(stat != 'D') {
    if (mySerial.available() > 0) {
      stat = mySerial.read();
    }
  }
  stat = '\0';
}
