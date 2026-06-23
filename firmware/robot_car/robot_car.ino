// RoboCar UNSM - firmware 2WD. Control por Bluetooth: idle/manual/evasion/linea.

#include <Servo.h>
#include <SoftwareSerial.h>

// ---- Pines ----
const int ENA = 5, IN1 = 7, IN2 = 8;
const int ENB = 6, IN3 = 9, IN4 = 10;
const int PIN_TRIG = 2, PIN_ECHO = 4;
const int PIN_SERVO = 3;
const int BT_RX = 11, BT_TX = 12;
const int NUM_SENSORES = 5;
const int SENSORES[NUM_SENSORES] = {A0, A1, A2, A3, A4};

// ---- Parametros ----
// Distancia de frenado dinamica: crece con la velocidad (frena antes si va rapido).
const int DIST_BASE_CM = 18;
const int DIST_FACTOR = 6;
const int ANG_DERECHA = 30, ANG_CENTRO = 90, ANG_IZQUIERDA = 150;
const unsigned long TIMEOUT_US = 25000;
const int UMBRAL_LINEA = 500;      // CALIBRAR con tu pista
const int VELOCIDAD_BASE = 130;
const float KP = 18.0;
const int SIN_LINEA = 9999;

// Arranque suave: la velocidad sube por escalones en vez de saltar de golpe.
const int RAMPA_PASO = 8;
const int RAMPA_MS = 12;

// Failsafe: si en modo manual deja de llegar movimiento, el robot frena solo.
const unsigned long FAILSAFE_MS = 600;
unsigned long ultimoComandoMs = 0;

// ---- Estado ----
enum Modo { IDLE, MANUAL, EVASION, LINEA };
Modo modo = IDLE;
int velocidad = 150;  // velocidad actual (manual y evasion); ajustable con +/- y V

// Velocidad con signo aplicada AHORA a cada motor (+ adelante, - atras).
int velIzqActual = 0;
int velDerActual = 0;

Servo servo;
SoftwareSerial bluetooth(BT_RX, BT_TX);

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT); pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  servo.attach(PIN_SERVO);
  servo.write(ANG_CENTRO);

  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("Firmware integrado listo. 0=stop 1=manual 2=evasion 3=linea");
  bluetooth.println("OK:ROBOT LISTO");  // telemetria: aviso de arranque

  detener();
}

void loop() {
  // 1) Leer Bluetooth: puede cambiar de modo o, en manual, mover el robot.
  if (bluetooth.available()) {
    procesarComando(bluetooth.read());
    ultimoComandoMs = millis();  // hubo actividad: reinicia el failsafe
  }

  // 2) Ejecutar el comportamiento del modo actual.
  switch (modo) {
    case IDLE:    detener();     break;
    case MANUAL:
      // Failsafe: si no llegan comandos (ej. se corto el Bluetooth), parar.
      if (millis() - ultimoComandoMs > FAILSAFE_MS) detener();
      break;
    case EVASION: pasoEvasion(); break;
    case LINEA:   pasoLinea();   break;
  }
}

// ---- Comandos ----
// Estado para leer el comando de velocidad "V<numero>" (ej. "V180\n").
bool leyendoVelocidad = false;
String numeroVelocidad = "";

void procesarComando(char c) {
  // Si estamos leyendo el numero de "V...", acumular digitos.
  if (leyendoVelocidad) {
    if (c >= '0' && c <= '9') {
      if (numeroVelocidad.length() < 3) numeroVelocidad += c;
      return;
    }
    // Un caracter no-digito (ej. '\n') cierra el numero.
    if (numeroVelocidad.length() > 0) {
      velocidad = constrain(numeroVelocidad.toInt(), 0, 255);
      Serial.print("VEL="); Serial.println(velocidad);
      bluetooth.print("OK:VEL="); bluetooth.println(velocidad);  // telemetria
    }
    leyendoVelocidad = false;
    numeroVelocidad = "";
    // Si el caracter terminador es un salto de linea, no hay mas que hacer.
    if (c == '\n' || c == '\r') return;
    // Si fue otro comando, seguimos para procesarlo abajo.
  }

  switch (c) {
    case 'V': case 'v':  // inicio del comando de velocidad
      leyendoVelocidad = true;
      numeroVelocidad = "";
      break;

    // Cambio de modo (responde por Bluetooth = telemetria para la app)
    case '0': modo = IDLE;    detener(); Serial.println("IDLE");
              bluetooth.println("OK:IDLE");    break;
    case '1': modo = MANUAL;  detener(); Serial.println("MANUAL");
              bluetooth.println("OK:MANUAL");  break;
    case '2': modo = EVASION;            Serial.println("EVASION");
              bluetooth.println("OK:EVASION"); break;
    case '3': modo = LINEA;              Serial.println("LINEA");
              bluetooth.println("OK:LINEA");
              adelante(VELOCIDAD_BASE);  // arranque suave hasta la velocidad base
              break;

    // Movimiento (solo tiene sentido en modo MANUAL)
    case 'F': case 'f': if (modo == MANUAL) adelante(velocidad);       break;
    case 'B': case 'b': if (modo == MANUAL) atras(velocidad);          break;
    case 'L': case 'l': if (modo == MANUAL) girarIzquierda(velocidad); break;
    case 'R': case 'r': if (modo == MANUAL) girarDerecha(velocidad);   break;
    case 'S': case 's': if (modo == MANUAL) detener();                 break;
    case '+': velocidad = min(velocidad + 25, 255); break;
    case '-': velocidad = max(velocidad - 25, 0);   break;
    default: break;  // ignorar lo desconocido
  }
}

// ---- Modo EVASION (del paso 4) ----
void pasoEvasion() {
  // Distancia de frenado segun la velocidad actual: a mas rapido, frena antes.
  int distSegura = DIST_BASE_CM + velocidad / DIST_FACTOR;

  long frente = medirDistanciaCm();
  if (frente >= 0 && frente < distSegura) {
    detener();
    delay(200);
    atras(velocidad);
    delay(400);
    detener();
    delay(200);

    long der = medirHacia(ANG_DERECHA);
    long izq = medirHacia(ANG_IZQUIERDA);
    servo.write(ANG_CENTRO);
    delay(300);

    if (despejeMayor(izq, der)) girarIzquierda(velocidad);
    else                        girarDerecha(velocidad);
    delay(400);
    detener();
    delay(150);
  } else {
    adelante(velocidad);
  }
}

long medirHacia(int angulo) {
  servo.write(angulo);
  delay(400);
  return medirDistanciaCm();
}

bool despejeMayor(long a, long b) {
  long da = (a < 0) ? 9999 : a;
  long db = (b < 0) ? 9999 : b;
  return da >= db;
}

// ---- Modo LINEA (del paso 6) ----
void pasoLinea() {
  int error = calcularErrorLinea();
  if (error == SIN_LINEA) {
    detener();
    return;
  }
  int correccion = (int)(KP * error);
  int velIzq = constrain(VELOCIDAD_BASE + correccion, 0, 255);
  int velDer = constrain(VELOCIDAD_BASE - correccion, 0, 255);

  // Correcciones chicas: se aplican directo, sin rampa.
  velIzqActual = velIzq;
  velDerActual = velDer;
  aplicarMotor(velIzqActual, ENA, IN1, IN2);
  aplicarMotor(velDerActual, ENB, IN3, IN4);
}

int calcularErrorLinea() {
  long sumaPesos = 0;
  int enLinea = 0;
  for (int i = 0; i < NUM_SENSORES; i++) {
    if (analogRead(SENSORES[i]) < UMBRAL_LINEA) {
      sumaPesos += i - (NUM_SENSORES / 2);
      enLinea++;
    }
  }
  if (enLinea == 0) return SIN_LINEA;
  return sumaPesos / enLinea;
}

// ---- Sensor ----
long medirDistanciaCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long duracion = pulseIn(PIN_ECHO, HIGH, TIMEOUT_US);
  if (duracion == 0) return -1;
  return duracion / 58;
}

// ---- Movimiento con arranque suave (compartido por todos los modos) ----
void adelante(int v)       { rampaHacia(v,  v);  }
void atras(int v)          { rampaHacia(-v, -v); }
void girarIzquierda(int v) { rampaHacia(-v, v);  }
void girarDerecha(int v)   { rampaHacia(v,  -v); }
void detener()             { rampaHacia(0,  0);  }

// Lleva gradualmente ambos motores a las velocidades objetivo (con signo).
void rampaHacia(int objIzq, int objDer) {
  while (velIzqActual != objIzq || velDerActual != objDer) {
    velIzqActual = acercar(velIzqActual, objIzq, RAMPA_PASO);
    velDerActual = acercar(velDerActual, objDer, RAMPA_PASO);
    aplicarMotor(velIzqActual, ENA, IN1, IN2);
    aplicarMotor(velDerActual, ENB, IN3, IN4);
    delay(RAMPA_MS);
  }
}

// Acerca 'actual' a 'objetivo' como mucho 'paso' unidades.
int acercar(int actual, int objetivo, int paso) {
  if (actual < objetivo) return min(actual + paso, objetivo);
  if (actual > objetivo) return max(actual - paso, objetivo);
  return actual;
}

// Mueve un motor: el signo da el sentido, el valor absoluto el PWM (0-255).
void aplicarMotor(int velSignada, int pinPWM, int pinDir1, int pinDir2) {
  bool haciaAdelante = velSignada >= 0;
  digitalWrite(pinDir1, haciaAdelante ? HIGH : LOW);
  digitalWrite(pinDir2, haciaAdelante ? LOW : HIGH);
  analogWrite(pinPWM, abs(velSignada));
}
