#include <Adafruit_NeoPixel.h>
#include "SPI.h"
#include "WiFly.h"
#include "PubSubClient.h"
#include <process.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);


//broker settings
//byte server[] = { 146, 185, 174, 52 }; // replace with the IP address of your broker - swap . for ,
char* serverDNS = "box.bento.is";
int port = 80; //default port is 1883. To help get around network restrictions,you may need broker set as port 80
boolean capturedStatus = false;
char subscribedChannel[] = "public/cai-fyp/status";
char deviceName[] = "cai"; // set a unique name for each device connected to the broker
   const int buttonPin = 4;     // the number of the pushbutton pin
   int buttonState = 0;         // variable for reading the pushbutton status
   int oldState;
   
//   Pull Chord Code
int pin = 2;
volatile int state = LOW;
boolean pullyState = false;
boolean pullyPreviousState = false;
int pullyValue;
   
//Process p;
//used to decode the message payload
String payloadString;
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
    Serial.println(payloadString);
    if(payloadString == "captured")
    {
        capturedStatus = true;
        strip.begin();
        setColor(255,255,255,255,0);
    }
    else if(payloadString == "saved")
    {
       capturedStatus = false;
    }
}
}

WiFlyClient wifiClient;
PubSubClient client(serverDNS, port, callback, wifiClient);

void connectToBroker(){
  //connect to the broker
    if (client.connect(deviceName)) {
      Serial.println("Connecting");
      //send a test message
      
      //client.publish("ezticken","Jumping and flipping");
      //subscribe to a channel
      client.subscribe(subscribedChannel);
      Serial.println("Connected");
    } else{
      Serial.println("Connection Error");
    }
  
}

void setup() {
  
  Serial.begin(115200);
  Serial.println("setup");
  
  //connect to network
  
  WiFly.begin();
  if (!WiFly.join("rupsje", "rupsje1002")) {
  //if (!WiFly.join("ssid")) { // use id it doesn't
    Serial.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }else{
    Serial.println("Joining");
    Serial.println(WiFly.ip());
  }
   pinMode(buttonPin, INPUT); 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
}

void loop() {  

    // read the input on analog pin 0:
      int sensorValue = digitalRead(5);
  if(sensorValue ==1)
  {
    cTouch();
  }
  
  //check and maintain the connection to the broker
  if(!client.connected()){
     Serial.println("Disconnected");
     connectToBroker();
  }else{
    client.loop();
  }
  
  buttonState = digitalRead(buttonPin);
  // wait for 30 milliseconds to see the dimming effect    
  
  if(oldState != buttonState && buttonState == 1)
  {
    if(capturedStatus == true)
    {
      Serial.println("Saved");
      client.publish("public/cai-fyp/status", "saved");
      capturedStatus = false;
    }
    else if(capturedStatus == false)
    {
      Serial.println("Could not save");
      Serial.println(capturedStatus);
    }
    else
    {
      Serial.println("Error, not working");
    }
  }
  oldState = buttonState;

  //  Pulley stuff
  pullyValue = analogRead(A5);
  if(pullyValue > 500 && pullyValue <520)
  {
  pullyState = true;
//  Serial.println(pullyState);
//  Serial.println(pullyPreviousState);
  }
  else
  { 
    pullyState = false;
//     Serial.println(pullyState);
//  Serial.println(pullyPreviousState);
  }
  
    if(pullyState != pullyPreviousState)
  {
    Serial.println("CHANGE");
  }
  
  pullyPreviousState = pullyState;
      delay(50);        // delay in between reads for stability 
}


// Neopixel functions
void setColor(int r, int g, int b, int brightness, int setDelay)
{
  for(int i = 0; i<=16; i++)
  {
    strip.setPixelColor(i, r, g, b);
    strip.setBrightness(brightness);
    strip.show();
    delay(setDelay);
  }
}

void cTouch()
{
  setColor(0,255,0,255,30);
}


