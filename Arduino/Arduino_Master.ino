#include <Adafruit_NeoPixel.h>
#include "SPI.h"
#include "PubSubClient.h"
#include <Ethernet.h>
#include <process.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
IPAddress ip(192,168,0,177);

//broker settings
//byte server[] = { 146, 185, 174, 52 }; // replace with the IP address of your broker - swap . for ,
char* serverDNS = "box.bento.is";
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int port = 80; //default port is 1883. To help get around network restrictions,you may need broker set as port 80
boolean capturedStatus = false;
char subscribedChannel[] = "public/cai-fyp/status";
String str = "";
// Scrape stuff
char server[] = "www.hivenodes.com";  
boolean firstScrape = false;
int scrapeCount = 0;
const char* theval;

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
   
//   Neopixel
int brightness = 10;
boolean brightnessBool = false;
int r = 0;
int g = 102;
int b = 204;

int numberOfItems;
EthernetClient ethClient;
PubSubClient client(serverDNS, port, callback, ethClient);
//Process p;

// Setup
void setup() {
  
  Serial.begin(9600);
  Serial.println("setup");
  
  //connect to network

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  else
  {
    Serial.println("Connected to the ethernet");
    if (ethClient.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    ethClient.println("GET /~hivenode/fyp/count.php?node=1");
    ethClient.println("Host: www.hivenodes.com");
    ethClient.println("Connection: close");
    ethClient.println();
    }
    
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
  pinMode(buttonPin, INPUT); 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
//  glowingBrightness();
  
}

// Loop

void loop() {   
  
          if(!firstScrape)
          {     
          if (ethClient.available()) {
          char c = ethClient.read();
          str += c;
          }

          // if the server's disconnected, stop the client:
          if (!ethClient.connected()) {
          Serial.println();
          Serial.println("disconnecting.");
          firstScrape = true;
            int findFirstBracket = (str.indexOf('[')) + 1;
            int findSecondBracket = str.indexOf(']');
            String cutString = (str.substring(findFirstBracket, findSecondBracket));
            numberOfItems =  cutString.toInt();
            Serial.println("Number of items: ");
            Serial.println(numberOfItems);
          }
          } 
          else {
            mainLoop();
          }
}


void mainLoop()
{
  
  if(capturedStatus)
  {
    setColor(255,255,255,250,0);
  }
  else if(numberOfItems != 0)
  {    
  glowingBrightness();
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
      client.publish("public/cai-fyp/node-1/status", "saved");
      capturedStatus = false;
      cTouch(0,255,0);
      numberOfItems = numberOfItems + 1;
    }
    else if(capturedStatus == false)
    {
      Serial.println("Could not save");
      cTouch(255,0,0);
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
//  Serial.println(pullyValue);
  if(pullyValue > 100 && pullyValue <520)
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
      cTouch(0,0,0);
      numberOfItems = 0;
      client.publish("public/cai-fyp/print", "print-node-1");
  }
  
      pullyPreviousState = pullyState;
      Serial.println(pullyValue);
      delay(50);        // delay in between reads for stability
}


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
        setColor(255,255,255,250,20);
    }
    else if(payloadString == "saved")
    {
       capturedStatus = false;
    }    
}
}

void connectToBroker(){
  //connect to the broker
    if (client.connect(deviceName)) {
      Serial.println("Connecting");
      client.subscribe(subscribedChannel);
      Serial.println("Connected");
    } else{
      Serial.println("Connection Error");
    }
  
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

void glowingBrightness()
{
  if(brightnessBool == false)
   {
     if(brightness >=200)
     {
       brightnessBool = true;
     }
     else
     {
       brightness = brightness + (numberOfItems * 3); // Speed is the number of links * appropriate number
     }
   }
   
   else if(brightnessBool == true)
   {
     if(brightness <= 30)
     {
       brightnessBool = false;
     }
     else
     {
       brightness = brightness - (numberOfItems * 3);
     }
   }
   
     setColor(r,g,b,brightness, 0);
}

void cTouch(int r, int g, int b)
{
  setColor(r,g,b,200,30);
  Serial.println("c touch");
//  delay(1000);
}

