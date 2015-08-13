/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x32 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 2
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

char* messageString[]={"ENTER #", "CALL:", "CALL IN PROGRESS", "CALL ENDED", "INCOMING CALL", "BAD NUMBER", "NO ACTIVE CALLS", "THEY HUNG UP!"};
 
  
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const byte ROWS = 5; //four rows
const byte COLS = 3; //three columns
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
  {'A','B','C'},
};
byte rowPins[ROWS] = {3, 4, 5, 6, 7}; 	//connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9, 10}; 		//connect to the column pinouts of the keypad
int debounce[3][5];              		//Debounce timer
int key = 0;							//What key is pressed
byte input[255];						//The input buffer
byte buffPos = 0;						//Where we are in the buffer
byte numberCall[10] = {'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', };
byte cursor = 0;
unsigned long timer = 0;

int battery = 1;						//Battery 0-9 powerz. Set to 1 to use as initialization flag
int signal = 0;							//Signal zero to 4 bars can you hear me now?
int message = 0;						//What message should be onscreen
int activeCall = 0;						//If a call is active or not

void setup()   {                
  Serial.begin(9600);

  //Define column pins for keyboard matrix
  pinMode(colPins[0], 1);
  pinMode(colPins[1], 1);
  pinMode(colPins[2], 1);
  
  pinMode(A0, 0);
  digitalWrite(A0, 1);
  
  //Define row pins for keyboard matrix

	for (int x = 0 ;  x < 5 ; x++) {
		pinMode(rowPins[x], 0);        //Set as input
		digitalWrite(rowPins[x], 1);   //Set internal pullup
	}

	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	display.setTextColor(WHITE);
	
	while(battery) {						//Wait for the phone module to boot 

		Serial.println("AT");					//Try and get a response
		if (Serial.available()) {				//A response?
			battery = 0;						//Continue!
		}
		display.clearDisplay();
		display.setTextSize(1);		
		display.setCursor(0, 0); 	
		display.print("WAITING FOR SIM");
		display.setCursor(0, 12);
		display.print(millis()/1000);
		display.display();	

		delay(1000);		
	}

	battery = 0;							//Used as flag, so reset it now
	Serial.println("AT+CHFA=1");			//Use external mic and speaker
	Serial.println("ATL9");					//Speaker full volume
	Serial.println("AT+CMIC=1,10");			//Set microphone gain level to +15dB
	
	drawDisplay();							//Show display
	
}

void loop() {

	if (digitalRead(A0) == 0 and message != 4 and message != 2) {
		message = 4;
		activeCall = 2;
		drawDisplay();
	}

	getKeys();
	
	if (key) {
		
		if (key > 64) {							//A B or C key?
		
			if (key == 'A') {
			
				if (activeCall) {
					message = 3;
					Serial.println("ATH0");		//End the call
					activeCall = 0;
				}
				else {
					message = 6;				//No current call
				}							
			}
			if (key == 'C') {
			
				if (activeCall == 2) {			//Someone calling us?
					activeCall = 1;
					Serial.println("ATA");		//Answer it
					message = 2;
				}
			
				if (activeCall == 0) {			//Call me maybe?
					
					if (cursor < 9) {			
						message = 5;			
					}
					else {
						makeCall();				
					}		
				}				
			}		
		
			if (key == 'B') {
				
				if (cursor == 9) {				//On last character?
				
					if (numberCall[cursor] == '_') {
						cursor -= 1;
						numberCall[cursor] = '_';
					}
					else {
						numberCall[cursor] = '_';	
					}
				
				}
				else {
					if (cursor > 0 and cursor < 9) {
						cursor -= 1;				//Decrement cursor
						numberCall[cursor] = '_';	//and erase whatever was there				
					
						if (cursor == 0) {
							message = 0;
						}					
					}						
				}						
			}				
		}
		else {
			numberCall[cursor] = key;
			
			if (cursor < 9) {			
				cursor += 1;			
			}
			
			message = 1;
		}

		drawDisplay();
		
		key = 0;
	}

	timer += 1;
	
	if (timer == 40000) {
		Serial.println("AT+CBC");		//Get battery status
	}
	if (timer == 80000) {
		Serial.println("AT+CSQ");		//Get signal status
		timer = 0;						//Reset timer
	}	
	if (Serial.available()) {			//Something available?
		
		input[buffPos] = Serial.read();	//Read it into buffer

		if (buffPos > 2 and input[buffPos - 3] == 13 and input[buffPos - 2] == 10 and input[buffPos - 1] == 13 and input[buffPos] == 10) {		//Did we get double new lines?
		//if (buffPos > 2 and input[buffPos - 3] == 65 and input[buffPos - 2] == 66 and input[buffPos - 1] == 67 and input[buffPos] == 68) {		//Did we get double new lines?
			
			interpretBuffer();
		
		}
		else {
			buffPos += 1;
			if (buffPos > 254) {
				clearBuffer();
			}
		}

	}
	
}

void makeCall() {

	message = 2;
	activeCall = 1;	

	Serial.print("ATD");				//Prefix
	
	for (int x = 0 ; x < 10 ; x++) {	//Stream the phone #	
		Serial.write(numberCall[x]);	
	}
	
	Serial.println(";");				//Ending character to start call

}

void getKeys() {

	for (int column = 0 ; column < 3 ; column++ ) {
	
		digitalWrite(colPins[0], 1);			//Set all high
		digitalWrite(colPins[1], 1);
		digitalWrite(colPins[2], 1);
	
		digitalWrite(colPins[column], 0);		//Then set active column LOW

		for (int row = 0 ; row < 5 ; row++) {

			if (digitalRead(rowPins[row]) == 0) {	//Is that switch pressed to active low?
				if (debounce[column][row] == 0) {	//Debounce cleared?
					key = hexaKeys[row][column];
					debounce[column][row] = 2000;
				}
				else {
					debounce[column][row] -= 1;		//If button pressed, count down debounce for repeat
				}
				
			}
			else {
				debounce[column][row] = 0;			//If button released, zero out debounce
			}

				
		}
	}

}

void drawDisplay() {

	display.clearDisplay();
	display.setTextColor(WHITE);

	//Draw the current message
	display.setTextSize(0);	
	display.setCursor(0, 2);
	//display.print(battery);	
	//display.print(" ");
	//display.print(signal);	
	display.print(messageString[message]);

	//Draw the signal bars
	
	int horizontal = 96;
	
	if (signal) {
		for (int x = 1 ; x < (signal + 1) ; x++) {
			display.drawLine(horizontal + x, 8, horizontal + x, 8 - (x * 2), WHITE);
			display.drawLine(horizontal + 1 + x, 8, horizontal + 1 + x, 8 - (x * 2), WHITE);
			horizontal += 3;
		}	
	}
	else {
		display.setTextSize(0);	
		display.setCursor(96, 1);
		display.print("X");		
	}

	//Draw the battery
	display.drawLine(114, 0, 126, 0, WHITE);
	display.drawLine(114, 0, 114, 8, WHITE);
	display.drawLine(114, 8, 126, 8, WHITE);	
	display.drawLine(126, 8, 126, 0, WHITE);
	display.drawLine(127, 2, 127, 6, WHITE);	
	
	for (int x = 0 ; x < (battery + 1) ; x++) {
		display.drawLine(115 + x, 1, 115 + x, 7, WHITE);
	}
		
	display.drawLine(0, 15, 127, 15, WHITE);
	
	display.setTextSize(2);	
	display.setCursor(4, 18);
	
	for (int x = 0 ; x < 10 ; x++) {
		display.write(numberCall[x]);
	}

	display.display();  

}

void interpret() {

	int length = Serial.available();

	for (int x = 0 ; x < (length + 1) ; x++) {		//Put incoming line on the buffer	
		input[x] = Serial.read();	
	}

	int flag = 255;
	
	for (int x = 0 ; x < (length + 1) ; x++) {		//Look for a semicolon
	
		//Serial.write(input[x]);		
		if (input[x] == ':') {
			//Serial.println("SEMICOLON FOUND!");
			flag = x;								//Mark the position			
		}	
	}

	if (flag == 255) {								//Didn't find it? Abort.
		//Serial.println("NO COMMAND FOUND");
		return;	
	}
	
	flag -= 3;
	
	if (flag < 0) {
		return;
	}

	if (input[flag] == 'C' and input[flag + 1] == 'B' and input[flag + 2] == 'C') {	//Battery report?
	
		//Serial.print("Battery Set to ");
		
		if (input[flag + 8] == ',') {	//Single digit
			battery = (input[flag + 7] - 48);
		}
		if (input[flag + 9] == ',') {	//Double digits
			battery = ((input[flag + 7] - 48) * 10) + (input[flag + 8] - 48);
		}		
		if (input[flag + 10] == ',') {	//Triple digits
			battery = 100;
		}		

		//Serial.print(battery);
		battery /= 10;
		drawDisplay();
	
	}

	if (input[flag] == 'C' and input[flag + 1] == 'S' and input[flag + 2] == 'Q') {	//Signal report?
	
		//Serial.print("Signal Set to ");
		
		if (input[flag + 6] == ',') {	//Single digit? Charge that sucker man!
			signal = (input[flag + 5] - 48); 			
		}
		else {
			signal = ((input[flag + 5] - 48) * 10) + (input[flag + 6] - 48); 		
		}

		//Serial.print(signal);

		signal /= 6;
		
		if (signal > 4) {
			signal = 4;
		}
		
		drawDisplay();
	
	}

}

void interpretBuffer() {

	int flag = 1;
	
	while(flag) {
	
		buffPos -= 1;
		
		if (input[buffPos] == ':') {	//Go backwards and find the colon
			flag = 0;
			//Serial.println("COLON FOUND!");
		}
		if (buffPos == 0 and flag == 1) {
			flag = 0;					//Didn't find it? Abort
			clearBuffer();
			return;
		}
	
	}

	flag = buffPos - 3;					//Look for the code CDC or CSQ

	if (input[flag] == 'C' and input[flag + 1] == 'B' and input[flag + 2] == 'C') {	//Battery report?
	
		//Serial.print("Battery Set to ");
		
		if (input[flag + 8] == ',') {	//Single digit
			battery = (input[flag + 7] - 48);
		}
		if (input[flag + 9] == ',') {	//Double digits
			battery = ((input[flag + 7] - 48) * 10) + (input[flag + 8] - 48);
		}		
		if (input[flag + 10] == ',') {	//Triple digits
			battery = 100;
		}		

		//Serial.print(battery);
		battery /= 10;
		drawDisplay();
	
	}

	if (input[flag] == 'C' and input[flag + 1] == 'S' and input[flag + 2] == 'Q') {	//Signal report?
	
		//Serial.print("Signal Set to ");
		
		if (input[flag + 6] == ',') {	//Single digit? Charge that sucker man!
			signal = (input[flag + 5] - 48); 			
		}
		else {
			signal = ((input[flag + 5] - 48) * 10) + (input[flag + 6] - 48); 		
		}

		//Serial.print(signal);

		signal /= 6;
		
		if (signal > 4) {
			signal = 4;
		}
		
		drawDisplay();
	
	}
	
	clearBuffer();

}

void clearBuffer() {

	for (int x = 0 ; x < 255 ; x++) {
		input[x] = 32;
	}
	
	buffPos = 0;

}

