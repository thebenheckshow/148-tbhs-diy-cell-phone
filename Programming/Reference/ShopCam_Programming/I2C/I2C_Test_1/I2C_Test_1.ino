#include <Servo.h> 
 
Servo myservo[2];
 
#include <Wire.h>
 
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;
 
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
 
// callback for received data
void receiveData(int byteCount){
 
    while(Wire.available()) {
        number = Wire.read();
        Serial.print("data received: ");
        Serial.println(number);
        myservo[0].write(number);  
 

     }
}
 
// callback for sending data
void sendData(){
    Wire.write(number);
}
