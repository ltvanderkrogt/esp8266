#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// pin config nodemcu
// scl = D1
// sda = D2 

// pin config esp8266 01
// scl = GPIO2
// sda = GPIO0 

// pin config esp8266 ESP-12
// scl = GPIO5
// sda = GPIO4 

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!");
    lcd.setCursor(0,1);
   lcd.print("Who's there?");
}

void loop()
{
  // Do nothing here...
}
