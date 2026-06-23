# Manual de verificación de conexiones

Lista para confirmar, **antes y después de energizar**, que todo esté bien
cableado según los componentes del proyecto. Marca cada casilla.

> Mapa de pines: [wiring.md](wiring.md) · Diagramas: [diagramas.md](diagramas.md)
> · Armado: [manual-cableado.md](manual-cableado.md)

---

## A. Alimentación (lo más crítico)

- [ ] Batería **4× 18650 en serie** → entrega ~16.8V (mídelo).
- [ ] La batería va a **IN+ / IN− del LM2596** (NO directo al L298N).
- [ ] El **display del LM2596 marca ~7.5V** en la salida (ajusta con el tornillo).
- [ ] **OUT+ / OUT− del LM2596 (7.5V)** → entrada de potencia del L298N
      (terminal **+12V / GND**, el bloque azul de 3 tornillos).
- [ ] **5V del L298N → 5V del Arduino** (con su jumper de 5V puesto).
- [ ] **GND común**: batería, LM2596, L298N y Arduino, todos unidos.

> ⚠️ Nunca los 16.8V directo a motores (3-6V) ni al Arduino.

## B. L298N (motores)

- [ ] **ENA** tiene jumper **o** cable a **D5** (¡si no, motor izq. no enciende!).
- [ ] **ENB** tiene jumper **o** cable a **D6** (¡si no, motor der. no enciende!).
- [ ] **IN1→D7, IN2→D8, IN3→D9, IN4→D10**.
- [ ] **OUT1/OUT2 → Motor izquierdo**, **OUT3/OUT4 → Motor derecho**.

> Regla: en cada ENA/ENB, **o jumper, o cable a D5/D6 — nunca los dos**.

## C. Bluetooth HC-05 (ZS-040)

- [ ] **VCC → 5V** (acepta 3.6–6V). **NUNCA al buck de 7.5V** (lo quema).
- [ ] **GND → GND**.
- [ ] **TXD del módulo → D11 del Arduino** (¡no a D0!).
- [ ] **RXD del módulo → D12 con divisor de voltaje** (1kΩ + 2kΩ, el RX es 3.3V).
- [ ] EN y STATE: se dejan **sin conectar**.

> El firmware usa SoftwareSerial en D11/D12. Si el TXD está en D0/D1, no funciona.

## D. Sensores

- [ ] **HC-SR04**: VCC→5V, GND→GND, **TRIG→D2, ECHO→D4**.
- [ ] **Servo SG-90**: rojo→5V, marrón→GND, **naranja→D3**.
- [ ] **Sensor de línea (IR/QTR)**: VCC→5V, GND→GND, **5 canales → A0–A4**.

## E. Reglas generales

- [ ] **D0 y D1 libres** (son el USB; no conectes nada ahí).
- [ ] Sin cables cruzados ni cortes; conexiones firmes (dupont flojos = fallos).
- [ ] Batería **desconectada** mientras revisas.

---

## F. Verificación con multímetro (voltajes esperados)

| Punto de medida | Valor esperado |
|-----------------|----------------|
| Batería (4× 18650 en serie) | ~16.8V |
| Salida del LM2596 (OUT+/OUT−) | **~7.5V** |
| Entrada de potencia del L298N | ~7.5V |
| 5V del Arduino | ~5V |
| Salida a motores (OUT) **con comando + ENA/ENB activos** | ~5.5V |

> Las salidas a motores dan **0V en reposo** — es normal. Solo tienen voltaje
> cuando hay comando en marcha **y** ENA/ENB activados.

## G. Verificación funcional (paso a paso)

1. **Solo Arduino por USB** (sin batería): sube `01_motor_test.ino`.
2. Pon el robot **sobre un soporte** (ruedas al aire) y conecta la potencia.
3. ¿Los motores siguen la secuencia (adelante/atrás/girar)?
   - ✅ Sí → motores, L298N y potencia OK.
   - ❌ No → revisa **ENA/ENB** (sección B) y la **potencia** (sección A).
4. Sube `robot_car.ino`, conecta el HC-05 y abre **Reportes** en la app:
   - ✅ Aparece `Robot: OK:ROBOT LISTO` → el Bluetooth funciona en ambos sentidos.
   - Toca **Manual** → `Robot: OK:MANUAL` → el robot recibe comandos.

---

## Diagnóstico rápido (si algo falla)

| Síntoma | Revisa |
|---------|--------|
| BT conecta pero el carro no responde | TXD→D11 (C), y `Robot: OK:...` en Reportes |
| No llega energía a los motores | ENA/ENB (B) y LM2596 a 7.5V (A) |
| Motores a tope, sin control de velocidad | jumpers ENA/ENB puestos → quítalos y usa D5/D6 |
| El Arduino se reinicia | servo o motores tiran mucha corriente; 5V aparte |
| HC-05 se calienta | VCC con sobre-voltaje (¿7.5V?) → debe ser 5V |
