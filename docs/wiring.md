# Wiring Guide

Complete pin connections and electrical connections for Robot Car Arduino.

## 📌 Arduino UNO Pin Allocation

### Summary

| Pin | Component | Function | Mode |
|-----|-----------|----------|------|
| D2 | L298N | Motor B PWM Speed | Output (PWM) |
| D3 | L298N | Motor A PWM Speed | Output (PWM) |
| D4 | L298N | Motor A Direction 1 | Output |
| D5 | L298N | Motor A Direction 2 | Output |
| D6 | SG90 | Servo Control | Output (PWM) |
| D7 | HC-SR04 | Ultrasonic Trigger | Output |
| D8 | HC-SR04 | Ultrasonic Echo | Input |
| D9 | Encoder | Left Motor Pulses | Input (INT) |
| D10 | Encoder | Right Motor Pulses | Input (INT) |
| D11 | HC-05 | Serial RX | Input |
| D12 | HC-05 | Serial TX | Output |
| A0-A7 | QTR-8A | Line Sensor Array 1-8 | Input (ADC) |

---

## 🔌 Detailed Connections

### Motor Driver L298N ↔ Arduino UNO

```
L298N         Arduino UNO
┌─────────┐   ┌─────────┐
│ IN1  ───┼─→ D4
│ IN2  ───┼─→ D5
│ IN3  ───┼─→ D2
│ IN4  ───┼─→ D3
│ ENA  ───┼─→ D2 (PWM)
│ ENB  ───┼─→ D3 (PWM)
│ +5V  ───┼─→ 5V
│ GND  ───┼─→ GND
└─────────┘   └─────────┘

Motors
OUT1/OUT2 → Motor Left
OUT3/OUT4 → Motor Right
```

### Ultrasonic Sensor HC-SR04 ↔ Arduino UNO

```
HC-SR04       Arduino UNO
┌────────┐    ┌─────────┐
│ VCC ───┼──→ 5V
│ GND ───┼──→ GND
│ TRIG ──┼──→ D7
│ ECHO ──┼──→ D8
└────────┘    └─────────┘

Operation:
1. Send 10µs HIGH to D7 (TRIG)
2. Wait for response on D8 (ECHO)
3. Measure pulse width time
4. Calculate: Distance = time × 343m/s / 2
```

### Servo Motor SG90 ↔ Arduino UNO

```
SG90 (3-wire)    Arduino UNO
┌──────────┐     ┌─────────┐
│ Brown ───┼───→ GND
│ Red   ───┼───→ 5V
│ Orange ──┼───→ D6 (PWM)
└──────────┘     └─────────┘

PWM Signal (50Hz):
- 1.0ms pulse  = 0°
- 1.5ms pulse  = 90°
- 2.0ms pulse  = 180°
```

### Bluetooth Module HC-05 ↔ Arduino UNO

```
HC-05         Arduino UNO (SoftwareSerial)
┌────────┐    ┌─────────┐
│ VCC ───┼──→ 5V
│ GND ───┼──→ GND
│ TX  ───┼──→ D11 (RX)
│ RX  ───┼──→ D12 (TX)
└────────┘    └─────────┘

Serial Protocol:
Baud Rate: 9600
Data Bits: 8
Stop Bits: 1
Parity: None
```

### Line Sensor Array QTR-8A ↔ Arduino UNO

```
QTR-8A           Arduino UNO (Analog)
┌──────────────┐ ┌─────────┐
│ VCC ────────→ 5V
│ GND ────────→ GND
│ OUT1 ──────→ A0
│ OUT2 ──────→ A1
│ OUT3 ──────→ A2
│ OUT4 ──────→ A3
│ OUT5 ──────→ A4
│ OUT6 ──────→ A5
│ OUT7 ──────→ D0
│ OUT8 ──────→ D1
└──────────────┘ └─────────┘

Sensor Analog Values:
Black Line: 0-100 (low reflection)
White Floor: 900-1023 (high reflection)
```

### Optional: Encoders ↔ Arduino UNO

```
Encoder (Left)      Arduino UNO
┌────────┐           ┌─────────┐
│ +5V ──→ 5V
│ GND ──→ GND
│ OUT ──→ D9 (INT0)
└────────┘           └─────────┘

Encoder (Right)     Arduino UNO
┌────────┐           ┌─────────┐
│ +5V ──→ 5V
│ GND ──→ GND
│ OUT ──→ D10 (INT1)
└────────┘           └─────────┘

Pulses per Revolution: ~20 PPR
```

---

## 🔋 Power Distribution

### Main Power Bus

```
Battery (5V)
    │
    ├──→ [Fuse 2-3A]
    │
    ├──→ [L298N Power Input]
    │    │
    │    ├──→ Motors (5V power)
    │    │
    │    └──→ Motor Ground
    │
    ├──→ [Arduino 5V Input] or [External 5V]
    │    │
    │    ├──→ Arduino VIN
    │    │
    │    └──→ Arduino GND
    │
    └──→ [Common Ground Point]
         │
         ├──→ All components GND
         └──→ Battery GND return
```

### Recommended Configuration

**Option A: Single 5V Supply (Recommended for small scale)**
```
Battery → Voltage Regulator (5V) → Distribution Board
                                    ├→ Arduino VIN
                                    ├→ L298N Vcc
                                    ├→ Sensors Vcc
                                    └→ HC-05 Vcc
```

**Option B: Dual Supply (Better for high load)**
```
Battery (5V)
├→ L298N (Direct) ──→ Motors only
│
└→ Arduino (USB/Vin) ──→ Sensors + Logic
```

---

## 📋 Full Pinout Reference Table

| Arduino Pin | Signal Type | Component | Function | Alt Use |
|-------------|-------------|-----------|----------|---------|
| D0 | RX/TX | Serial | Hardware UART | QTR-8A OUT7 |
| D1 | RX/TX | Serial | Hardware UART | QTR-8A OUT8 |
| D2 | PWM | L298N | Motor B Speed | Interrupt |
| D3 | PWM | L298N | Motor A Speed | Interrupt |
| D4 | Digital | L298N | Motor A Dir 1 | - |
| D5 | Digital | L298N | Motor A Dir 2 | - |
| D6 | PWM | SG90 | Servo Control | - |
| D7 | Digital | HC-SR04 | Trigger | - |
| D8 | Digital | HC-SR04 | Echo Input | - |
| D9 | PWM/INT | Encoder | Left Pulses | - |
| D10 | PWM/INT | Encoder | Right Pulses | - |
| D11 | Digital | HC-05 | Soft RX | - |
| D12 | Digital | HC-05 | Soft TX | - |
| A0 | ADC | QTR-8A | Sensor 1 | - |
| A1 | ADC | QTR-8A | Sensor 2 | - |
| A2 | ADC | QTR-8A | Sensor 3 | - |
| A3 | ADC | QTR-8A | Sensor 4 | - |
| A4 | ADC | QTR-8A | Sensor 5 (SDA) | I2C Clock |
| A5 | ADC | QTR-8A | Sensor 6 (SCL) | I2C Data |
| 5V | Power | All | +5V Distribution | - |
| GND | Power | All | Ground (3+ points) | - |

---

## 🔗 Connection Checklist

**Before Power-Up:**

- [ ] L298N IN1-IN4 connected to D4, D5, D2, D3
- [ ] L298N ENA, ENB connected to PWM pins
- [ ] Motor connections: OUT1/2 and OUT3/4
- [ ] HC-SR04 TRIG on D7, ECHO on D8
- [ ] Servo signal on D6 (PWM capable)
- [ ] HC-05 TX/RX on D12/D11
- [ ] QTR-8A sensors on A0-A5
- [ ] All GND connections secure (multiple points)
- [ ] 5V power distribution verified
- [ ] No crossed wires or shorts
- [ ] Battery disconnected initially

**Power-Up Sequence:**

1. Verify all connections with continuity tester
2. Connect battery to L298N power input
3. Check: L298N LED lights up
4. Arduino uploads sketch (pre-tested)
5. Verify: All sensors responsive
6. Test: Motors respond to commands
7. Calibrate: Line sensors reading values
8. Ready for operation

---

## 🚨 Common Wiring Mistakes

| Mistake | Problem | Solution |
|--------|---------|----------|
| Reversed polarity | Destroyed components | Use diodes + fuse |
| No common ground | Sporadic failures | Connect all GND together |
| Thin power wires | Voltage drop/heat | Use 18AWG min |
| Long signal wires | Noise/crosstalk | Shield twisted pairs |
| No decoupling caps | Glitches | Add 0.1µF + 10µF near power |
| Mixed logic levels | False readings | Keep signal traces short |
| Improper PWM pins | No speed control | Use D2, D3, D6, D9, D10 only |
| HC-05 power surge | Arduino resets | Use external 5V supply |

---

## 🛠️ Tools Required

- Breadboard or perfboard
- Jumper wires (22AWG)
- Power wires (18-20AWG)
- Soldering iron + solder
- Wire strippers
- Multimeter
- Continuity tester
- Hot glue / heat shrink tubing

---

## 📐 Recommended PCB Layout

For permanent installation, consider:

1. Arduino mounting: Center top
2. L298N: Below Arduino (motor control)
3. Power distribution: Left side
4. Sensor connections: Arranged by function
5. HC-05: Top right (antenna clear)
6. Servo connector: Front center

---

## 🔍 Testing & Verification

After wiring complete:

```cpp
// Test L298N Connection
digitalWrite(D4, HIGH);
digitalWrite(D5, LOW);
analogWrite(D2, 200); // Left motor forward

// Test HC-SR04
// Measure pulseIn(D8, HIGH)

// Test SG90
// Write angle to servo

// Test HC-05
// Serial communication 9600 baud

// Test QTR-8A
// Read analog values A0-A5
```

---

## 📚 Additional Resources

- [Arduino I/O Pin Reference](https://www.arduino.cc/en/Reference/PinMode)
- [Wiring Best Practices](https://learn.sparkfun.com/tutorials/how-to-power-a-project)
- [Fritzing Breadboard Diagram Tool](https://fritzing.org/)
