# Manual de instalación del cableado

Guía paso a paso para cablear el Robot Car 2WD con los componentes del kit.
Hazlo **en este orden**: primero la alimentación, después motores y al final los
sensores. No conectes la batería hasta el último paso.

> Mapa de pines y diagramas: [wiring.md](wiring.md) · [diagramas.md](diagramas.md)

---

## Herramientas y materiales

- Multímetro (imprescindible para ajustar el LM2596).
- Destornillador pequeño (bornes del L298N y del buck).
- Cables dupont (vienen en el kit) y un par de resistencias: **1 kΩ y 2 kΩ**
  (para el HC-05).
- Cinta doble cara o separadores para fijar las placas.

## Componentes que vas a cablear

| Componente | Función |
|-----------|---------|
| Arduino UNO | Controlador |
| L298N | Driver de los 2 motores |
| LM2596 (con display) | Baja 16.8V → 7.5V *(comprar aparte)* |
| 4× 18650 (en serie) | Batería = 16.8V |
| 2 motorreductores | Tracción |
| HC-SR04 | Distancia (obstáculos) |
| Servo SG-90 | Mueve el HC-SR04 |
| HC-05 | Bluetooth (control por app) |
| QTR-8A | Sensor de línea |

## ⚠️ Seguridad antes de empezar

- **Batería desconectada** durante todo el cableado.
- Los 16.8V de la batería **nunca** tocan los motores (3-6V) ni el Arduino.
- Revisa polaridad (+ rojo, − negro) en cada conexión.

---

## Paso 1 — Ajustar el LM2596 (antes de cablear nada más)

1. Conecta **solo** la batería (16.8V) a la entrada del buck: **IN+ / IN−**.
2. Mira el display (o mide con el multímetro en **OUT+ / OUT−**) y gira el
   tornillo del potenciómetro hasta leer **7.5 V**.
3. Desconecta la batería. Ya quedó calibrado.

> Si el módulo tiene botón para alternar entrada/salida en el display,
> asegúrate de estar viendo la **salida (OUT)** al ajustar.

## Paso 2 — Preparar el L298N (quitar jumpers)

- Quita los **jumpers de ENA y ENB** (los capuchones de 2 pines). Si no, los
  motores van a tope fijo y el control de velocidad no funcionará.
- Más adelante, el cable del Arduino irá al pin de **señal** de cada header
  (el otro pin es +5V y se deja libre — ver [wiring.md](wiring.md)).
- Deja el **jumper de 5V** del L298N **puesto** (sirve para que el L298N
  entregue 5V al Arduino; es válido porque la entrada será 7.5V ≤ 12V).

## Paso 3 — Alimentación (la base de todo)

```
Batería 16.8V  →  LM2596 (7.5V)  →  L298N (VS)  →  Arduino (5V del L298N)
```

1. Batería **+ / −** → **IN+ / IN−** del LM2596.
2. **OUT+ / OUT−** del LM2596 (7.5V) → bornes **+12V / GND** del L298N.
3. Pin **5V** del L298N → pin **5V** del Arduino.
4. **GND común:** une los GND de batería, LM2596, L298N y Arduino.

> ⚠️ La batería va al **LM2596**, NUNCA directo al L298N.

## Paso 4 — Motores

- **OUT1 / OUT2** del L298N → **Motor izquierdo**.
- **OUT3 / OUT4** del L298N → **Motor derecho**.

> Si un motor gira al revés, intercambia sus dos cables (o se ajusta en código).

## Paso 5 — Señales del L298N al Arduino

| L298N | Arduino |
|-------|---------|
| ENA (señal) | D5 (PWM) — velocidad izq. |
| ENB (señal) | D6 (PWM) — velocidad der. |
| IN1 | D7 |
| IN2 | D8 |
| IN3 | D9 |
| IN4 | D10 |

## Paso 6 — Sensor ultrasónico HC-SR04

| HC-SR04 | Arduino |
|---------|---------|
| VCC | 5V |
| GND | GND |
| TRIG | D2 |
| ECHO | D4 |

## Paso 7 — Servo SG-90

| Cable servo | Arduino |
|-------------|---------|
| Naranja (señal) | D3 (PWM) |
| Rojo | 5V |
| Marrón | GND |

> Si el Arduino se reinicia al mover el servo, aliméntalo de un 5V con más
> corriente (no solo del L298N).

## Paso 8 — Bluetooth HC-05 (¡ojo con el voltaje!)

| HC-05 | Arduino |
|-------|---------|
| VCC | 5V |
| GND | GND |
| TX | D11 |
| RX | D12 **con divisor de voltaje** |

El **RX del HC-05 es de 3.3V**. No le metas los 5V de D12 directo. Divisor:

```
D12 ──[ 1 kΩ ]──┬──→ RX del HC-05
                │
             [ 2 kΩ ]
                │
               GND
```

## Paso 9 — Sensor de línea QTR-8A

| QTR-8A | Arduino |
|--------|---------|
| VCC | 5V |
| GND | GND |
| 5 canales | A0, A1, A2, A3, A4 |

> El QTR tiene 8 canales pero el UNO solo tiene A0–A5: se usan 5. Reparte los
> cables a lo ancho del sensor para cubrir bien la línea.

---

## Verificación antes de energizar

- [ ] LM2596 ya ajustado a 7.5V (Paso 1).
- [ ] Jumpers de ENA/ENB quitados; jumper de 5V puesto.
- [ ] Batería → LM2596 (no al L298N).
- [ ] GND común entre batería, LM2596, L298N y Arduino.
- [ ] HC-05 con divisor de voltaje en RX.
- [ ] Sin cables cruzados ni cortos.
- [ ] D0/D1 libres (son el USB).

## Secuencia de encendido

1. Verifica continuidad con el multímetro.
2. Sube el sketch al Arduino (empieza por `01_motor_test`).
3. **Levanta el robot sobre un soporte** (ruedas al aire).
4. Conecta la batería y enciende el interruptor.
5. Comprueba que enciende el LED del L298N.
6. Prueba los motores; luego sensores; al final `robot_car.ino`.

## Problemas comunes

| Síntoma | Causa probable | Solución |
|---------|----------------|----------|
| Motores no giran | Sin alimentación / GND no común | Revisa LM2596→L298N y GND común |
| Van siempre a tope | Jumpers ENA/ENB puestos | Quítalos y cablea D5/D6 |
| Un motor al revés | Cables OUT invertidos | Intercambia sus 2 cables |
| Arduino se reinicia | Servo o motores tiran mucha corriente | 5V aparte para el servo |
| HC-05 no responde | RX sin divisor / TX-RX cruzados | Divisor en RX; TX→D11, RX→D12 |
| Línea no detecta | QTR sin calibrar | Calibrar umbral (ver testing.md) |

## Relacionado
- [wiring.md](wiring.md) — referencia de cableado
- [diagramas.md](diagramas.md) — diagramas de energía/pines/circuito
- [getting-started.md](getting-started.md) — armado y carga de sketches
