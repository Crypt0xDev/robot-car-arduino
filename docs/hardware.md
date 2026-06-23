# Hardware Specifications

Complete hardware documentation for Robot Car Arduino platform.

## 📋 Overview

Robot Car Arduino es una plataforma de 2 ruedas (2WD) basada en Arduino UNO R3.
El hardware corresponde al kit "Carro Robot 2WD Multifunción" (todo incluido).

### 📦 Contenido del kit (lista de materiales real)

| Cant. | Componente | Uso en el proyecto |
|-------|-----------|--------------------|
| 1 | Arduino UNO R3 + cable USB | Microcontrolador |
| 1 | Chasis del coche | Estructura |
| 2 | Motorreductor (1:48) | Tracción 2WD |
| 1 | Driver L298N | Control de motores |
| 1 | Módulo Bluetooth HC-05 | Control por celular |
| 1 | Sensor de línea QTR-8A (8 canales) | Seguidor de línea (se usan 5-6) |
| 1 | Ultrasonido HC-SR04 | Detección de obstáculos |
| 1 | Servomotor SG-90 | Mueve el HC-SR04 para escanear |
| 1 | Caja de batería + interruptor | Alojamiento de las 18650 |
| 4 | Batería 18650 Li-ion 4.2V (en serie = 16.8V) | Alimentación (requiere buck, ver abajo) |
| 1 | Convertidor reductor LM2596 *(comprar aparte)* | Baja 16.8V → 7.5V para motores/Arduino |
| 1 | Rueda universal (loca) | Apoyo delantero |
| 1 | Bus de 20 cables dupont M-H | Conexiones |
| 12 / 8 | Tornillos / tuercas | Montaje |

> **Falta comprar:** un convertidor reductor **LM2596**. La batería (4x 18650
> en serie) entrega **16.8V**, que es demasiado para los motores (3-6V) y el
> Arduino; el LM2596 la baja a ~7.5V. Ver
> [wiring.md → Power Distribution](wiring.md#-power-distribution).

> **Nota:** este proyecto **no usa encoders**. Los 3 modos (manual, evasión,
> línea) no los necesitan. Si en el futuro quisieras medir velocidad/distancia
> recorrida, se podrían agregar, pero no forman parte de este armado.

---

## 🧠 Microcontroller

### Arduino UNO R3

**Specifications:**
- **Processor:** ATmega328P
- **Operating Voltage:** 5V
- **Flash Memory:** 32 KB (0.5 KB bootloader)
- **SRAM:** 2 KB
- **EEPROM:** 1 KB
- **Clock Speed:** 16 MHz
- **Analog Inputs:** 6 (A0-A5)
- **Digital I/O:** 14 pins (6 PWM capable)
- **Power Consumption:** ~50 mA (typical)

**Used Pins:**
| Pin | Function | Module |
|-----|----------|--------|
| D2 | Ultrasonic Trigger | HC-SR04 |
| D3 | Servo Control (PWM) | SG90 |
| D4 | Ultrasonic Echo | HC-SR04 |
| D5, D6 | Motor PWM Speed (ENA/ENB) | L298N |
| D7-D10 | Motor Direction (IN1-IN4) | L298N |
| D11, D12 | Serial Communication | HC-05 |
| A0-A4 | Line Sensors | QTR-8A (5 de 8 canales) |

> Mapa de pines completo y oficial en [wiring.md](wiring.md).

---

## 🔌 Motor Driver: L298N

**Purpose:** Dual DC motor driver for 2WD wheel control

**Specifications:**
- **Type:** Dual H-Bridge Motor Driver
- **Operating Voltage:** 5V-35V
- **Output Current:** 2A continuous per channel (peak 3A)
- **PWM Frequency Support:** Up to 40kHz
- **Channels:** 2 (for dual motors)

**Pinout:**
| L298N Pin | Function | Arduino Pin |
|-----------|----------|------------|
| IN1 | Motor A Direction | D4 |
| IN2 | Motor A Direction | D5 |
| IN3 | Motor B Direction | D2 |
| IN4 | Motor B Direction | D3 |
| ENA | Motor A Speed (PWM) | D2 (PWM) |
| ENB | Motor B Speed (PWM) | D3 (PWM) |
| OUT1, OUT2 | Motor A Output | DC Motor Left |
| OUT3, OUT4 | Motor B Output | DC Motor Right |
| GND | Ground | GND |
| +5V | Power | 5V |

**Functionality:**
- Forward/Backward movement
- Left/Right turning
- Variable speed control via PWM (0-255)
- Smooth acceleration/deceleration

---

## 📏 Distance Sensor: HC-SR04

**Purpose:** Ultrasonic distance measurement for obstacle detection

**Specifications:**
- **Detection Range:** 2cm - 400cm
- **Measuring Angle:** 15°
- **Operating Frequency:** 40kHz
- **Operating Voltage:** 5V
- **Operating Current:** 15mA
- **Resolution:** 0.3cm
- **Accuracy:** ±3%

**Pinout:**
| HC-SR04 Pin | Function | Arduino Pin |
|-------------|----------|------------|
| VCC | Power | 5V |
| GND | Ground | GND |
| TRIG | Trigger Pulse | D7 |
| ECHO | Echo Response | D8 |

**Operation:**
1. Send 10µs pulse to TRIG pin
2. HC-SR04 transmits 8 ultrasonic pulses
3. Measures echo return time on ECHO pin
4. Distance = (Echo Time × Speed of Sound) / 2

---

## 🎛️ Servo Motor: SG90

**Purpose:** Ultrasonic sensor scanning and object orientation

**Specifications:**
- **Operating Voltage:** 5V
- **Operating Speed:** 0.1sec/60°
- **Torque:** 2.5kg·cm
- **Rotation Angle:** 0° - 180°
- **Dead Band:** 1µs
- **Pulse Width Range:** 500µs - 2500µs

**Pinout:**
| SG90 Pin | Function | Arduino Pin |
|----------|----------|------------|
| Brown | Ground | GND |
| Red | Power | 5V |
| Orange | Signal | D6 (PWM) |

**Control:**
- PWM Signal: 50Hz (20ms period)
- Pulse Width: 1000µs (0°) to 2000µs (180°)
- Used for HC-SR04 sensor sweep

---

## 📡 Bluetooth Module: HC-05

**Purpose:** Wireless remote control and telemetry communication

**Specifications:**
- **Protocol:** Bluetooth 2.1 (SPP)
- **Operating Voltage:** 3.3V-5V
- **Baud Rate:** 9600 bps (default)
- **Range:** 10-100 meters (depending on antenna)
- **Frequency:** 2.4 GHz ISM band
- **Current:** ~30mA active, ~30µA sleep mode

**Pinout:**
| HC-05 Pin | Function | Arduino Pin |
|-----------|----------|------------|
| VCC | Power | 5V |
| GND | Ground | GND |
| TX | Serial Data Out | D11 (RX) |
| RX | Serial Data In | D12 (TX) |

**Communication Protocol:**
- UART Serial: 9600 baud, 8N1
- One-way: Robot → Mobile App (telemetry)
- Bi-directional: Robot ↔ Controller app

---

## 📍 Line Sensor Array: QTR-8A (del kit)

**Purpose:** Detección de línea por infrarrojo para el modo seguidor de línea.

**Specifications:**
- **Sensors:** array QTR-8A de 8 canales infrarrojos de reflexión
- **Operating Voltage:** 5V
- **Output:** 8 salidas analógicas

> ⚠️ El UNO solo tiene 6 entradas analógicas (A0-A5). Por eso del QTR-8A se
> conectan **5-6 de sus 8 canales** (en este proyecto: 5 canales a A0-A4). Con
> 5 sensores el seguimiento de línea funciona perfectamente; no se necesitan los 8.

**Conexión usada (5 canales):**
| QTR-8A | Función | Arduino Pin |
|--------|---------|-------------|
| VCC | Power | 5V |
| GND | Ground | GND |
| OUT (5 canales repartidos) | Salida analógica | A0, A1, A2, A3, A4 |
| Canales restantes | sin usar | — |

**Layout (5 canales activos, de izquierda a derecha):**
```
   [A0][A1][A2][A3][A4]
   <--- Frente del robot --->
```

**Operation:**
- LED emitter IR light
- Phototransistor detects reflection
- White line: High analog value (~1023)
- Black surface: Low analog value (~0)

---

## ⚙️ DC Gear Motors

**Purpose:** Dual motor drive system for 2WD platform

**Specifications per Motor:**
- **Voltage:** 3V - 6V (optimal 5V)
- **Speed:** ~200 RPM @ 5V (no load)
- **Gear Ratio:** 48:1
- **Torque:** ~0.8 kg·cm @ 5V
- **Current Draw:** ~100mA per motor (no load)
- **Shaft:** 2mm diameter
- **Weight:** ~30g per motor

**Configuration:**
- **Quantity:** 2 motors (left + right)
- **Connection:** Directly to L298N motor driver
- **Control:** PWM speed + direction pins
- **Power:** Drawn from L298N (rated 2A per channel)

---

## 📊 Encoders — no se usan

Este proyecto **no incluye encoders**. La medición de velocidad/distancia por
odometría queda como posible mejora futura, pero no forma parte de este armado
ni del firmware actual.

---

## 🔋 Power System

### Battery Specifications

| Aspect | Specification |
|--------|---------------|
| **Type** | 4x 18650 Li-ion 4.2V en serie (4S) |
| **Voltage** | 16.8V cargadas / 14.8V nominal / ~12V descargadas |
| **Reductor obligatorio** | LM2596 buck → ~7.5V (ver Power Distribution en wiring.md) |
| **Peak Current** | 2.5A (pico de motores) |
| **Charging** | Cargador específico para 18650 (no cargar en serie sin BMS) |

> ⚠️ Los 16.8V NUNCA van directo a motores (3-6V) ni al Arduino. Siempre pasan
> primero por el LM2596.

### Power Budget

| Component | Voltage | Current | Notes |
|-----------|---------|---------|-------|
| Arduino UNO | 5V | 50mA | Baseline + peripherals |
| L298N (Idle) | 5V | 10mA | At rest |
| L298N (2 Motors) | 5V | 200mA | Forward @ medium speed |
| L298N (Peak) | 5V | 300mA | Peak acceleration |
| HC-SR04 (Idle) | 5V | 2mA | Standby |
| HC-SR04 (Active) | 5V | 15mA | During measurement |
| HC-05 | 5V | 30mA | Active communication |
| SG90 | 5V | 5-10mA | Servo holding |
| QTR-8A | 5V | ~40mA | Todos los LEDs encendidos |
| **Total Peak** | - | **~400mA** | All components active |

### Batería usada

- **Configuración:** 4x 18650 Li-ion en serie (4S = 16.8V)
- **Reductor:** LM2596 ajustado a ~7.5V entre batería y robot (obligatorio)
- **Capacidad típica:** 2000-3000mAh por celda
- **Pico soportado:** hasta ~2.5A (motores)

---

## 🔌 Connectors & Wiring

### Connector Types Used

| Connection | Type | Gauge |
|-----------|------|-------|
| Power Distribution | Micro USB or JST | 18AWG |
| Motor Connections | Soldered or M2 connectors | 20AWG |
| Sensor Connections | Pin headers (2.54mm) | 22AWG |
| Servo Connection | 3-pin JST | 22AWG |

### Recommended Cable Specifications

- **Power Lines (5V):** 18-20 AWG, rated 2.5A
- **Signal Lines:** 22 AWG, standard hookup wire
- **Ground Return:** Multiple points recommended

---

## ⚡ Electrical Considerations

### Voltage Regulation
- Arduino UNO includes onboard 5V regulator (1A max)
- External 5V supply recommended for motors
- L298N provides motor isolation from microcontroller

### Current Distribution
- Motors: Draw from external 5V supply → L298N
- Sensors: Draw from Arduino 5V out (max 200mA)
- Logic: Internal Arduino power management

### Protection
- Fuse on main power line (2-3A rating)
- Reverse polarity protection recommended
- Diodes on motor outputs (to prevent spikes)

---

## 📐 Physical Dimensions

### Robot Footprint
- **Length:** ~150mm (including sensor mount)
- **Width:** ~100mm (wheel to wheel)
- **Height:** ~80mm (including servo mount)
- **Weight:** ~400-500g (including battery)

### Component Placement

```
        [Servo + HC-SR04]
              |
    [L298N]---+---[Arduino]
              |
[Motor L]---[Wheels]---[Motor R]
              |
         [Battery]
```

---

## 🔧 Assembly Notes

1. **Motor Installation:** Secure motors to chassis with brackets
2. **Wheel Mounting:** Use M3 screws for wheel hubs
3. **Sensor Positioning:** Mount HC-SR04 at 100mm height
4. **Servo Mounting:** Secure with servo horn at center
5. **Battery Placement:** Low center of gravity for stability
6. **Wiring:** Use cable clips to prevent tangling
7. **Connector Strain Relief:** Provide bend relief at connections

---

## 📚 References

- [Arduino UNO Datasheet](https://store.arduino.cc/products/arduino-uno-rev3)
- [L298N Documentation](https://www.st.com/en/motor-drivers/l298.html)
- [HC-SR04 Datasheet](https://datasheetspdf.com/pdf-file/HC-SR04-Datasheet)
- [SG90 Servo Datasheet](https://servodatabase.com/servo/towerpro/sg90)
- [HC-05 Datasheet](https://datasheetspdf.com/pdf-file/HC-05-Datasheet)
