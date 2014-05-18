/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include "PubSubClient.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char* serverDNS = "box.bento.is";
String str = "";
int port = 80;
char subscribedChannel[] = "public/cai-fyp/status";
char deviceName[] = "cai"; // set a unique name for each device connected to the broker
boolean scrapeStatus = false;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.hivenodes.com";    // name address for Google (using DNS)
String payloadString;
   const int buttonPin = 7;     // the number of the pushbutton pin
   int buttonState = 0;         // variable for reading the pushbutton status
      int oldState;
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,177);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
PubSubClient wifiClient(serverDNS, port, callback, client);
void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }

  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:

}

void loop()
{
  if(!scrapeStatus){  
   if(!wifiClient.connected()){
//     Serial.println("Disconnected");
     connectToBroker();
  }else{
    wifiClient.loop();
  }
  }

  if (client.available()) {
  char c = client.read();
  str += c;
  Serial.print(c); 
   }

}


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

void httpRequest()
{
    if (client.connect(server, 80)) {
    Serial.println("connected to server for http request");
    // Make a HTTP request:
    client.println("GET /~hivenode/fyp/read.php?node=2");
    client.println("Host: www.hivenodes.com");
    client.println("Connection: close");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
}

void connectToBroker(){
  //connect to the broker
    if (wifiClient.connect(deviceName)) {
      Serial.println("Connecting to broker");
      wifiClient.subscribe(subscribedChannel);
      Serial.println("Connected to broker");
    } else{
      Serial.println("Connection Error");
    }
  
}

void callback(char* topic, byte* payload, unsigned int length) {
 // Serial.println("Message Recieved");
  
  // handle message
  
  //check the topic - use this is you want to sunbscribe to more than one channel
  if (String(topic) == String(subscribedChannel)){
//    Serial.println(topic); 
    //convert the payload to a string, then print it out
    convertPayload(payload, length);
    Serial.println(payloadString);
    if(payloadString == "print-node-1")
    {
    scrapeStatus = true;
    wifiClient.disconnect();
    httpRequest();
    Serial.println("I have been triggered");
    }
    }
}

