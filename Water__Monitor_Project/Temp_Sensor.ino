/*-----------------------------------------------------------------------------------------
  Manager for the Temperature Probe

  Sensor Connected to Pin 8 of Arduino

  ------------------------------------------------------------------------------------------*/
#include <OneWire.h>
#include <DallasTemperature.h>

#define TemperatureBusPin 8
int counter = 0;

OneWire oneWire(TemperatureBusPin);
DallasTemperature sensors(&oneWire);

void setupTemp() {
  sensors.begin();
  sensors.setWaitForConversion(false);
}

void readTemperatures() {
  if (counter == 0) {
    sensors.requestTemperatures();
  } else if (counter == 2) {
    counter = 0;
  }
  counter++;
}

float generateTemperatureC() {
  if (counter == 0) {
    sensors.requestTemperatures();
    //Serial.println(sensors.getTempCByIndex(0));
    //counter++;
    return sensors.getTempCByIndex(0);
  } else if (counter == 2) {
    counter = 0;
  } else {
    counter++;
    //Serial.println(counter);
    return TempVal;
  }
}

float generateTemperatureF() {
  if (counter == 0) {
    sensors.requestTemperatures();
  } else if (counter == 2) {
    counter = 0;
  }
  counter++;
  return ((sensors.getTempCByIndex(0)) * 9.0 / 5.0) + 32.0;
}
