#include <SPI.h>
#include <Ethernet.h>
#include "PubSubClient.h"
#include <Adafruit_Thermal.h>
#include "SoftwareSerial.h"
#include "hivesmall.h"


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA };
int printer_RX_Pin = 5;  // This is the green wire
int printer_TX_Pin = 6;  // This is the yellow wire
int itemEnd, itemStart;
int node;
Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);
int count = 1;
String str = "";
char* serverDNS = "box.bento.is";
int port = 80;  
char subscribedChannel[] = "public/cai-fyp/print";
char deviceName[] = "printer"; // set a unique name for each device connected to the broker
boolean scrapeStatus = false;
boolean moreThanOne;
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
  Serial.begin(19200);
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
  if(!scrapeStatus){  
   if(!wifiClient.connected()){
     connectToBroker();
  }
  else{
    wifiClient.loop();
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
  
    delay(25);
}

void cutString(String str)
  {                          
          
    while(scrapeStatus)
    {
      scrape();
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

void scrape()
{
       
      if(count == 1)
      {
          printer.inverseOn();
          printer.boldOn();
          printer.underlineOn();
          printer.setSize('M');
//          printer.println("Node " + String(node));
          printer.println("Hive - Lounge");
          printer.boldOff();
          printer.underlineOff();
          printer.inverseOff();
      }
            
            String instance = "[it" + String(count) + "]";
            itemStart = str.indexOf(instance);           
            itemEnd = str.indexOf("[/it]", itemStart);
            
            if(itemStart == -1)
            {
                  itemStart=0;          
                  str = "";
                  Serial.println(F("No items"));
                  if(moreThanOne)
                  {
                     printer.feed(1);
                     printer.printBitmap(hivesmall_width, hivesmall_height, hivesmall_data);
                     printer.feed(4);
                  }
                  scrapeStatus = false; 
            }
            
            else
            {
            moreThanOne = true;              
            String stringCut = str.substring(itemStart,itemEnd);
            String url = innerScrape(stringCut, "[u]");
            String title = innerScrape(stringCut, "[t]");
            String description = innerScrape(stringCut, "[de]");
            String date = innerScrape(stringCut, "[d]");
            String time = innerScrape(stringCut, "[ti]");
            if(title == "")
            {
              title = "Untitled";
            }            
            printer.feed(1);
            delay(25);      
            printer.setSize('S');      
            printer.println(title);
            delay(25);
            printer.feed(1);
            if (description != "...")
            {              
            printer.println(description);
            printer.feed(1);
            }
            printer.boldOn();
            printer.underlineOn();
            printer.println(url);
            printer.boldOff();
            printer.underlineOff();
            printer.feed(1);
            printer.setSize('M');
            printer.println(date);
            printer.println(time);
            printer.feed(1);
            count++;   
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
  // handle message
  
  //check the topic - use this is you want to sunbscribe to more than one channel
  if (String(topic) == String(subscribedChannel)){
//    Serial.println(topic); 
    //convert the payload to a string, then print it out
    convertPayload(payload, length);
    Serial.println(payloadString);
    if(payloadString == "print-node-1")
    {
      printMe(1);
    }
    else if(payloadString == "print-node-2")
    {
      printMe(2); 
    }
    else if(payloadString == "print-node-3")
    {
      printMe(3); 
    }
    }
}


int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void printMe(int nodeToPrint)
{
 
    printer.feed(1);
    node = nodeToPrint;
    delay(1);
    str = "";
    delay(1);
    count = 1;
    delay(1);
    wifiClient.disconnect();
    delay(1);
    scrapeStatus = true;    
    httpRequest(nodeToPrint); 
  
}
