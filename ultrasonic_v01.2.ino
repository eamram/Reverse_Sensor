#include <NewPing.h> // For Ultrasonic library
#include <Wire.h> // Must include Wire library for I2C
#include <SparkFun_MMA8452Q.h> // Includes the SFE_MMA8452Q library

///////////////////////////////////////////////////////
//                 DEFINITIONS                       //
///////////////////////////////////////////////////////
// I/O Pins
#define TRIGGER_PIN_A            2
#define ECHO_PIN_A               3
#define TRIGGER_PIN_B            4
#define ECHO_PIN_B               5
#define TRIGGER_PIN_C            6
#define ECHO_PIN_C               7
#define TRIGGER_PIN_D            8
#define ECHO_PIN_D               9
#define LED_A_PIN               10
#define LED_B_PIN               11
#define LED_C_PIN               12
#define LED_D_PIN               13
//#define LED_E_PIN               A5
#define LED_F_PIN               A0
#define LED_G_PIN               A1
#define LED_H_PIN               A2
#define LED_I_PIN               A3
//#define LED_J_PIN               A4

// General
#define MAX_DISTANCE           400  // [cm]
#define DISTANCE_LEVEL_A        80//50  // [cm]
#define DISTANCE_LEVEL_B        60  // [cm]
#define DISTANCE_LEVEL_C        40  // [cm]
#define DISTANCE_LEVEL_D        20  // [cm]
#define DISTANCE_LEVEL_E        20  // [cm]
#define DISPLAY_DIST_GAP         5  // [cm]
#define SERIAL_COM_RATE     115200
#define SONAR_SAMPLE_TIME       50 // [mSec]
#define SONAR_NOF_SAMPLES        3
#define MODE_LEVEL_DEF           5 
#define MODE_LEVEL_A             4
#define MODE_LEVEL_B             3    
#define MODE_LEVEL_C             2
#define MODE_LEVEL_D             1
#define MODE_LEVEL_E             0         


///////////////////////////////////////////////////////
//           GLOBAL VARIABLES DEFINITION             //
///////////////////////////////////////////////////////
NewPing sonar_a(TRIGGER_PIN_A, ECHO_PIN_A, MAX_DISTANCE);
NewPing sonar_b(TRIGGER_PIN_B, ECHO_PIN_B, MAX_DISTANCE);
NewPing sonar_c(TRIGGER_PIN_C, ECHO_PIN_C, MAX_DISTANCE);
NewPing sonar_d(TRIGGER_PIN_D, ECHO_PIN_D, MAX_DISTANCE);

MMA8452Q accel;

int cycle_cnt;
int sample_id;
int dist_a[SONAR_NOF_SAMPLES];
int dist_a_avg;
int dist_b[SONAR_NOF_SAMPLES];
int dist_b_avg;
int dist_c[SONAR_NOF_SAMPLES];
int dist_c_avg;
int dist_d[SONAR_NOF_SAMPLES];
int dist_d_avg;
int dist_min;
int display_number;
int remainder;
int upper_display_number;
int lower_display_number;
int mode_level;
unsigned long AccelReadTime,
              LastMoveTime,
              AccelLastInitTime,
              CurTime;
int DispalyEnableFlag;
int IgnoreAccelFlag,
    RightSideOutOfRangeFlag,
    LeftSideOutOfRangeFlag;
float AccelPrevX,
      AccelPrevY,
      AccelPrevZ;
///////////////////////////////////////////////////////
//                SETUP()                            //
///////////////////////////////////////////////////////
void setup() 
{
    Serial.begin(SERIAL_COM_RATE); 
    pinMode(LED_A_PIN , OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);
    pinMode(LED_C_PIN, OUTPUT);
    pinMode(LED_D_PIN, OUTPUT); 
//  pinMode(LED_E_PIN, OUTPUT);
    pinMode(LED_F_PIN, OUTPUT);
    pinMode(LED_G_PIN, OUTPUT);
    pinMode(LED_H_PIN, OUTPUT);   
    pinMode(LED_I_PIN, OUTPUT);
//  pinMode(LED_J_PIN, OUTPUT);
    accel.init();
    AccelLastInitTime  = millis();
       
    cycle_cnt = 0;
    DispalyEnableFlag = 1;
    IgnoreAccelFlag = 0;
    AccelPrevX = 0;
    AccelPrevY = 0;
    AccelPrevZ = 0;
}


///////////////////////////////////////////////////////
//                 LOOP()                            //
///////////////////////////////////////////////////////
void loop() 
{
    // Variable Initialization
    sample_id = (cycle_cnt % SONAR_NOF_SAMPLES);
    dist_a[sample_id] = sonar_a.ping_cm();
    dist_b[sample_id]= sonar_b.ping_cm();
    dist_c[sample_id]= sonar_c.ping_cm();
    dist_d[sample_id]= sonar_d.ping_cm();

    Serial.print("**************** Cycle ID: ");      
    Serial.print(cycle_cnt);      
    Serial.println(" ****************");          
    // Check for Out of Range - Right & Left Sides
    if ( (dist_a[sample_id] == 0) || (dist_b[sample_id] == 0) )
    {
      RightSideOutOfRangeFlag = 1;
      Serial.println("####### Right Side - Out Of Range ######");      
    }
    else
    {
          RightSideOutOfRangeFlag = 0;
    }
  
    if ( (dist_c[sample_id] == 0) || (dist_d[sample_id] == 0) )
    {
      LeftSideOutOfRangeFlag = 1;
      Serial.println("####### Left Side - Out Of Range ######");      
    }
    else
    {
          LeftSideOutOfRangeFlag = 0;
    }  
  
    if (accel.available())
    {
        // First, use accel.read() to read the new variables:
        accel.read();
        IgnoreAccelFlag = 0;
        AccelReadTime = millis();
        Serial.print(accel.cx, 3);
        Serial.print(" \t");
        Serial.print(accel.cy, 3);
        Serial.print(" \t");
        Serial.print(accel.cz, 3);        
        Serial.println();
        
        // Check for movement
        if ( abs(AccelPrevX - accel.cx) > 0.1 ||  abs(AccelPrevY - accel.cy) > 0.1  || abs(AccelPrevZ - accel.cz) > 0.1 )
        {
            LastMoveTime = millis();
            DispalyEnableFlag = 1;
        }
        AccelPrevX = accel.cx;
        AccelPrevY = accel.cy;        
        AccelPrevZ = accel.cz;        
    }
    else
    {
        CurTime = millis();
        if ( (CurTime - AccelReadTime) > 1000 && IgnoreAccelFlag == 0 )
        {
            Serial.println("No Signal from Accel Sensor for more than 1 Sec");
            DispalyEnableFlag = 1;
            IgnoreAccelFlag = 1;
        }
       if ( ( CurTime - AccelLastInitTime ) > 1000  && IgnoreAccelFlag == 1 )
       {
          accel.init();         
          AccelLastInitTime = CurTime;         
       }
    } 
    
    CurTime = millis();
    if ( IgnoreAccelFlag == 0 && (CurTime - LastMoveTime) > 5000 && DispalyEnableFlag == 1 )    
    {
            Serial.println("No movement for more than 5 Sec");
            DispalyEnableFlag = 0;
    }
    
    // Calculate average distance samples and call dispaly and beep functions
    if (sample_id == 0 )
    {
        // ------------------------------------
        // Right Sensor Data & Display handler
        // ------------------------------------
        dist_a_avg = (dist_a[0]+dist_a[1]+dist_a[2])/3;
        dist_b_avg =  (dist_b[0]+dist_b[1]+dist_b[2])/3;   
        dist_min = max(dist_a_avg, dist_b_avg);
 
        // Calculate display number
        remainder = dist_min % DISPLAY_DIST_GAP;
        lower_display_number = dist_min - remainder;
        upper_display_number = lower_display_number + DISPLAY_DIST_GAP;
        if (remainder <= (DISPLAY_DIST_GAP / 2 ) )
        {
            display_number = lower_display_number;
        }
        else
        {
            display_number = upper_display_number;
        }
        
        // Set level mode
        mode_level = MODE_LEVEL_DEF;
        if ( display_number < DISTANCE_LEVEL_E )
        {
            mode_level = MODE_LEVEL_E;
        }        
        else if ( display_number < DISTANCE_LEVEL_D )
        {
            mode_level = MODE_LEVEL_D;
        }
        else if ( display_number < DISTANCE_LEVEL_C )
        {
            mode_level = MODE_LEVEL_C;
        }
        else if ( display_number < DISTANCE_LEVEL_B )
        {
            mode_level = MODE_LEVEL_B;            
        }
        else if ( display_number < DISTANCE_LEVEL_A )
        {
            mode_level = MODE_LEVEL_A;            
        }  
        
        if ( DispalyEnableFlag == 0  || RightSideOutOfRangeFlag == 1 )
        {
            mode_level = MODE_LEVEL_DEF;            
        }

        // Print to serial port
        Serial.print(dist_a_avg);
        Serial.println(" a cm");
        Serial.print(dist_b_avg);
        Serial.println(" b cm");  
        Serial.println("----------");
        Serial.print(dist_min);
        Serial.println(" cm min");
        Serial.println("----------");
        Serial.print("Display Number: ");
        Serial.println(display_number);
        Serial.println("----------"); 
        Serial.print("Mode: ");
        Serial.println(mode_level);
        Serial.println("----------");        
        
        // Set Display LED behavior according to mode level   
        switch (mode_level)
        {
            case MODE_LEVEL_A:   
                digitalWrite(LED_A_PIN, HIGH);  
                digitalWrite(LED_B_PIN, LOW);  
                digitalWrite(LED_C_PIN, LOW);              
                digitalWrite(LED_D_PIN, LOW);                   
                //digitalWrite(LED_E_PIN, LOW);                
                break;
                
            case MODE_LEVEL_B:
                digitalWrite(LED_A_PIN, HIGH);  
                digitalWrite(LED_B_PIN, HIGH);  
                digitalWrite(LED_C_PIN, LOW);
                digitalWrite(LED_D_PIN, LOW);
                //digitalWrite(LED_E_PIN, LOW);                  
                break;
                
            case MODE_LEVEL_C:
                digitalWrite(LED_A_PIN, HIGH);  
                digitalWrite(LED_B_PIN, HIGH);  
                digitalWrite(LED_C_PIN, HIGH);
                digitalWrite(LED_D_PIN, LOW);
                //digitalWrite(LED_E_PIN, LOW);                  
                break;
                
            case MODE_LEVEL_D:
                digitalWrite(LED_A_PIN, HIGH);  
                digitalWrite(LED_B_PIN, HIGH);  
                digitalWrite(LED_C_PIN, HIGH);
                digitalWrite(LED_D_PIN, HIGH); 
                //digitalWrite(LED_E_PIN, LOW);                  
                break;                
                
            case MODE_LEVEL_E:
                digitalWrite(LED_A_PIN, HIGH);  
                digitalWrite(LED_B_PIN, HIGH);  
                digitalWrite(LED_C_PIN, HIGH);
                digitalWrite(LED_D_PIN, HIGH); 
                //digitalWrite(LED_E_PIN, HIGH);  
                break;                
                
            default:        
                digitalWrite(LED_A_PIN, LOW);  
                digitalWrite(LED_B_PIN, LOW);  
                digitalWrite(LED_C_PIN, LOW);
                digitalWrite(LED_D_PIN, LOW);  
                //digitalWrite(LED_E_PIN, LOW);                  
                break;
        } // switch (mode_level)
    
        // ------------------------------------
        // Left Sensor Data & Display handler
        // ------------------------------------
        dist_c_avg =  (dist_c[0]+dist_c[1]+dist_c[2])/3;
        dist_d_avg =  (dist_d[0]+dist_d[1]+dist_d[2])/3;        
        dist_min = max(dist_c_avg, dist_d_avg);
        
        // Calculate display number
        remainder = dist_min % DISPLAY_DIST_GAP;
        lower_display_number = dist_min - remainder;
        upper_display_number = lower_display_number + DISPLAY_DIST_GAP;
        if (remainder <= (DISPLAY_DIST_GAP / 2 ) )
        {
            display_number = lower_display_number;
        }
        else
        {
            display_number = upper_display_number;
        }
        
        // Set level mode
        mode_level = MODE_LEVEL_DEF;
        if ( display_number < DISTANCE_LEVEL_E )
        {
            mode_level = MODE_LEVEL_E;
        }        
        else if ( display_number < DISTANCE_LEVEL_D )
        {
            mode_level = MODE_LEVEL_D;
        }
        else if ( display_number < DISTANCE_LEVEL_C )
        {
            mode_level = MODE_LEVEL_C;
        }
        else if ( display_number < DISTANCE_LEVEL_B )
        {
            mode_level = MODE_LEVEL_B;            
        }
        else if ( display_number < DISTANCE_LEVEL_A )
        {
            mode_level = MODE_LEVEL_A;            
        }
        
        if ( DispalyEnableFlag == 0 || LeftSideOutOfRangeFlag == 1)
        {
            mode_level = MODE_LEVEL_DEF;            
        }

        // Print to serial port
        Serial.print(dist_c_avg);
        Serial.println(" c cm");
        Serial.print(dist_d_avg);
        Serial.println(" d cm");        
        Serial.println("----------");
        Serial.print(dist_min);
        Serial.println(" cm min");
        Serial.println("----------");
        Serial.print("Display Number: ");
        Serial.println(display_number);
        Serial.println("----------"); 
        Serial.print("Mode: ");
        Serial.println(mode_level);
        Serial.println("----------");        
        
        // Set Display LED behavior according to mode level   
        switch (mode_level)
        {
            case MODE_LEVEL_A:   
                digitalWrite(LED_F_PIN, HIGH);  
                digitalWrite(LED_G_PIN, LOW);  
                digitalWrite(LED_H_PIN, LOW);              
                digitalWrite(LED_I_PIN, LOW);                   
                //digitalWrite(LED_J_PIN, LOW);                
                break;
                
            case MODE_LEVEL_B:
                digitalWrite(LED_F_PIN, HIGH);  
                digitalWrite(LED_G_PIN, HIGH);  
                digitalWrite(LED_H_PIN, LOW);
                digitalWrite(LED_I_PIN, LOW);
                //digitalWrite(LED_J_PIN, LOW);                  
                break;
                
            case MODE_LEVEL_C:
                digitalWrite(LED_F_PIN, HIGH);  
                digitalWrite(LED_G_PIN, HIGH);  
                digitalWrite(LED_H_PIN, HIGH);
                digitalWrite(LED_I_PIN, LOW);
                //digitalWrite(LED_J_PIN, LOW);                  
                break;
                
            case MODE_LEVEL_D:
                digitalWrite(LED_F_PIN, HIGH);  
                digitalWrite(LED_G_PIN, HIGH);  
                digitalWrite(LED_H_PIN, HIGH);
                digitalWrite(LED_I_PIN, HIGH); 
                //digitalWrite(LED_J_PIN, LOW);                  
                break;                
                
            case MODE_LEVEL_E:
                digitalWrite(LED_F_PIN, HIGH);  
                digitalWrite(LED_G_PIN, HIGH);  
                digitalWrite(LED_H_PIN, HIGH);
                digitalWrite(LED_I_PIN, HIGH); 
                //digitalWrite(LED_J_PIN, HIGH);  
                break;                
                
            default:        
                digitalWrite(LED_F_PIN, LOW);  
                digitalWrite(LED_G_PIN, LOW);  
                digitalWrite(LED_H_PIN, LOW);
                digitalWrite(LED_I_PIN, LOW);  
                //digitalWrite(LED_J_PIN, LOW);                  
                break;
        } // switch (mode_level)
    } // if (sample_id == 0 )    
    
    delay(SONAR_SAMPLE_TIME);
    cycle_cnt++;
} // void loop()
