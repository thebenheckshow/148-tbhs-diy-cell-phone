 
This project is to make a phone with a cellular phone communicating via an Adafruit FONA driven by an Atmel microcontroller.

Here is a discussion threads:
http://forums.adafruit.com/viewtopic.php?f=22&t=57948&p=292924#p292924
http://forums.adafruit.com/viewtopic.php?f=19&t=57997&p=293202#p293202.

..
├── Arduino
│   ├── Libraries
│   │   ├── Adafruit_FONA
│   │   │   ├── Adafruit_FONA.cpp
│   │   │   ├── Adafruit_FONA.h
│   │   │   ├── examples
│   │   │   │   └── FONAtest
│   │   │   │       └── FONAtest.ino
│   │   │   └── README.md
│   │   ├── Adafruit_FONA_Library-master.zip
│   │   ├── Adafruit_GFX
│   │   │   ├── Adafruit_GFX.cpp
│   │   │   ├── Adafruit_GFX.h
│   │   │   ├── glcdfont.c
│   │   │   ├── license.txt
│   │   │   └── README.txt
│   │   ├── Adafruit-GFX-Library-master.zip
│   │   ├── Adafruit_ILI9341
│   │   │   ├── Adafruit_ILI9341.cpp
│   │   │   ├── Adafruit_ILI9341.h
│   │   │   ├── examples
│   │   │   │   ├── breakouttouchpaint
│   │   │   │   │   └── breakouttouchpaint.ino
│   │   │   │   ├── graphicstest
│   │   │   │   │   └── graphicstest.ino
│   │   │   │   ├── onoffbutton
│   │   │   │   │   └── onoffbutton.ino
│   │   │   │   ├── onoffbutton_breakout
│   │   │   │   │   └── onoffbutton_breakout.ino
│   │   │   │   ├── spitftbitmap
│   │   │   │   │   └── spitftbitmap.ino
│   │   │   │   └── touchpaint
│   │   │   │       └── touchpaint.ino
│   │   │   └── README.txt
│   │   ├── Adafruit_ILI9341-master.zip
│   │   ├── Keypad
│   │   │   ├── examples
│   │   │   │   ├── CustomKeypad
│   │   │   │   │   └── CustomKeypad.ino
│   │   │   │   ├── DynamicKeypad
│   │   │   │   │   └── DynamicKeypad.ino
│   │   │   │   ├── EventKeypad
│   │   │   │   │   └── EventKeypad.ino
│   │   │   │   ├── HelloKeypad
│   │   │   │   │   └── HelloKeypad.ino
│   │   │   │   ├── HelloKeypad3
│   │   │   │   │   └── HelloKeypad3.ino
│   │   │   │   ├── loopCounter
│   │   │   │   │   └── loopCounter.ino
│   │   │   │   └── MultiKey
│   │   │   │       └── MultiKey.ino
│   │   │   ├── Keypad.cpp
│   │   │   ├── Keypad.h
│   │   │   ├── keywords.txt
│   │   │   └── utility
│   │   │       ├── Key.cpp
│   │   │       └── Key.h
│   │   ├── keypad.zip
│   │   ├── TouchScreen
│   │   │   ├── examples
│   │   │   │   ├── touchscreendemo
│   │   │   │   │   └── touchscreendemo.pde
│   │   │   │   └── touchscreendemoshield
│   │   │   │       └── touchscreendemoshield.ino
│   │   │   ├── README.txt
│   │   │   ├── TouchScreen.cpp
│   │   │   └── TouchScreen.h
│   │   └── Touch-Screen-Library-master.zip
│   ├── Programming
│   │   ├── Development
│   │   │   └── ArduinoPhone
│   │   │       └── ArduinoPhone.ino
│   │   ├── Reference
│   │   │   ├── radio_code
│   │   │   │   └── radio_code.ino
│   │   │   └── ShopCam_Programming
│   │   │       ├── I2C
│   │   │       │   ├── I2C_Test_0
│   │   │       │   │   └── I2C_Test_0.ino
│   │   │       │   └── I2C_Test_1
│   │   │       │       └── I2C_Test_1.ino
│   │   │       ├── IR
│   │   │       │   ├── Get_IR
│   │   │       │   │   └── Get_IR.ino
│   │   │       │   └── ircommander
│   │   │       │       ├── ircodes.h
│   │   │       │       └── ircommander.ino
│   │   │       └── Servo
│   │   │           ├── Servo_Control_0
│   │   │           │   └── Servo_Control.ino
│   │   │           ├── Servo_Control_1
│   │   │           │   └── Servo_Control_1.ino
│   │   │           └── Servo_Control_2
│   │   │               ├── ircodes.h
│   │   │               └── Servo_Control_2.ino
│   │   └── Test
│   │       ├── FONAtest
│   │       │   ├── FONAtest_00
│   │       │   │   └── FONAtest_00.ino
│   │       │   └── FONAtest_01
│   │       │       └── FONAtest_01.ino
│   │       ├── keypadtest
│   │       │   ├── keypad
│   │       │   │   └── keypad.ino
│   │       │   ├── MatrixTest
│   │       │   │   └── MatrixTest.ino
│   │       │   └── MultiKey
│   │       │       └── MultiKey.ino
│   │       └── ScreenTest
│   │           ├── graphicstest
│   │           │   └── graphicstest.ino
│   │           └── touchpaint
│   │               └── touchpaint.ino
│   ├── Schematic
│   │   └── Matrix
│   │       ├── Atmel_FONA_TFT_Button-Matrix_Schematic_01.png
│   │       ├── Atmel_FONA_TFT_Button-Matrix_Schematic_02.png
│   │       ├── Atmel_FONA_TFT_Button-Matrix_Schematic_03.png
│   │       ├── Atmel_FONA_TFT_Button-Matrix_Schematic_04.png
│   │       ├── Atmel_FONA_TFT_Button-Matrix_Schematic_05.png
│   │       ├── Atmel_FONA_TFT_Button-Matrix_Schematic.png
│   │       ├── Keypad.brd
│   │       ├── Keypad.s#1
│   │       ├── Keypad.s#2
│   │       ├── Keypad.s#3
│   │       ├── Keypad.s#4
│   │       ├── Keypad.s#5
│   │       ├── Keypad.s#6
│   │       ├── Keypad.s#7
│   │       ├── Keypad.s#8
│   │       └── Keypad.sch
│   └── Tools
│       ├── Compiler
│       │   ├── avr-gnu-toolchain
│       │   │   ├── avr32
│       │   │   │   ├── avr32-gnu-toolchain-3.4.2.435-linux.any.x86_64.tar.gz
│       │   │   │   ├── avr32-gnu-toolchain-3.4.2.435-linux.any.x86.tar.gz
│       │   │   │   └── avr32-gnu-toolchain-3.4.2.435-readme.pdf
│       │   │   ├── avr8
│       │   │   │   ├── avr8-gnu-toolchain-3.4.4.1229-linux.any.x86.tar.gz
│       │   │   │   └── avr8-gnu-toolchain-3.4.4.1229-readme.pdf
│       │   │   └── headers
│       │   │       ├── atmel-headers-6.1.3.1475.zip
│       │   │       └── avr8-headers-6.2.0.334.zip
│       │   └── source
│       │       ├── avr32
│       │       │   ├── Atmel-AVR-Toolchain-3.4.2.html
│       │       │   ├── avr32-binutils-2.23.1.tar.gz
│       │       │   ├── avr32-gcc-4.4.7.tar.gz
│       │       │   ├── avr32-newlib-1.16.0.tar.gz
│       │       │   ├── avr32-patches.tar.gz
│       │       │   ├── build-avr32-gnu-toolchain.sh
│       │       │   └── SOURCES.README
│       │       ├── avr8_4.7
│       │       │   ├── avr-binutils-2.23.1.tar.gz
│       │       │   ├── avr-gcc-4.7.2.tar.gz
│       │       │   ├── avr-headers-6.1.3.1475.zip
│       │       │   ├── avr-libc-1.8.0.tar.gz
│       │       │   ├── avr-patches.tar.gz
│       │       │   ├── build-avr8-gnu-toolchain.sh
│       │       │   └── SOURCES.README
│       │       └── avr8_4.8
│       │           ├── avr8-gnu-toolchain-3.4.4.1229-readme.pdf
│       │           ├── avr8-headers-6.2.0.334.zip
│       │           ├── avr-binutils-2.24.tar.bz2
│       │           ├── avr-gcc-4.8.1.tar.bz2
│       │           ├── avr-gdb-7.7.tar.bz2
│       │           ├── avr-libc-1.8.0.tar.bz2
│       │           ├── build-avr8-gnu-toolchain-git.sh
│       │           └── SOURCES.README
│       └── Serial
│           ├── Serial_Port_Test
│           │   ├── sscom32E.exe
│           │   └── sscom.ini
│           └── Serial_Port_Test.zip
├── Documentation
│   └── README
└── RaspberryPi
    └── Notes
        └── PiPhoneEvolution.pdf

72 directories, 108 files
