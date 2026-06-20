# Firmware

Código real y funcional del robot, organizado en sketches independientes que se
prueban **por separado y en orden**. Cada uno se sube al Arduino UNO desde el
Arduino IDE (`Herramientas → Placa → Arduino Uno`, selecciona el puerto y Upload).

> Mapa de pines oficial: ver [docs/wiring.md](../docs/wiring.md). Todos los
> sketches usan el mismo mapa, así que el cableado no cambia entre pasos.

## Orden recomendado

| # | Sketch | Qué prueba | Hardware necesario |
|---|--------|-----------|--------------------|
| 1 | [01_motor_test](01_motor_test/01_motor_test.ino) | Movimiento básico 2WD | L298N + 2 motores |
| 2 | [02_ultrasonic_test](02_ultrasonic_test/02_ultrasonic_test.ino) | Medir distancia | HC-SR04 |
| 3 | [03_servo_test](03_servo_test/03_servo_test.ino) | Apuntar el sensor | SG90 |
| 4 | [04_obstacle_avoidance](04_obstacle_avoidance/04_obstacle_avoidance.ino) | Evasión autónoma (1+2+3) | L298N + HC-SR04 + SG90 |
| 5 | [05_bluetooth_control](05_bluetooth_control/05_bluetooth_control.ino) | Control manual desde el celular | L298N + HC-05 |
| 6 | [06_line_follower](06_line_follower/06_line_follower.ino) | Seguir una línea | L298N + QTR-8A |
| ★ | [robot_car](robot_car/robot_car.ino) | **Firmware integrado**: todos los modos en uno, con cambio por Bluetooth | Todo |

## Firmware integrado (`robot_car/`)

Una vez que los pasos 1-6 funcionan por separado, `robot_car.ino` los une en un
solo programa con una máquina de estados (`enum Modo` + `switch`). El modo se
elige por Bluetooth:

| Comando BT | Modo |
|-----------|------|
| `0` | Detener (idle) |
| `1` | Manual (luego `F/B/L/R/S`, `+/-`) |
| `2` | Evasión de obstáculos (autónomo) |
| `3` | Seguidor de línea (autónomo) |

## Cómo trabajar

1. **Prueba cada sketch antes de pasar al siguiente.** Si el paso 1 no mueve bien
   los motores, los pasos 4, 5 y 6 tampoco funcionarán (todos los usan).
2. **Primero sobre un soporte** (ruedas al aire) y mirando el Monitor Serie
   (9600 baudios) para validar la lógica sin riesgo; luego en el suelo.
3. Los sketches 4 y 6 tienen valores que **dependen de tu robot** (velocidad,
   duración de giro, umbral de línea). Están marcados como "ajustar/CALIBRAR".

## Librerías usadas

Todas vienen incluidas en el Arduino IDE, no instalas nada extra:
- `Servo` (pasos 3 y 4)
- `SoftwareSerial` (paso 5)
