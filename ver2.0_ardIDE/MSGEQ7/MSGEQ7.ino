
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Arduino.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <Adafruit_NeoPixel.h>
#include "definitions.h"

#include "classes.h"
octave myOctaves[7];

#include "externalFunc.h"
RGB colorTMP(0, 0, 0);



void setup() {

  Serial.begin(115200);
  pinMode(STROBE_PIN , OUTPUT);
  pinMode(RESET_PIN , OUTPUT);
  digitalWrite(RESET_PIN , LOW);
  digitalWrite(STROBE_PIN , HIGH);
  DEBUGGING("INIT");

  strip.begin();
  strip.show();
  
  WifiConnect();
  WebSocketConnect();
  MDNSConnect();
  HTTPUpdateConnect();

  for (int i = 0; i < 7; i++) myOctaves[i].octaveInit(i);
  RGB color1(0, 255, 0);
  RGB color2(255, 255, 0);
  RGB color3(255, 0, 0);
  RGB color4(0, 0, 255);
  for (int i = 0; i < 7; i++) myOctaves[i].changeColorScheme(1, color1, color2, color3, 0.5, 1);
  for (int i = 0; i < 7; i++) myOctaves[i].changeColorScheme(3, color3, color2, color3, 0.5, 1);

}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WifiConnect();
    WebSocketConnect();
    MDNSConnect();
    HTTPUpdateConnect();
  }
  else {

    webSocket.loop();
    refreshSpectrum();
    delay(0);

    if (millis() - lastTimeHost > 10) {
      httpServer.handleClient();
      lastTimeHost = millis();
    }
    
    if (rainbowFlag && millis() - timePassed  >  rainbowSpeed) {

        for (int i = 0; i < 7; i++) {
          myOctaves[i].changeColorScheme(4, colorTMP, colorTMP, colorTMP, 0.0, 0.0);        
          delay(0);
        }
        timePassed = millis();    
    }

  }


}





