/*
 * 04_obstacle_avoidance.ino
 * -----------------------------------------------------------
 * Evasion de obstaculos autonoma: junta motores + HC-SR04 + servo.
 * Es la integracion de los pasos 1, 2 y 3.
 *
 * Comportamiento:
 *   - Avanza mientras el frente este despejado.
 *   - Si detecta un obstaculo cerca:
 *       1. Se detiene y retrocede un poco.
 *       2. Con el servo mira a la derecha y a la izquierda y mide.
 *       3. Gira hacia el lado mas despejado.
 *       4. Vuelve a avanzar.
 *
 * SEGURIDAD: prueba primero con el robot sobre un soporte (ruedas
 * al aire) para confirmar la logica en el Monitor Serie, y recien
 * despues pruebalo en el suelo.
 *
 * Conexiones (mapa oficial del proyecto, ver docs/wiring.md):
 *   L298N : ENA=D5, ENB=D6, IN1=D7, IN2=D8, IN3=D9, IN4=D10
 *   HC-SR04: TRIG=D2, ECHO=D4
 *   SG90  : senal=D3
 *   GND comun entre bateria, L298N y Arduino (obligatorio).
 */

#include <Servo.h>

// ---- Pines ----
const int ENA = 5, IN1 = 7, IN2 = 8;     // motor izquierdo
const int ENB = 6, IN3 = 9, IN4 = 10;    // motor derecho
const int PIN_TRIG = 2, PIN_ECHO = 4;    // ultrasonico
const int PIN_SERVO = 3;                 // servo

// ---- Parametros de comportamiento ----
const int VELOCIDAD = 150;            // 0-255
const int DISTANCIA_SEGURA_CM = 20;   // si hay algo mas cerca, evade
const int ANG_DERECHA = 30, ANG_CENTRO = 90, ANG_IZQUIERDA = 150;
const unsigned long TIMEOUT_US = 25000;

Servo servo;

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT); pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  servo.attach(PIN_SERVO);
  servo.write(ANG_CENTRO);

  Serial.begin(9600);
  Serial.println("Evasion de obstaculos lista.");
  detener();
  delay(1000);
}

void loop() {
  long distanciaFrente = medirDistanciaCm();

  // Sin lectura (-1) lo tratamos como "despejado": el sensor no ve nada cerca.
  if (distanciaFrente >= 0 && distanciaFrente < DISTANCIA_SEGURA_CM) {
    evadir();
  } else {
    adelante(VELOCIDAD);
  }

  delay(60);  // ritmo del ciclo; suficiente para reaccionar a tiempo
}

// ---- Secuencia de evasion ----
void evadir() {
  Serial.println("Obstaculo! Evadiendo...");
  detener();
  delay(200);

  // Retroceder un poco para tener espacio de maniobra.
  atras(VELOCIDAD);
  delay(400);
  detener();
  delay(200);

  // Mirar a la derecha y a la izquierda con el servo y medir.
  long distDerecha = medirHacia(ANG_DERECHA);
  long distIzquierda = medirHacia(ANG_IZQUIERDA);
  servo.write(ANG_CENTRO);  // dejar el sensor mirando al frente otra vez
  delay(300);

  Serial.print("Derecha: "); Serial.print(distDerecha);
  Serial.print(" cm | Izquierda: "); Serial.print(distIzquierda);
  Serial.println(" cm");

  // Girar hacia el lado mas despejado. (-1 = sin eco = muy despejado)
  bool izquierdaMejor = comparaDespeje(distIzquierda, distDerecha);
  if (izquierdaMejor) {
    Serial.println("Giro a la izquierda");
    girarIzquierda(VELOCIDAD);
  } else {
    Serial.println("Giro a la derecha");
    girarDerecha(VELOCIDAD);
  }
  delay(400);  // duracion del giro; ajustar segun cuanto gire tu robot
  detener();
  delay(150);
}

// Mueve el servo a un angulo, espera a que llegue y mide la distancia.
long medirHacia(int angulo) {
  servo.write(angulo);
  delay(400);  // que el servo llegue antes de medir
  return medirDistanciaCm();
}

// true si 'a' esta mas despejado que 'b'. Un -1 (sin eco) cuenta como
// "muy lejos / despejado", por eso lo convertimos a un valor grande.
bool comparaDespeje(long a, long b) {
  long da = (a < 0) ? 9999 : a;
  long db = (b < 0) ? 9999 : b;
  return da >= db;
}

// ---- Sensor ----
long medirDistanciaCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duracion = pulseIn(PIN_ECHO, HIGH, TIMEOUT_US);
  if (duracion == 0) return -1;  // sin eco
  return duracion / 58;
}

// ---- Movimiento (mismas funciones del paso 1) ----
void adelante(int v)       { motorIzq(true, v);  motorDer(true, v); }
void atras(int v)          { motorIzq(false, v); motorDer(false, v); }
void girarIzquierda(int v) { motorIzq(false, v); motorDer(true, v); }
void girarDerecha(int v)   { motorIzq(true, v);  motorDer(false, v); }
void detener()             { analogWrite(ENA, 0); analogWrite(ENB, 0); }

void motorIzq(bool adelante, int v) {
  digitalWrite(IN1, adelante ? HIGH : LOW);
  digitalWrite(IN2, adelante ? LOW : HIGH);
  analogWrite(ENA, v);
}

void motorDer(bool adelante, int v) {
  digitalWrite(IN3, adelante ? HIGH : LOW);
  digitalWrite(IN4, adelante ? LOW : HIGH);
  analogWrite(ENB, v);
}
