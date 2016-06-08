/*
 * Program to read temperature and humidity from DHT sensor
 *
 * Configuration:
 *  5V  => First pin sensor
 *  GND => Last pin sensor
 *  AX  => Second pin sensor
 */

#include<DHT.h>

/* dht(Pin, Sensor Model) */
DHT dht1(A1, DHT11);
DHT dht2(A2, DHT11);

void setup()
{
    Serial.begin(9600);
    dht1.begin();
    dht2.begin();
}

void loop()
{
    float humidity1 = dht1.readHumidity();
    float temperature1 = dht1.readTemperature();

    float humidity2 = dht2.readHumidity();
    float temperature2 = dht2.readTemperature();

    Serial.print("Sensor 1 - ");
    Serial.print("Humidity: ");
    Serial.print(humidity1);
    Serial.print(" | Temperature: ");
    Serial.println(temperature1);

    Serial.print("Sensor 2 - ");
    Serial.print("Humidity: ");
    Serial.print(humidity2);
    Serial.print(" | Temperature: ");
    Serial.println(temperature2);

    Serial.println();

    delay(2000);
}
