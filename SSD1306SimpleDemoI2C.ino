/**
   The MIT License (MIT)

   Copyright (c) 2016 by Daniel Eichhorn

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

*/

// For a connection via I2C using Wire include
//#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 4, 5);   //display(I2C address, SDA, SCL)
// Wemos D1 + SD1306 display(I2C address, SDA = D2, SCL = D1)
// ESP-12 + SD1306 display(I2C address, SDA = GPIO4, SCL = GPIO5)

void setup() {
  //  Serial.begin(115200);   //for debug by terminal
  //  Serial.println();   //for debug by terminal
  //  Serial.println("Debug");   //for debug by terminal

  display.init();  // Initialising the UI will init the display too.
  display.flipScreenVertically();   // up side down or straight
  display.setFont(ArialMT_Plain_10);  //default font. Create more fonts at http://oleddisplay.squix.ch/

}

void loop() {
  String MyTekst = "Hello world";
  display.clear();    // clear the display

  display.setFont(ArialMT_Plain_24);    // change font
  display.drawString(0, 26, MyTekst);   // draw string (column(0-127), row(0-63, "string")

  display.display();  //display all you have in display memory

  delay(10);  // give some time to finish function loops
}
