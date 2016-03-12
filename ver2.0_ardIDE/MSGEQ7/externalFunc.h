void refreshSpectrum() {
  digitalWrite(RESET_PIN , HIGH);
  digitalWrite(RESET_PIN , LOW);

  for (int i = 0; i < 7  ; i++)  {  // Change it to reverse
    myOctaves[6-i].refreshBuffer();
    digitalWrite(STROBE_PIN , LOW);
    myOctaves[6-i].readVal();
    digitalWrite(STROBE_PIN , HIGH);
    delay(0);
  }
  delay(numOfDelay);
  // DEBUGGING("");

  for (int i = 0; i < 7  ; i++)  {
    myOctaves[i].CalcLedsVal();
    myOctaves[i].drawLeds();
    delay(0);
  }
  strip.show();
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      WSClients[num] = false;
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        DEBUGGING(ip);
        WSClients[num] = true;
      }
      break;

    case WStype_TEXT:
      {
        String text = String((char *) &payload[0]);
        String chs = text.substring(1, 2);
        RGB clr1, clr2, clr3;
        double var1, var2;
        String tmp;
        if (text.startsWith("1")) {
          rainbowFlag = false;
          clr1.getColorFromString(text.substring(2, 11));
          clr2.getColorFromString(text.substring(11, 20));
          clr3.getColorFromString(text.substring(20, 29));
          tmp = text.substring(29, 32);
          var1 = ((double)tmp.toInt()) / 100;
          tmp = text.substring(32, 35);
          var2 = ((double)tmp.toInt()) / 100;
          if (var2 < var1) var2 = var1;
          if (chs == "A") {
            for (int i = 0; i < 7; i++) {
              myOctaves[i].changeColorScheme(1, clr1, clr2, clr3, var1, var2);
              delay(0);
            }
          }
          else {
            int idx = chs.toInt() - 1;
            myOctaves[idx].changeColorScheme(1, clr1, clr2, clr3, var1, var2);
          }
        }
        if (text.startsWith("2")) {
          rainbowFlag = false;
          clr1.getColorFromString(text.substring(2, 11));
          clr2.getColorFromString(text.substring(11, 20));
          clr3.getColorFromString(text.substring(20, 29));
          tmp = text.substring(29, 32);
          var1 = ((double)tmp.toInt()) / 100;
          tmp = text.substring(32, 35);
          var2 = ((double)tmp.toInt()) / 100;
          if (var2 < var1) var2 = var1;
          if (chs == "A") {
            for (int i = 0; i < 7; i++) {
              myOctaves[i].changeColorScheme(2, clr1, clr2, clr3, var1, var2);
              delay(0);
            }
          }
          else {
            int idx = chs.toInt() - 1;
            myOctaves[idx].changeColorScheme(2, clr1, clr2, clr3, var1, var2);
          }
        }
        if (text.startsWith("3")) {
          rainbowFlag = false;
          clr1.getColorFromString(text.substring(2, 11));
          if (chs == "A") {
            for (int i = 0; i < 7; i++) {
              myOctaves[i].changeColorScheme(3, clr1, clr1, clr1, 0, 0);
              delay(0);
            }
          }
          else {
            int idx = chs.toInt() - 1;
            myOctaves[idx].changeColorScheme(3, clr1, clr1, clr1, 0, 0);
          }
        }
        if (text.startsWith("4")) { //rainbow
          tmp = text.substring(29, 32);
          var1 = ((double)tmp.toInt()); //speed  
          rainbowFlag = true;
          rainbowSpeed = var1;
        }
        if (text.startsWith("5")) { // config
          tmp = text.substring(29, 32);
          var1 = ((double)tmp.toInt()); //numOfHistory
          tmp = text.substring(32, 35);
          var2 = ((double)tmp.toInt()); //numOfDelay
          numOfHistory = var1;
          numOfDelay = var2;
          }
        if (text.startsWith("6")) { // config noise level
           tmp = text.substring(29, 32);
          var1 = ((double)tmp.toInt()); //noise level out of 1024
          if (chs == "A") {
            for (int i = 0; i < 7; i++) {
              myOctaves[i].changeColorScheme(6, clr1, clr1, clr1, var1, 0.0);
              delay(0);
            }
          }
          else {
            int idx = chs.toInt() - 1;
            myOctaves[idx].changeColorScheme(6, clr1, clr1, clr1, var1, 0.0);
          }
        }
        webSocket.broadcastTXT(payload, lenght);
        /*for(uint8_t i = 0; i < 10; i++) {
                if (WSClients[i]) {
                    webSocket.broadcastTXT(payload, lenght);
                    delay(0);
                }
                
        }*/
      }
      break;

    case WStype_BIN:
      //hexdump(payload, lenght);
      // echo data back to browser
      //webSocket.sendBIN(num, payload, lenght);
      break;
  }
}



// Wifi Connection
void WifiConnect() {

  // WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  DEBUGGING("Connected");
  DEBUGGING(WiFi.localIP());

}

// WebSocket Connection
void WebSocketConnect() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

// MDNS
void MDNSConnect() {
  if (!MDNS.begin(host)) {
    DEBUGGING("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  DEBUGGING("mDNS responder started");
  MDNS.addService("ws", "tcp", 81);
  MDNS.addService("http", "tcp", 80);
}

// HTTP updater connection
void HTTPUpdateConnect() {
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  DEBUGGING_L("HTTPUpdateServer ready! Open http://");
  DEBUGGING_L(host);
  DEBUGGING(".local/update in your browser\n");
}

