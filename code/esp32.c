#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

// Pines L298N
int IN1 = 27;
int IN2 = 26;
int IN3 = 25;
int IN4 = 33;
int ENA = 14;
int ENB = 12;

int velocidad = 200; // 0 - 255

void setup() {
  Serial.begin(115200);
  SerialBT.begin("CarritoESP32"); // Nombre Bluetooth

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  detener();
}

void loop() {
  if (SerialBT.available()) {
    char comando = SerialBT.read();

    switch (comando) {
      case 'F':
        adelante();
        break;
      case 'B':
        atras();
        break;
      case 'L':
        izquierda();
        break;
      case 'R':
        derecha();
        break;
      case 'S':
        detener();
        break;
    }
  }
}

// FUNCIONES

void adelante() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void atras() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void izquierda() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void derecha() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void detener() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
