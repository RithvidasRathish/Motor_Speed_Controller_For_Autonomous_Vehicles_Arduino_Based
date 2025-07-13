/**
 * @file smart_motor_control.ino
 * @brief Intelligent motor control system using DHT11 temperature and IR obstacle detection
 * 
 * @author 
 * Rithvidas Rathish
 * 
 * @details
 * This sketch uses a DHT11 sensor to monitor temperature and an IR sensor to detect obstacles.
 * Based on temperature and obstacle data, the motor speed is adjusted using PWM.
 * TaskScheduler library is used to periodically check sensor data.
 */

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <TaskScheduler.h>

// ===================== Pin Definitions =====================
#define DHTPIN D4           ///< Pin connected to DHT11 sensor
#define IRSensor A1         ///< Pin connected to IR obstacle sensor
#define ENABLE_PIN 9        ///< Motor driver enable pin (PWM)
#define IN1_PIN 8           ///< Motor driver IN1 pin
#define IN2_PIN 7           ///< Motor driver IN2 pin

#define DHTTYPE DHT11       ///< Define sensor type for DHT

// ===================== Global Variables =====================
int sensorStatus = 0;
int a = 0;                  ///< PWM value for motor control
int temp = 0;               ///< Current temperature reading

DHT_Unified dht(DHTPIN, DHTTYPE);  ///< DHT11 sensor object
uint32_t delayMS;                  ///< Delay based on sensor spec

// ===================== Task Callbacks =====================
/**
 * @brief Reads temperature and adjusts motor control based on thresholds.
 */
void t1Callback();

/**
 * @brief Reads IR sensor to detect obstacle and adjusts motor control accordingly.
 */
void t2Callback();

// ===================== Tasks =====================
Task t4();  // Placeholder (not used)
Task t1(2000, TASK_FOREVER , &t1Callback);
Task t2(3000, TASK_FOREVER, &t2Callback);

Scheduler runner;

// ===================== Task Implementations =====================

void t1Callback() {
  delay(delayMS);  // Wait based on sensor timing requirement

  sensors_event_t event;
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    temp = event.temperature;
    Serial.print(F("Temperature: "));
    Serial.print(temp);
    Serial.println(F("°C"));
  }

  if (temp > 33) {
    t2.disable();
    a = 170;
    Serial.println("Motor is too hot");
  } else {
    a = 255;
    t2.enable();
    Serial.println("Motor is in Normal Temperature");
  }

  Serial.print("t1: ");
  Serial.println(millis());
}

void t2Callback() {
  sensorStatus = digitalRead(IRSensor); 
  
  if (sensorStatus == 1) {
    a = 255;
    Serial.println("Obstacle is not Detected");
  } else {
    a = 150;
    Serial.println("Obstacle is Detected");
  }

  Serial.print("t2: ");
  Serial.println(millis());
}

// ===================== Setup =====================
void setup() {
  Serial.begin(115200);

  pinMode(IRSensor, INPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Initialized"));

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  runner.init();
  runner.addTask(t1);
  runner.addTask(t2);

  delay(5000);  // Wait for sensors to stabilize

  t1.enable();
  t2.enable();

  Serial.println("Scheduler initialized and tasks enabled");
}

// ===================== Main Loop =====================
void loop() {
  runner.execute();

  analogWrite(ENABLE_PIN, a);         // Set motor speed
  digitalWrite(IN1_PIN, HIGH);        // Set motor direction
  digitalWrite(IN2_PIN, LOW);

  delay(1000);  // Maintain motion for stability
}
