
#define IRpin_PIN      PIND
#define IRpin          2

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
#define NUMPULSES 50

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20 

// What percent we will allow in variation to match the same code
#define FUZZINESS 20

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing

#include "ircodes.h"

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

    Serial.begin(9600);
    
    Wire.begin(SLAVE_ADDRESS);    
    Wire.onReceive(receiveData);  // define callbacks for i2c communication
    Wire.onRequest(sendData);
    
    //servoCam[0].attach(10);  // X TESTER
    //servoCam[1].attach(9);  // Y
    
    servoCam[0].attach(3);  // X
    servoCam[1].attach(4);  // Y
    moveX(90);              //Center camera
    moveY(110);  
  
    Serial.println("Ready!");
}
 
void loop() {
  int numberpulses;
  
  while (IRpin_PIN & (1 << IRpin)) {  
  }
    
  numberpulses = listenForIR();

  if (IRcompare(numberpulses, IRup, sizeof(IRup)/4)) {
    moveY(yPosition + 10);    
    Serial.println("UP");
  }
  if (IRcompare(numberpulses, IRdown, sizeof(IRdown)/4)) {
    moveY(yPosition - 10);
    Serial.println("DOWN");
  }  
  if (IRcompare(numberpulses, IRleft, sizeof(IRleft)/4)) {
    moveX(xPosition + 10);
    Serial.println("LEFT");
  }
  if (IRcompare(numberpulses, IRright,sizeof(IRright)/4)) {
    moveX(xPosition - 10);
    Serial.println("RIGHT");
  }
  if (IRcompare(numberpulses, IRmenu, sizeof(IRmenu)/4)) {
    moveX(90);
    moveY(110);
    Serial.println("HOME");
  }    
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


//KGO: added size of compare sample. Only compare the minimum of the two
boolean IRcompare(int numpulses, int Signal[], int refsize) {
  int count = min(numpulses,refsize);

  for (int i=0; i< count-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;
      
    // check to make sure the error is less than FUZZINESS percent
    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {

    } else {

      // we didn't match perfectly, return a false match
      return false;
    }
      
    if ( abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {

    } else {

      // we didn't match perfectly, return a false match
      return false;
    }

  }
  // Everything matched!
  return true;
}


int listenForIR(void) {
  currentpulse = 0;
  
  while (1) {
    uint16_t highpulse, lowpulse;  // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length
  
//  while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
       // pin is still HIGH

       // count off another few microseconds
       highpulse++;
       delayMicroseconds(RESOLUTION);

       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       
       // KGO: Added check for end of receive buffer
       if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    // we didn't time out so lets stash the reading
    pulses[currentpulse][0] = highpulse;
  
    // same as above
    while (! (IRpin_PIN & _BV(IRpin))) {
       // pin is still LOW
       lowpulse++;
       delayMicroseconds(RESOLUTION);
        // KGO: Added check for end of receive buffer
        if (((lowpulse >= MAXPULSE)  && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;

    // we read one high-low pulse successfully, continue!
    currentpulse++;
  }
}




