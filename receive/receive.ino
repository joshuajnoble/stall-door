#include <MANCHESTER.h>

unsigned char bufferSize = 1;
unsigned char *bufferData;

boolean occupied = false;

void setup()
{
  
  Serial.begin(57600);
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  bufferData = (unsigned char*) malloc(1);

  // Set digital TX pin
  MANRX_SetRxPin(5);
  // Prepare interrupts
  MANRX_SetupReceive();
  // Begin receiving data
  MANRX_BeginReceiveBytes(1, bufferData);
}

void loop()
{

if (MANRX_ReceiveComplete())
  {
    unsigned char receivedSize = 1;
    MANRX_GetMessageBytes(&receivedSize, &bufferData);

    if(bufferData[0] == 1 ) {
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
    } else {
      digitalWrite(4, HIGH);
      digitalWrite(3, LOW);
    }
    
    MANRX_BeginReceiveBytes(1, bufferData);
    
  }
}
