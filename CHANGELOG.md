# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

> Nota de honestidad: este changelog registra **solo lo que existe de verdad
> en el repositorio**. Las versiones futuras (v1.0+) son objetivos del roadmap,
> no releases publicadas. Ver [docs/roadmap.md](docs/roadmap.md).

---

## [Unreleased] — En desarrollo

### Added
- Estructura inicial del repositorio (Core/, Drivers/, Features/, Communication/, Config/).
- Documentación de arquitectura, hardware, wiring, estructura y getting-started.
- Código funcional en `firmware/` (sketches probados por separado):
  - `01_motor_test` — movimiento 2WD con L298N.
  - `02_ultrasonic_test` — medición de distancia con HC-SR04.
  - `03_servo_test` — control del servo SG90.
  - `04_obstacle_avoidance` — evasión autónoma (motores + sensor + servo).
  - `05_bluetooth_control` — control manual por HC-05 con failsafe.
  - `06_line_follower` — seguidor de línea IR con control proporcional.
  - `robot_car` — firmware integrado: máquina de estados (enum + switch) que
    une los modos manual/evasión/línea con cambio por Bluetooth.
- `firmware/README.md` con el orden de pruebas y mapa de pines.

### Fixed
- Conflictos de pines en los ejemplos de `getting-started.md` (varias funciones
  asignadas a los pines 2 y 3). Reasignados a un mapa válido para el UNO.
- URL incorrecta de ESP32 en el paso de instalación de "Arduino AVR Boards".

### Changed
- README: estado de las features actualizado a su situación real
  (en desarrollo / planeado) en vez de "implementado".

### Próximos pasos (roadmap, aún NO implementado)
- Calibración de los sensores IR y ajuste fino del control de línea (PID completo).
- Sistema de telemetría por Bluetooth (enviar distancia/modo al celular).
- Pruebas físicas y ajuste de velocidades/giros por hardware real.

---

## Roadmap de versiones (objetivos, no releases)

| Versión | Objetivo | Estado |
|---------|----------|--------|
| v0.1 | Estructura + prueba de motores | 🔄 En curso |
| v1.0 | Movimiento 2WD + Bluetooth + detección de obstáculos | 📋 Planeado |
| v1.1 | Evasión de obstáculos + servo | 📋 Planeado |
| v1.2 | Seguimiento de línea (IR) | 📋 Planeado |
| v2.0 | Refactor a módulos según haga falta | 📋 Planeado |
| v3.0 | Migración a ESP32 + WiFi | 🔮 Futuro |

---

## License

All changes are licensed under MIT License. See [LICENSE](LICENSE) for details.
