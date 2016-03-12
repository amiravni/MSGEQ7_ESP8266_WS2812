//#define DEBUGGING(...) Serial.println( __VA_ARGS__ )
//#define DEBUGGING_L(...) Serial.print( __VA_ARGS__ )

#ifndef DEBUGGING
#define DEBUGGING(...)
#endif
#ifndef DEBUGGING_L
#define DEBUGGING_L(...)
#endif

#define SSID_ME "MY_SSID"
#define PW_ME "MY_PW"
#define HOST_ME "esp8266EQ"
#define STROBE_PIN 12
#define RESET_PIN 2
#define LEDSPIN 4



WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
const char* host = HOST_ME;
const char* ssid     = SSID_ME;
const char* password = PW_ME;


const int numOfLeds = 252;
const int numOfLedsSingle = numOfLeds / 7;
const int bufferSize = 10;
int spectrumValue[bufferSize];
int octVal[7];
int ledsColor[numOfLeds * 3];
int topColor[7 * 3];
unsigned long timePassed = 0;
unsigned long lastTimeHost = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numOfLeds, LEDSPIN, NEO_GRB + NEO_KHZ800);

//changable integers
bool WSClients[10] = {false};
int minVals[7] = {200, 200, 200, 250, 200, 200, 100};
int numOfHistory = 10;
int numOfDelay = 10;
bool rainbowFlag = false;
int rainbowSpeed = 100;

