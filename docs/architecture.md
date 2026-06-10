# Robot Car Project Architecture

```text
├── Core
│   ├── RobotController
│   ├── StateMachine
│   └── EventManager
│
├── Drivers
│   ├── MotorDriver
│   ├── UltrasonicDriver
│   ├── ServoDriver
│   └── EncoderDriver
│
├── Features
│   ├── ObstacleAvoidance
│   ├── LineFollower
│   ├── ManualControl
│   └── AutonomousMode
│
├── Communication
│   ├── Bluetooth
│   ├── WiFi (Future)
│   └── Telemetry
│
└── Config
```

## Core

Contiene la lógica central del robot y coordina todos los módulos del sistema.

### RobotController

Controlador principal del robot.

Responsabilidades:

* Inicializar componentes.
* Coordinar sensores y actuadores.
* Gestionar modos de operación.
* Ejecutar el ciclo principal del sistema.

### StateMachine

Gestiona los estados del robot.

Ejemplos:

* Idle
* Manual Control
* Obstacle Avoidance
* Line Following
* Autonomous Navigation

Permite cambiar de comportamiento de forma organizada y escalable.

### EventManager

Sistema de eventos internos.

Responsabilidades:

* Detectar eventos.
* Notificar cambios de estado.
* Coordinar la comunicación entre módulos.

Ejemplos:

* ObstacleDetected
* LineLost
* BluetoothConnected
* ModeChanged

## Drivers

Capa de acceso directo al hardware.

### MotorDriver

Control de motores DC.

Funciones:

* Forward
* Backward
* Left
* Right
* Stop
* Speed Control

### UltrasonicDriver

Control del sensor HC-SR04.

Funciones:

* Medición de distancia.
* Detección de obstáculos.

### ServoDriver

Control del servomotor SG90.

Funciones:

* Movimiento angular.
* Escaneo del entorno.

### EncoderDriver

Lectura de encoders.

Funciones:

* Velocidad.
* Distancia recorrida.
* Retroalimentación para navegación.

## Features

Funcionalidades de alto nivel del robot.

### ObstacleAvoidance

Sistema de evasión de obstáculos.

Capacidades:

* Escaneo frontal.
* Detección de obstáculos.
* Selección de ruta libre.

### LineFollower

Sistema de seguimiento de línea.

Capacidades:

* Lectura de sensores IR.
* Corrección automática de trayectoria.

### ManualControl

Control remoto del robot.

Capacidades:

* Movimiento manual.
* Control de velocidad.
* Comandos remotos.

### AutonomousMode

Modo completamente autónomo.

Capacidades:

* Navegación automática.
* Toma de decisiones.
* Integración de sensores.

## Communication

Comunicación externa del sistema.

### Bluetooth

Comunicación inalámbrica actual.

Capacidades:

* Recepción de comandos.
* Control desde aplicación móvil.

### WiFi (Future)

Expansión futura mediante ESP32.

Capacidades previstas:

* Dashboard Web.
* API REST.
* WebSockets.
* OTA Updates.

### Telemetry

Sistema de monitoreo.

Información enviada:

* Estado actual.
* Velocidad.
* Distancia.
* Modo activo.
* Eventos del sistema.

## Config

Configuración global del proyecto.

Contiene:

* Definición de pines.
* Constantes del sistema.
* Umbrales de sensores.
* Parámetros de navegación.
* Configuración de hardware.
