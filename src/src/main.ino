#include <NilRTOS.h>
#include <NilSerial.h>
#include <DHT.h>

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Full System configurations
 */

/*
 * This is the limit of temperature
 * 
 * If a higher temperature is detected, the system will alert the problem
 */
#define TMP_LIMIT 25        // The limit of temperature, if its above, the system will alert

/*
 * This is the limit of humidity
 * 
 * If a higher humidity is detected, the system will alert the problem
 */
#define HMD_LIMIT 70        // The limit of humidity, if its above, the system will alert

/*
 * This is to allow system to print extra informations
 * Allowing the user to see what is going on behind the scenes
 */
//#define DEBUG_RTDMS         // Uncomment this line to activate DEBUG

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * NilRTOS Config's
 */

#define Serial NilSerial    // NilSerial is lighter than Serial

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Check Water Flood Task Config's
 */

#define FLOOD_PIN A0        // Pin used for read from rain sensor
#define WATER_LIMIT 700     // The limit value for rain sensor to consider flood

//-----------------------------------------------------------------------------------------------------------------------------

/* 
 * Alert Task Config's 
 */

#define ALERT_PIN 9         // Output Pin
#define ALERT_DELAY 200     // In Milliseconds

//-----------------------------------------------------------------------------------------------------------------------------

/* 
 * Check Temperature and Check Humidity Tasks Config's 
 */

#define QTD_SENSORS 3       // How many sensors are available to use
#define TOLERANCE 2         // The difference of degrees allowed by system

/* 
 * All configuration needed for the sensors
 *
 * This includes:
 *   - Pin for Read
 *   - Sensor Type
 */
DHT sensors[] = {
    DHT(A1, DHT11),
    DHT(A2, DHT11),
    DHT(A3, DHT11)
};

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Signature of helper functions
 *
 * The helper functions implementations are in the end of the file
 */

/* This function deals with a possible fault in sensor and computes a valid mean */
float get_fault_free_value(float values[]);

/* This function uses the buzzer's configuration to beep at a interval */
void beep(unsigned char interval);

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Alert Task
 * 
 * This is the first task because it is the most important
 * If this task does not execute or execute with delay, several
 * problems can happen to the Datacenter
 *
 */

// This task doesn't need a stack
NIL_WORKING_AREA(waThread1, 0);

// Thread function for Alert Task
NIL_THREAD(Thread1, arg)
{
    while (TRUE) {
        beep(ALERT_DELAY);

        // Just to test the other tasks
        nilThdSleepMilliseconds(1000);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Check Temperature Task
 *
 * This task uses 3 sensors to check and validate the temperature
 * WARNING: The computing time for this task was measured with 3
 * sensors. If more sensors are added, these times need to be recalculated
 */

// Stack with 384 bytes beyond context switch and interrupt needs
NIL_WORKING_AREA(waThread2, 384);

// Thread function for check temperature and humidity task
NIL_THREAD(Thread2, arg)
{
    while (TRUE) {
        float temps[QTD_SENSORS];   // To store the temperatures readed by the sensors

        for (unsigned char i = 0; i < QTD_SENSORS; i++) {
            float aux = sensors[i].readTemperature();

            /* Checks if temperature is not a number (a.k.a. NAN)*/
            if (aux == NAN)
                temps[i] = 0.0;
            else 
                temps[i] = aux;

            #ifdef DEBUG_RTDMS
                Serial.print("S ");
                Serial.print(i);
                Serial.print(" - T: ");
                Serial.println(temps[i]);
            #endif
        }

        if (get_fault_free_value(temps) >= TMP_LIMIT) {
            Serial.println("Temp alert"); 
        }

        /* Waits 1 second to wake */
        nilThdSleepMilliseconds(2000);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Check Humidity Task
 *
 * This task uses 3 sensors to check and validate the humidity
 * WARNING: The computing time for this task was measured with 3
 * sensors. If more sensors are added, these times need to be recalculated
 */

// Stack with 384 bytes beyond context switch and interrupt needs
NIL_WORKING_AREA(waThread3, 384);

// Thread function for check water flood task
NIL_THREAD(Thread3, arg)
{
    while (TRUE) {
        float hmdts[QTD_SENSORS];   // To store the humidities readed by the sensors

        for (unsigned char i = 0; i < QTD_SENSORS; i++) {
            float aux = sensors[i].readHumidity();

            /* Checks if humidity is not a number (a.k.a. NAN)*/
            if (aux == NAN)
                hmdts[i] = 0.0;
            else 
                hmdts[i] = aux;

            #ifdef DEBUG_RTDMS
                Serial.print("S ");
                Serial.print(i);
                Serial.print(" - H: ");
                Serial.println(hmdts[i]);
            #endif
        }

        if (get_fault_free_value(hmdts) >= HMD_LIMIT) {
            Serial.println("Humd alert"); 
        }

        /* Waits 1 second to wake */
        nilThdSleepMilliseconds(2000);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Check water flood task
 */

// Stack with 384 bytes beyond context switch and interrupt needs
NIL_WORKING_AREA(waThread4, 384);

// Thread function for check water flood task
NIL_THREAD(Thread4, arg)
{
    while (TRUE) {
        if (analogRead(FLOOD_PIN) < WATER_LIMIT){
            Serial.println("Flood detected!");
        } 
        #ifdef DEBUG_RTDMS
        else {
            Serial.println("No flood detected!"); 
        }
        #endif

        /* Sleep for 1 second */
        nilThdSleepMilliseconds(1000);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Threads static table, one entry per thread.  A thread's priority is
 * determined by its position in the table with highest priority first.
 * 
 */
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, Thread1, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_ENTRY(NULL, Thread3, NULL, waThread3, sizeof(waThread3))
NIL_THREADS_TABLE_ENTRY(NULL, Thread4, NULL, waThread4, sizeof(waThread4))
NIL_THREADS_TABLE_END()

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Implementation of main functions of arduino
 *
 * Loop functions must be empty for NilRTOS
 */

void setup()
{
    // Start serial communication
    Serial.begin(9600);

    // Declare alert task pin to be an output (Alert Task)
    pinMode(ALERT_PIN, OUTPUT);

    // Init the temperature and humidity sensors
    for (unsigned char i = 0; i < QTD_SENSORS; i++) {
        sensors[i].begin(); 
    }

    // Beep 3 fast times to show that system was activated
    beep(50);
    beep(50);
    beep(50);

    // Start Nil RTOS
    nilSysBegin();
}

// Loop is the idle thread. The idle thread must not invoke any 
// kernel primitive able to change its state to not runnable.
void loop()
{
    // Not used
}

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Implementation of helper functions declared in the beggining of the file
 */

float get_fault_free_value(float values[]){
    float bigger = 0, average = 0;
    _Bool repeated = false;
    float diff[QTD_SENSORS];

    for (unsigned char i = 0; i < QTD_SENSORS; i++)
        average += values[i]; 

    average /= QTD_SENSORS;

    /* Identifies which sensor have the bigger difference and if it is alone */
    for (unsigned char i = 0; i < QTD_SENSORS; i++) {
        diff[i] = values[i] - average;        

        /* Get absolute value from numbers */
        if (diff[i] < 0)
            diff[i] *= -1;

        if (diff[i] > bigger){
            bigger = diff[i];
            repeated = false;
        } else if (diff[i] == bigger) {
            repeated = true; 
        }
    }

    /* Recompute the average in the case of 1 sensor failure */
    if (repeated == false && bigger >= TOLERANCE) {
        /* Set average to 0 */
        average = 0;

        for (unsigned char i = 0; i < QTD_SENSORS; i++){
            if (diff[i] != bigger) 
                average += values[i];
        } 

        /* Final temperature for 1 sensor failure */
        average /= (QTD_SENSORS - 1);
    } 

    return average;
}

void beep(unsigned char interval){
    analogWrite(ALERT_PIN, 20);         // Almost any value can be used except 0 and 255 to turn it on
    delay(interval);                    // Wait for a delayms ms
    analogWrite(ALERT_PIN, 0);          // 0 turns it off
    delay(interval);                    // Wait for a delayms ms   
}
