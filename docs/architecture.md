# System Architecture

Complete technical architecture documentation for Robot Car Arduino.

---

## 🏗️ Architecture Overview

Robot Car Arduino implements a **layered, modular architecture** designed for scalability, maintainability, and real-time performance.

### Architectural Layers

```
┌─────────────────────────────────────────────────┐
│         Application / User Interface            │  ← Bluetooth Remote Control
├─────────────────────────────────────────────────┤
│            Features (Behaviors)                  │  ← High-Level Autonomy
│  [ObstacleAvoidance][LineFollower][Manual/Auto] │
├─────────────────────────────────────────────────┤
│         Core Logic (Orchestration)              │  ← Main Coordinator
│        [RobotController][StateMachine]          │
│           [EventManager][Telemetry]             │
├─────────────────────────────────────────────────┤
│         Drivers (Hardware Abstraction)           │  ← Sensor/Actuator Control
│  [Motors][Ultrasonic][Servo][LineIR sensors]   │
├─────────────────────────────────────────────────┤
│      Configuration & Constants (Global)         │  ← Settings & Parameters
│  [PinDefinitions][Constants][Thresholds]        │
├─────────────────────────────────────────────────┤
│          Hardware (Physical Devices)            │  ← Real Components
│  Arduino UNO + Sensors + Motors + Actuators     │
└─────────────────────────────────────────────────┘
```

---

## 📊 Module Organization

### Layer 1: Core (Lógica Central)

**Purpose:** System orchestration and coordination

```
RobotController
├─ Initialization
├─ Cycle Management
├─ State Transitions
└─ Module Coordination

StateMachine
├─ Current State
├─ Valid Transitions
├─ State Handlers
└─ Event Routing

EventManager
├─ Event Registry
├─ Event Listeners
├─ Event Broadcasting
└─ Error Propagation
```

**Key Responsibilities:**
- Initialize all subsystems on startup
- Manage robot operational modes (Idle → Active → Error)
- Coordinate inter-module communication
- Handle emergency stop and safety protocols

**Design Pattern:** Observer pattern for events

---

### Layer 2: Drivers (Hardware Abstraction)

**Purpose:** Consistent interface to physical hardware

```
MotorDriver (L298N)
├─ Speed Control (PWM)
├─ Direction Control
├─ Acceleration Curves
└─ Error Handling

UltrasonicDriver (HC-SR04)
├─ Distance Measurement
├─ Calibration
├─ Threshold Detection
└─ Error Recovery

ServoDriver (SG90)
├─ Angle Positioning
├─ Sweep Patterns
├─ Speed Control
└─ Limit Checking

EncoderDriver
├─ Pulse Counting
├─ Velocity Calculation
├─ Distance Tracking
└─ Synchronization
```

**Characteristics:**
- Abstracts low-level hardware
- Provides consistent C++ interface
- Handles error conditions
- Manages resource cleanup

**Design Pattern:** Adapter pattern for hardware

---

### Layer 3: Features (Autonomous Behaviors)

**Purpose:** High-level robot capabilities

```
ObstacleAvoidance
├─ Scanning Strategy
├─ Decision Algorithm
├─ Path Selection
└─ Evasion Execution

LineFollower
├─ Sensor Reading
├─ Error Calculation
├─ PID Controller
└─ Motor Adjustment

ManualControl
├─ Command Parsing
├─ Input Validation
├─ Direct Execution
└─ Feedback

AutonomousMode
├─ Navigation Planning
├─ Sensor Fusion
├─ Decision Making
└─ Recovery
```

**Characteristics:**
- Uses drivers for hardware access
- Independent operation
- Event-based communication
- Performance optimized

**Design Pattern:** Strategy pattern for behaviors

---

### Layer 4: Communication (External Interface)

**Purpose:** Robot-to-world interaction

```
BluetoothHandler (HC-05)
├─ Serial Communication
├─ Command Reception
├─ Response Sending
└─ Connection Management

Telemetry
├─ State Transmission
├─ Sensor Reporting
├─ Performance Metrics
└─ Debug Information

WiFi (Future)
├─ Network Connection
├─ Protocol Implementation
├─ Cloud Integration
└─ OTA Updates
```

---

### Layer 5: Config (Global Settings)

**Purpose:** Centralized parameter management

```cpp
// PinDefinitions.h - Hardware pin mapping (ver wiring.md)
#define MOTOR_LEFT_PWM    5   // ENA (PWM)
#define MOTOR_LEFT_DIR1   7   // IN1

// Constants.h - System parameters
#define MAX_MOTOR_SPEED   255
#define OBSTACLE_THRESHOLD 20

// Thresholds.h - Calibration values
#define BLACK_LINE_THRESHOLD 500
#define WHITE_FLOOR_THRESHOLD 900
```

---

## 🔄 Data Flow Architecture

### State Machine Diagram

```
                    ┌─────────────┐
                    │    IDLE     │
                    └──────┬──────┘
                           │
                ┌──────────┼──────────┐
                │          │          │
                ▼          ▼          ▼
        ┌──────────┐  ┌────────┐  ┌──────────────┐
        │  MANUAL  │  │ FOLLOW │  │   OBSTACLE   │
        │ CONTROL  │  │  LINE  │  │  AVOIDANCE   │
        └────┬─────┘  └───┬────┘  └──────┬───────┘
             │            │               │
             └────────────┼───────────────┘
                          │
                    ┌─────▼─────┐
                    │  AUTONOMOUS│
                    └─────┬─────┘
                          │
                    ┌─────▼─────┐
                    │   ERROR    │
                    └────────────┘
```

### Event Flow

```
Hardware Event (Sensor reading)
    │
    ▼
EventManager
    │
    ├─→ RobotController (updated)
    ├─→ Feature Modules (notified)
    └─→ Telemetry (logged)

Feature Decision
    │
    ▼
RobotController (processes)
    │
    ▼
Drivers (execute)
    │
    ▼
Hardware Actuation
```

### Obstacle Avoidance Flow (Detailed)

```
┌──────────────────────────────────────────────────┐
│ 1. UltrasonicDriver reads distance               │
│    Every 100ms → getDistance()                   │
└──────────┬───────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────┐
│ 2. EventManager publishes ObstacleDetected       │
│    if distance < THRESHOLD (20cm)                │
└──────────┬───────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────┐
│ 3. ObstacleAvoidance module receives event       │
│    Initiates scanning sequence (0° → 180°)      │
└──────────┬───────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────┐
│ 4. Servo scans 3 sectors:                        │
│    Left (45°) | Front (90°) | Right (135°)     │
└──────────┬───────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────┐
│ 5. Selects clearest path based on distances     │
│    Decision algorithm: avoid < 25cm, prefer >30 │
└──────────┬───────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────┐
│ 6. Motor commands issued:                        │
│    Left turn:  L:100, R:200 (forward)           │
│    Right turn: L:200, R:100 (forward)           │
│    Backup:     L:150, R:150 (backward)          │
└──────────┬───────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────┐
│ 7. Movement executed via MotorDriver            │
│    Smooth acceleration over 500ms               │
└──────────────────────────────────────────────────┘
```

### Line Following Flow (Detailed)

```
IR Sensor Array (3-5 sensores, A0-A5 del UNO)
    │
    ├─ Sensores (valores analogicos)
    │
    ▼
LineFollower::calculatePosition()
    │
    ├─ weighted sum: Σ(weight_i × sensor_i)
    ├─ total weight: Σ(1 if sensor > threshold)
    │
    ▼
Error = Position - Center
    │
    ├─ Negative = Left of line
    ├─ Zero = Center on line
    └─ Positive = Right of line
    │
    ▼
PID Controller
    │
    ├─ P: Proportional × error
    ├─ I: Integral × sum_error
    └─ D: Derivative × error_rate
    │
    ▼
Motor Speed Adjustment
    │
    ├─ Left motor speed = Base - Correction
    └─ Right motor speed = Base + Correction
    │
    ▼
Smooth motion on line
(repeated at 100Hz)
```

---

## 🎯 Design Patterns Used

### 1. Observer Pattern (Event System)

```cpp
// Publisher
EventManager.publish(EventType::OBSTACLE_DETECTED);

// Subscribers
ObstacleAvoidance.onObstacleDetected();
Telemetry.logEvent(EventType::OBSTACLE_DETECTED);
RobotController.handleEvent(event);
```

**Advantages:**
- Loose coupling between modules
- Dynamic subscription
- Easy to add new listeners

---

### 2. Strategy Pattern (Behaviors)

```cpp
// Strategy interface
class Behavior {
  virtual void execute() = 0;
};

// Concrete strategies
class ObstacleAvoidance : public Behavior { };
class LineFollower : public Behavior { };
class ManualControl : public Behavior { };

// Context
RobotController {
  Behavior* current_behavior;
  void switchBehavior(Behavior* new_behavior);
};
```

---

### 3. State Machine Pattern

```cpp
enum State { IDLE, MANUAL, LINE_FOLLOW, OBSTACLE_AVOID, ERROR };

class StateMachine {
  void setState(State new_state);
  void handleEvent(Event event);
  bool isValidTransition(State from, State to);
};
```

---

### 4. Adapter Pattern (Drivers)

```cpp
// Hardware abstraction
class MotorDriver {
  // Public interface
  void forward(speed);
  void backward(speed);

  // Internal implementation details hidden
  private:
    setPWM(pin, value);
    setDirection(pin, dir);
};
```

---

## 🔌 Interface Definitions

### MotorDriver Interface

```cpp
class MotorDriver {
  public:
    // Basic movement
    void forward(int speed);      // 0-255
    void backward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    void stop();

    // Speed control
    void setSpeed(int left, int right);
    int getLeftSpeed();
    int getRightSpeed();

    // Status
    bool isMoving();
    MotorState getState();
};
```

### UltrasonicDriver Interface

```cpp
class UltrasonicDriver {
  public:
    // Measurement
    long getDistance();           // cm
    long getAverageDistance(n);   // Multiple readings

    // Thresholds
    bool isObstacleDetected(threshold);
    bool isPathClear(threshold);

    // Configuration
    void setThreshold(int cm);
    int getThreshold();
};
```

### ServoDriver Interface

```cpp
class ServoDriver {
  public:
    // Positioning
    void setAngle(int degrees);   // 0-180
    int getAngle();

    // Scanning
    void scan();                  // 0→180→0
    void scanSectors(left, center, right);

    // Speed
    void setScanSpeed(int ms);
};
```

---

## ⚡ Real-Time Considerations

### Loop Frequency Analysis

```
Main Loop Cycle: ~10ms (100Hz)
├─ Core update:      2ms
├─ Sensor reads:     3ms
├─ Decision making:  2ms
├─ Motor command:    1ms
└─ Communications:   2ms
                    ────
                    Total: ~10ms
```

### Priority Levels

```
1. CRITICAL (< 1ms)
   - Safety/Emergency stop
   - Motor control feedback
   - Collision detection

2. HIGH (1-5ms)
   - Sensor readings
   - State transitions
   - Feature decision-making

3. MEDIUM (5-20ms)
   - Telemetry updates
   - Event processing
   - Calibration

4. LOW (> 20ms)
   - Logging
   - LED indicators
   - Debug output
```

---

## 🔐 Safety Architecture

### Fail-Safe Mechanisms

```
Emergency Situations
        │
        ├─ Motor timeout (> 2s no command) → STOP
        ├─ Sensor failure detected → Fall back to manual
        ├─ Communication loss → Local autonomous
        ├─ Obstacle detection within 5cm → Immediate reverse
        └─ Battery critical (< 3.5V) → Graceful shutdown
```

### Error Recovery

```
Error Detected
        │
        ├─ Log error with timestamp
        ├─ State → ERROR
        ├─ Attempt recovery (if applicable)
        ├─ If recovery fails → Manual mode only
        └─ Alert user via telemetry
```

---

## 📈 Scalability Design

### Adding New Sensors

```cpp
// 1. Create driver
class NewSensorDriver {
  void init();
  Data read();
};

// 2. Add to RobotController
controller.addSensor(new_sensor);

// 3. Generate events
EventManager.publish(NewSensorEvent);

// 4. Features use data
feature.onNewSensorEvent(data);
```

### Adding New Features

```cpp
// 1. Implement Feature interface
class NewFeature : public Behavior {
  void execute() override;
};

// 2. Register with StateMachine
stateMachine.addBehavior(new_feature);

// 3. Connect to events
eventManager.subscribe(this, EventType::RELEVANT);

// 4. Use existing drivers
motorDriver.forward(speed);
```

---

## 🧠 Decision Algorithm

### Obstacle Avoidance Decision Tree

```
Is obstacle detected?
├─ NO  → Continue moving forward
│
└─ YES → Scan three sectors
    │
    ├─ LEFT (45°): distance = d_left
    ├─ FRONT (90°): distance = d_front
    └─ RIGHT (135°): distance = d_right
    │
    └─ Choose action based on max clear sector:
        ├─ LEFT clear (>30cm) & closest = turn left
        ├─ RIGHT clear (>30cm) & closest = turn right
        └─ BOTH blocked = reverse & rescan
```

### Line Following Decision

```
Current position = weighted_average(sensors)
Error = position - center

├─ |Error| < 5: Straight (no adjustment)
├─ Error -20 to -5: Gentle left correction
├─ Error < -20: Sharp left turn
├─ Error 5 to 20: Gentle right correction
└─ Error > 20: Sharp right turn
```

---

## 🔗 Dependency Graph

```
Config
 ↓
 ├→ Drivers
 │   ├→ Features
 │   │   └→ Core
 │   │       └→ Communication
 │   │
 │   └→ Core
 │
 └→ Communication
     └→ Core
```

**Rules:**
- Lower layers independent
- Higher layers depend on lower
- No circular dependencies
- Interfaces well-defined

---

## 📚 Related Documentation

- [Structure Guide](structure.md) - Directory organization
- [Hardware Specs](hardware.md) - Component details
- [Wiring Guide](wiring.md) - Pin connections
- [Getting Started](getting-started.md) - Setup guide
