# Getting Started

Quick start guide to build and deploy Robot Car Arduino.

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

1. Open **Preferences** (File → Preferences)
2. Add to "Additional Boards Manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. Open **Boards Manager** (Tools → Board → Boards Manager)
4. Search for "Arduino AVR Boards"
5. Click Install
6. Restart Arduino IDE

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
// Motor pins
const int MOTOR_LEFT_PWM = 2;
const int MOTOR_LEFT_DIR1 = 4;
const int MOTOR_LEFT_DIR2 = 5;
const int MOTOR_RIGHT_PWM = 3;
const int MOTOR_RIGHT_DIR1 = 2;
const int MOTOR_RIGHT_DIR2 = 3;

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
// Motor Control
#define MOTOR_LEFT_PWM    2
#define MOTOR_LEFT_DIR1   4
#define MOTOR_LEFT_DIR2   5
#define MOTOR_RIGHT_PWM   3
#define MOTOR_RIGHT_DIR1  2
#define MOTOR_RIGHT_DIR2  3

// Sensors
#define ULTRASONIC_TRIG   7
#define ULTRASONIC_ECHO   8
#define SERVO_PIN         6

// Communication
#define HC05_RX           11
#define HC05_TX           12

// Line Sensors
#define QTR_SENSOR_1      A0
#define QTR_SENSOR_2      A1
// ... etc
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
