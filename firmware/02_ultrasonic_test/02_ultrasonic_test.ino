/*
 * 02_ultrasonic_test.ino
 * -----------------------------------------------------------
 * Prueba del sensor de distancia HC-SR04.
 * Objetivo: verificar que medimos la distancia a un obstaculo
 * de forma estable, ANTES de usarla para evasion (paso 4).
 *
 * Que hace:
 *   Mide la distancia cada 300ms y la imprime por el Monitor Serie.
 *   Si hay un obstaculo a menos de 20cm, avisa "OBSTACULO".
 *
 * Como probar:
 *   1. Sube el sketch al Arduino.
 *   2. Abre el Monitor Serie a 9600 baudios.
 *   3. Acerca/aleja la mano frente al sensor y observa el valor en cm.
 *
 * Conexiones HC-SR04 -> Arduino UNO:
 *   VCC  -> 5V
 *   GND  -> GND
 *   TRIG -> D2
 *   ECHO -> D4
 */

const int PIN_TRIG = 2;
const int PIN_ECHO = 4;

const int UMBRAL_OBSTACULO_CM = 20;   // distancia que consideramos "obstaculo"
const unsigned long TIMEOUT_US = 25000;  // ~4m max; evita bloqueos si no hay eco

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  Serial.begin(9600);
  Serial.println("Prueba HC-SR04 lista.");
}

void loop() {
  long distancia = medirDistanciaCm();

  if (distancia < 0) {
    // pulseIn devolvio 0 (timeout): sin eco / fuera de rango
    Serial.println("Sin lectura (fuera de rango o sin eco)");
  } else {
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.print(" cm");
    if (distancia < UMBRAL_OBSTACULO_CM) {
      Serial.print("  -> OBSTACULO");
    }
    Serial.println();
  }

  delay(300);
}

// Devuelve la distancia en cm, o -1 si no hubo eco (timeout).
long medirDistanciaCm() {
  // Pulso de disparo de 10us
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Mide cuanto tarda en volver el eco (en microsegundos)
  long duracion = pulseIn(PIN_ECHO, HIGH, TIMEOUT_US);
  if (duracion == 0) {
    return -1;  // timeout
  }

  // El sonido viaja ida y vuelta: dividir entre 2.
  // velocidad sonido ~343 m/s -> distancia(cm) = duracion_us / 58
  return duracion / 58;
}
