#include <Arduino.h>
#include<DHT.h>
void setup();
void loop();
#line 1 "src/sketch.ino"
/*
 * Program to read temperature and humidity from DHT sensor
 *
 * Configuration:
 *  5V  => First pin sensor
 *  GND => Last pin sensor
 *  AX  => Second pin sensor
 */

//#include<DHT.h>

/* dht(Pin, Sensor Model) */
DHT dht(A1, DHT11);

void setup()
{
    Serial.begin(9600);
    dht.begin();
}

void loop()
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" | Temperature: ");
    Serial.println(temperature);

    delay(2000);
}
