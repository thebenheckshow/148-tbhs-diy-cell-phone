#include <Servo.h> 
 
Servo myservo[2];  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
#include <Wire.h>
 
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

int xPos = 90;
int yPos = 65;

 
void setup() {
    pinMode(13, OUTPUT);
    Serial.begin(9600);         // start serial for output
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);
 
    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);

    myservo[0].attach(10);  // X
    myservo[1].attach(9);  // Y
  
    Serial.println("Ready!");
}
 
void loop() {
    delay(100);
}

void receiveData(int byteCount){
 
    while(Wire.available()) {        
        if (Wire.read() == 'X') {
          moveX(Wire.read());  
        }
        if (Wire.read() == 'Y') {
          moveY(Wire.read());  
        }        
    }
}

void sendData(){
    Wire.write(number);
}


void moveX(int whereTo) {

  if (whereTo < 10 or whereTo > 170) {
    return;  
  }
  
  xPos = whereTo;
  myservo[0].write(xPos);  
    
}

void moveY(int whereTo) {

  if (whereTo < 20 or whereTo > 110) {
    return;  
  }
  
  yPos = whereTo;
  myservo[1].write(yPos);  
    
}




