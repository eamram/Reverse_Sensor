#include <NewPing.h> //Load UltraSonic Libra ry
#include <LiquidCrystal.h> //Load Liquid Crystal Library


 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define TRIGGER_PIN_LEFT  9
#define ECHO_PIN_LEFT     8

#define TRIGGER_PIN_RIGHT  1
#define ECHO_PIN_RIGHT     13
#define MAX_DISTANCE 400

NewPing sonar_left(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE);
NewPing sonar_right(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE);

int greenLed = 6;
int redLed = 10;
int buzz_pin = 7;

void setup() {
 //Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("testing...");
 
  // initialize digital pin LEDs as an output.
  pinMode(redLed, OUTPUT);
  pinMode(greenLed,OUTPUT);
  pinMode(buzz_pin,OUTPUT);
  
}
void beep(int duration)
{
 int t=0;
 for (t=0;t<duration/10;t++)
 {
  digitalWrite(buzz_pin, HIGH);   // turn the LED off (LOW is the voltage level)
  delay(10);
  digitalWrite(buzz_pin, LOW);   // turn the LED off (LOW is the voltage level)
  
 }
}

void loop() {

  long cm;
  delay(50);
  //Serial.print("Ping: ");
  
  int dist_left = sonar_left.ping_cm();
  int dist_right = sonar_right.ping_cm();

  int dist_min = min(dist_left, dist_right);

 
  //Serial.print(dist_left);
  //Serial.println(" left cm");
  //Serial.print(dist_right);
  //Serial.println(" right cm");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Min: ");
  lcd.print(dist_min); 
  lcd.print(" cm");
  delay(50);

  if (dist_min <20 )

  {
    beep(50);
    digitalWrite(redLed, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else if (dist_min <50)
  {
    beep(100);
    digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
    else if (dist_min <80)
  {
   // digitalWrite(yellowLed, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);
  }
  
  delay(400);
  digitalWrite(greenLed, LOW);   // turn the LED off (LOW is the voltage level)
  digitalWrite(redLed, LOW);   // turn the LED off (LOW is the voltage level)
  
}
