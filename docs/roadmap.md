# Development Roadmap

Strategic development plan for Robot Car Arduino project.

---

## 🎯 Vision

Create a production-grade autonomous robot platform for education, research, and hobbyist applications with emphasis on modularity, scalability, and reliability.

---

## 📈 Version History & Roadmap

### 📋 v1.0 - Foundation (Planeado)

**Focus:** Plataforma base

**Features objetivo:**
- [ ] Control de 2 motores DC con L298N
- [ ] Distribución de energía
- [ ] Detección básica de obstáculos (HC-SR04)
- [ ] Control manual por Bluetooth (HC-05)

**Documentation:**
- [x] README.md
- [x] Especificaciones de hardware
- [x] Guía de cableado
- [x] Arquitectura inicial

**Status:** 🔄 En desarrollo (sketch de motores listo, resto pendiente)

---

### 📋 v1.1 - Evasión de obstáculos (Planeado)

**Focus:** Navegación autónoma alrededor de obstáculos

**Features objetivo:**
- [ ] Integración del servo (SG90)
- [ ] Escaneo con HC-SR04
- [ ] Algoritmo de detección de obstáculos
- [ ] Lógica de evasión autónoma
- [ ] Selección de ruta

**Implementación (diseño previsto):**
```
Escaneo ultrasónico:
  0° (frente) → escaneo izquierda → escaneo derecha

Lógica de decisión:
  SI obstáculo < 20cm: evadir
  SI izquierda libre: girar izquierda
  SI derecha libre: girar derecha
  SI no: retroceder
```

**Status:** 📋 Planeado (sin implementar)

---

### 📋 v1.2 - Seguidor de línea (Planeado)

**Focus:** Seguimiento de línea por infrarrojo

**Features objetivo:**
- [ ] Integración del QTR-8A (5 canales en A0-A4)
- [ ] Procedimiento de calibración IR
- [ ] Seguimiento con controlador PID
- [ ] Detección de pérdida de línea

**Implementación (diseño previsto):**
```
Algoritmo de seguimiento:
  1. Leer los valores de los 5 canales del QTR-8A
  2. Calcular posición ponderada de la línea
  3. Calcular error respecto al centro
  4. Aplicar controlador PID
  5. Ajustar velocidades de motor
```

**Status:** 📋 Planeado (sin implementar)

---

### 🔄 v2.0 - Enhanced Architecture (Current)

**Target Release:** 2026-Q4
**Focus:** Professional-grade system maturity

**Planned Features:**
- [ ] Advanced state machine (15+ states)
- [ ] Event-driven architecture
- [ ] Error recovery mechanisms
- [ ] Performance monitoring
- [ ] Data logging system
- [ ] Encoder-based odometry
- [ ] Multi-mode coordination

**Architecture Improvements:**
```
Event System:
  - Motor events
  - Sensor events
  - State transition events
  - Error events

State Machine:
  Idle → [Manual | Autonomous | LineFollow] → Error → Recovery
```

**New Capabilities:**
- Combined line + obstacle avoidance
- Smart battery management
- System diagnostics
- Real-time telemetry

**Testing Requirements:**
- 100+ test cases
- Integration test suite
- Stress testing (8-hour runtime)
- Performance benchmarking

**Timeline:**
- Weeks 1-2: Architecture refactor
- Weeks 3-4: State machine implementation
- Weeks 5-6: Event system integration
- Weeks 7-8: Comprehensive testing

**Status:** 🔄 In Progress

---

### 🔮 v3.0 - Connectivity & Expansion (Planned)

**Target Release:** 2027-Q1
**Focus:** IoT integration and web connectivity

**Planned Features:**
- [ ] ESP32 module integration
- [ ] WiFi connectivity
- [ ] MQTT protocol support
- [ ] Cloud telemetry
- [ ] Web dashboard
- [ ] OTA (Over-The-Air) updates
- [ ] Multi-robot coordination

**Hardware Upgrades:**
```
Current (Arduino UNO):
  └─ Single robot control
  └─ Local Bluetooth only

v3.0 (ESP32 addition):
  ├─ Primary: Arduino UNO (motor control, sensors)
  └─ Secondary: ESP32 (WiFi, communication)
```

**Web Interface:**
- Real-time robot status
- Live video streaming (future)
- Remote control dashboard
- Performance analytics
- Mission planning

**Connectivity:**
- Local WiFi (5-10 robots)
- Cloud integration (scalable)
- MQTT broker support
- REST API

**Timeline:**
- Q1 2027: Design phase
- Q2 2027: Prototype & testing
- Q3 2027: Production release

---

### 🚀 v4.0 - Vision & Intelligence (Future Vision)

**Target Release:** 2027-Q3+
**Focus:** Computer vision and advanced autonomy

**Aspirational Features:**
- [ ] Camera module integration
- [ ] OpenCV integration
- [ ] Object recognition
- [ ] Visual SLAM
- [ ] Advanced navigation
- [ ] Machine learning inference
- [ ] Multi-robot swarm coordination

**Hardware Evolution:**
```
v4.0 Platform:
  ├─ Arduino UNO (legacy motor control)
  ├─ ESP32 (primary processing)
  ├─ OV7670 or OV2640 camera
  ├─ IMU sensor (accelerometer + gyro)
  └─ Expanded storage (SPIFFS)
```

**AI Capabilities:**
- Object detection (YOLO / TensorFlow Lite)
- Environment mapping
- Autonomous exploration
- Adaptive behavior learning

**Challenges:**
- Computational power requirements
- Power consumption management
- Real-time processing constraints
- Memory limitations

**Research & Development:**
- Edge computing optimization
- Lightweight ML models
- Efficient inference engines
- Multi-threading architecture

---

## 🛠️ Technology Stack Evolution

| Component | v1.x | v2.0 | v3.0 | v4.0 |
|-----------|------|------|------|------|
| **MCU** | Arduino UNO | Arduino UNO | Arduino + ESP32 | ESP32 Primary |
| **Connectivity** | Bluetooth | Bluetooth | WiFi + BT | WiFi + BT |
| **Communication** | Serial | Serial | MQTT | MQTT + REST |
| **Storage** | EEPROM 1KB | EEPROM 1KB | SD Card | SPIFFS 4MB |
| **Sensors** | Basic | Extended | + IMU | + Camera |
| **Processing** | Real-time | Event-driven | Cloud-connected | AI-enabled |

---

## 📊 Performance Targets

### v2.0 Targets

| Metric | Target | Actual |
|--------|--------|--------|
| **Obstacle Detection** | < 100ms | — (sin medir) |
| **Line Tracking Speed** | 50cm/s | — (sin medir) |
| **Battery Runtime** | 2 hours | — (sin medir) |
| **System Reliability** | 99% uptime | — (sin medir) |
| **Response Latency** | < 50ms | — (sin medir) |

> Los valores "Actual" se llenarán cuando exista código y se midan de verdad.

### v3.0 Targets

- WiFi connectivity: Sub-100ms
- Cloud telemetry: Real-time
- OTA update time: < 2 minutes
- Multi-robot sync: < 50ms

### v4.0 Targets

- Vision processing: 30 FPS
- Object detection: 95% accuracy
- AI inference: < 500ms per frame
- Autonomous range: > 100m

---

## 🎯 Priority Features

### Critical (MVP)
1. [ ] Control de motores confiable (sketch de prueba listo, falta integrar)
2. [ ] Detección de obstáculos
3. [ ] Seguimiento de línea estable
4. [ ] Control manual por Bluetooth
5. [ ] Optimización de rendimiento

### Important (v2.0)
6. [ ] Error recovery mechanisms
7. [ ] System diagnostics
8. [ ] Data logging
9. [ ] Battery management
10. [ ] Multi-mode coordination

### Nice-to-have (v3.0+)
11. [ ] WiFi integration
12. [ ] Web dashboard
13. [ ] Cloud sync
14. [ ] Advanced analytics
15. [ ] AI integration

---

## 🔄 Maintenance & Support

### Current Maintenance

- **Security Updates:** As needed
- **Bug Fixes:** Within 1 week of report
- **Documentation:** Updated with each release
- **Testing:** Continuous integration

### Support Lifecycle

```
v1.0 ──┬─ EOL: 2027-Q2
       │
v1.1 ──┼─ EOL: 2027-Q3
       │
v1.2 ──┼─ EOL: 2027-Q4
       │
v2.0 ──┴─ LTS: Until v3.0 stable
       └─ Maintenance: 12 months

v3.0 ──┬─ Production: 2027-Q1
       └─ LTS: 24 months support
```

---

## 🤝 Community Contributions

### Encouraged Contributions for v2.0

1. **Performance Optimization**
   - Motor PWM tuning
   - Sensor calibration algorithms
   - PID controller refinement

2. **Additional Sensors**
   - Temperature sensors
   - Gyroscope/Accelerometer
   - Additional ultrasonic units

3. **Documentation**
   - Tutorial videos
   - Usage examples
   - Troubleshooting guides

4. **Alternative Platforms**
   - ARM Cortex-M platforms
   - STM32 ports
   - Raspberry Pi Pi Zero variants

### Contribution Process

1. Fork repository
2. Create feature branch
3. Implement feature
4. Add tests
5. Submit pull request
6. Code review
7. Merge to main

See [CONTRIBUTING.md](../CONTRIBUTING.md) for details.

---

## 📋 Milestone Schedule

```
v0.1  Estructura + prueba de motores   🔄 En curso
v1.0  Plataforma base + Bluetooth       📋 Planeado
v1.1  Evasión de obstáculos             📋 Planeado
v1.2  Seguimiento de línea (IR)         📋 Planeado
v2.0  Refactor a módulos (si hace falta)📋 Planeado
v3.0  ESP32 + WiFi                       🔮 Futuro
v4.0  Visión / cámara                    🚀 Visión
```

---

## 🔗 Related Documentation

- [Architecture Details](architecture.md)
- [Hardware Specifications](hardware.md)
- [Testing Procedures](testing.md)
- [Contributing Guidelines](../CONTRIBUTING.md)
