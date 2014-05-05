/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 This example code is in the public domain.
 */
#include <SPI.h>

int pin = 2;
volatile int state = LOW;
boolean pullyState = false;
boolean pullyPreviousState = false;
int pullyValue;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600); 
//  pinMode(pin, INPUT_PULLUP);
//  attachInterrupt(pin, blink, CHANGE);
//  digitalWrite(pin, HIGH);       // turn on pullup resistors
  Serial.println("Hello World");
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
//  int sensorValue = analogRead(A0);
  // print out the value you read:
//  Serial.println(sensorValue);
  pullyValue = analogRead(A5);
  if(pullyValue > 510 && pullyValue <520)
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


void blink()
{
  state = !state;
  Serial.println('test');
}
