#include <Adafruit_NeoPixel.h>
#include "WiFly.h"
#include "SPI.h"
#include "PubSubClient.h"
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_GRB + NEO_KHZ800);

// Variables

// MQTT Broker
//byte server[] = { 146, 185, 174, 52 }; // replace with the IP address of your broker - swap . for ,
//byte server[] = { 75, 101, 161, 236 }; // replace with the IP address of your broker - swap . for ,
char* serverDNS = "box.bento.is";
int port = 80; //default port is 1883. To help get around network restrictions,you may need broker set as port 80
char subscribedChannel[] = "public/cai-fyp";
char deviceName[] = "cai"; // set a unique name for each device connected to the broker
int brightnessControl;

String payloadString;
WiFlyClient wifiClient;

PubSubClient client(serverDNS, port, callback, wifiClient);

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int brightness = 127;
boolean brightnessBool = false;
int r = 180;
int g = 145;
int b = 4;
int speed = 3;


void setup() {
  Serial.begin(9600);
  // Connect to Wifi
//  connectToWifly();
  //Start Neopixel ring.
  strip.begin();
  setColor(255,255,255,127);
}

void loop() {
  Serial.println(brightness);
  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
   //check and maintain the connection to the broker
//  if(!client.connected()){
//     Serial.println("Disconnected");
//     connectToBroker();
//  }else{
//    client.loop();  
//  }
       glowingBrightness();
//       turnOff();
}

void setColor(int r, int g, int b, int brightness)
{
  for(int i = 0; i<=16; i++)
  {
    strip.setPixelColor(i, r, g, b);
    strip.setBrightness(brightness);
    strip.show();
  }


}

// Broker Functions
void convertPayload(byte array[], byte len){
  payloadString = "";
  for (byte i = 0; i < len;){
    char c = array[i++];
    if (c < 0x20 || c > 0x7e){
     Serial.print('.');
     payloadString += '.';
    }
    else {
      payloadString += char(c);
    }
  }
  
}

//triggered when a message is recieved on a subscribed channel
void callback(char* topic, byte* payload, unsigned int length) {
 // Serial.println("Message Recieved");
  
  // handle message
  
  //check the topic - use this is you want to sunbscribe to more than one channel
  if (String(topic) == String(subscribedChannel)){
//    Serial.println(topic); 
    //convert the payload to a string, then print it out
    convertPayload(payload, length);
    brightnessControl = payloadString.toInt();
    Serial.println("Motion: " + payloadString);
  
    Serial.println(brightnessControl);
    manualSetBrightness(brightnessControl);
  }
  
}

int mapBrightness(int b)
{
//  b = b * 4.5;
  return b;
}

void manualSetBrightness(int socketValue)
{
    int mappedB = mapBrightness(socketValue);
    Serial.println(mappedB);
    setColor(r,g,b,mappedB);
}

void glowingBrightness()
{
  if(brightnessBool == false)
   {
     if(brightness >250)
     {
       brightnessBool = true;
     }
     else
     {
       brightness = brightness + speed;
     }
   }
   
   else if(brightnessBool == true)
   {
     if(brightness <= 20)
     {
       brightnessBool = false;
     }
     else
     {
       brightness = brightness - speed;
     }
   }
         Serial.println(brightness);
     setColor(r,g,b,brightness);
}

//void connectToBroker(){
//  //connect to the broker
//    if (client.connect(deviceName)) {
//      Serial.println("Connecting");
//      //send a test message
//      
////      client.publish("cai-fy","Jumping and flipping");
//      //subscribe to a channel
//      client.subscribe(subscribedChannel);
//      Serial.println("Connected");
//    } else{
//      Serial.println("Connection Error");
//    }
//  
//}

// Wifi Function
//
//void connectToWifly()
//{
//  //connect to network
//  WiFly.begin();
//  if (!WiFly.join("rupsje", "rupsje1002")) {
//  //if (!WiFly.join("ssid")) { // use id it doesn't
//    Serial.println("Association failed.");
//    while (1) {
//      // Hang on failure.
//    }
//  }else{
//    Serial.println("Joining");
//    Serial.println(WiFly.ip());
//  }
//}

// Debugging Functions

void ringOn()
{
    setColor(255,255,255,255);
}

void turnOff()
{
    r = 0;
    g = 0;
    b = 0;
}
