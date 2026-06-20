/*
 * 01_motor_test.ino
 * -----------------------------------------------------------
 * Prueba basica de los 2 motores DC del robot (2WD) con driver L298N.
 * Objetivo: verificar que los motores giran en el sentido correcto
 * ANTES de construir cualquier comportamiento (evasion, linea, etc).
 *
 * Que hace el robot al encender:
 *   adelante 2s -> parar 1s -> atras 2s -> parar 1s ->
 *   girar izquierda 1s -> girar derecha 1s -> parar 2s (y repite)
 *
 * IMPORTANTE (seguridad):
 *   - Levanta el robot sobre un soporte para que las ruedas giren al aire.
 *   - Empieza con velocidad baja.
 *
 * Conexiones L298N -> Arduino UNO (sin pines repetidos):
 *   ENA (PWM motor izquierdo)  -> D5   (pin PWM)
 *   IN1                        -> D7
 *   IN2                        -> D8
 *   IN3                        -> D9
 *   IN4                        -> D10
 *   ENB (PWM motor derecho)    -> D6   (pin PWM)
 *   GND del L298N              -> GND del Arduino (masa comun obligatoria)
 *
 * Nota: los pines PWM validos en el UNO son 3, 5, 6, 9, 10, 11.
 * Por eso ENA/ENB van en D5 y D6.
 */

// ---- Mapa de pines (cambiar aqui si tu cableado es distinto) ----
const int ENA = 5;   // velocidad motor izquierdo (PWM)
const int IN1 = 7;   // direccion motor izquierdo
const int IN2 = 8;   // direccion motor izquierdo
const int IN3 = 9;   // direccion motor derecho
const int IN4 = 10;  // direccion motor derecho
const int ENB = 6;   // velocidad motor derecho (PWM)

// ---- Parametros ----
const int VELOCIDAD = 160;  // 0-255. Empieza bajo (150-180) y ajusta.

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  Serial.println("Prueba de motores lista. Robot sobre soporte!");

  detener();  // arrancar siempre con los motores quietos
}

void loop() {
  Serial.println("Adelante");
  adelante(VELOCIDAD);
  delay(2000);

  Serial.println("Stop");
  detener();
  delay(1000);

  Serial.println("Atras");
  atras(VELOCIDAD);
  delay(2000);

  Serial.println("Stop");
  detener();
  delay(1000);

  Serial.println("Giro izquierda");
  girarIzquierda(VELOCIDAD);
  delay(1000);

  Serial.println("Giro derecha");
  girarDerecha(VELOCIDAD);
  delay(1000);

  Serial.println("Stop");
  detener();
  delay(2000);
}

// ---- Funciones de movimiento ----
// Cada motor se controla con 2 pines de direccion + 1 pin de velocidad (PWM).
// Si un motor gira al reves, intercambia sus dos pines IN (o invierte HIGH/LOW).

void adelante(int velocidad) {
  motorIzquierdo(true, velocidad);
  motorDerecho(true, velocidad);
}

void atras(int velocidad) {
  motorIzquierdo(false, velocidad);
  motorDerecho(false, velocidad);
}

void girarIzquierda(int velocidad) {
  // rueda izquierda atras, derecha adelante -> gira sobre su eje
  motorIzquierdo(false, velocidad);
  motorDerecho(true, velocidad);
}

void girarDerecha(int velocidad) {
  motorIzquierdo(true, velocidad);
  motorDerecho(false, velocidad);
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// adelante = true mueve la rueda hacia adelante; false hacia atras.
void motorIzquierdo(bool haciaAdelante, int velocidad) {
  digitalWrite(IN1, haciaAdelante ? HIGH : LOW);
  digitalWrite(IN2, haciaAdelante ? LOW : HIGH);
  analogWrite(ENA, velocidad);
}

void motorDerecho(bool haciaAdelante, int velocidad) {
  digitalWrite(IN3, haciaAdelante ? HIGH : LOW);
  digitalWrite(IN4, haciaAdelante ? LOW : HIGH);
  analogWrite(ENB, velocidad);
}
