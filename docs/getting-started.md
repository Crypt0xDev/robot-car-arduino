# Getting Started

Guía para armar y poner en marcha el Robot Car (kit "Carro Robot 2WD
Multifunción"). Sigue las secciones en orden: armado → cableado → carga de
sketches.

> Lista completa de componentes del kit en [hardware.md](hardware.md).
> Mapa de pines oficial en [wiring.md](wiring.md).

---

## 🔧 Armado del kit (paso a paso)

### Paso A — Montaje mecánico del chasis

1. **Motores a las ruedas:** fija los 2 motorreductores al chasis con los
   sujetadores y tornillos. Conecta cada motor a una rueda.
2. **Rueda loca:** atornilla la rueda universal en la parte delantera (da
   apoyo; no tiene motor).
3. **Caja de batería:** monta la caja de pilas y el interruptor de encendido
   en el chasis. NO pongas las pilas todavía.
4. **Arduino + L298N:** fija el Arduino UNO y el driver L298N sobre el chasis
   (cinta doble cara, tornillos o separadores).
5. **Sensores:**
   - El **QTR-8A** va **debajo del chasis**, cerca del suelo, mirando hacia
     abajo (para ver la línea).
   - El **HC-SR04** va montado sobre el **servo SG-90**, al frente, mirando
     hacia adelante (para escanear obstáculos).

### Paso B — Conexiones eléctricas

Sigue exactamente el mapa de [wiring.md](wiring.md). Resumen:

| Módulo | Pines Arduino |
|--------|---------------|
| L298N (motores) | ENA=D5, ENB=D6, IN1=D7, IN2=D8, IN3=D9, IN4=D10 |
| HC-SR04 | TRIG=D2, ECHO=D4 |
| Servo SG-90 | señal=D3 |
| Bluetooth HC-05 | TX→D11, RX→D12 (con divisor de voltaje en RX) |
| QTR-8A | 5 canales → A0, A1, A2, A3, A4 |

⚠️ **Reglas de oro del cableado:**
- **GND común**: la batería, el **LM2596**, el L298N y el Arduino deben
  compartir tierra.
- La batería **nunca** va directo al L298N: pasa primero por el **LM2596**
  (7.5V), y esa salida alimenta el VS del L298N.
- **Quita los jumpers de ENA y ENB** del L298N; si no, no hay control de
  velocidad (van a tope fijo).
- El pin **RX del HC-05 es de 3.3V**: usa un divisor de voltaje (1kΩ + 2kΩ)
  desde D12, o el módulo puede dañarse.
- El **servo** da picos de corriente: si el Arduino se reinicia al moverlo,
  aliméntalo de un 5V con más corriente (no solo del L298N).

### Paso C — Batería y encendido

⚠️ La batería es **4× 18650 Li-ion (4.2V) en serie = 16.8V**. Ese voltaje es
demasiado alto para los motores (3-6V) y el Arduino. **Necesitas un convertidor
reductor LM2596** ajustado a ~7.5V entre la batería y el robot.
Ver el esquema completo en [wiring.md → Power Distribution](wiring.md#-power-distribution).

1. Carga las 18650 y colócalas en el portapilas (en serie).
2. Ajusta el **LM2596 a 7.5V con un multímetro** ANTES de conectar el robot.
3. Deja el interruptor **apagado** hasta terminar de revisar el cableado.
4. Primer encendido: **levanta el robot sobre un soporte** (ruedas al aire)
   antes de probar los motores.

> ⚠️ Las 18650 mal usadas son peligrosas: no las cortocircuites, no inviertas
> la polaridad y usa un cargador adecuado.

---

## ▶️ Carga de sketches (en orden)

El código está en [`firmware/`](../firmware/README.md). **Prueba cada sketch
antes de pasar al siguiente** — si los motores no andan bien en el paso 1, los
modos autónomos tampoco funcionarán.

1. `01_motor_test` — confirma que los motores giran bien (y en el sentido correcto).
2. `02_ultrasonic_test` — confirma que mide distancia (Monitor Serie a 9600).
3. `03_servo_test` — confirma que el servo apunta izq/centro/der.
4. `04_obstacle_avoidance` — evasión autónoma (los tres juntos).
5. `05_bluetooth_control` — control manual desde el celular.
6. `06_line_follower` — seguidor de línea (calibra el umbral primero).
7. `robot_car` — **firmware integrado**: todos los modos, cambias con `0/1/2/3`
   por Bluetooth.

Para subir cualquiera: ábrelo en el Arduino IDE → `Herramientas → Placa →
Arduino Uno` → selecciona el puerto → **Upload (→)**.

---

## 📋 Prerequisites

Before starting, ensure you have:

- **Hardware:** Complete robot assembly (see [hardware.md](hardware.md))
- **Software:** Arduino IDE 1.8.x or newer
- **Connectivity:** USB cable for Arduino
- **Knowledge:** Basic C++ programming, Arduino concepts

### System Requirements

| Requirement | Specification |
|-------------|---------------|
| **OS** | Windows, macOS, or Linux |
| **Arduino IDE** | v1.8.13 or higher |
| **USB Cable** | USB Type-B (standard Arduino cable) |
| **Board Package** | Arduino AVR Boards |
| **Memory** | 32 KB flash minimum |

---

## 🚀 Installation Steps

### Step 1: Install Arduino IDE

1. Download from [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. Install for your operating system
3. Launch Arduino IDE

### Step 2: Install Board Package

1. Open **Boards Manager** (Tools → Board → Boards Manager)
2. Search for "Arduino AVR Boards"
3. Click Install (normalmente ya viene preinstalado en el IDE)
4. Restart Arduino IDE

> Nota: el paquete "Arduino AVR Boards" (para el UNO) ya viene integrado en el
> Arduino IDE; no necesitas agregar ninguna URL extra. La URL de ESP32 solo
> aplica cuando migres a esa placa (roadmap v3).

### Step 3: Select Board & Port

1. **Select Board:** Tools → Board → Arduino Uno
2. **Select Port:** Tools → Port → COM# (or /dev/ttyUSB0)
3. **Verify:** Upload → Sketch → Verify

### Step 4: Install Libraries

Required libraries:

```
1. Servo (built-in)
2. SoftwareSerial (built-in)
3. (Additional as needed)
```

To install:
1. Sketch → Include Library → Manage Libraries
2. Search for library name
3. Click Install (by Arduino)

---

## 📝 First Program: Blink Test

Verify Arduino and IDE setup:

```cpp
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Arduino Ready!");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
```

**Steps:**
1. Copy code above into Arduino IDE
2. Click Verify (checkmark icon)
3. Connect Arduino to USB
4. Click Upload (arrow icon)
5. Watch LED blink on/off
6. Open Serial Monitor (Tools → Serial Monitor)
7. Should see "Arduino Ready!" message

**Expected Result:** ✅ LED blinks every 1 second

---

## 🔧 Hardware Connection Verification

### Pre-Assembly Checklist

- [ ] All components present and functional
- [ ] Solder joints clean and secure
- [ ] No visible damage or burns
- [ ] Wires properly sized (18-20 AWG for power)
- [ ] Connectors crimped or soldered correctly

### Post-Assembly Verification

1. **Visual Inspection**
   - No loose wires
   - No crossed connections
   - Battery secured safely
   - All sensors firmly mounted

2. **Continuity Testing** (with multimeter)
   - Check ground connections (all points connected)
   - Verify power rail continuity
   - Test signal lines for shorts

3. **No-Power Testing**
   - Upload simple test sketch
   - Don't supply motor power yet
   - Verify Arduino responds

See [wiring.md](wiring.md) for detailed connection guide.

---

## 🎮 First Motor Control

Simple forward/backward test:

```cpp
// Motor pins (L298N) - cada pin se usa una sola vez (sin conflictos)
// PWM validos en UNO: 3, 5, 6, 9, 10, 11 -> ENA/ENB van en 5 y 6.
const int MOTOR_LEFT_PWM  = 5;  // ENA
const int MOTOR_LEFT_DIR1 = 7;  // IN1
const int MOTOR_LEFT_DIR2 = 8;  // IN2
const int MOTOR_RIGHT_PWM  = 6;  // ENB
const int MOTOR_RIGHT_DIR1 = 9;  // IN3
const int MOTOR_RIGHT_DIR2 = 10; // IN4

void setup() {
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_LEFT_DIR1, OUTPUT);
  pinMode(MOTOR_LEFT_DIR2, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR1, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Forward 2 seconds
  moveForward(200);
  delay(2000);

  // Stop 1 second
  stop();
  delay(1000);

  // Backward 2 seconds
  moveBackward(200);
  delay(2000);

  // Stop 1 second
  stop();
  delay(1000);
}

void moveForward(int speed) {
  digitalWrite(MOTOR_LEFT_DIR1, HIGH);
  digitalWrite(MOTOR_LEFT_DIR2, LOW);
  analogWrite(MOTOR_LEFT_PWM, speed);

  digitalWrite(MOTOR_RIGHT_DIR1, HIGH);
  digitalWrite(MOTOR_RIGHT_DIR2, LOW);
  analogWrite(MOTOR_RIGHT_PWM, speed);

  Serial.println("Moving forward");
}

void moveBackward(int speed) {
  digitalWrite(MOTOR_LEFT_DIR1, LOW);
  digitalWrite(MOTOR_LEFT_DIR2, HIGH);
  analogWrite(MOTOR_LEFT_PWM, speed);

  digitalWrite(MOTOR_RIGHT_DIR1, LOW);
  digitalWrite(MOTOR_RIGHT_DIR2, HIGH);
  analogWrite(MOTOR_RIGHT_PWM, speed);

  Serial.println("Moving backward");
}

void stop() {
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
  Serial.println("Stopped");
}
```

**Safety First:**
- Lift robot off ground (testing stand)
- Start with low speed (150-200)
- Have power kill switch nearby
- Clear test area of obstacles

---

## 🎯 Running Test Suite

See [testing.md](testing.md) for comprehensive test procedures:

1. **Motor Tests:** Forward, backward, turning
2. **Sensor Tests:** Ultrasonic, IR sensors
3. **Communication Tests:** Bluetooth connectivity
4. **System Tests:** Full integration

---

## 📡 Bluetooth Connection Setup

### Android Phone Setup

1. **Enable Bluetooth** on phone
2. **Pair Device:**
   - Settings → Bluetooth
   - Scan for devices
   - Select "HC-05"
   - Enter PIN (default: 1234)
3. **Download App:**
   - Search "Arduino Bluetooth Terminal"
   - Install from Play Store
4. **Connect:**
   - Open app
   - Select HC-05
   - Send test commands

### Commands

```
Command Format: [MODE][VALUE]
Examples:
  F200 = Forward, speed 200
  B150 = Backward, speed 150
  L100 = Left turn, speed 100
  R100 = Right turn, speed 100
  S0   = Stop
```

---

## ⚙️ Configuration

### Customize Pin Definitions

Edit `Config/PinDefinitions.h`:

```cpp
// Motor Control (L298N) - sin pines repetidos
#define MOTOR_LEFT_PWM    5   // ENA (PWM)
#define MOTOR_LEFT_DIR1   7   // IN1
#define MOTOR_LEFT_DIR2   8   // IN2
#define MOTOR_RIGHT_PWM   6   // ENB (PWM)
#define MOTOR_RIGHT_DIR1  9   // IN3
#define MOTOR_RIGHT_DIR2  10  // IN4

// Sensors (libres tras asignar motores en 5-10)
#define ULTRASONIC_TRIG   2
#define ULTRASONIC_ECHO   4
#define SERVO_PIN         3   // PWM libre

// Communication
#define HC05_RX           11
#define HC05_TX           12

// Line Sensors (UNO solo tiene A0-A5 => maximo 6 sensores analogicos)
#define IR_SENSOR_1       A0
#define IR_SENSOR_2       A1
#define IR_SENSOR_3       A2
// A3-A5 disponibles si se usan mas sensores
```

### Adjust Speed Constants

Edit `Config/Constants.h`:

```cpp
#define MAX_MOTOR_SPEED   255
#define MIN_MOTOR_SPEED   50
#define DEFAULT_SPEED     150
#define TURN_SPEED        100
```

---

## 🐛 Troubleshooting

### Arduino Not Detected

**Problem:** "Port not found" error

**Solutions:**
1. Check USB cable (try different cable/port)
2. Reinstall CH340 driver (Windows):
   - Download from [https://sparks.gogo.co.nz/ch340.html](https://sparks.gogo.co.nz/ch340.html)
   - Restart computer
3. Try different USB port
4. Update Arduino IDE

### Motors Not Turning

**Problem:** Motors don't respond to commands

**Solutions:**
1. Verify L298N connections (see [wiring.md](wiring.md))
2. Test with multimeter: Check voltage at motor terminals
3. Verify motor power supply (should see 5V)
4. Check for burnt L298N chip
5. Test with different PWM value (try 200)

### Ultrasonic Not Working

**Problem:** No distance readings or incorrect values

**Solutions:**
1. Verify HC-SR04 connections (TRIG on D7, ECHO on D8)
2. Test with known distance (place object at 10cm)
3. Check for dust/dirt on sensors
4. Verify 5V power to sensor
5. Use multimeter to check signal lines

### Bluetooth Connection Issues

**Problem:** HC-05 won't pair or connect

**Solutions:**
1. Check HC-05 power (should see LED blink)
2. Verify RX/TX connections (D11/D12)
3. Test with AT commands (communication mode)
4. Try factory reset: power cycle 3x quickly
5. Check baud rate is 9600

### Line Sensors Not Calibrated

**Problem:** Readings all white or all black

**Solutions:**
1. Run calibration procedure (see testing.md)
2. Check sensor power (5V at VCC)
3. Verify A0-A5 connections
4. Test on black/white surface
5. Clean sensor optics

---

## 📚 Next Steps

After successful first test:

1. **Review [architecture.md](architecture.md)** - Understand system design
2. **Study [hardware.md](hardware.md)** - Learn component details
3. **Follow [testing.md](testing.md)** - Run comprehensive tests
4. **Read [structure.md](structure.md)** - Understand code organization
5. **Implement features** - Add behaviors from [roadmap.md](roadmap.md)

---

## 🤝 Getting Help

**Having Issues?**
- Check [troubleshooting section](#-troubleshooting)
- Review [wiring.md](wiring.md) for connections
- See [testing.md](testing.md) for diagnostics
- Open [GitHub Issue](../../issues)

**Documentation:**
- Full [Architecture Guide](architecture.md)
- [Hardware Specifications](hardware.md)
- [Testing Procedures](testing.md)

**Community:**
- Arduino Forum: [https://forum.arduino.cc/](https://forum.arduino.cc/)
- GitHub Discussions: [Project Discussions](../../discussions)

---

## 🎓 Learning Resources

### Arduino Basics
- [Arduino Official Tutorials](https://www.arduino.cc/en/Tutorial)
- [Arduino Language Reference](https://www.arduino.cc/reference/en/)

### Robotics Concepts
- Line Following Algorithm
- PID Control Loop
- Obstacle Avoidance Techniques
- State Machine Design

### Electronics
- PWM (Pulse Width Modulation)
- H-Bridge Motor Control
- Sensor Interfacing

---

## ✅ Verification Checklist

Before proceeding to autonomous operation:

- [ ] Arduino compiles and uploads successfully
- [ ] LED blink test passes
- [ ] Motors respond to forward/backward commands
- [ ] Ultrasonic sensor returns reasonable distances
- [ ] Servo moves smoothly through full range
- [ ] Bluetooth connects and receives commands
- [ ] Line sensors detect black/white correctly
- [ ] Robot drives in straight line (balanced motors)
- [ ] All safety systems verified
- [ ] Battery holds sufficient charge

**Ready to deploy!** 🚀

---

## 📞 Support

For detailed documentation, see:
- [README.md](../README.md) - Project overview
- [CONTRIBUTING.md](../CONTRIBUTING.md) - How to contribute
- [Docs directory](.) - Complete documentation
