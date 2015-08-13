#include <EEPROM.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(A5, A4, A0, A3, A1, A2);

#define reset	5
#define clock	6
#define row0	10
#define row1	11
#define row2	12

byte keyboard[10][3] = {		//Column/Row mapping of the keys
  {'E', 'D', 'C'},
  {'W', 'S', 'X'},
  {'R', 'F', 'V'},
  {'U', 'J', 'M'},
  {'O', 'L', ' '},
  {'Q', 'A', 'Z'},
  {'T', 'G', 'B'},
  {'Y', 'H', 'N'}, 
  {'I', 'K', ' '},
  {'P', 8, 13}   
};

int debounce[10][3];

byte state = 0;					//What our keyboard is doing. State 0 = typing
byte temp = 0;

int cursorTimer = 0;
byte inputBuffer[34];			//What is coming in
byte inputBufferPos = 0;		//Where we are on the input buffer
byte messageFlag = 0;			//Flag that a message is incoming
int memWhich = 0;				//Which message we are viewing (0 - memTop)
int memTop = 0;				//What message in memory is the newest (0 - 30)
byte screenBuffer[32];
byte bufferPos = 0;				//Where we are in the buffer
byte lastKey = 0;				//What was pressed (for menus)

void setup() {

	lcd.begin(16, 2);
	clearScreen();
	lcd.setCursor(0, 0);
	lcd.print("-TEXTING  RADIO-");
	lcd.setCursor(0, 1);	
 	lcd.print("MESSAGES: ");
	
	Serial.begin(19200);

	pinMode(reset, OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(row0, INPUT);
	pinMode(row1, INPUT);
	pinMode(row2, INPUT);

	memTop = EEPROM.read(1023);
	lcd.print(memTop);	
	
	delay(1000);
	
}

void loop() {

	switch (state) {				
		case 0:
			getKeys();			
			drawScreen();
			cursorTimer += 1;
			
			if (cursorTimer == 50) {
				screenBuffer[bufferPos] = ' ';	
			}
			if (cursorTimer == 100) {
				screenBuffer[bufferPos] = '_';
				cursorTimer = 0;
			}				
			break;
			
		case 1:
			getKeys();
			drawScreen();
			if (lastKey == 'Q') {
				lastKey = 0;
				if (memWhich > 0) {
					memWhich -= 1;
					lcd.clear();
					lcd.print("MESSAGE #");
					lcd.print(memWhich);
					delay(300);
					getMessage(memWhich);				
				}
			}
			if (lastKey == 'Z') {
				lastKey = 0;
				if (memWhich < (memTop - 1)) {
					memWhich += 1;
					lcd.clear();
					lcd.print("MESSAGE #");
					lcd.print(memWhich);
					delay(300);
					getMessage(memWhich);				
				}
			}	
			if (lastKey == 13) {
				lastKey = 0;
				clearScreen();
				state = 0;
			}
			if (lastKey == 'P') {
				lastKey = 0;
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("Y = CLEAR ALL");
				lcd.setCursor(0, 1);
				lcd.print("N = CANCEL  ");
				state = 2;
			}				
			break;
		case 2:
			getKeys();
			if (lastKey == 'Y') {
				lastKey = 0;
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("ERASING EEPROM..");											
				clearAll();
				lcd.setCursor(0, 1);	
				lcd.print("DONE!");					
				memWhich = 0;
				state = 0;						//Straight to text entry!
				delay(500);
			}	
			if (lastKey == 'N') {
				lastKey = 0;
				lcd.clear();
				lcd.print("MESSAGE #");
				lcd.print(memWhich);
				delay(300);
				getMessage(memWhich);				
				state = 1;
			}	
			break;				
	}

	if (Serial.available()) {		//Data came in?
		checkSerialIn();
	}
	
}

void checkSerialIn() {

	temp = Serial.read();
	
	if (temp == '[') {
		inputBufferPos = 0;
		messageFlag = 1;
		return;
	}
	if (temp == ']') {
		messageFlag = 0;
		getData();
		return;
	}
	
	if (messageFlag == 1) {
		inputBuffer[inputBufferPos++] = temp;
		
		if (inputBufferPos > 31) {
			inputBufferPos = 31;
		}
	}
}

void getKeys() {

	digitalWrite(reset, 1);		//Reset the column counter
	digitalWrite(reset, 0);

	for (int column = 0 ; column < 10 ; column++ ) {
	
		if (debounce[column][0]) {
			debounce[column][0] -= 1;
		}	
		if (digitalRead(row0)) {
			if (debounce[column][0] == 0) {		//Something pressed on that row?
				type(keyboard[column][0]);
				lastKey = keyboard[column][0];
				debounce[column][0] = 30;
			}
		}
		else {
			debounce[column][0] = 0;
		}

		if (debounce[column][1]) {
			debounce[column][1] -= 1;
		}		
		if (digitalRead(row1)) {
			if (debounce[column][1] == 0) {		//Something pressed on that row?
				type(keyboard[column][1]);
				lastKey = keyboard[column][1];
				debounce[column][1] = 30;
			}
		}
		else {
			debounce[column][1] = 0;
		}
		
		if (debounce[column][2]) {
			debounce[column][2] -= 1;
		}		
		if (digitalRead(row2)) {
			if (debounce[column][2] == 0) {		//Something pressed on that row?
				type(keyboard[column][2]);
				lastKey = keyboard[column][2];
				debounce[column][2] = 30;
			}
		}
		else {
			debounce[column][2] = 0;
		}

		digitalWrite(clock, 1);		//Advance column #
		digitalWrite(clock, 0);	
		
	}
}

void type(byte whichChar) {

	if (state != 0) {
		return;
	}

	if (whichChar == 13) {	//Return?
		sendData();
		return;
	}

	if (whichChar == 8) {	//Backspace?
	
		if (bufferPos == 0) {
			if (memTop > 0) {
				state = 1;							//Set to READ state
				memWhich = memTop - 1;				//Show last message we got
				lcd.clear();
				lcd.print("MESSAGE #");
				lcd.print(memWhich);
				delay(300);
				getMessage(memWhich);
				lastKey = 0;
			}
			return;
		}
		else {
			screenBuffer[bufferPos ] = ' ';		//Blank out cursor position
			bufferPos -= 1;
			cursorTimer = 99;					//So it shows up right away
		}		
		return;
	}
	
	screenBuffer[bufferPos] = whichChar;
	
	bufferPos += 1;
	
	if (bufferPos == 32) {
		bufferPos = 31;
	}

	screenBuffer[bufferPos] = '_';	

}

void drawScreen() {

	lcd.setCursor(0, 0);
	
	for (int x = 0 ; x < 16 ; x++) {
		lcd.write(screenBuffer[x]);
	}
	
	lcd.setCursor(0, 1);
	
	for (int x = 0 ; x < 16 ; x++) {
		lcd.write(screenBuffer[x + 16]);
	}
}

void clearScreen() {

	for (int x = 0 ; x < 32 ; x++) {
		screenBuffer[x] = 32;
	}	
	bufferPos = 0;
	cursorTimer = 99;
	
}

void sendData() {

	Serial.write('[');						//Courtesy, the end of line club!
	
	for (int x = 0 ; x < bufferPos ; x++) {
		Serial.write(screenBuffer[x]);
	}
		
	Serial.write(']');						//Courtesy, the end of line club!
	
	for (int x = 0 ; x < 32 ; x++) {
		lcd.setCursor(x / 2, 0);
		lcd.write('>');
		lcd.setCursor(x / 2, 1);
		lcd.write('>');
		delay(10);
	}	

	for (int x = 0 ; x < 32 ; x++) {
		lcd.setCursor(x / 2, 0);
		lcd.write(' ');
		lcd.setCursor(x / 2, 1);
		lcd.write(' ');
		delay(10);
	}	
	
	delay(250);
	
	clearScreen();
	
}

void getData() {

	if (state != 1) {
		lcd.setCursor(0, 1);
		lcd.print("INCOMING MESSAGE");	
	}

	for (int x = 0 ; x < inputBufferPos ; x++) {		//Copy to EEPROM
	
		EEPROM.write((memTop * 32) + x, inputBuffer[x]);
		inputBuffer[x] = 0;
			
	}

	inputBufferPos = 0;

	memTop += 1;							//Increase pointer
	
	if (memTop > 30) {
		memTop = 0;
	}
	
	EEPROM.write(1023, memTop);

	delay(250);
	
	if (state == 1) {						//Reading messages when this came in?
	
		memWhich = memTop - 1;
		lcd.clear();
		lcd.print("NEW MESSAGE #");
		lcd.print(memWhich);
		delay(300);
		getMessage(memWhich);	
	
	}
	
}

void getMessage(int whichMessage) {

	whichMessage *= 32;
	
	for (int x = 0 ; x < 32 ; x++) {
	
		screenBuffer[x] = EEPROM.read(whichMessage + x);
	
	}	

}

void clearAll() {

	memTop = 0;

	while (memTop < 31) {
		EEPROM.write((memTop * 32) + 0, '<');
		for (int x = 1 ; x < 31 ; x++) {		
			EEPROM.write((memTop * 32) + x, 32);
			screenBuffer[x] = inputBuffer[x];
		}
		EEPROM.write((memTop * 32) + 31, '>');		
		memTop += 1;
	}
		
	memTop = 0;
	EEPROM.write(1023, memTop);

	clearScreen();
	
}



