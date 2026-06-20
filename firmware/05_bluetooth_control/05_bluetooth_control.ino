/*
 * 05_bluetooth_control.ino
 * -----------------------------------------------------------
 * Control manual del robot por Bluetooth (HC-05) desde el celular.
 * Reutiliza las funciones de movimiento del paso 1.
 *
 * Comandos (envialos como un caracter desde una app de terminal BT):
 *   F = adelante      B = atras
 *   L = girar izq.    R = girar der.
 *   S = parar
 *   + = mas velocidad  - = menos velocidad
 *
 * Seguridad: si no llega ningun comando en 1 segundo, el robot se
 * detiene solo (failsafe ante perdida de conexion).
 *
 * Conexiones HC-05 -> Arduino UNO:
 *   VCC -> 5V
 *   GND -> GND
 *   TX  -> D11 (RX del Arduino)
 *   RX  -> D12 (TX del Arduino)  [usa divisor de voltaje: el RX del HC-05 es 3.3V]
 *
 *   L298N: ENA=D5, ENB=D6, IN1=D7, IN2=D8, IN3=D9, IN4=D10
 *
 * Nota: usamos SoftwareSerial en D11/D12 para dejar el USB (D0/D1)
 * libre para programar y depurar por el Monitor Serie.
 */

#include <SoftwareSerial.h>

const int ENA = 5, IN1 = 7, IN2 = 8;
const int ENB = 6, IN3 = 9, IN4 = 10;
const int BT_RX = 11, BT_TX = 12;

int velocidad = 150;                     // 0-255, ajustable con + / -
const unsigned long FAILSAFE_MS = 1000;  // parar si no hay comandos
unsigned long ultimoComando = 0;

SoftwareSerial bluetooth(BT_RX, BT_TX);

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  Serial.begin(9600);       // depuracion por USB
  bluetooth.begin(9600);    // HC-05 (baud por defecto)
  Serial.println("Control Bluetooth listo. Empareja el HC-05 y envia F/B/L/R/S.");

  detener();
}

void loop() {
  if (bluetooth.available()) {
    char comando = bluetooth.read();
    procesarComando(comando);
    ultimoComando = millis();
  }

  // Failsafe: si se corto la conexion, no dejar el robot andando solo.
  if (millis() - ultimoComando > FAILSAFE_MS) {
    detener();
  }
}

void procesarComando(char c) {
  switch (c) {
    case 'F': case 'f': adelante(velocidad);       Serial.println("Adelante"); break;
    case 'B': case 'b': atras(velocidad);          Serial.println("Atras");    break;
    case 'L': case 'l': girarIzquierda(velocidad); Serial.println("Izquierda");break;
    case 'R': case 'r': girarDerecha(velocidad);   Serial.println("Derecha");  break;
    case 'S': case 's': detener();                 Serial.println("Stop");     break;
    case '+': velocidad = min(velocidad + 25, 255); Serial.println(velocidad); break;
    case '-': velocidad = max(velocidad - 25, 0);   Serial.println(velocidad); break;
    default: /* ignorar caracteres desconocidos */ break;
  }
}

// ---- Movimiento (igual que el paso 1) ----
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
