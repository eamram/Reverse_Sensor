/*
 * 
Pin assignments are:
CLK - D2
DIO - D3
5V or 3.3V supply to Display
GND to Display



 

 
 
void setup() {
}
 
void loop() {

}
*/

#include <TM1637Display.h>
#include <NewPing.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 400

const int CLK = 2; //Set the CLK pin connection to the display
const int DIO = 3; //Set the DIO pin connection to the display


TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
  display.setBrightness(0x6);  //set the diplay to maximum brightness
  Serial.begin(115200);
}


void loop() {
 delay(100);
 Serial.print("Ping: ");
 Serial.print(sonar.ping_cm());
 Serial.println("cm");
 display.showNumberDec(sonar.ping_cm());
}

