#include <NilRTOS.h>

#include <NilSerial.h>

#define Serial NilSerial

// The LED is attached to pin 13 on Arduino.
const uint8_t LED_PIN = 13;

/*
 * Check temperature and humidity task
 */
// Stack with 128 bytes beyond context switch and interrupt needs
NIL_WORKING_AREA(waThread1, 128);

// Thread function for check temperature and humidity task
NIL_THREAD(Thread1, arg){
    while (TRUE) {
        // Turn LED off.
        digitalWrite(LED_PIN, LOW);

        // Sleep for 200 miliseconds
        nilThdSleepMilliseconds(500);

        // Turn LED off.
        digitalWrite(LED_PIN, HIGH);

        Serial.println("Thread 1");

        // Sleep for 200 miliseconds
        nilThdSleepMilliseconds(1000);
    }
}

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

/*
 * Threads static table, one entry per thread.  A thread's priority is
 * determined by its position in the table with highest priority first.
 * 
 */
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, Thread1, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_END()

void setup(){
    Serial.begin(9600);
    // Start Nil RTOS
    nilSysBegin();
}

// Loop is the idle thread. The idle thread must not invoke any 
// kernel primitive able to change its state to not runnable.
void loop(){
    // Not used
}
