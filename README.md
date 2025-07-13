# 🌡️ Motor_Speed_Controller_For_Autonomous_Vehicles_Arduino_Based

A temperature- and obstacle-aware smart motor control system built using a STM32 microcontroller. The system dynamically adjusts motor speed based on DHT11 temperature readings and IR sensor-based obstacle detection using periodic tasks scheduled by the TaskScheduler library.

---

## 📌 Features

- Reads temperature data from a DHT11 sensor.
- Detects obstacles using an IR sensor.
- Controls DC motor speed using PWM (via L293D motor driver).
- Uses TaskScheduler for non-blocking sensor management.
- Automatically slows down motor if:
  - Temperature exceeds safe threshold.
  - Obstacle is detected.

---

## ⚙️ Hardware Requirements

- NodeMCU / ESP8266 or compatible board
- DHT11 temperature sensor
- IR obstacle detection sensor
- L293D motor driver IC or module
- DC Motor (5V/12V)
- External power supply (for motor)
- Breadboard and jumper wires

---

## 🧠 Software Requirements

- Arduino IDE
- Board support for ESP8266
- Libraries:
  - `Adafruit Unified Sensor`
  - `DHT sensor library`
  - `TaskScheduler` by Arkhipenko

You can install these libraries via the Arduino Library Manager.

---

## 📐 Circuit Diagram

| Pin                | Connection                  |
|--------------------|-----------------------------|
| `D4` (GPIO2)       | DHT11 Data                  |
| `A1`               | IR Sensor Output            |
| `D9` (GPIO9)       | L293D Enable Pin (PWM)      |
| `D8`, `D7`         | L293D IN1 and IN2           |
| Motor              | L293D OUT1 and OUT2         |
| VCC & GND          | Common Ground and 5V Power  |

> Make sure the motor is powered using an external power supply if required.

---

## 🧪 How It Works

- **Task 1 (Every 2s)**: 
  - Reads the temperature.
  - If temperature > 33°C, motor slows down and IR task is disabled.
  - Else, IR obstacle task is enabled, and motor runs at full speed.
- **Task 2 (Every 3s)**: 
  - Reads the IR sensor.
  - If obstacle detected, motor speed is reduced.
  - Else, motor runs normally.

---

## 🚀 Getting Started

1. Clone this repository.
2. Open the `.ino` file in Arduino IDE.
3. Select the correct board and port.
4. Upload the code.
5. Monitor output using the Serial Monitor at 115200 baud.

---

## 📸 Demo

> *(Include image or video link of the system running, if available)*

---

## 🙋‍♂️ Author

**Rithvidas Rathish**  
Embedded Systems | Firmware | Edge AI  
📧 Reach me on LinkedIn or GitHub for collaborations!
https://www.linkedin.com/in/rithvidas-rathish-embedded-firmware-developer/
---


