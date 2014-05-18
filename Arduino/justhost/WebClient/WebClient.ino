#include <MemoryFree.h>


#include <Adafruit_Thermal.h>
#include "SoftwareSerial.h"

#include <SPI.h>
#include <Ethernet.h>
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int printer_RX_Pin = 5;  // This is the green wire
int printer_TX_Pin = 6;  // This is the yellow wire
Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.hivenodes.com";    // name address for Google (using DNS)
String str = "";
boolean stopScraping = false;
int count = 1;
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,177);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  printer.begin();
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
  if (client.connect(server, 80)) {
    Serial.println("connected");
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

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  
  if (client.available() && printStatus) {
  char c = client.read();
  str += c;
  }
          
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    client.stop();    
    if(!stopScraping)
    {
      cutString();
    }
  }
  Serial.println(freeRam());
}

  void cutString()
  {               
          
    while(!stopScraping)
          
          {
            
            String instance = "[it" + String(count) + "]";
            int itemStart = str.indexOf(instance);           
            int itemEnd = str.indexOf("[/it]", itemStart);
            String stringCut = str.substring(itemStart,itemEnd);
            printer.println(innerScrape(stringCut, "[t]"));
            printer.println(innerScrape(stringCut, "[u]"));
            printer.println(innerScrape(stringCut, "[d]"));
            printer.println(innerScrape(stringCut, "[ti]"));
            count++;
            
            delay(1000);           
            
            if(itemStart == -1)
            {
              stopScraping = true; 
              
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

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
