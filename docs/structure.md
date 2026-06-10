# Project Structure

Comprehensive guide to the Robot Car Arduino project organization and module hierarchy.

## 📁 Directory Tree

```
robot-car-arduino/
│
├── 📚 docs/                           # Complete documentation
│   ├── architecture.md               # System design and patterns
│   ├── hardware.md                   # Hardware specifications
│   ├── wiring.md                     # Pin connections and diagrams
│   ├── structure.md                  # This file
│   ├── testing.md                    # Testing procedures
│   ├── roadmap.md                    # Development roadmap
│   └── getting-started.md            # Quick start guide
│
├── 🧠 Core/                          # Central logic layer
│   ├── README.md                     # Core module overview
│   ├── RobotController/              # Main coordinator
│   │   ├── RobotController.h
│   │   ├── RobotController.cpp
│   │   └── README.md
│   ├── StateMachine/                 # State management
│   │   ├── StateMachine.h
│   │   ├── StateMachine.cpp
│   │   └── README.md
│   └── EventManager/                 # Event system
│       ├── EventManager.h
│       ├── EventManager.cpp
│       └── README.md
│
├── 🔌 Drivers/                       # Hardware abstraction
│   ├── README.md                     # Drivers overview
│   ├── MotorDriver/                  # DC motor control
│   │   ├── MotorDriver.h
│   │   ├── MotorDriver.cpp
│   │   └── README.md
│   ├── UltrasonicDriver/             # Distance sensor
│   │   ├── UltrasonicDriver.h
│   │   ├── UltrasonicDriver.cpp
│   │   └── README.md
│   ├── ServoDriver/                  # Servo control
│   │   ├── ServoDriver.h
│   │   ├── ServoDriver.cpp
│   │   └── README.md
│   └── EncoderDriver/                # Odometry feedback
│       ├── EncoderDriver.h
│       ├── EncoderDriver.cpp
│       └── README.md
│
├── 🎮 Features/                      # High-level behaviors
│   ├── README.md                     # Features overview
│   ├── ObstacleAvoidance/            # Autonomous collision avoidance
│   │   ├── ObstacleAvoidance.h
│   │   ├── ObstacleAvoidance.cpp
│   │   └── README.md
│   ├── LineFollower/                 # IR-based line tracking
│   │   ├── LineFollower.h
│   │   ├── LineFollower.cpp
│   │   └── README.md
│   ├── ManualControl/                # Remote control mode
│   │   ├── ManualControl.h
│   │   ├── ManualControl.cpp
│   │   └── README.md
│   └── AutonomousMode/               # Fully autonomous operation
│       ├── AutonomousMode.h
│       ├── AutonomousMode.cpp
│       └── README.md
│
├── 📡 Communication/                 # External communication
│   ├── README.md                     # Communication overview
│   ├── Bluetooth/                    # HC-05 wireless link
│   │   ├── BluetoothHandler.h
│   │   ├── BluetoothHandler.cpp
│   │   └── README.md
│   ├── Telemetry/                    # System monitoring
│   │   ├── Telemetry.h
│   │   ├── Telemetry.cpp
│   │   └── README.md
│   └── WiFi/                         # Future ESP32 support
│       ├── WiFiHandler.h
│       ├── WiFiHandler.cpp
│       └── README.md
│
├── ⚙️ Config/                        # Global configuration
│   ├── README.md                     # Configuration overview
│   ├── PinDefinitions.h              # Arduino pin mappings
│   ├── Constants.h                   # System constants
│   ├── Thresholds.h                  # Sensor thresholds
│   └── HardwareConfig.h              # Hardware settings
│
├── 🧪 tests/                         # Test suite
│   ├── README.md                     # Testing overview
│   ├── unit/                         # Unit tests
│   │   ├── test_motors.cpp
│   │   ├── test_sensors.cpp
│   │   └── test_communication.cpp
│   ├── integration/                  # Integration tests
│   │   ├── test_obstacle_avoidance.cpp
│   │   ├── test_line_following.cpp
│   │   └── test_autonomous_mode.cpp
│   └── calibration/                  # Calibration utilities
│       ├── calibrate_sensors.ino
│       └── calibrate_motors.ino
│
├── 📊 schematics/                    # Hardware diagrams
│   ├── README.md                     # Schematics overview
│   ├── schematic.pdf                 # Main circuit diagram
│   ├── wiring_diagram.pdf            # Pin connections
│   └── breadboard_layout.png         # Physical layout
│
├── 🖼️ assets/                        # Multimedia resources
│   ├── README.md                     # Assets overview
│   ├── images/                       # Photos & diagrams
│   │   ├── robot_overview.jpg
│   │   ├── assembly_guide.pdf
│   │   └── components.png
│   ├── videos/                       # Demonstrations
│   │   ├── obstacle_avoidance_demo.mp4
│   │   ├── line_following_demo.mp4
│   │   └── autonomous_nav_demo.mp4
│   └── audio/                        # Sound effects
│       ├── beep_forward.wav
│       └── beep_obstacle.wav
│
├── 🔐 .github/                       # GitHub configuration
│   ├── workflows/                    # CI/CD pipelines
│   └── ISSUE_TEMPLATE/               # Issue templates
│       └── bug_report.md
│
├── 📄 Root Documentation
│   ├── README.md                     # Main landing page
│   ├── CHANGELOG.md                  # Version history
│   ├── CONTRIBUTING.md               # Contribution guidelines
│   ├── LICENSE                       # MIT License
│   └── .gitignore                    # Git ignore rules
│
└── 📋 Configuration Files
    └── .gitignore                    # Git configuration
```

---

## 🏗️ Module Organization

### Layer 1: Core (Lógica Central)

**Purpose:** Orchestration and system coordination

**Components:**
- **RobotController:** Central hub managing all subsystems
- **StateMachine:** Mode transitions (Idle → Manual → Autonomous)
- **EventManager:** Internal event propagation

**Responsibilities:**
- Initialize all modules on startup
- Manage robot operational states
- Coordinate inter-module communication
- Handle emergency stop and safety

**Dependencies:** None (lowest level abstraction)

---

### Layer 2: Drivers (Hardware Abstraction)

**Purpose:** Direct hardware control with consistent interfaces

**Components:**
- **MotorDriver:** DC motor control with speed/direction
- **UltrasonicDriver:** Distance measurement abstraction
- **ServoDriver:** Servo positioning control
- **EncoderDriver:** Rotation counting and velocity

**Interfaces:**
```cpp
MotorDriver:
  - forward(speed)
  - backward(speed)
  - stop()

UltrasonicDriver:
  - getDistance() → cm
  - isObstacleDetected(threshold) → bool

ServoDriver:
  - setAngle(degrees)
  - sweep()

EncoderDriver:
  - getPulseCount() → uint32_t
  - getVelocity() → rpm
```

**Dependencies:** Config (pin definitions)

---

### Layer 3: Features (High-Level Behaviors)

**Purpose:** Autonomous behaviors using driver abstractions

**Components:**
- **ObstacleAvoidance:** Scanning + decision making
- **LineFollower:** IR sensor reading + trajectory correction
- **ManualControl:** Bluetooth command processing
- **AutonomousMode:** Full autonomous operation

**Workflow:**
```
Feature receives input
  ↓
Processes via Drivers
  ↓
Updates system state via EventManager
  ↓
Returns control to RobotController
```

**Dependencies:** Drivers, Config, Communication

---

### Layer 4: Communication (External Interface)

**Purpose:** Robot-to-world communication

**Components:**
- **Bluetooth:** Wireless remote control (HC-05)
- **Telemetry:** Real-time status transmission
- **WiFi:** Future IoT connectivity (ESP32)

**Protocols:**
- Bluetooth: 9600 baud serial
- Telemetry: JSON-like messages
- WiFi: MQTT/REST (planned)

**Dependencies:** Core, Drivers

---

### Layer 5: Config (Global Settings)

**Purpose:** Centralized configuration management

**Files:**
- **PinDefinitions.h:** Arduino pin mapping
- **Constants.h:** System parameters
- **Thresholds.h:** Sensor calibration values
- **HardwareConfig.h:** Device identifiers

**Usage:**
```cpp
#include "Config/PinDefinitions.h"

// Prevents magic numbers in code
digitalWrite(MOTOR_LEFT_PIN, HIGH);
```

**Dependencies:** None

---

## 📦 Module Dependencies

```
Config
  ↓
  ├→ Drivers
  │   ├→ Features
  │   │   ├→ Communication
  │   │   └→ Core
  │   └→ Core
  │
  └→ Communication
      └→ Core
```

### Dependency Rules

1. **Core** is at the top (depends on nothing architectural)
2. **Drivers** depend only on **Config**
3. **Features** depend on **Drivers** + **Config**
4. **Communication** depends on **Core** + **Drivers**
5. **No circular dependencies** allowed

---

## 🔄 Data Flow

### Obstacle Avoidance Flow

```
UltrasonicDriver
  ↓ (raw distance measurement)
ObstacleAvoidance
  ↓ (obstacle detected event)
EventManager
  ↓ (broadcast to listeners)
RobotController
  ↓ (decide action)
MotorDriver
  ↓ (execute movement)
Robot Motion
```

### Line Following Flow

```
LineFollower (QTR-8A input)
  ↓ (sensor array values)
Compute error (line position)
  ↓
Adjust motor speeds
  ↓
MotorDriver
  ↓
Stay on line
```

### Remote Control Flow

```
BluetoothHandler (HC-05 data)
  ↓ (parse command)
ManualControl
  ↓ (interpret action)
EventManager
  ↓ (event dispatch)
RobotController
  ↓ (execute command)
Drivers (Motors/Servo)
  ↓
Robot responds
```

---

## 📊 File Organization Best Practices

### Naming Conventions

| Type | Pattern | Example |
|------|---------|---------|
| Header Files | `ModuleName.h` | `MotorDriver.h` |
| Source Files | `ModuleName.cpp` | `MotorDriver.cpp` |
| Test Files | `test_modulename.cpp` | `test_motors.cpp` |
| Constants | `CONSTANT_NAME` | `MOTOR_MAX_SPEED` |
| Functions | `camelCase` | `getDistance()` |
| Classes | `PascalCase` | `MotorDriver` |

### Directory Guidelines

- Each module gets its own directory
- README.md in every directory
- Related files stay together
- Avoid deep nesting (max 3 levels)
- Configuration files centralized

---

## 🚀 Adding New Features

### Step 1: Plan in correct layer

- High-level behavior? → Features layer
- Low-level hardware? → Drivers layer
- Global config? → Config layer

### Step 2: Create directory structure

```
NewFeature/
├── NewFeature.h
├── NewFeature.cpp
└── README.md
```

### Step 3: Declare dependencies

```cpp
// In NewFeature.h
#include "../Config/PinDefinitions.h"
#include "../Drivers/MotorDriver.h"
```

### Step 4: Integrate with Core

```cpp
// In RobotController.cpp
#include "Features/NewFeature.h"

NewFeature newFeature;

void RobotController::update() {
  newFeature.execute();
}
```

### Step 5: Document

- Add comprehensive README.md
- Document interfaces
- Provide usage examples
- List dependencies

---

## 🧪 Testing Structure

### Unit Tests
- Test individual modules in isolation
- Mock external dependencies
- Verify correct behavior

### Integration Tests
- Test modules working together
- Verify data flows correctly
- Test error handling

### Calibration Tools
- Sensor calibration utilities
- Motor speed verification
- System baseline establishment

---

## 📈 Scalability Considerations

Current architecture supports:
- ✅ Adding new sensors (just add driver)
- ✅ Adding new behaviors (just add feature)
- ✅ Alternative communication (just add handler)
- ✅ Different hardware (update config + drivers)
- ✅ Multiple robots (replicate instance)

---

## 🔗 Cross-References

- [Architecture Details](architecture.md) - Design patterns used
- [Hardware Specifications](hardware.md) - Component specs
- [Wiring Guide](wiring.md) - Pin connections
- [Testing Guide](testing.md) - Test procedures
- [Getting Started](getting-started.md) - Quick start
