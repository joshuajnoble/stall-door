
// HALL SENSOR REF http://arduining.com/2012/07/17/arduino-hall-effect-sensor-gaussmeter/
#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/wdt.h> //Needed to enable/disable watch dog timer
#include <MANCHESTER.h>

int ls;
boolean occupied = false;
unsigned char data[1];

#define HALL_POWER_PIN 2
#define HALL_PIN 3

void setup() 
{  
  pinMode(HALL_POWER_PIN, OUTPUT);
  pinMode(HALL_PIN, INPUT);
  MANCHESTER.SetTxPin(4);
}

void loop()
{
 
  ADCSRA &= ~(1<<ADEN); //Disable ADC, saves ~230uA
  setup_watchdog(7); //Setup watchdog to go off after 1sec
  digitalWrite(HALL_POWER_PIN, LOW); // power down hall sensor
  sleep_mode(); //Go to sleep! Wake up 1sec later and check water
  
  digitalWrite(HALL_POWER_PIN, HIGH); // power up hall sensor
  ADCSRA |= (1<<ADEN); //Enable ADC
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
  
}

//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
//From: http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void setup_watchdog(int timerPrescaler) {

  if (timerPrescaler > 9 ) timerPrescaler = 9; //Limit incoming amount to legal settings

  byte bb = timerPrescaler & 7; 
  if (timerPrescaler > 7) bb |= (1<<5); //Set the special 5th bit if necessary

  //This order of commands is important and cannot be combined
  MCUSR &= ~(1<<WDRF); //Clear the watch dog reset
  WDTCR |= (1<<WDCE) | (1<<WDE); //Set WD_change enable, set WD enable
  WDTCR = bb; //Set new watchdog timeout value
  WDTCR |= _BV(WDIE); //Set the interrupt enable, this will keep unit from resetting after each int
}

//This runs each time the watch dog wakes us up from sleep
ISR(WDT_vect) {
  //Don't do anything. This is just here so that we wake up.
}
