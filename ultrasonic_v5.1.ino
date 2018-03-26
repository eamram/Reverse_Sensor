#include <NewPing.h> //Load UltraSonic Library
#include <LiquidCrystal.h> //Load Liquid Crystal Library
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define TRIGGER_PIN  9
#define ECHO_PIN     8
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int greenLed = 6;
int yellowLed = 7;
int redLed = 10;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("testing...");
 
  // initialize digital pin LEDs as an output.
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
}

void loop() {

  long cm;
  delay(400);
  Serial.print("Ping: ");

  int dist = sonar.ping_cm();

  Serial.print(dist);
  Serial.println("cm");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(dist);
  lcd.print("  cm");
  delay(100);

  if (dist <20 )

  {

    digitalWrite(redLed, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);

    digitalWrite(redLed, LOW);   // turn the LED off (LOW is the voltage level)
    digitalWrite(greenLed, LOW);   // turn the LED off (LOW is the voltage level)
    delay(100);

  }
  else if (dist <50)
  {
    digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(yellowLed, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);

    digitalWrite(greenLed, LOW);   // turn the LED off (LOW is the voltage level)
    digitalWrite(yellowLed, LOW);   // turn the LED off (LOW is the voltage level)
    delay(100);
  
  }
    else if (dist <80)
  {
    digitalWrite(yellowLed, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);

    digitalWrite(yellowLed, LOW);   // turn the LED off (LOW is the voltage level)
    delay(100);
  
  }
}
