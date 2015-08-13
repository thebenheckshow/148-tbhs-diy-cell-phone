/* @
|| @version 1.0
|| @author 
|| @contact 
||
|| @description
|| | 
|| #
*/
//#include <Adafruit_FONA.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>

// For the Adafruit shield, these are the default.
#define TFT_CS 10
#define TFT_DC A0

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// These are the four touchscreen analog pins
#define YP A2  // must be an analog pin, use "An" notation!
#define XP A3  // must be an analog pin, use "An" notation!
#define YM A4   // can be a digital pin
#define XM A5   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Define pins for Fona.
#define FONA_RX 0
#define FONA_TX 1
// #define FONA_RESET 10
// #define FONA_KEY 11
// #define FONA_POWERSTATUS // < not connected >
// #define FONA_NETWORKSTATUS // < not connected >
// #define FONA_RINGINDICATOR // < not connected >


const byte ROWS = 5; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
  {'A','B','C'},
};
byte rowPins[ROWS] = {5, 6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  //Serial.begin(9600);
  //Serial.begin(115200);
  
  tft.begin();
  
  tft.fillScreen(ILI9341_BLACK);
  
   // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  tft.print("Display Power Mode: 0x "); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  tft.print("\nMADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  tft.print("\nPixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  tft.print("\nImage Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  tft.print("\nSelf Diagnostic: 0x"); Serial.println(x, HEX); 
  
 
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    switch (customKey) {				
		case 'A':
                        Serial.println(F("Benchmark                Time (microseconds)"));
                        Serial.print(F("Screen fill              "));
                        Serial.println(testFillScreen());
                        delay(500);
			break;
		case 'B':
                        tft.fillScreen(ILI9341_BLACK);
                        tft.setCursor(0, 0);				
			break;
		case 'C':
                        Serial.println(F("Benchmark                Time (microseconds)"));
                        Serial.print(F("Text                     "));
                        Serial.println(testText());
                        delay(3000);	
			break;
                default:
                        tft.print(customKey);	
                        break;			
	}
  }
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  tft.fillScreen(ILI9341_RED);
  tft.fillScreen(ILI9341_GREEN);
  tft.fillScreen(ILI9341_BLUE);
  tft.fillScreen(ILI9341_BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

