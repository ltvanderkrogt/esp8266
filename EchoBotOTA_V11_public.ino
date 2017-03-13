/*******************************************************************
    this is a basic example how to program a Telegram Bot
    using TelegramBOT library on ESP8266
	
 *                                                                 *
    Open a conversation with the bot, you can command via Telegram
    a led from ESP8266 GPIO
 *                                                                 *
    written by Giancarlo Bacchio
    modified by Leon van der Krogt (OTA, group commands and security) 
	Over the Air programming: http://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html
 *******************************************************************/
/*Pin(board)  Function  	WEMOS ESP8266 correspondence
  TX   TXD       				TXD
  RX   RXD       				RXD
  A0   Analog input, max 3.3V 	input  A0
  D0  IO  						GPIO16
  D1  IO, SCL 					GPIO5
  D2  IO, SDA 					GPIO4
  D3  IO, 10k 					Pull-up GPIO0
  D4  IO, 10k Pull-up, 			BUILTIN_LED  GPIO2  // do not use this as output, esp won't start at power on!
  D5  IO, SCK 					GPIO14
  D6  IO, MISO  				GPIO12
  D7  IO, MOSI  				GPIO13
  D8  IO, 10k Pull-down, SS 	GPIO15
  G   GND 						GND
  5V  5V  						5V
  3V3 3.3V  					3.3V
  RST Reset 					RST
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>   // https://github.com/Gianbacchio/ESP8266-TelegramBot

int Blue = 14;
int Red = 13;
int Green = 12;
int Switch = 16;

// mac 1a1a7f Gateway_Breskens
// mac 0dcdeb Webcam_Breskens

// Initialize Wifi connection to the router
char ssid[] = "****************";             // your network SSID (name)
char pass[] = "****************";           // your network key

// ************************** Begin OTA **************************
// OTA Includes. Use IDE 1.6.7 or higher (https://github.com/esp8266/Arduino/blob/master/doc/ota_updates/readme.md)
#include <ESP8266mDNS.h>
#include <WiFiUdp.h> 
#include <ArduinoOTA.h>
// ************************** End OTA **************************

// ArduinoOTA.setPassword((const char *)"123"); // doesn't work? 

/*
Create an account in Telegram(mobile number) 
Create a new bot for your device. https://web.telegram.org/#/im?p=@BotFather 
  /newbot 

  Edit your bot settings and Cut&paste these commands in the Telegram account @BotFather; DO NOT USE CAPITALS!

  status - status
  aan - zet spanning aan
  20sec - 20 sec off
  groen_aan - groen led aan
  groen_uit - groen led uit
  uit - zet spanning uit
*/

// Initialize Telegram BOT
// mac 1a1a7f 


#define BOTtoken "**********:******************************"  //token of your bot
#define BOTname "**********"
#define BOTusername "**********"

TelegramBOT bot(BOTtoken, BOTname, BOTusername);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

String LedGreenStatus;
String PowerStatus;

/********************************************
   EchoMessages - function to Echo messages
 ********************************************/

void Bot_ExecMessages() {
  for (int i = 1; i < bot.message[0][0].toInt() + 1; i++)      {
    bot.message[i][5] = bot.message[i][5].substring(1, bot.message[i][5].length());

    if (bot.message[i][1] == bot.message[i][4])  { // Security: Group members only!
      String wellcome = "You are not authorised!";
      String wellcome1 = "If you think you should....";
      String wellcome2 = "@leonvanderkrogt";
      digitalWrite(Blue, HIGH);  // turn on LED
      delay(50);                      // wait split second
      digitalWrite(Blue, LOW);   // turn off LED

      bot.sendMessage(bot.message[i][4], wellcome, "");
      bot.sendMessage(bot.message[i][4], wellcome1, "");
      bot.sendMessage(bot.message[i][4], wellcome2, "");
      Start = true;

    }
    else {
      int findString = bot.message[i][5].indexOf('@');                //find @ in text
      String BotName = bot.message[i][5].substring(findString + 1); // use text before @ only (reply from group command is start@accountname_bot
      String FindString = String(findString); // int to String

      if (BotName == BOTusername) {

        bot.message[i][5] = bot.message[i][5].substring(0, findString); // use text before @ only (reply from group command is start@accountname_bot

        if (bot.message[i][5] == "groen_aan") {
          digitalWrite(Green, HIGH);   // turn the LED on
          LedGreenStatus = "groene LED is aan";
          bot.sendMessage(bot.message[i][4], "groen is aan", "");
        }
        if (bot.message[i][5] == "groen_uit") {
          digitalWrite(Green, LOW);   // turn the LED on
          LedGreenStatus = "groene LED is uit";
          bot.sendMessage(bot.message[i][4], "groene LED uit", "");
        }
        if (bot.message[i][5] == "aan") {
          digitalWrite(Switch, HIGH);   // turn the LED on
          digitalWrite(Red, LOW);   // turn the LED on
          digitalWrite(Green, HIGH);   // turn the LED on
          bot.sendMessage(bot.message[i][4], "is aan", "");
          delay(10000);
          digitalWrite(Green, LOW);   // turn the LED on
          LedGreenStatus = "groene LED is uit";

        }
        if (bot.message[i][5] == "uit") {
          digitalWrite(Switch, LOW);   // turn the LED on
          digitalWrite(Green, LOW);   // turn the LED on
          digitalWrite(Red, HIGH);   // turn the LED on
          PowerStatus = "stroom is ar af";
          bot.sendMessage(bot.message[i][4], "is uit", "");
        }
        if (bot.message[i][5] == "20sec") {
          bot.sendMessage(bot.message[i][4], "Okay, let me sleep...", "");
          digitalWrite(Blue, HIGH);  // turn on LED
          delay(100);                      // wait split second
          digitalWrite(Blue, LOW);   // turn off LED
          digitalWrite(Switch, LOW);   // turn the LED on
          for (int i = 0; i <= 10; i++) {
            digitalWrite(Green, LOW);   // turn the LED on
            digitalWrite(Red, HIGH);   // turn the LED on
            delay(1000);
            digitalWrite(Red, LOW);   // turn the LED on
            digitalWrite(Green, HIGH);   // turn the LED on
            delay(10000);

          }
          digitalWrite(Switch, HIGH);   // turn the LED on
          digitalWrite(Red, LOW);   // turn the LED on
          digitalWrite(Green, HIGH);   // turn the LED on

          PowerStatus = "stroom is er op";
          bot.sendMessage(bot.message[i][4], "Hello, I am back again!", "");
          delay(10000);
          digitalWrite(Green, LOW);   // turn the LED on
          LedGreenStatus = "groene LED is uit";
        }

        if (bot.message[i][5] == "status") {
          String wellcome = BotName;
          String wellcome1 = ssid;
          String wellcome2 = LedGreenStatus;
          String wellcome3 = PowerStatus;
          String wellcome4 = BOTusername;
          digitalWrite(Blue, HIGH);  // turn on LED
          delay(100);                      // wait split second
          digitalWrite(Blue, LOW);   // turn off LED

          bot.sendMessage(bot.message[i][4], wellcome, "");
          bot.sendMessage(bot.message[i][4], wellcome1, "");
          bot.sendMessage(bot.message[i][4], wellcome2, "");
          bot.sendMessage(bot.message[i][4], wellcome3, "");
          bot.sendMessage(bot.message[i][4], wellcome4, "");
          digitalWrite(Green, LOW);   // turn the LED on
          LedGreenStatus = "groene LED is uit";
        }
        if (bot.message[i][5] == "\start") {
          String wellcome = "Welkom, dit zijn de opdrachten die ik ken;";
          String wellcome1 = "/aan : spanning aan";
          String wellcome2 = "/uit : spanning uit ";
          String wellcome3 = "/groen_aan : schakel de groene LED aan";
          String wellcome4 = "/groen_uit : schakel de groene LED uit";
          String wellcome5 = "/20sec : schakel de spanning 20 seconden uit";
          String wellcome6 = "Bij aanschakelen gaat de spanning direct aan, wordt de WiFi verbonden en gaat na een minuut de groene LED uit.";
          //         String wellcome7 = WiFiStatus;
          for (int i = 0; i <= 3; i++) {
            digitalWrite(Blue, HIGH);  // turn on LED
            delay(100);                      // wait split second
            digitalWrite(Blue, LOW);   // turn off LED
          }
          bot.sendMessage(bot.message[i][4], wellcome, "");
          bot.sendMessage(bot.message[i][4], wellcome1, "");
          bot.sendMessage(bot.message[i][4], wellcome2, "");
          bot.sendMessage(bot.message[i][4], wellcome3, "");
          bot.sendMessage(bot.message[i][4], wellcome4, "");
          bot.sendMessage(bot.message[i][4], wellcome5, "");
          bot.sendMessage(bot.message[i][4], wellcome6, "");
          //          bot.sendMessage(bot.message[i][4], WiFiStatus, "");
          Start = true;
        }
      }
    }
    bot.message[0][0] = "";   // All messages have been replied - reset new messages
  }
}
void setup() {
  // ************************** Begin OTA **************************
  ArduinoOTA.begin();
  // ************************** End OTA **************************
  Serial.begin(115200);
  pinMode(Blue, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(Red, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(Green, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(Switch, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  //  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  // default
  digitalWrite(Switch, HIGH);   // turn the LED on
  //  digitalWrite(Red, LOW);   // turn the LED on
  digitalWrite(Green, HIGH);   // turn the LED on
  //  digitalWrite(Switch, LOW);   // turn the LED on
  PowerStatus = "stroom is er op";
  LedGreenStatus = "groene LED is aan";
  
  // Wait for connection
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); // client only, not visible as access point.
  WiFi.begin ( ssid, pass);
  Serial.println("Booting");

  while ( WiFi.status() != WL_CONNECTED ) {
    //   String WiFiStatus = WiFi.status();
    delay ( 100 );
    Serial.print(".");
    digitalWrite(Red, LOW);   	// turn the LED on
    digitalWrite(Green, LOW);   // turn the LED on
    digitalWrite(Blue, HIGH);   // turn on LED
    delay(500);                 // wait one second
    digitalWrite(Blue, LOW);    // turn off LED

  }
  digitalWrite(Green, HIGH);   // turn the LED on
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // ************************** Begin OTA **************************
  ArduinoOTA.handle();
  // ************************** End OTA **************************

  if (millis() > Bot_lasttime + Bot_mtbs)  {
    delay(50);
    bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
    delay(50);
    Bot_ExecMessages();   // reply to message with Echo
    Bot_lasttime = millis();
  }
}



