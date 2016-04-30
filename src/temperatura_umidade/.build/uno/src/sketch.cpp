#include <Arduino.h>
#include<DHT.h>
void setup();
void loop();
#line 1 "src/sketch.ino"
//#include<DHT.h>

/* Pin - Sensor Model */
DHT dht(A1,DHT11);

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
