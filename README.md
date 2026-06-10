# Robot Car Arduino

Autonomous 2WD Robot Platform Based on Arduino UNO

## Overview

Robot Car Arduino es una plataforma robótica móvil diseñada para la experimentación con sistemas embebidos, navegación autónoma y control remoto.

El proyecto implementa una arquitectura modular que separa la lógica de negocio, los controladores de hardware y las funcionalidades del robot, permitiendo una mayor mantenibilidad, escalabilidad y reutilización del código.

## Features

### Navigation

* Obstacle Avoidance
* Autonomous Navigation
* Environment Scanning

### Tracking

* Line Following
* Path Correction

### Remote Control

* Bluetooth Communication
* Manual Driving Mode

### Monitoring

* Telemetry System
* Event Tracking

## Hardware

* Arduino UNO
* L298N Motor Driver
* HC-SR04 Ultrasonic Sensor
* SG90 Servo Motor
* IR Line Tracking Sensors
* DC Gear Motors
* Bluetooth Module

## Software Architecture

```text
Core
├── RobotController
├── StateMachine
└── EventManager

Drivers
├── MotorDriver
├── UltrasonicDriver
├── ServoDriver
└── EncoderDriver

Features
├── ObstacleAvoidance
├── LineFollower
├── ManualControl
└── AutonomousMode

Communication
├── Bluetooth
├── WiFi (Future)
└── Telemetry

Config
```

### Core

Central coordination layer responsible for robot control, state management and event handling.

### Drivers

Hardware abstraction layer responsible for sensors, actuators and low-level device control.

### Features

High-level robot capabilities such as obstacle avoidance, line following and autonomous navigation.

### Communication

Wireless communication and telemetry services.

### Config

Global configuration, constants and hardware settings.

## Project Structure

```text
├── docs/
├── assets/
├── src/
├── tests/
├── schematics/
└── README.md
```

For a complete explanation of the project structure, see:

```text
docs/structure.md
```

## Current Capabilities

* Manual Control
* Obstacle Detection
* Obstacle Avoidance
* Line Following
* Autonomous Mode

## Roadmap

### v1.0

* Basic Robot Platform
* Bluetooth Control
* Obstacle Detection

### v1.1

* Obstacle Avoidance System
* Servo Scanning

### v1.2

* Line Following System

### v2.0

* Improved Architecture
* Advanced State Management
* Telemetry

### v3.0

* ESP32 Migration
* WiFi Connectivity
* Web Dashboard

### v4.0

* Camera Integration
* Computer Vision
* Smart Navigation

## License

MIT License
