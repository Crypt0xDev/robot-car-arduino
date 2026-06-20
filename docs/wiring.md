# Wiring Guide

Conexiones de pines del Robot Car Arduino (Arduino UNO).

> Este mapa de pines es el **oficial** del proyecto y coincide con el sketch
> `firmware/01_motor_test/01_motor_test.ino`. Cada pin se usa una sola vez.

## 📌 Arduino UNO Pin Allocation

| Pin | Componente | Función | Modo |
|-----|-----------|---------|------|
| D2 | HC-SR04 | Ultrasonic Trigger | Output |
| D3 | SG90 | Servo Control | Output (PWM) |
| D4 | HC-SR04 | Ultrasonic Echo | Input |
| D5 | L298N | ENA — velocidad motor izquierdo | Output (PWM) |
| D6 | L298N | ENB — velocidad motor derecho | Output (PWM) |
| D7 | L298N | IN1 — dirección motor izquierdo | Output |
| D8 | L298N | IN2 — dirección motor izquierdo | Output |
| D9 | L298N | IN3 — dirección motor derecho | Output |
| D10 | L298N | IN4 — dirección motor derecho | Output |
| D11 | HC-05 | Serial RX (SoftwareSerial) | Input |
| D12 | HC-05 | Serial TX (SoftwareSerial) | Output |
| A0-A4 | QTR-8A | Array de línea (5 de 8 canales) | Input (ADC) |

> Pines PWM válidos en el UNO: 3, 5, 6, 9, 10, 11. Por eso ENA/ENB (velocidad)
> van en D5/D6 y el servo en D3. D0/D1 se dejan libres (UART de hardware,
> necesario para cargar programas y el Monitor Serie).

---

## 🔌 Detailed Connections

### Motor Driver L298N ↔ Arduino UNO

```
L298N         Arduino UNO
┌─────────┐   ┌─────────┐
│ ENA  ───┼─→ D5 (PWM)   velocidad motor izquierdo
│ IN1  ───┼─→ D7
│ IN2  ───┼─→ D8
│ IN3  ───┼─→ D9
│ IN4  ───┼─→ D10
│ ENB  ───┼─→ D6 (PWM)   velocidad motor derecho
│ GND  ───┼─→ GND        (masa común obligatoria)
└─────────┘   └─────────┘

Motores:
OUT1/OUT2 → Motor izquierdo
OUT3/OUT4 → Motor derecho

Alimentación de motores: entrada de potencia del L298N (no desde el 5V del
Arduino). Comparte siempre GND con el Arduino.
```

### Ultrasonic Sensor HC-SR04 ↔ Arduino UNO

```
HC-SR04       Arduino UNO
┌────────┐    ┌─────────┐
│ VCC ───┼──→ 5V
│ GND ───┼──→ GND
│ TRIG ──┼──→ D2
│ ECHO ──┼──→ D4
└────────┘    └─────────┘

Operación:
1. Pulso de 10µs HIGH en D2 (TRIG)
2. Mide el ancho del pulso en D4 (ECHO) con pulseIn()
3. Distancia(cm) = duracion_us / 58
```

### Servo Motor SG90 ↔ Arduino UNO

```
SG90 (3 cables)  Arduino UNO
┌──────────┐     ┌─────────┐
│ Marrón ──┼───→ GND
│ Rojo  ───┼───→ 5V
│ Naranja ─┼───→ D3 (PWM)
└──────────┘     └─────────┘

Señal PWM (50Hz): 1.0ms=0°, 1.5ms=90°, 2.0ms=180°
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

Serial: 9600 baud, 8 data bits, 1 stop bit, sin paridad.
Nota: el pin RX del HC-05 es 3.3V; usa divisor de voltaje desde D12 (5V).
```

### Line Sensor Array QTR-8A ↔ Arduino UNO

```
QTR-8A           Arduino UNO (Analog)
┌──────────────┐ ┌─────────┐
│ VCC ────────→ 5V
│ GND ────────→ GND
│ canal 1 ─────→ A0
│ canal 2 ─────→ A1
│ canal 3 ─────→ A2
│ canal 4 ─────→ A3
│ canal 5 ─────→ A4
│ canales 6-8 ─→ (sin usar)
└──────────────┘ └─────────┘

El QTR-8A tiene 8 canales, pero el UNO solo tiene A0-A5 (6 entradas
analógicas). Se conectan 5 canales (A0-A4); con eso basta para seguir línea.
Reparte los 5 cables a lo ancho del array para cubrir bien la línea.

Valores típicos (analogRead, 0-1023):
Línea negra: bajo (poca reflexión)
Piso blanco: alto (mucha reflexión)
Los umbrales se calibran (ver testing.md).
```

---

## 🔋 Power Distribution

### ⚠️ Batería: 4x 18650 Li-ion en serie = 16.8V (¡requiere reductor!)

La batería es **4 celdas 18650 de 4.2V en serie**:
- 16.8V cargadas / 14.8V nominal / ~12V descargadas.

Ese voltaje es **demasiado alto** para conectarlo directo:
- Motores del kit: **3-6V** → 16.8V los **quema**.
- Arduino VIN: máx. recomendado **12V** → 16.8V **daña el regulador**.

**Solución obligatoria:** un convertidor reductor (buck) **LM2596** ajustado a
~7.5V entre la batería y el robot.

```
Batería 4S 18650 (16.8V)
   │
   ▼
[LM2596 buck]  ── ajustar salida a ~7.5V CON MULTIMETRO antes de conectar
   │
   ├──→ L298N VS (entrada de motores)  → motores reciben ~5.5V ✅ (3-6V OK)
   │
   └──→ con el jumper de 5V del L298N puesto (válido si VS ≤ 12V),
        su salida de 5V alimenta el Arduino (pin 5V) ✅

⚠️ GND COMÚN obligatorio entre batería, buck, L298N y Arduino.
⚠️ NUNCA conectar los 16.8V directo al Arduino ni a los motores.
```

**Pasos seguros:**
1. Conecta solo la batería al LM2596 y ajusta su salida a **7.5V** con un
   multímetro (gira el tornillo del potenciómetro). Hazlo ANTES de conectar
   el resto.
2. Lleva la salida del buck al VS del L298N.
3. Verifica que el 5V del L298N realmente da ~5V antes de enchufar el Arduino.

---

## 📋 Full Pinout Reference Table

| Arduino Pin | Tipo | Componente | Función |
|-------------|------|-----------|---------|
| D0 | RX | Serial HW | UART (cargar/Monitor) — libre |
| D1 | TX | Serial HW | UART (cargar/Monitor) — libre |
| D2 | Digital | HC-SR04 | Trigger |
| D3 | PWM | SG90 | Servo |
| D4 | Digital | HC-SR04 | Echo |
| D5 | PWM | L298N | ENA (vel. izq.) |
| D6 | PWM | L298N | ENB (vel. der.) |
| D7 | Digital | L298N | IN1 |
| D8 | Digital | L298N | IN2 |
| D9 | Digital | L298N | IN3 |
| D10 | Digital | L298N | IN4 |
| D11 | Digital | HC-05 | Soft RX |
| D12 | Digital | HC-05 | Soft TX |
| D13 | Digital | LED_BUILTIN | Indicador/diagnóstico |
| A0 | ADC | QTR-8A | Canal de línea 1 |
| A1 | ADC | QTR-8A | Canal de línea 2 |
| A2 | ADC | QTR-8A | Canal de línea 3 |
| A3 | ADC | QTR-8A | Canal de línea 4 |
| A4 | ADC | QTR-8A | Canal de línea 5 |
| A5 | ADC | — | Libre |
| 5V | Power | Todos | Distribución +5V |
| GND | Power | Todos | Masa común (varios puntos) |

---

## 🔗 Connection Checklist

**Antes de energizar:**

- [ ] LM2596 ajustado a 7.5V (con multímetro) ANTES de conectar el robot
- [ ] Jumpers de ENA y ENB del L298N **quitados** (si no, no hay control de velocidad)
- [ ] L298N: ENA→D5, ENB→D6, IN1→D7, IN2→D8, IN3→D9, IN4→D10
- [ ] Motores: OUT1/2 (izq.) y OUT3/4 (der.)
- [ ] HC-SR04: TRIG→D2, ECHO→D4
- [ ] Servo: señal→D3 (PWM)
- [ ] HC-05: TX→D11, RX→D12 (con divisor de voltaje en RX)
- [ ] QTR-8A: 5 canales en A0-A4 (canales 6-8 sin usar)
- [ ] GND común entre batería, **LM2596**, L298N y Arduino
- [ ] Sin cables cruzados ni cortos
- [ ] Batería desconectada al inicio

**Secuencia de encendido:**

1. Verificar conexiones con multímetro (continuidad)
2. Ajustar el LM2596 a 7.5V (solo batería + buck, sin el resto)
3. Conectar la batería al **LM2596** (NO directo al L298N) y la salida del
   buck (7.5V) al VS del L298N
4. Comprobar que enciende el LED del L298N
5. Subir sketch (ya probado) al Arduino
6. Probar motores sobre soporte (ruedas al aire)
7. Calibrar sensores

---

## 🚨 Common Wiring Mistakes

| Error | Problema | Solución |
|-------|----------|----------|
| **16.8V directo al L298N** (sin LM2596) | **Quema los motores (3-6V)** | Siempre pasar por el LM2596 a 7.5V |
| **Jumpers de ENA/ENB puestos** | Velocidad fija a tope, el slider no hace nada | Quitar ambos jumpers y cablear a D5/D6 |
| Servo desde el 5V del L298N | Picos del servo reinician el Arduino | Alimentar el servo de un 5V con más corriente |
| Polaridad invertida | Componentes dañados | Diodo + fusible |
| Sin GND común | Fallos esporádicos | Unir todos los GND |
| Cables de potencia finos | Caída de voltaje/calor | Usar 18-20 AWG |
| Sensores en D0/D1 | Rompe carga y Serial | Dejar D0/D1 libres |
| ENA/ENB en pin no-PWM | Sin control de velocidad | Usar 3,5,6,9,10,11 |
| RX del HC-05 a 5V directo | Daña el módulo | Divisor de voltaje en RX |

---

## 📚 Additional Resources

- [Arduino Pin Reference](https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/)
- [Wiring Best Practices](https://learn.sparkfun.com/tutorials/how-to-power-a-project)
- [Fritzing](https://fritzing.org/)
