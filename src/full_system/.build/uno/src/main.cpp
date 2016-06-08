#include <Arduino.h>
#include <NilRTOS.h>
#include <NilSerial.h>
void setup();
void loop();
#line 1 "src/main.ino"
//#include <NilRTOS.h>
//#include <NilSerial.h>

/* NilSerial is lighter than Serial */
#define Serial NilSerial

/* Alert Task Config's */
#define ALERT_PIN 9
#define ALERT_DELAY 200

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

// Thread function for check temperature and humidity task
NIL_THREAD(Thread1, arg){
    while (TRUE) {
        analogWrite(ALERT_PIN, 20);         // Almost any value can be used except 0 and 255 to turn it on
        delay(ALERT_DELAY);                 // Wait for a delayms ms
        analogWrite(ALERT_PIN, 0);          // 0 turns it off
        delay(ALERT_DELAY);                 // Wait for a delayms ms   

        nilThdSleepMilliseconds(300);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------

/*
 * Check water flood task
 */

// Stack with 128 bytes beyond context switch and interrupt needs
NIL_WORKING_AREA(waThread2, 128);

// Thread function for check water flood task
NIL_THREAD(Thread2, arg){
    while (TRUE) {
        // Sleep for 200 miliseconds
        nilThdSleepMilliseconds(200);

        Serial.println("Thread 2");

        // Sleep for 200 miliseconds
        nilThdSleepMilliseconds(200);
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
NIL_THREADS_TABLE_END()

//-----------------------------------------------------------------------------------------------------------------------------

void setup(){
    // Start serial communication
    Serial.begin(9600);

    // Declare alert task pin to be an output (Alert Task)
    pinMode(ALERT_PIN, OUTPUT);

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
