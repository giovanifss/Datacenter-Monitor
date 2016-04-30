/*
 * Program to read/use rain sensor
 * Rain sensor: FC-37 
 */

/* Pin used for read from sensor */
#define SENSOR_PIN A0

int sensorValue = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    sensorValue = analogRead(SENSOR_PIN);
    
    Serial.print("Reading humidity: ");
    Serial.println(sensorValue);

    if (sensorValue < 700){
        Serial.println("Warning!!! Flood detected!!!");
    }

    delay(2000);
}
