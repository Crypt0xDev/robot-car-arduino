/*
 * 06_line_follower.ino
 * -----------------------------------------------------------
 * Seguidor de linea con el sensor QTR-8A del kit. Sigue una linea
 * negra sobre fondo blanco usando control proporcional.
 *
 * El QTR-8A tiene 8 canales, pero el UNO solo tiene 6 entradas
 * analogicas (A0-A5). Conectamos 5 canales a A0-A4; con eso basta.
 *
 * Idea:
 *   - Cada sensor IR da un valor analogico (0-1023).
 *   - Calculamos la "posicion" de la linea como promedio ponderado.
 *   - El error (que tan a un lado esta la linea) ajusta la velocidad
 *     de cada motor para volver al centro. Eso es control proporcional.
 *
 * IMPORTANTE - CALIBRACION:
 *   El umbral entre "negro" y "blanco" depende de tu pista y luz.
 *   Sube primero el sketch 'docs/testing.md' de lectura de sensores,
 *   anota los valores en negro y en blanco, y ajusta UMBRAL_LINEA.
 *
 * Conexiones:
 *   QTR-8A -> 5 canales a A0, A1, A2 (centro), A3, A4 (izq -> der)
 *   L298N: ENA=D5, ENB=D6, IN1=D7, IN2=D8, IN3=D9, IN4=D10
 */

// ---- Pines ----
const int ENA = 5, IN1 = 7, IN2 = 8;
const int ENB = 6, IN3 = 9, IN4 = 10;

const int NUM_SENSORES = 5;
const int SENSORES[NUM_SENSORES] = {A0, A1, A2, A3, A4};  // izq -> der

// ---- Parametros ----
const int UMBRAL_LINEA = 500;   // < UMBRAL = linea negra (CALIBRAR)
const int VELOCIDAD_BASE = 130; // velocidad al ir recto
const float KP = 18.0;          // fuerza de correccion (ajustar)
const int SIN_LINEA = 9999;     // marca de "ningun sensor ve la linea"

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  Serial.begin(9600);
  Serial.println("Seguidor de linea listo. Coloca el robot sobre la linea.");
}

void loop() {
  int error = calcularError();

  if (error == SIN_LINEA) {
    // Linea perdida: parar (mejora futura: girar para reencontrarla).
    detener();
    Serial.println("Linea perdida");
    return;
  }

  // Control proporcional: la correccion crece con el error.
  int correccion = (int)(KP * error);
  int velIzq = VELOCIDAD_BASE + correccion;
  int velDer = VELOCIDAD_BASE - correccion;

  // Mantener las velocidades en el rango valido de PWM.
  velIzq = constrain(velIzq, 0, 255);
  velDer = constrain(velDer, 0, 255);

  motorIzq(true, velIzq);
  motorDer(true, velDer);
}

// Devuelve el error de posicion de la linea respecto al centro.
//   negativo = linea hacia la izquierda, positivo = hacia la derecha,
//   0 = centrada, SIN_LINEA = ningun sensor ve la linea.
// Con 5 sensores, los pesos van de -2 a +2.
int calcularError() {
  long sumaPesos = 0;
  int sensoresEnLinea = 0;

  for (int i = 0; i < NUM_SENSORES; i++) {
    int valor = analogRead(SENSORES[i]);
    if (valor < UMBRAL_LINEA) {       // este sensor esta sobre la linea
      int peso = i - (NUM_SENSORES / 2);  // -2,-1,0,1,2
      sumaPesos += peso;
      sensoresEnLinea++;
    }
  }

  if (sensoresEnLinea == 0) return SIN_LINEA;
  return sumaPesos / sensoresEnLinea;
}

// ---- Movimiento ----
void detener() { analogWrite(ENA, 0); analogWrite(ENB, 0); }

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
