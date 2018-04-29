#include <NewPing.h> //Load UltraSonic Library
#include <TM1637Display.h>

#define TRIGGER_PIN_RIGHT  2
#define ECHO_PIN_RIGHT     3
#define TRIGGER_PIN_LEFT  4
#define ECHO_PIN_LEFT     5
#define MAX_DISTANCE 400
#define BUZZ_PIN 7

const int CLK = 8; //Set the CLK pin connection to the display
const int DIO = 9; //Set the DIO pin connection to the display

NewPing sonar_right(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE);
NewPing sonar_left(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE);
TM1637Display display(CLK, DIO);

void setup() {
  Serial.begin(115200);
  pinMode(BUZZ_PIN,OUTPUT);
  display.setBrightness(0x7);  //set the diplay to maximum brightness 
}
void beep(int duration)
{
 int t=0;
 for (t=0;t<duration/10;t++)
 {
  digitalWrite(BUZZ_PIN, HIGH);   // turn the BUZZ on (LOW is the voltage level)
  delay(10);
  digitalWrite(BUZZ_PIN, LOW);   // turn the BUZZ off (LOW is the voltage level)
  
 }
}

void loop() {

  long cm;
  delay(50);  
  int dist_left = sonar_left.ping_cm();
  int dist_right = sonar_right.ping_cm();
  int dist_min = min(dist_left, dist_right);
  Serial.print(dist_left);
  Serial.println(" left cm");
  Serial.print(dist_right);
  Serial.println(" right cm");
  Serial.println("----------");
  Serial.print(dist_min);
  Serial.println(" cm min");
  Serial.println("----------");
  display.showNumberDec(dist_min);
  
  if (dist_min <20 )
  {
    beep(50);
   // digitalWrite(redLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else if (dist_min <50)
  {
    beep(100);
   // digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
    else if (dist_min <80)
  {
   // digitalWrite(yellowLed, HIGH);   // turn the LED on (HIGH is the voltage level)
 //   delay(100);
  }
  
 // delay(400);
 // digitalWrite(greenLed, LOW);   // turn the LED off (LOW is the voltage level)
 // digitalWrite(redLed, LOW);   // turn the LED off (LOW is the voltage level)
 
  delay(400);
}
