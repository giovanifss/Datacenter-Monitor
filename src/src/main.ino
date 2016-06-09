#include <NilRTOS.h>
#include <NilSerial.h>
#include <DHT.h>

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Global configurations for program
 */

#define DEBUG_RTDMS         // Activates extra logs and add a sleep time of 1 second to alert task

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
 * Check Temperature and Humidity Task Config's 
 */

#define QTD_SENSORS 3       // How many sensors are available to use

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
 * Alert Task
 * 
 * This is the first task because it is the most important
 * If this task does not execute or execute with delay, several
 * problems can happen to the Datacenter
 *
 */

// Stack with 128 bytes beyond context switch and interrupt needs
NIL_WORKING_AREA(waThread1, 128);

// Thread function for Alert Task
NIL_THREAD(Thread1, arg){
    while (TRUE) {
        analogWrite(ALERT_PIN, 20);         // Almost any value can be used except 0 and 255 to turn it on
        delay(ALERT_DELAY);                 // Wait for a delayms ms
        analogWrite(ALERT_PIN, 0);          // 0 turns it off
        delay(ALERT_DELAY);                 // Wait for a delayms ms   

        #ifdef DEBUG_RTDMS
            // Just to test the other tasks
            nilThdSleepMilliseconds(1000);
        #endif
    }
}

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Check Temperature and Humidity Task
 *
 * This task uses 3 sensors to check and validate the temperature
 * WARNING: The computing time for this task was measured with 3
 * sensors. If more sensors are added, these times need to be recalculated
 */

// Stack with 128 bytes beyond context switch and interrupt needs
NIL_WORKING_AREA(waThread2, 256);

// Thread function for check temperature and humidity task
NIL_THREAD(Thread2, arg){
    while (TRUE) {
        for (unsigned char i = 0; i < QTD_SENSORS; i++) {
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.print(" - Humidity: ");
            Serial.print(sensors[i].readHumidity());
            Serial.print(" | Temperature: ");
            Serial.println(sensors[i].readTemperature());
        }

        /* Jumps one line to not mess output */
        Serial.println();

        /* Waits 1 second to wake */
        nilThdSleepMilliseconds(1000);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Check water flood task
 */

// Stack with 128 bytes beyond context switch and interrupt needs
NIL_WORKING_AREA(waThread3, 256);

// Thread function for check water flood task
NIL_THREAD(Thread3, arg){
    while (TRUE) {
        if (analogRead(FLOOD_PIN) < WATER_LIMIT){
            Serial.println("Warning!!! Flood detected!!!");
        } 
        /* If in DEBUG mode, prints that it has not detected flood */
        #ifdef DEBUG_RTDMS
        else {
            Serial.println("Ok! No Flood detected!!"); 
        }
        #endif

        /* Jumps one line to not mess output */
        Serial.println();

        /* Sleep for 1 second */
        nilThdSleepMilliseconds(400);
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
//NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_ENTRY(NULL, Thread3, NULL, waThread3, sizeof(waThread2))
NIL_THREADS_TABLE_END()

//-----------------------------------------------------------------------------------------------------------------------------

void setup(){
    // Start serial communication
    Serial.begin(9600);

    // Declare alert task pin to be an output (Alert Task)
    pinMode(ALERT_PIN, OUTPUT);

    // Init the temperature and humidity sensors
    //for (unsigned char i = 0; i < QTD_SENSORS; i++) {
    //    sensors[i].begin(); 
    //}

    // Beep 3 fast times to show that system was activated
    analogWrite(ALERT_PIN, 20);         // Almost any value can be used except 0 and 255 to turn it on
    delay(50);                          // Wait for a delayms ms
    analogWrite(ALERT_PIN, 0);          // 0 turns it off
    delay(50);                          // Wait for a delayms ms   

    analogWrite(ALERT_PIN, 20);         // Almost any value can be used except 0 and 255 to turn it on
    delay(50);                          // Wait for a delayms ms
    analogWrite(ALERT_PIN, 0);          // 0 turns it off
    delay(50);                          // Wait for a delayms ms   

    analogWrite(ALERT_PIN, 20);         // Almost any value can be used except 0 and 255 to turn it on
    delay(50);                          // Wait for a delayms ms
    analogWrite(ALERT_PIN, 0);          // 0 turns it off
    delay(50);                          // Wait for a delayms ms   

    // Start Nil RTOS
    nilSysBegin();
}

// Loop is the idle thread. The idle thread must not invoke any 
// kernel primitive able to change its state to not runnable.
void loop(){
    // Not used
}
