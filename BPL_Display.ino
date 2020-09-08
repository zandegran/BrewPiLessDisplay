/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 by Ansel Zandegran
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

// Include the correct display library
// For a connection via I2C using Wire include

#include <ESP8266WiFi.h>
//#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
using namespace websockets;

WebsocketsClient client;
StaticJsonDocument<300> doc;
// or #include "SH1106Wire.h", legacy include: `#include "SH1106.h"`
// For a connection via I2C using brzo_i2c (must be installed) include
// #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// #include "SH1106Brzo.h"
// For a connection via SPI include
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// #include "SH1106SPi.h"

// Include custom images
#include "images.h"

///// WiFi SETTINGS - Replace with your values /////////////////
const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";
IPAddress ip(192,168,0,221);      // set a fixed IP for the NodeMCU
IPAddress gateway(192,168,0,1);  // Your router IP
IPAddress subnet(255,255,255,0); // Subnet mask
////////////////////////////////////////////////////////////////////

const char* websockets_server = "ws://192.168.0.220/ws"; // Address of BrewPiLesss

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(D0, D2);

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D1, D2);
// SH1106 display(0x3c, D3, D5);


#define DURATION 3000
typedef void (*Screen)(void);

int currentScreen = 0;
int counter = 1;

float frs = 0;
float frt = 0;
float bs = 0;
float bt = 0;
int sig = 0;
const char* md = "o";
int st = 0;
const char* messageText = "";


void WiFiStart() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
//  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("_");
  }
  Serial.println();
  Serial.println("Done");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
                   
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFiStart();
}

void setValues() {
  frs = doc["fs"];
  frs /= 100;
  frt = doc["ft"];
  frt /= 100;
  bs = doc["bs"];
  bs /= 100;
  bt = doc["bt"];
  bt /= 100;
  sig = doc["rssi"];
  sig *= -1;
  md = doc["md"];
  md = sizeof(md) > 0 ? md : "o";
  st = doc["st"];
  messageText = doc["sl"];
}

void onMessageCallback(WebsocketsMessage message) {
    Serial.println(message.data());
    if (sizeof(message.data()) > 0 && message.data()[0] == 'A')  {
      String input = message.data().substring(2);
      DeserializationError error = deserializeJson(doc, input);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        messageText = error.c_str();
      }
      else {
        setValues();
      }
    }
}

void setupWebsockets() {
  // Setup Callbacks
  client.onMessage(onMessageCallback);   
  // Connect to server
  client.connect(websockets_server);
}

void setup() {
  Serial.begin(115200);
  setupWifi();
  Serial.println();
  Serial.println();
  setupWebsockets();

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

}

void drawStats() {
    // create more fonts at http://oleddisplay.squix.ch/
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0,  String("    : ") + (bt < -9 ? String("--") : (String(bt) + String(" º"))));
    display.drawXbm(0, 0, beer_width, beer_height, beer_bits);
    display.drawString(0, 16, String("    : ") + (frt < -9 ? String("--") : (String(frt) + String(" º"))));
    display.drawXbm(2, 16, fridge_width, fridge_height, fridge_bits);
    display.setFont(ArialMT_Plain_10);
    String mode = "OFF";   
    if (strcmp(md, "f") == 0) {
      mode = String("Target fridge: ") + String(frs) + String(" º");
    }
    if (strcmp(md, "b") == 0) {
      mode = String("Target beer: ") + String(bs) + String(" º");
    }
    if (strcmp(md, "p") == 0) {
      mode = String("Beer profile mode");
    }
    display.drawString(0, 38, mode);
    display.drawString(0, 52, messageText);
    drawGraphics();
}

void drawGraphics() {
  int height = 25;
  int width = 4;
  int xpos = 120;
  int ypos = 0;
  int offset = 7;
  if (sig >= 60)
  display.fillRect(xpos, ypos, width, height);
  if (sig >= 50)
  display.fillRect(xpos - offset, ypos, width, height);
  if (sig >= 40)
  display.fillRect(xpos - offset * 2, ypos, width, height);
  if (st != 0)
  display.fillCircle(115, 50, 6);
}


void drawLogo() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    display.drawXbm(30, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

//Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
Screen screens[] = {drawStats, drawLogo};
int seqLength = (sizeof(screens) / sizeof(Screen));
long timeSinceLastModeSwitch = 0;

void loop() {
  // Poll websockets
  client.poll();
  // clear the display
  display.clear();
  // draw the current demo method
  screens[currentScreen]();

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(10, 128, String(millis()));
  // write the buffer to the display
  display.display();

  if (millis() - timeSinceLastModeSwitch > DURATION) {
    currentScreen = (currentScreen + 1)  % seqLength;
    timeSinceLastModeSwitch = millis();
  }
  counter++;
  delay(10);
  //Reconnect on lost WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    WiFiStart();
  }
}
