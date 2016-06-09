/*
 * Program to read temperature and humidity from DHT sensor
 *
 * Configuration:
 *  5V  => First pin sensor
 *  GND => Last pin sensor
 *  AX  => Second pin sensor
 */

#include<DHT.h>

#define QTD_SENSORS 3

/* dht(Pin, Sensor Model) */
DHT sensors[] = {
    DHT(A1, DHT11),
    DHT(A2, DHT11),
    DHT(A3, DHT11)
};

void setup()
{
    Serial.begin(9600);

    for (unsigned char i = 0; i < QTD_SENSORS; i++) {
        sensors[i].begin(); 
    }
}

void loop()
{
    for (unsigned char i = 0; i < QTD_SENSORS; i++) {
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(" - Humidity: ");
        Serial.print(sensors[i].readHumidity());
        Serial.print(" | Temperature: ");
        Serial.println(sensors[i].readTemperature());
    }

    Serial.println();

    delay(2000);
}
