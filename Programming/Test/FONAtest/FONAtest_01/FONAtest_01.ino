/*************************************************** 
  This is an example for our Adafruit FONA Cellular Module

  Designed specifically to work with the Adafruit FONA 
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/* 
THIS CODE IS STILL IN PROGRESS!

Open up the serial console on the Arduino at 115200 baud to interact with FONA
*/

//#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include <Adafruit_ILI9341.h>

#define FONA_RX A1//2
#define FONA_TX A2//3
#define FONA_RST A3//4

// For the Adafruit shield, these are the default.
#define TFT_CS 10
#define TFT_DC A0

// this is a large buffer for replies
char replybuffer[255];

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(&fonaSS, FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

void setup() {
  //tft.begin(115200);
  tft.begin();
  tft.println(F("FONA basic test"));
  tft.println(F("Initializing....(May take 3 seconds)"));
 
  
  // See if the FONA is responding
  if (! fona.begin(4800)) {  // make it slow so its easy to read!
    tft.println(F("Couldn't find FONA"));
    while (1);
  }
  tft.println(F("FONA is OK"));

  printMenu();
}

void printMenu(void) {
   tft.println(F("-------------------------------------"));
   tft.println(F("[?] Print this menu"));
   tft.println(F("[a] read the ADC (2.8V max)"));
   tft.println(F("[b] read the Battery V"));
   tft.println(F("[C] read the SIM CCID"));
   tft.println(F("[U] Unlock SIM with PIN code"));
   tft.println(F("[i] read RSSI"));
   tft.println(F("[n] get Network status"));
   tft.println(F("[v] set audio Volume"));
   tft.println(F("[V] get Volume"));
   tft.println(F("[H] set Headphone audio"));
   tft.println(F("[e] set External audio"));
   tft.println(F("[T] play audio Tone"));
   tft.println(F("[f] tune FM radio"));
   tft.println(F("[F] turn off FM"));
   tft.println(F("[m] set FM volume"));
   tft.println(F("[M] get FM volume"));
   tft.println(F("[q] get FM station signal level"));
   tft.println(F("[P] PWM/Buzzer out"));
   tft.println(F("[c] make phone Call"));
   tft.println(F("[h] Hang up phone"));
   tft.println(F("[N] Number of SMSs"));
   tft.println(F("[r] Read SMS #"));
   tft.println(F("[R] Read All SMS"));
   tft.println(F("[d] Delete SMS #"));
   tft.println(F("[s] Send SMS"));
   tft.println(F("[G] Enable GPRS"));
   tft.println(F("[g] Disable GPRS"));
   tft.println(F("[l] Query GSMLOC (GPRS)"));
   tft.println(F("[w] Read webpage (GPRS)"));
   tft.println(F("[S] create Serial passthru tunnel"));
   tft.println(F("-------------------------------------"));
   tft.println(F(""));
  
}
void loop() {
  tft.print(F("FONA> "));
  while (! tft.available() );
  
  char command = tft.read();
  tft.println(command);
  
  
  switch (command) {
    case '?': {
      printMenu();
      break;
    }
    
    case 'a': {
      // read the ADC
      uint16_t adc;
      if (! fona.getADCVoltage(&adc)) {
        tft.println(F("Failed to read ADC"));
      } else {
        tft.print(F("ADC = ")); tft.print(adc); tft.println(F(" mV"));
      }
      break;
    }
    
    case 'b': {
        // read the battery voltage
        uint16_t vbat;
        if (! fona.getBattVoltage(&vbat)) {
          tft.println(F("Failed to read Batt"));
        } else {
          tft.print(F("VBat = ")); tft.print(vbat); tft.println(F(" mV"));
        }
        break;
    }

    case 'U': {
        // Unlock the SIM with a PIN code
        char PIN[5];
        flushSerial();
        tft.println(F("Enter 4-digit PIN"));
        readline(PIN, 3);
        tft.println(PIN);
        tft.print(F("Unlocking SIM card: "));
        if (! fona.unlockSIM(PIN)) {
          tft.println(F("Failed"));
        } else {
          tft.println(F("OK!"));
        }        
        break;
    }

    case 'C': {
        // read the CCID
        fona.getSIMCCID(replybuffer);  // make sure replybuffer is at least 21 bytes!
        tft.print(F("SIM CCID = ")); tft.println(replybuffer);
        break;
    }

    case 'i': {
        // read the RSSI
        uint8_t n = fona.getRSSI();
        int8_t r;
        
        tft.print(F("RSSI = ")); tft.print(n); tft.print(": ");
        if (n == 0) r = -115;
        if (n == 1) r = -111;
        if (n == 31) r = -52;
        if ((n >= 2) && (n <= 30)) {
          r = map(n, 2, 30, -110, -54);
        }
        tft.print(r); tft.println(F(" dBm"));
       
        break;
    }
    
    case 'n': {
        // read the network/cellular status
        uint8_t n = fona.getNetworkStatus();
        tft.print(F("Network status ")); 
        tft.print(n);
        tft.print(F(": "));
        if (n == 0) tft.println(F("Not registered"));
        if (n == 1) tft.println(F("Registered (home)"));
        if (n == 2) tft.println(F("Not registered (searching)"));
        if (n == 3) tft.println(F("Denied"));
        if (n == 4) tft.println(F("Unknown"));
        if (n == 5) tft.println(F("Registered roaming"));
        break;
    }
    
    /*** Audio ***/
    case 'v': {
      // set volume
      flushSerial();
      tft.print(F("Set Vol %"));
      uint8_t vol = readnumber();
      tft.println();
      if (! fona.setVolume(vol)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("OK!"));
      }
      break;
    }

    case 'V': {
      uint8_t v = fona.getVolume();
      tft.print(v); tft.println("%");
    
      break; 
    }
    
    case 'H': {
      // Set Headphone output
      if (! fona.setAudio(FONA_HEADSETAUDIO)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("OK!"));
      }
      fona.setMicVolume(FONA_HEADSETAUDIO, 15);
      break;
    }
    case 'e': {
      // Set External output
      if (! fona.setAudio(FONA_EXTAUDIO)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("OK!"));
      }

      fona.setMicVolume(FONA_EXTAUDIO, 10);
      break;
    }

    case 'T': {
      // play tone
      flushSerial();
      tft.print(F("Play tone #"));
      uint8_t kittone = readnumber();
      tft.println();
      // play for 1 second (1000 ms)
      if (! fona.playToolkitTone(kittone, 1000)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("OK!"));
      }
      break;
    }
    
    /*** FM Radio ***/
    
    case 'f': {
      // get freq
      flushSerial();
      tft.print(F("FM Freq (eg 1011 == 101.1 MHz): "));
      uint16_t station = readnumber();
      tft.println();
      // FM radio ON using headset
      if (fona.FMradio(true, FONA_HEADSETAUDIO)) {
        tft.println(F("Opened"));
      }
     if (! fona.tuneFMradio(station)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("Tuned"));
      }
      break;
    }
    case 'F': {
      // FM radio off
      if (! fona.FMradio(false)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("OK!"));
      }
      break;
    }
    case 'm': {
      // Set FM volume.
      flushSerial();
      tft.print(F("Set FM Vol [0-6]:"));
      uint8_t vol = readnumber();
      tft.println();
      if (!fona.setFMVolume(vol)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("OK!"));
      }
      break;
    }
    case 'M': {
      // Get FM volume.
      uint8_t fmvol = fona.getFMVolume();
      if (fmvol < 0) {
        tft.println(F("Failed"));
      } else {
        tft.print(F("FM volume: "));
        tft.println(fmvol, DEC);
      }
      break;
    }
    case 'q': {
      // Get FM station signal level (in decibels).
      flushSerial();
      tft.print(F("FM Freq (eg 1011 == 101.1 MHz): "));
      uint16_t station = readnumber();
      tft.println();
      int8_t level = fona.getFMSignalLevel(station);
      if (level < 0) {
        tft.println(F("Failed! Make sure FM radio is on (tuned to station)."));
      } else {
        tft.print(F("Signal level (dB): "));
        tft.println(level, DEC);
      }
      break;
    }
    
    /*** PWM ***/
    
    case 'P': {
      // PWM Buzzer output @ 2KHz max
      flushSerial();
      tft.print(F("PWM Freq, 0 = Off, (1-2000): "));
      uint16_t freq= readnumber();
      tft.println();
      if (! fona.PWM(freq)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("OK!"));
      }
      break;
    }

    /*** Call ***/
    case 'c': {      
      // call a phone!
      char number[30];
      flushSerial();
      tft.print(F("Call #"));
      readline(number, 30);
      tft.println();
      tft.print(F("Calling ")); tft.println(number);
      if (!fona.callPhone(number)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("Sent!"));
      }
      
      break;
    }
    case 'h': {
       // hang up! 
      if (! fona.hangUp()) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("OK!"));
      }
      break;     
    }
    
    /*** SMS ***/
    
    case 'N': {
        // read the number of SMS's!
        int8_t smsnum = fona.getNumSMS();
        if (smsnum < 0) {
          tft.println(F("Could not read # SMS"));
        } else {
          tft.print(smsnum); 
          tft.println(F(" SMS's on SIM card!"));
        }
        break;
    }
    case 'r': {
      // read an SMS
      flushSerial();
      tft.print(F("Read #"));
      uint8_t smsn = readnumber();
      
      tft.print(F("\n\rReading SMS #")); tft.println(smsn);
      uint16_t smslen;
      if (! fona.readSMS(smsn, replybuffer, 250, &smslen)) { // pass in buffer and max len!
        tft.println("Failed!");
        break;
      }
      tft.print(F("***** SMS #")); tft.print(smsn); 
      tft.print(" ("); tft.print(smslen); tft.println(F(") bytes *****"));
      tft.println(replybuffer);
      tft.println(F("*****"));
      
      break;
    }
    case 'R': {
      // read all SMS
      int8_t smsnum = fona.getNumSMS();
      uint16_t smslen;
      for (int8_t smsn=1; smsn<=smsnum; smsn++) {
        tft.print(F("\n\rReading SMS #")); tft.println(smsn);
        if (!fona.readSMS(smsn, replybuffer, 250, &smslen)) {  // pass in buffer and max len!
           tft.println(F("Failed!"));
           break;
        }
        // if the length is zero, its a special case where the index number is higher
        // so increase the max we'll look at!
        if (smslen == 0) {
          tft.println(F("[empty slot]"));
          smsnum++;
          continue;
        }
        
        tft.print(F("***** SMS #")); tft.print(smsn); 
        tft.print(" ("); tft.print(smslen); tft.println(F(") bytes *****"));
        tft.println(replybuffer);
        tft.println(F("*****"));
      }
      break;
    }

    case 'd': {
      // delete an SMS
      flushSerial();
      tft.print(F("Delete #"));
      uint8_t smsn = readnumber();
      
      tft.print(F("\n\rDeleting SMS #")); tft.println(smsn);
      if (fona.deleteSMS(smsn)) {
        tft.println(F("OK!"));
      } else {
        tft.println(F("Couldn't delete"));
      }
      break;
    }
    
    case 's': {
      // send an SMS!
      char sendto[21], message[141];
      flushSerial();
      tft.print(F("Send to #"));
      readline(sendto, 20);
      tft.println(sendto);
      tft.print(F("Type out one-line message (140 char): "));
      readline(message, 140);
      tft.println(message);
      if (!fona.sendSMS(sendto, message)) {
        tft.println(F("Failed"));
      } else {
        tft.println(F("Sent!"));
      }
      
      break;
    }
    /*********************************** GPRS */
    
    case 'g': {
       // turn GPRS off
       if (!fona.enableGPRS(false))  
         tft.println(F("Failed to turn off"));
       break;
    }
    case 'G': {
       // turn GPRS on
       if (!fona.enableGPRS(true))  
         tft.println(F("Failed to turn on"));
       break;
    }
    case 'l': {
       // check for GSMLOC (requires GPRS)
       uint16_t returncode;
       
       if (!fona.getGSMLoc(&returncode, replybuffer, 250))
         tft.println(F("Failed!"));
       if (returncode == 0) {
         tft.println(replybuffer);
       } else {
         tft.print(F("Fail code #")); tft.println(returncode);
       }
       
       break;
    }
    case 'w': {
      // read website URL
      uint16_t statuscode;
      int16_t length;
      char url[80];
      
      flushSerial();
      tft.println(F("NOTE: in beta! Use small webpages to read!"));
      tft.println(F("URL to read (e.g. www.adafruit.com/testwifi/index.html):"));
      tft.print(F("http://")); readline(url, 79);
      tft.println(url);
      
       tft.println(F("****"));
       if (!fona.HTTP_GET_start(url, &statuscode, (uint16_t *)&length)) {
         tft.println("Failed!");
         break;
       }
       while (length > 0) {
         while (fona.available()) {
           char c = fona.read();
           
           // tft.write is too slow, we'll write directly to Serial register!
           loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
           UDR0 = c;
           
           length--;
           if (! length) break;
         }
       }
       tft.println(F("\n****"));
       fona.HTTP_GET_end();
       break;
    }
    
    
    /*****************************************/
      
    case 'S': {
      tft.println(F("Creating SERIAL TUBE"));
      while (1) {
        while (tft.available()) {
          fona.write(tft.read());
        }
        if (fona.available()) {
          tft.write(fona.read());
        }
      }
      break;
    }
    
    default: {
      tft.println(F("Unknown command"));
      printMenu();
      break;
    }
  }
  // flush input
  flushSerial();
  while (fona.available()) {
    tft.write(fona.read());
  }

}

void flushSerial() {
    while (tft.available()) 
    tft.read();
}

char readBlocking() {
  while (!tft.available());
  return tft.read();
}
uint16_t readnumber() {
  uint16_t x = 0;
  char c;
  while (! isdigit(c = readBlocking())) {
    //tft.print(c);
  }
  tft.print(c);
  x = c - '0';
  while (isdigit(c = readBlocking())) {
    tft.print(c);
    x *= 10;
    x += c - '0';
  }
  return x;
}
  
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
  uint16_t buffidx = 0;
  boolean timeoutvalid = true;
  if (timeout == 0) timeoutvalid = false;
  
  while (true) {
    if (buffidx > maxbuff) {
      //tft.println(F("SPACE"));
      break;
    }

    while(tft.available()) {
      char c =  tft.read();

      //tft.print(c, HEX); tft.print("#"); tft.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0)   // the first 0x0A is ignored
          continue;
        
        timeout = 0;         // the second 0x0A is the end of the line
        timeoutvalid = true;
        break;
      }
      buff[buffidx] = c;
      buffidx++;
    }
    
    if (timeoutvalid && timeout == 0) {
      //tft.println(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}
