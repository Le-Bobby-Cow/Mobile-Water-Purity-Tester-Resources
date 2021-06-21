/*-----------------------------------------------------------------------------------------
  Manager for the pH Probe

  Sensor Connected to Pin A2 of Arduino

  ------------------------------------------------------------------------------------------*/
#define SensorPin A2
#define Offset 0.00
#define samplingInterval 20
#define numReadings 10

float pHArray[numReadings];

float calibration_value = 21.2;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

float ph_act;


void setuppH() {
}

float averageArr() {
  float avgVal = 0;
  for (int i = 0; i < numReadings; i++) {
    avgVal += pHArray[i];
  }
  avgVal = avgVal / numReadings;
  return avgVal;
}

float readpH() {
  float voltage;
  for (int i = 0; i < numReadings; i++) {
    pHArray[i] = float(analogRead(SensorPin)) * analogConstant;
    delay(samplingInterval);
  }
  voltage = averageArr() * (5.0 / 1024);
  return voltage;
}

float calculatepH() {
  //  float pHValue;
  //  pHValue = (3.5 * readpH()) + Offset;
  //  return pHValue;

  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = float(analogRead(SensorPin)) * analogConstant;
    delay(5);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1024 / 6;
  volt -= 0.22;
  //Serial.println(volt);
  ph_act = -5.70 * volt + calibration_value;
  //Serial.println(ph_act);
  return ph_act;
}
