/*-----------------------------------------------------------------------------------------
  Manager for the Turbidity Probe

  Sensor Connected to Pin A0 of Arduino

  ------------------------------------------------------------------------------------------*/
#define SensorPin A0

float voltage = 0;
float sensorValue = 0;

float NTUs[5];
int counts = 0;

void setupTurb() {
}

float readTurbidity() {
  sensorValue = float(analogRead(SensorPin)) * analogConstant;
  return sensorValue;
}

float calculateTurbidity() {
  float NTU;
  voltage = readTurbidity() * (5.0 / 1024.0);
  //Serial.println(voltage);
  if (voltage < 2.5) {
    NTU = 3000;
  } else {
    NTU = -1120.4 * square(voltage) + 5742.3 * voltage - 4353.8;
  }
  if (counts < 5) {
    counts++;
    for (int i = 0; i < counts - 1; i++) {
      NTUs[i + 1] = NTUs[i];
    }
    NTUs[0] = NTU;
    float total = 0;
    for (int i = 0; i < counts; i++) {
      total += NTUs[i];
    }
    NTU = total / counts;
  } else {
    for (int i = 0; i < 4; i++) {
      NTUs[i + 1] = NTUs[i];
    }
    NTUs[0] = NTU;
    float total = 0;
    for (int i = 0; i < 5; i++) {
      total += NTUs[i];
    }
    NTU = total / 5;
  }
  return constrain(NTU, 0.0, 3000.0);
}
