/**
 *  Keyboard controller for 11 Column, 8 Row Keyboard
 *  Configured for Bondwell B-200 KB VER 2.0
 *  That is the keyboard of the Highscreen/Bondwell B200 XP Laptop (german Layout)
 * 
 *  Copyright 2019 by Jan-Hendrik Caspers (mail[at]jh-caspers.de)
 *
 *  MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
 *  associated documentation files (the "Software"), to deal in the Software without restriction, 
 *  including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *  and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or substantial 
 *  portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
 *  LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *  
 * */
#include <Keyboard.h>

unsigned int pinColumn; // currently scanned column
unsigned int pinRow;    // currently scanned row
unsigned int rowPins[] = {18,19,20,21,22,23,11,12}; // This pins are connnected to the Keyboard rows and will send an high pulse
unsigned int columnPins[] = {0,1,2,3,4,5,6,7,8,9,10}; // This pins read from the Columns of the Keyboard, actively pulled low externally!

unsigned int keyState[8][11] = {  {0,0,0,0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0,0,0,0},
                                 }; // initialize all Keys not pressed, (0=not pressed, 1=pressed, 2=debounce pressed, 3=debounce released)
// Scancodes from https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
// special chars have to be the us key 
char keyCodes[8][11] = {  //'Ctrl','s','a','g','f','j','h','k','l','ö','d'
                          {0x80,'s','a','g','f','j','h','k','l',';','d'},
                          //'Tab','w','q','t','r','u','z','i','o','p','e'
                          {0xB3,'w','q','t','r','u','y','i','o','p','e'},
                          //'F1','F3','F2','F6','F5','F8','F7','F9','F10','Esc','F4'
                          {0xC2,0xC4,0xC3,0xC7,0xC6,0xC9,0xC8,0xCA,0xCB,0xB1,0xC5},
                          //'1','3','2','6','5','8','7','9','0','ß','4'
                          {'1','3','2','6','5','8','7','9','0','-','4'},
                          //'Shift (l)','x','y','b','v','m','n',',','.','-','c'
                          {0x81,'x','z','b','v','m','n',',','.','/','c'},
                          //'','','','Scroll Lock -->Break','NumLock','Ins','','PgDn','Enter','ä','Caps Lock'
                          {0x00,0x00,0x00,0xD0,0xDB,0xD1,0x00,0xD6,0xB0,'\'',0xC1},
                          //'Alt','Space','<','Down','Left','Del','Right','End','Up','Shift ( r)','#'
                          {0x82,' ',0xEC,0xD9,0xD8,0xD4,0xD7,0xD5,0xDA,0x85,'\\'},
                          //'',''','','PrtSc','Home','Fn','','PgUp','+','ü','BackSpace'
                          {0x00,'=',0x00,0xCE,0xD2,0x86,0x00,0xD3,']','[',0xB2},
                        };

void setup() {
 
    // set Column to input
   for(pinColumn=0; pinColumn<11;pinColumn++) pinMode(columnPins[pinColumn], INPUT);
   // set LED to output
   pinMode(13, OUTPUT);
   // set Rows to Output
   for(pinRow=0; pinRow<8;pinRow++) pinMode(rowPins[pinRow], OUTPUT);
    // wait 5 sec before starting Keyboard
   digitalWrite(13, HIGH); // LED ON
   delay(5000);           // wait 5 sec before activating Keyboard
   digitalWrite(13, LOW); // LED OFF
   Keyboard.begin();
}



void loop() {
  // Scan all rows
  for(pinRow=0; pinRow<8;pinRow++) {
   digitalWrite(rowPins[pinRow], HIGH);
   delay(1); // just some delay to let the lines stabilize
   for(pinColumn=0; pinColumn<11;pinColumn++) { // read all columns
      if (digitalRead(columnPins[pinColumn]) == HIGH) { 
        if (keyState[pinRow][pinColumn]!=1) { // if column is high, check if switch was not pressed before
          if (keyState[pinRow][pinColumn]==2) { // if in debounce send the keycode
            Keyboard.press(keyCodes[pinRow][pinColumn]);
            keyState[pinRow][pinColumn]=1;
          } else keyState[pinRow][pinColumn]=2; // otherwise set debounce for 1 cycle
        }
      } else { // if key is depressed/input low
        if (keyState[pinRow][pinColumn]!=0) { // check if it was not depressed before
          if (keyState[pinRow][pinColumn]==3) { // if in debounce, release key
            Keyboard.release(keyCodes[pinRow][pinColumn]);
            keyState[pinRow][pinColumn]=0;
          } else keyState[pinRow][pinColumn]=3; // otherwise set debounce for 1 cycle
        }
      }
   }
   digitalWrite(rowPins[pinRow], LOW);
   delay(1);
  }
}
