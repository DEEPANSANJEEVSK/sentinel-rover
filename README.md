# 🤖 SENTINEL ROVER

## AI-Driven Autonomous Multi-Sensor Industrial Inspection Rover

Sentinel Rover is a mobile robotic inspection system designed for industrial environments. The rover is intended to navigate through a factory, approach machines, collect machine-condition parameters, and identify abnormal operating conditions for predictive maintenance.

---

## 🎯 Problem Statement

Industrial factories contain multiple machines distributed across large production areas. Manual inspection of every machine is time-consuming, repetitive, and may not provide continuous monitoring of machine conditions.

Sentinel Rover is designed to provide a mobile platform for automated machine-condition inspection.

---

## 💡 Proposed Solution

The Sentinel Rover combines:

- Mobile robotic movement
- Obstacle detection
- Machine-condition sensing
- Multi-sensor monitoring
- LCD-based health indication
- Vibration analysis
- Acoustic analysis
- Temperature monitoring
- Motor-current monitoring

The rover can move through the inspection environment and collect condition data from machines.

---

## 🏗️ Hardware Components

### Controller
- Arduino UNO

### Motor Control
- L298N Motor Driver
- 4 × DC Motors
- Rover chassis and wheels

### Sensors
- HC-SR04 Ultrasonic Sensor
- Temperature Sensor
- Vibration Sensor
- Current Sensor
- Acoustic / Sound Sensor

### Display
- I2C LCD Display

### Power
- Battery / DC Power Supply
- Buck Converter

---

## ⚙️ System Architecture

```text
              SENTINEL ROVER
                    |
                    V
              ARDUINO UNO
                    |
        +-----------+-----------+
        |           |           |
        V           V           V
    SENSORS     MOTOR DRIVER    LCD
        |           |
        |           V
        |        DC MOTORS
        |           |
        V           V
 MACHINE DATA    ROVER MOVEMENT
        |
        V
 SENSOR ANALYSIS
        |
        V
 MACHINE HEALTH
        |
        V
 HEALTH STATUS
