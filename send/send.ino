
// HALL SENSOR REF http://arduining.com/2012/07/17/arduino-hall-effect-sensor-gaussmeter/

#include <MANCHESTER.h>

int ls;
boolean occupied = false;
unsigned char data[1];

#define HALL_PIN 3

void setup() 
{  
  pinMode(HALL_PIN, INPUT);
  pinMode(1, OUTPUT);
  MANCHESTER.SetTxPin(4);
  
  digitalWrite(1, LOW);
}

void loop()
{
 
  int hall = analogRead(HALL_PIN);
  
//  Serial.println(hall);
   
  if(!occupied) { 
    if(hall < 200 || hall > 800) // either side of the magnetic field
    {
      uint16_t m = 1;
      data[0] = 2;
      MANCHESTER.TransmitBytes(1, &data[0]);
      occupied = true;
      digitalWrite(1, HIGH);
    }
  }
  else // we're occupied
  {
    if(hall > 200 && hall < 800) // either side of the magnetic field
    {
      uint16_t m = 0;
      data[0] = 1;
      MANCHESTER.TransmitBytes(1, &data[0]);
      occupied = false;
      digitalWrite(1, LOW);
    }
  }
  
  delay(1000);
  
}
