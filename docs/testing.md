# Testing Guide

Comprehensive testing procedures and protocols for Robot Car Arduino.

## 🧪 Testing Overview

This guide covers unit testing, integration testing, calibration procedures, and validation protocols to ensure system reliability.

---

## 🔍 Pre-Flight Checklist

Before any testing, verify:

- [ ] All connections are secure
- [ ] No visible damage to components
- [ ] Battery is charged
- [ ] Arduino IDE is installed
- [ ] Correct board selected (Arduino UNO)
- [ ] Correct COM port selected
- [ ] No USB cables in way of robot
- [ ] Testing area is clear and safe

---

## ⚡ Unit Tests

### 1. Motor Driver Testing

**Objective:** Verify L298N and DC motors function correctly

**Setup:**
```cpp
#include <Arduino.h>

// Motor pins (from PinDefinitions.h)
const int MOTOR_LEFT_PWM = D2;
const int MOTOR_LEFT_DIR1 = D4;
const int MOTOR_LEFT_DIR2 = D5;
const int MOTOR_RIGHT_PWM = D3;
const int MOTOR_RIGHT_DIR1 = D2;
const int MOTOR_RIGHT_DIR2 = D3;
```

**Test: Forward Motion**
```cpp
void test_motor_forward() {
  // Left motor forward
  digitalWrite(MOTOR_LEFT_DIR1, HIGH);
  digitalWrite(MOTOR_LEFT_DIR2, LOW);
  analogWrite(MOTOR_LEFT_PWM, 200); // 0-255

  // Right motor forward
  digitalWrite(MOTOR_RIGHT_DIR1, HIGH);
  digitalWrite(MOTOR_RIGHT_DIR2, LOW);
  analogWrite(MOTOR_RIGHT_PWM, 200);

  delay(2000);

  // Stop
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
}
```

**Expected Result:** Both wheels rotate forward at same speed

**Test: Backward Motion**
```cpp
void test_motor_backward() {
  // Reverse direction pins
  digitalWrite(MOTOR_LEFT_DIR1, LOW);
  digitalWrite(MOTOR_LEFT_DIR2, HIGH);
  analogWrite(MOTOR_LEFT_PWM, 200);

  digitalWrite(MOTOR_RIGHT_DIR1, LOW);
  digitalWrite(MOTOR_RIGHT_DIR2, HIGH);
  analogWrite(MOTOR_RIGHT_PWM, 200);

  delay(2000);
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
}
```

**Expected Result:** Both wheels rotate backward

**Test: Left Turn**
```cpp
void test_motor_left_turn() {
  digitalWrite(MOTOR_LEFT_DIR1, LOW);
  digitalWrite(MOTOR_LEFT_DIR2, HIGH);
  analogWrite(MOTOR_LEFT_PWM, 150); // Reverse slower

  digitalWrite(MOTOR_RIGHT_DIR1, HIGH);
  digitalWrite(MOTOR_RIGHT_DIR2, LOW);
  analogWrite(MOTOR_RIGHT_PWM, 200); // Forward faster

  delay(1000);
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
}
```

**Expected Result:** Robot turns left (left wheel slower)

**Test: Speed Control**
```cpp
void test_motor_speed_control() {
  for (int speed = 50; speed <= 255; speed += 50) {
    analogWrite(MOTOR_LEFT_PWM, speed);
    analogWrite(MOTOR_RIGHT_PWM, speed);

    Serial.print("Speed: ");
    Serial.println(speed);

    delay(1000);
  }

  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
}
```

**Expected Result:** Smooth acceleration from stop to max speed

---

### 2. Ultrasonic Sensor Testing

**Objective:** Verify HC-SR04 distance measurements

**Setup:**
```cpp
const int TRIGGER_PIN = 7;  // D7
const int ECHO_PIN = 8;     // D8

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}
```

**Test: Distance Measurement**
```cpp
long getDistance() {
  // Send 10µs pulse
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure echo time
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // Distance = time × speed of sound / 2
  // Speed of sound = 343 m/s = 0.0343 cm/µs
  long distance = duration * 0.0343 / 2;

  return distance;
}

void test_distance_measurement() {
  for (int i = 0; i < 10; i++) {
    long distance = getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);
  }
}
```

**Expected Result:** Consistent distance readings (±3% accuracy)

**Test: Obstacle Detection**
```cpp
void test_obstacle_detection() {
  const int THRESHOLD = 20; // 20cm

  for (int i = 0; i < 20; i++) {
    long distance = getDistance();

    if (distance < THRESHOLD) {
      Serial.println("OBSTACLE DETECTED!");
    } else {
      Serial.println("Clear");
    }

    delay(200);
  }
}
```

**Expected Result:** Detects obstacles within 20cm range

**Physical Test:**
- Hold hand at 5cm → detects obstacle
- Move hand at 30cm → no obstacle
- Move hand slowly → tracks distance smoothly

---

### 3. Servo Motor Testing

**Objective:** Verify SG90 servo control and sweep

**Setup:**
```cpp
#include <Servo.h>

const int SERVO_PIN = 6;
Servo myServo;

void setup() {
  myServo.attach(SERVO_PIN);
  Serial.begin(9600);
}
```

**Test: Angle Control**
```cpp
void test_servo_angles() {
  // Move to 0°
  myServo.write(0);
  delay(1000);
  Serial.println("0 degrees");

  // Move to 90°
  myServo.write(90);
  delay(1000);
  Serial.println("90 degrees");

  // Move to 180°
  myServo.write(180);
  delay(1000);
  Serial.println("180 degrees");

  // Return to center
  myServo.write(90);
}
```

**Expected Result:** Servo moves smoothly to each position

**Test: Sweep Pattern**
```cpp
void test_servo_sweep() {
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);
    Serial.println(angle);
    delay(100);
  }

  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle);
    Serial.println(angle);
    delay(100);
  }
}
```

**Expected Result:** Servo sweeps 0° → 180° → 0°

---

### 4. Line Sensor Testing

**Objective:** Verify QTR-8A IR sensor array

**Setup:**
```cpp
const int QTR_PINS[] = {A0, A1, A2, A3, A4, A5};
const int NUM_SENSORS = 6;

void setup() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(QTR_PINS[i], INPUT);
  }
  Serial.begin(9600);
}
```

**Test: Sensor Readout**
```cpp
void test_sensor_readout() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < NUM_SENSORS; j++) {
      int value = analogRead(QTR_PINS[j]);
      Serial.print(value);
      Serial.print("\t");
    }
    Serial.println();
    delay(200);
  }
}
```

**Expected Output:**
```
Black line:    0     0     50    100   50    0
White floor:   1023  1023  950   900   950   1023
```

**Test: Line Detection**
```cpp
int getLinePosition() {
  int sum = 0;
  int weightedSum = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    int value = analogRead(QTR_PINS[i]);
    int weight = i - (NUM_SENSORS / 2);

    if (value < 500) { // Black line threshold
      sum += 1;
      weightedSum += weight;
    }
  }

  if (sum == 0) return 999; // No line found
  return weightedSum / sum;
}

void test_line_position() {
  for (int i = 0; i < 20; i++) {
    int position = getLinePosition();
    Serial.println(position);
    delay(100);
  }
}
```

**Physical Test:**
- Place robot on black line → Position value near 0 (centered)
- Shift left → Negative position value
- Shift right → Positive position value
- Off line → Value 999

---

### 5. Bluetooth Communication Testing

**Objective:** Verify HC-05 wireless link

**Setup:**
```cpp
#include <SoftwareSerial.h>

const int RX_PIN = 11;
const int TX_PIN = 12;
SoftwareSerial bluetooth(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600); // HC-05 default baud rate
}
```

**Test: Serial Echo**
```cpp
void test_bluetooth_echo() {
  if (bluetooth.available()) {
    char data = bluetooth.read();
    Serial.print("Received: ");
    Serial.println(data);

    // Echo back
    bluetooth.write(data);
    Serial.println("Echoed back");
  }
}

void loop() {
  test_bluetooth_echo();
}
```

**Expected Result:**
- Connect with Bluetooth terminal app
- Send character → Robot echoes it back
- Verify round-trip communication

**Test: Command Parsing**
```cpp
void test_bluetooth_commands() {
  if (bluetooth.available()) {
    String command = bluetooth.readStringUntil('\n');

    if (command == "FORWARD") {
      // Move forward
      Serial.println("Command: FORWARD");
    } else if (command == "STOP") {
      Serial.println("Command: STOP");
    }
  }
}
```

**Expected Result:** Robot responds to Bluetooth commands

---

## 🔧 Integration Tests

### Full System Test

**Objective:** Verify all subsystems work together

**Procedure:**

1. **Initialization Test**
   ```cpp
   void test_initialization() {
     // Initialize all modules
     robotController.init();

     // Verify all LEDs/indicators
     delay(2000);

     // Ready for operation
     Serial.println("System initialized");
   }
   ```

2. **Obstacle Avoidance Integration**
   - Robot starts moving forward
   - Place hand in front
   - Verify robot stops/backs up
   - Verify servo scans left/right
   - Verify robot chooses clear path

3. **Line Following Integration**
   - Place robot on black line
   - Start line following mode
   - Verify smooth line tracking
   - Test at intersections
   - Test at line edges

4. **Manual Control Integration**
   - Connect via Bluetooth
   - Send forward command
   - Verify motor response
   - Test all directions
   - Test speed control

---

## 📊 Calibration Procedures

### Motor Speed Calibration

**Objective:** Balance left/right motor speeds

**Procedure:**
```cpp
void calibrate_motor_speed() {
  // Mark starting position
  // Set both motors to same PWM value (150)
  analogWrite(MOTOR_LEFT_PWM, 150);
  analogWrite(MOTOR_RIGHT_PWM, 150);

  // Drive forward 1 meter
  delay(4000); // Adjust time based on speed

  // Measure deviation
  if (left_motor_faster) {
    // Reduce left motor speed
    // Next attempt: left=140, right=150
  }
}
```

**Repeat until:** Robot drives straight without drifting

### Sensor Calibration

**Objective:** Establish white/black thresholds

**Procedure:**

1. **White Calibration**
   ```cpp
   for (int i = 0; i < NUM_SENSORS; i++) {
     white_values[i] = analogRead(QTR_PINS[i]);
   }
   ```

2. **Black Calibration**
   ```cpp
   for (int i = 0; i < NUM_SENSORS; i++) {
     black_values[i] = analogRead(QTR_PINS[i]);
   }
   ```

3. **Calculate Threshold**
   ```cpp
   threshold[i] = (white_values[i] + black_values[i]) / 2;
   ```

### Distance Sensor Calibration

**Objective:** Verify HC-SR04 accuracy

**Procedure:**
1. Place object at known distance (10cm)
2. Record 10 measurements
3. Calculate average
4. Verify within ±3% of actual distance
5. Repeat at 15cm, 20cm, 30cm

---

## ✅ Test Report Template

```
Test: [Test Name]
Date: [YYYY-MM-DD]
Tester: [Name]
Status: PASS / FAIL

Setup:
- [Equipment/configuration]

Procedure:
- [Steps taken]

Expected Results:
- [What should happen]

Actual Results:
- [What actually happened]

Issues:
- [Any problems found]

Notes:
- [Additional observations]

Recommendation:
- READY FOR DEPLOYMENT
- NEEDS ADJUSTMENT
- BLOCKED ON [ISSUE]
```

---

## 🚀 Deployment Validation

Before deploying to real-world testing:

**Checklist:**
- [ ] All unit tests pass
- [ ] All integration tests pass
- [ ] Motors balanced and tested
- [ ] Sensors calibrated
- [ ] Bluetooth connection stable
- [ ] Safety mechanisms verified
- [ ] Battery capacity confirmed
- [ ] No loose connections
- [ ] Complete system test passed
- [ ] Performance meets specifications

---

## 🔗 Related Documentation

- [Hardware Guide](hardware.md) - Component specifications
- [Wiring Guide](wiring.md) - Connection verification
- [Getting Started](getting-started.md) - Setup procedures
