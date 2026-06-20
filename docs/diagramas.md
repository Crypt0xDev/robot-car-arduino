# Diagramas del sistema

Diagramas de conexión del Robot Car 2WD para armado y sustentación.
Todas las conexiones coinciden con [wiring.md](wiring.md) y el firmware
`firmware/robot_car/robot_car.ino`.

---

## 1. Diagrama de energía (flujo de voltaje)

Cómo viaja la corriente desde la batería hasta los motores.

```
        🔋 Batería 4× 18650 (en serie)
                 │  16.8 V
                 ▼
        ⚡ Regulador LM2596 (con display)   ← ajustar salida a 7.5 V
                 │  7.5 V
                 ▼
        ┌────────────────────┐
        │     Driver L298N    │── 5 V ──► Arduino UNO
        │      (VS = 7.5 V)   │◄─ señales ─ (ENA/ENB, IN1–IN4)
        └─────────┬──────────┘
                  │  ~5.5 V
                  ▼
            ⚙️ 2 Motores DC  (rango 3–6 V)
```

> ⚠️ Reglas de oro
> - La batería entrega **16.8 V**: NUNCA va directo a motores (3–6 V) ni al Arduino.
> - La salida del LM2596 se ajusta a **7.5 V** (no 14 V, no 16.8 V).
> - **GND común** obligatorio entre batería, LM2596, L298N y Arduino.

---

## 2. Mapa de pines (Arduino UNO)

| Módulo | Conexión → Pin Arduino |
|--------|------------------------|
| **L298N** (motores) | ENA→D5, ENB→D6, IN1→D7, IN2→D8, IN3→D9, IN4→D10 |
| **HC-SR04** (ultrasónico) | TRIG→D2, ECHO→D4 |
| **Servo SG-90** | Señal→D3 (PWM) |
| **HC-05** (Bluetooth) | TX→D11, RX→D12 (con divisor de voltaje) |
| **QTR-8A** (línea) | 5 canales → A0, A1, A2, A3, A4 |
| **Alimentación** | 5V y GND comunes |

> D0/D1 quedan libres (UART/USB para programar y el Monitor Serie). A5 libre.

---

## 3. Tabla de conexiones (para armar o reproducir en Fritzing)

| Desde | Hasta |
|-------|-------|
| Batería + / − | LM2596 IN+ / IN− |
| LM2596 OUT+ / − (7.5 V) | L298N +12V / GND |
| L298N +5V / GND | Arduino 5V / GND |
| Arduino D5, D6, D7, D8, D9, D10 | L298N ENA, ENB, IN1, IN2, IN3, IN4 |
| L298N OUT1/OUT2 | Motor izquierdo |
| L298N OUT3/OUT4 | Motor derecho |
| HC-SR04 VCC/GND/TRIG/ECHO | 5V / GND / D2 / D4 |
| Servo señal/VCC/GND | D3 / 5V / GND |
| HC-05 TX/RX/VCC/GND | D11 / D12* / 5V / GND |
| QTR-8A (5 canales)/VCC/GND | A0–A4 / 5V / GND |

\* El RX del HC-05 es de 3.3 V: usar divisor de voltaje (1kΩ + 2kΩ) desde D12.

---

## Notas

- **Hardware del kit:** Arduino UNO, L298N, HC-SR04, servo SG-90, QTR-8A,
  HC-05, encoders (sin usar en el firmware actual), 2 motorreductores.
- **Componente extra a comprar:** regulador **LM2596** (idealmente el modelo
  con display, para ajustar los 7.5 V sin multímetro).
- Para un diagrama pictórico imprimible se recomienda
  [Fritzing](https://fritzing.org) usando la tabla de conexiones de arriba.

## Relacionado
- [wiring.md](wiring.md) — guía de cableado detallada
- [hardware.md](hardware.md) — lista de componentes
- [getting-started.md](getting-started.md) — armado paso a paso
