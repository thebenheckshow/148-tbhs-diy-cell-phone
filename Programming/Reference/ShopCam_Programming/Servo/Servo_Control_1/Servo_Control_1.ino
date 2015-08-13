#include <Servo.h> 
#include <Wire.h>

#define SLAVE_ADDRESS 0x04

int number = 0;
int state = 0;

unsigned char command = 0;
int parameter = 0;

int xPosition = 90;
int yPosition = 65;

Servo servoCam[2];  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
void setup() {
    pinMode(13, OUTPUT);
    Serial.begin(9600);         // start serial for output
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);
 
    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);

    servoCam[0].attach(10);  // X
    servoCam[1].attach(9);  // Y
  
    Serial.println("Ready!");
}
 
void loop() {
    delay(100);
}

void receiveData(int byteCount){
 
    while(Wire.available()) {

        command = Wire.read();

        if (command & B10000000) {  //Y command?
          command &= B01111111;          
          command = map(command, 0, 127, 20, 110);          
          moveY(command);
          Serial.print("Y: ");
          Serial.println(command);  
        }
        else {                      //X command?
          command &= B01111111;
          command = map(command, 0, 127, 10, 170);
          moveX(command);
          Serial.print("X: ");
          Serial.println(command);           
        }
        
    }
}

void sendData(){
    Wire.write(parameter);
}


void moveX(int directionX) {

  if (directionX < 10 or directionX > 170) {
    return;  
  }
  
  xPosition = directionX;
  servoCam[0].write(xPosition);  
    
}

void moveY(int directionY) {

  if (directionY < 20 or directionY > 110) {
    return;  
  }
  
  yPosition = directionY;
  servoCam[1].write(yPosition);  
    
}




