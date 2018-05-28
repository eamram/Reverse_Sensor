///////////////////////////////////////////////////////
//                 HEADER FILES                      //
///////////////////////////////////////////////////////
#include <NewPing.h> //Load UltraSonic Library
#include <TM1637Display.h>

///////////////////////////////////////////////////////
//                 DEFINITIONS                       //
///////////////////////////////////////////////////////
#define TRIGGER_PIN_RIGHT        2
#define ECHO_PIN_RIGHT           3
#define TRIGGER_PIN_LEFT         4
#define ECHO_PIN_LEFT            5
#define MAX_DISTANCE           400
#define BUZZ_PIN                 7
#define DISPLAY_CLK_PIN          8
#define DISPLAY_DIO_PIN          9
#define SERIAL_COM_RATE     115200
#define SONAR_SAMPLE_TIME      150 // [mSec]
#define SONAR_NOF_SAMPLES        3


///////////////////////////////////////////////////////
//           GLOBAL VARIABLES DEFINITION             //
///////////////////////////////////////////////////////
NewPing sonar_right(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE);
NewPing sonar_left(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);
int cycle_cnt;
int sample_id;
int dist_left[SONAR_NOF_SAMPLES];
int dist_left_avg;
int dist_right[SONAR_NOF_SAMPLES];
int dist_right_avg;
int dist_min;


///////////////////////////////////////////////////////
//                       BEEP()                      //
///////////////////////////////////////////////////////
void beep(int duration)
{
    int t = 0;
    for (t = 0; t < (duration/10); t++)
    {
        digitalWrite(BUZZ_PIN, HIGH);   // turn the BUZZ on (LOW is the voltage level)
        delay(10);
        digitalWrite(BUZZ_PIN, LOW);   // turn the BUZZ off (LOW is the voltage level)
    }
}


///////////////////////////////////////////////////////
//                SETUP()                            //
///////////////////////////////////////////////////////
void setup() 
{
    Serial.begin(SERIAL_COM_RATE);
    pinMode(BUZZ_PIN,OUTPUT);
    display.setBrightness(0x7);  //set the diplay to maximum brightness 
    cycle_cnt = 0;
}


///////////////////////////////////////////////////////
//                 LOOP()                            //
///////////////////////////////////////////////////////
void loop() {
    // Variable Initialization
    sample_id = (cycle_cnt % SONAR_NOF_SAMPLES);
    dist_left[sample_id] = sonar_left.ping_cm();
    dist_right[sample_id]= sonar_right.ping_cm();
    
    // Calculate average distance samples and call dispaly and beep functions
    if (sample_id == 0 )
    {
        dist_left_avg = (dist_left[0]+dist_left[1]+dist_left[2])/3;
        dist_right_avg =  (dist_right[0]+dist_right[1]+dist_right[2])/3;
        dist_min = min(dist_left_avg, dist_right_avg);

        // Print to serial port
        Serial.print(dist_left_avg);
        Serial.println(" left cm");
        Serial.print(dist_right_avg);
        Serial.println(" right cm");
        Serial.println("----------");
        Serial.print(dist_min);
        Serial.println(" cm min");
        Serial.println("----------");

        display.showNumberDec(dist_min);


        if (dist_min < 20)
        {
            beep(50);
            // digitalWrite(redLed, HIGH);   // turn the LED on (HIGH is the voltage level)
            //  digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else if (dist_min < 50)
        {
            beep(100);
            // digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else if (dist_min < 80)
        {
            // digitalWrite(yellowLed, HIGH);   // turn the LED on (HIGH is the voltage level)
            //   delay(100);
        }
    }
    
    delay(SONAR_SAMPLE_TIME);
    cycle_cnt++;
}
