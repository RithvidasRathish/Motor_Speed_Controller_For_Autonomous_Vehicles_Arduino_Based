
 
#include <Adafruit_Sensor.h>
#include <DHT.h>
// #include <DHT_U.h>
#include <TaskScheduler.h>

#define DHTPIN D4

#define IRSensor A1
#define ENABLE_PIN 9    // Pin to enable the motor driver
#define IN1_PIN 8       // Pin connected to IN1 of L293D
#define IN2_PIN 7 


#define DHTTYPE    DHT11   

int sensorStatus=0;
int a=0;
int temp=0;


DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
// Callback methods prototypes
void t1Callback();
void t2Callback();
// void t3Callback();

//Tasks
Task t4();
Task t1(2000, TASK_FOREVER , &t1Callback);
Task t2(3000, TASK_FOREVER, &t2Callback);
// Task t3(5000, TASK_FOREVER, &t3Callback);

Scheduler runner;


void t1Callback() {
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    temp=event.temperature;
    Serial.print(temp);
    Serial.println(F("°C"));
  }

    Serial.print("t1: ");
    Serial.println(millis());
    
    if (temp>33) {
      t2.disable();
      a=170;
      Serial.println("Motor is too hot");
    }
    else {
      a=255;
      t2.enable();
      Serial.println("Motor is in Normal Temperature");
    }
    
    // if (temp) {
      
    //   // runner.deleteTask(t3);
    //   t2.setInterval(500);
    //   Serial.println("t1: disable t3 and delete it from the chain. t2 interval set to 500");
    // }
}

void t2Callback() {
  sensorStatus = digitalRead(IRSensor); 
  
  if (sensorStatus == 1) // Check if the pin high or not
  {
    // if the pin is high turn off the onboard Led
    a=255; // LED LOW
    Serial.println("Obstacle is not Detected"); // print Motion Detected! on the serial monitor window
  }
  else  {
    //else turn on the onboard LED
    a=150; // LED High
    Serial.println("Obstacle is Deteced"); // print Motion Ended! on the serial monitor window
  }

    Serial.print("t2: ");
    Serial.println(millis());
  
}

void t3Callback() {
  
    Serial.print("t3: ");
    Serial.println(millis());
    
  
}

void setup () {
  Serial.begin(115200);
  pinMode(IRSensor, INPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  
  delayMS = sensor.min_delay / 1000;
  Serial.println("Scheduler TEST");
  
  runner.init();
  Serial.println("Initialized scheduler");
  
  runner.addTask(t1);
  Serial.println("added t1");
  
  runner.addTask(t2);
  Serial.println("added t2");

  delay(5000);
  
  t1.enable();
  Serial.println("Enabled t1");
  t2.enable();
  Serial.println("Enabled t2");
}


void loop () {
  runner.execute();
  analogWrite(ENABLE_PIN,a);
  digitalWrite(IN1_PIN, HIGH); // Set direction
  digitalWrite(IN2_PIN, LOW);
  delay(1000);  // Rotate for 3 seconds
  

}