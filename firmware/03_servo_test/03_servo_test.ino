/*
 * 03_servo_test.ino
 * -----------------------------------------------------------
 * Prueba del servo SG90 que mueve el sensor ultrasonico.
 * Objetivo: verificar que el servo apunta bien a izquierda,
 * centro y derecha ANTES de usarlo para escanear (paso 4).
 *
 * Que hace:
 *   1) Barrido suave de 0 a 180 y de vuelta (ver rango completo).
 *   2) Posiciones de escaneo que usaremos en la evasion:
 *      derecha (30) -> centro (90) -> izquierda (150).
 *
 * Como probar:
 *   Sube el sketch y observa el servo. Abre el Monitor Serie (9600)
 *   para ver a que angulo apunta en cada momento.
 *
 * Conexiones SG90 -> Arduino UNO:
 *   Marron -> GND
 *   Rojo   -> 5V
 *   Naranja-> D3 (PWM)
 *
 * Nota: si el servo "tiembla" o resetea el Arduino, alimentalo
 * desde una fuente de 5V aparte (comparte GND). El SG90 consume
 * picos de corriente que el USB a veces no aguanta.
 */

#include <Servo.h>

const int PIN_SERVO = 3;

// Angulos de escaneo que reutilizaremos en la evasion de obstaculos.
const int ANG_DERECHA   = 30;
const int ANG_CENTRO    = 90;
const int ANG_IZQUIERDA = 150;

Servo servo;

void setup() {
  servo.attach(PIN_SERVO);
  Serial.begin(9600);
  Serial.println("Prueba de servo SG90 lista.");

  servo.write(ANG_CENTRO);  // empezar mirando al frente
  delay(500);
}

void loop() {
  // 1) Barrido completo para ver el rango fisico real del servo.
  Serial.println("Barrido 0 -> 180");
  for (int angulo = 0; angulo <= 180; angulo += 5) {
    servo.write(angulo);
    delay(20);
  }
  Serial.println("Barrido 180 -> 0");
  for (int angulo = 180; angulo >= 0; angulo -= 5) {
    servo.write(angulo);
    delay(20);
  }

  // 2) Posiciones de escaneo (las que usaremos para decidir ruta).
  apuntar("Derecha", ANG_DERECHA);
  apuntar("Centro", ANG_CENTRO);
  apuntar("Izquierda", ANG_IZQUIERDA);
  apuntar("Centro", ANG_CENTRO);

  delay(1000);
}

// Mueve el servo a un angulo y le da tiempo a llegar antes de seguir.
void apuntar(const char* nombre, int angulo) {
  Serial.print("Apuntando: ");
  Serial.print(nombre);
  Serial.print(" (");
  Serial.print(angulo);
  Serial.println(" grados)");

  servo.write(angulo);
  delay(500);  // el SG90 tarda ~0.1s/60deg; 500ms es margen seguro
}
