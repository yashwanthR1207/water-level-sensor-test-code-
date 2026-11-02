#include <Arduino.h>

#define SENSOR_PIN 34
#define READ_INTERVAL_MS 1000
#define NUM_SAMPLES 20

const int SENSOR_DRY = 80;
const int SENSOR_MAX_WET = 2900;
const float REF_VOLTAGE = 3.3;
const int ADC_MAX = 4095;

unsigned long lastReadTime = 0;

int readSmoothedADC(int pin, int samples) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delayMicroseconds(400);
  }
  return (int)(sum / samples);
}

String getLevelStatus(float pct) {
  if (pct <= 5.0) return "EMPTY (Dry)";
  if (pct < 30.0) return "LOW";
  if (pct < 70.0) return "NORMAL";
  if (pct < 90.0) return "HIGH";
  return "FULL / OVERFLOW";
}

void setup() {
  Serial.begin(115200);
  analogSetPinAttenuation(SENSOR_PIN, ADC_11db);
  analogReadResolution(12);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastReadTime >= READ_INTERVAL_MS) {
    lastReadTime = currentMillis;

    int rawADC = readSmoothedADC(SENSOR_PIN, NUM_SAMPLES);
    float voltage = ((float)rawADC / (float)ADC_MAX) * REF_VOLTAGE;
    float percentage = ((float)(rawADC - SENSOR_DRY) / (float)(SENSOR_MAX_WET - SENSOR_DRY)) * 100.0;
    percentage = constrain(percentage, 0.0, 100.0);

    Serial.print("Raw ADC: ");
    Serial.print(rawADC);
    Serial.print(" | Voltage: ");
    Serial.print(voltage, 2);
    Serial.print("V | Level: ");
    Serial.print(percentage, 1);
    Serial.print("% | Status: ");
    Serial.println(getLevelStatus(percentage));
  }
}
