///////////////////////////////////////////////////////
//                 HEADER FILES                      //
///////////////////////////////////////////////////////
#include <NewPing.h> //Load UltraSonic Library
#include <TM1637Display.h>

///////////////////////////////////////////////////////
//                 DEFINITIONS                       //
///////////////////////////////////////////////////////
#define TRIGGER_PIN_A            2
#define ECHO_PIN_A               3
#define TRIGGER_PIN_B            4
#define ECHO_PIN_B               5
#define TRIGGER_PIN_C            6
#define ECHO_PIN_C               7
#define DISPLAY_CLK_PIN          8
#define DISPLAY_DIO_PIN          9
#define BUZZ_PIN                10
#define MAX_DISTANCE           400
#define DISTANCE_LEVEL_A        40
#define DISTANCE_LEVEL_B        20
#define DISTANCE_LEVEL_C        15
#define SERIAL_COM_RATE     115200
#define SONAR_SAMPLE_TIME      150 // [mSec]
#define SONAR_NOF_SAMPLES        3


///////////////////////////////////////////////////////
//           GLOBAL VARIABLES DEFINITION             //
///////////////////////////////////////////////////////
NewPing sonar_a(TRIGGER_PIN_A, ECHO_PIN_A, MAX_DISTANCE);
NewPing sonar_b(TRIGGER_PIN_B, ECHO_PIN_B, MAX_DISTANCE);
NewPing sonar_c(TRIGGER_PIN_C, ECHO_PIN_C, MAX_DISTANCE);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);
int cycle_cnt;
int sample_id;
int dist_a[SONAR_NOF_SAMPLES];
int dist_a_avg;
int dist_b[SONAR_NOF_SAMPLES];
int dist_b_avg;
int dist_c[SONAR_NOF_SAMPLES];
int dist_c_avg;
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
    dist_a[sample_id] = sonar_a.ping_cm();
    dist_b[sample_id]= sonar_b.ping_cm();
    dist_c[sample_id]= sonar_c.ping_cm();
    
    // Calculate average distance samples and call dispaly and beep functions
    if (sample_id == 0 )
    {
        dist_a_avg = (dist_a[0]+dist_a[1]+dist_a[2])/3;
        dist_b_avg =  (dist_b[0]+dist_b[1]+dist_b[2])/3;
        dist_c_avg =  (dist_c[0]+dist_c[1]+dist_c[2])/3;
        dist_min = min(dist_a_avg, dist_b_avg);
        dist_min = min(dist_min, dist_c_avg);

        // Print to serial port
        Serial.print(dist_a_avg);
        Serial.println(" a cm");
        Serial.print(dist_b_avg);
        Serial.println(" b cm");
        Serial.print(dist_c_avg);
        Serial.println(" c cm");
        Serial.println("----------");
        Serial.print(dist_min);
        Serial.println(" cm min");
        Serial.println("----------");

        display.showNumberDec(dist_min);


        if (dist_min < 15)
        {
            beep(DISTANCE_LEVEL_C);
            // digitalWrite(redLed, HIGH);   // turn the LED on (HIGH is the voltage level)
            //  digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else if (dist_min < DISTANCE_LEVEL_B)
        {
            beep(100);
            // digitalWrite(greenLed, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else if (dist_min < DISTANCE_LEVEL_A)
        {
            // digitalWrite(yellowLed, HIGH);   // turn the LED on (HIGH is the voltage level)
            //   delay(100);
        }
    }
    
    delay(SONAR_SAMPLE_TIME);
    cycle_cnt++;
}
