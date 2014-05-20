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
#include <Adafruit_Thermal.h>
#include "SoftwareSerial.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int printer_RX_Pin = 5;  // This is the green wire
int printer_TX_Pin = 6;  // This is the yellow wire
int itemEnd, itemStart;
int node;
Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);
int count = 1;
String str = "";
char* serverDNS = "box.bento.is";
int port = 80;
char subscribedChannel[] = "public/cai-fyp/status";
char deviceName[] = "printer"; // set a unique name for each device connected to the broker
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
   printer.begin();
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }

  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println(F("connecting..."));

  // if you get a connection, report back via serial:

}

void loop()
{
  //  if (!client.connected()) {
//    if(!stopScraping)
//    {
////      cutString();
//    }
//  }
  
  if(!scrapeStatus){  
   if(!wifiClient.connected()){
     connectToBroker();
  }else{
    wifiClient.loop();
//    Serial.println("loop");
  }
  }

  if (client.available()) {
    char c = client.read();
    str += c;
  Serial.print(c); 
  
  if(c == '~')
  {
  cutString(str); 
  }
  
  }
  
    delay(1);
//    Serial.println(freeRam());
}

void cutString(String str)
  {                          
          
    while(scrapeStatus)
    {
      
      if(count ==1)
      {
          printer.inverseOn();
          printer.println("Node " + String(node));
          printer.inverseOff();
      }
            
            Serial.println(freeRam());
            String instance = "[it" + String(count) + "]";
            itemStart = str.indexOf(instance);           
            itemEnd = str.indexOf("[/it]", itemStart);
            String stringCut = str.substring(itemStart,itemEnd);
            String url = innerScrape(stringCut, "[u]");
            printer.feed(1);      
            printer.setSize('L');      
            printer.println(innerScrape(stringCut, "[t]"));
            printer.setSize('s');
            printer.println(url);
            printer.setSize('M');
            printer.println(innerScrape(stringCut, "[d]"));
            printer.println(innerScrape(stringCut, "[ti]"));
            printer.feed(1);            
            count++;
            if(itemStart == -1)
            {
                  scrapeStatus = false; 
                  wifiClient.disconnect();
                  connectToBroker();
                  itemStart=0;          
                  str = "";
            }
            
    }
  }
  
  
  
String innerScrape(String strInstance, String attribute)
{
 
 int attributeLength = attribute.length();
// Serial.print(attributeLength);
 int strStart = strInstance.indexOf(attribute) + attributeLength;
 int strEnd = strInstance.indexOf(attribute, strStart);
 
 return strInstance.substring(strStart,strEnd);

  
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

void httpRequest(int node)
{
    if (client.connect(server, 80)) {
    Serial.println(F("connected to server for http request"));
    // Make a HTTP request:
    client.println("GET /~hivenode/fyp/read.php?node=" + String(node));
    client.println(F("Host: www.hivenodes.com"));
    client.println(F("Connection: close"));
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println(F("connection failed"));
  }
}


void connectToBroker(){
  //connect to the broker
    if (wifiClient.connect(deviceName)) {
      Serial.println(F("Connecting to broker"));
      wifiClient.subscribe(subscribedChannel);
      Serial.println(F("Connected to broker"));
    } else{
      Serial.println(F("Connection Error"));
    }
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Message Recieved");
  // handle message
  
  //check the topic - use this is you want to sunbscribe to more than one channel
  if (String(topic) == String(subscribedChannel)){
//    Serial.println(topic); 
    //convert the payload to a string, then print it out
    convertPayload(payload, length);
    delay(1);
    Serial.println(payloadString);
    if(payloadString == "print-node-1")
    {
    node = 1;
    delay(1);
    str = "";
    delay(1);
    count = 1;
    delay(1);
    wifiClient.disconnect();
    delay(1);
    scrapeStatus = true;
    delay(1);
    httpRequest(1);
    delay(1);
    }
    }
}


int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

