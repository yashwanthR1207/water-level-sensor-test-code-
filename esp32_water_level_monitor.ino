/**
 * @file esp32_water_level_monitor.ino
 * @brief ESP32 Water Level Monitoring Test Code
 * @author Yashwanth
 * @details Reads analog water level sensor data using GPIO 34 (ADC1_CH6).
 *          Includes multi-sample noise filtering, voltage conversion,
 *          percentage calculation, and level status categorization.
 * 
 * Pin Connections:
 * - Sensor VCC (Power) -> ESP32 3.3V (Recommended to stay within ADC range)
 * - Sensor GND (Ground)-> ESP32 GND
 * - Sensor SIG (Signal)-> ESP32 GPIO 34 (ADC1_CH6 - Input-only pin)
 */

#include <Arduino.h>

// ==========================================
// 1. Pin & Timing Configuration
// ==========================================
#define SENSOR_PIN          34    // GPIO 34 (ADC1_CH6, input only)
#define READ_INTERVAL_MS    1000  // Reading interval in milliseconds
#define NUM_SAMPLES         20    // Number of samples for smoothing / oversampling

// ==========================================
// 2. Calibration Thresholds
// ==========================================
// Adjust these values based on your specific water sensor & liquid type.
// 1) Test with sensor completely dry in air -> Record raw ADC -> Set SENSOR_DRY
// 2) Submerge sensor to maximum desired depth -> Record raw ADC -> Set SENSOR_MAX_WET
const int SENSOR_DRY     = 80;    // Baseline raw ADC reading when completely dry
const int SENSOR_MAX_WET = 2900;  // Raw ADC reading when fully submerged

// ADC and Voltage Constants
const float REF_VOLTAGE  = 3.3;   // ESP32 ADC reference voltage (3.3V)
const int   ADC_MAX      = 4095;  // 12-bit ADC max value (0 to 4095)

// Non-blocking timer variable
unsigned long lastReadTime = 0;

// ==========================================
// 3. Helper Functions
// ==========================================

/**
 * @brief Reads the ADC pin multiple times and averages the results to reduce noise.
 * @param pin The analog GPIO pin number.
 * @param samples The count of samples to average.
 * @return Smoothed raw ADC value (0 - 4095).
 */
int readSmoothedADC(int pin, int samples) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delayMicroseconds(400); // Short delay for ADC stabilization
  }
  return (int)(sum / samples);
}

/**
 * @brief Categorizes water level percentage into human-readable alerts.
 * @param pct Water level percentage (0.0 to 100.0).
 * @return String description of the water level state.
 */
String getLevelStatus(float pct) {
  if (pct <= 5.0)   return "EMPTY (Dry)";
  if (pct < 30.0)   return "LOW";
  if (pct < 70.0)   return "NORMAL / MEDIUM";
  if (pct < 90.0)   return "HIGH";
  return "FULL / OVERFLOW WARNING";
}

// ==========================================
// 4. Arduino Setup
// ==========================================
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial && millis() < 2500); // Allow USB Serial to enumerate

  Serial.println();
  Serial.println(F("=================================================="));
  Serial.println(F("     ESP32 Water Level Monitoring Test Rig        "));
  Serial.println(F("=================================================="));
  Serial.print(F("Sensor Pin: GPIO "));
  Serial.println(SENSOR_PIN);

  // Configure ADC attenuation (11dB attenuation gives full 0 - 3.3V range)
  analogSetPinAttenuation(SENSOR_PIN, ADC_11db);
  
  // Explicitly set 12-bit resolution (0 - 4095)
  analogReadResolution(12);

  Serial.println(F("ADC configured: 12-bit (0-4095), 11dB attenuation (~0 - 3.3V)"));
  Serial.println(F("Status: Starting real-time monitoring..."));
  Serial.println(F("--------------------------------------------------"));
}

// ==========================================
// 5. Arduino Main Loop
// ==========================================
void loop() {
  unsigned long currentMillis = millis();

  // Non-blocking interval check
  if (currentMillis - lastReadTime >= READ_INTERVAL_MS) {
    lastReadTime = currentMillis;

    // 1. Read smoothed analog value
    int rawADC = readSmoothedADC(SENSOR_PIN, NUM_SAMPLES);

    // 2. Calculate approximate input voltage
    float voltage = ((float)rawADC / (float)ADC_MAX) * REF_VOLTAGE;

    // 3. Compute percentage based on calibrated dry and wet thresholds
    float percentage = ((float)(rawADC - SENSOR_DRY) / (float)(SENSOR_MAX_WET - SENSOR_DRY)) * 100.0;
    percentage = constrain(percentage, 0.0, 100.0);

    // 4. Get status classification
    String status = getLevelStatus(percentage);

    // 5. Output formatted telemetry to Serial Monitor
    Serial.print(F("["));
    Serial.print(currentMillis / 1000);
    Serial.print(F("s] Raw ADC: "));
    if (rawADC < 1000) Serial.print(F(" "));
    if (rawADC < 100)  Serial.print(F(" "));
    Serial.print(rawADC);

    Serial.print(F(" | Voltage: "));
    Serial.print(voltage, 2);
    Serial.print(F("V | Level: "));
    if (percentage < 10.0) Serial.print(F(" "));
    Serial.print(percentage, 1);
    Serial.print(F("% | Status: "));
    Serial.println(status);
  }
}
