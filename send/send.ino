
// HALL SENSOR REF http://arduining.com/2012/07/17/arduino-hall-effect-sensor-gaussmeter/

#include <MANCHESTER.h>

int ls;
boolean occupied = false;
unsigned char data[1];

#define HALL_PIN 3

void setup() 
{  
  pinMode(HALL_PIN, INPUT);
  MANCHESTER.SetTxPin(4);
}

void loop()
{
 
  int hall = analogRead(HALL_PIN);
   
  if(!occupied) { 
    if(hall < 200 || hall > 800) // either side of the magnetic field
    {
      data[0] = 2;
      MANCHESTER.TransmitBytes(1, &data[0]);
      occupied = true;
    }
  }
  else // we're occupied
  {
    if(hall > 200 && hall < 800) // either side of the magnetic field
    {
      data[0] = 1;
      MANCHESTER.TransmitBytes(1, &data[0]);
      occupied = false;
    }
  }
  
  delay(1000);
  
}
