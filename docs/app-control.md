# App de Control (MIT App Inventor)

Guía paso a paso para crear la app Android que controla el robot por Bluetooth
(módulo HC-05). La app solo **envía caracteres** que el firmware
[`robot_car.ino`](../firmware/robot_car/robot_car.ino) ya entiende; no hay que
modificar el código del Arduino.

> Requisitos: PC con internet, una cuenta Google y un **celular Android**.
> (El HC-05 es Bluetooth Clásico; no funciona con iPhone.)

---

## 📋 Comandos que enviará la app

| Botón | Carácter | Acción en el robot |
|-------|----------|--------------------|
| Detener | `0` | Modo idle (parar) |
| Manual | `1` | Activa modo manual |
| Evasión | `2` | Evasión de obstáculos (autónomo) |
| Línea | `3` | Seguidor de línea (autónomo) |
| ⬆️ Adelante | `F` | Avanza (solo en modo manual) |
| ⬇️ Atrás | `B` | Retrocede |
| ⬅️ Izquierda | `L` | Gira a la izquierda |
| ➡️ Derecha | `R` | Gira a la derecha |
| ⏹️ Parar | `S` | Detiene el movimiento |
| ➕ / ➖ | `+` / `-` | Sube / baja la velocidad |

---

## Paso 1 — Crear el proyecto

1. Entra a **https://appinventor.mit.edu** → **Create Apps!** → inicia sesión con Google.
2. **Projects → Start new project** → nómbralo `RobotCar`.
3. Verás dos vistas (arriba a la derecha): **Designer** (diseño) y **Blocks**
   (lógica). Empezamos en **Designer**.

---

## Paso 2 — Diseño de la pantalla (Designer)

Desde la columna **Palette** (izquierda) arrastra estos componentes a la
pantalla. Los renombras en **Components** (botón **Rename**) para no perderte.

### Conexión Bluetooth
- **ListPicker** (Palette → Layout/User Interface) → renómbralo `selectorBT`.
  En Properties pon Text = "Conectar HC-05".
- **BluetoothClient** (Palette → **Connectivity**) → es **no visible**, aparece
  abajo. Déjalo como `BluetoothClient1`.
- **Label** → renómbralo `estado`. Text = "Desconectado".

### Botones de modo
Agrega 4 **Button** (Palette → User Interface):
- `btnDetener`  (Text: "Detener")
- `btnManual`   (Text: "Manual")
- `btnEvasion`  (Text: "Evasión")
- `btnLinea`    (Text: "Línea")

> Truco de orden: usa un **HorizontalArrangement** (Palette → Layout) para
> poner los 4 botones en fila.

### Flechas de manejo (modo manual)
Agrega 5 **Button** y acomódalos en cruz con **HorizontalArrangement** /
**VerticalArrangement**:
- `btnAdelante` (⬆️)
- `btnIzquierda` (⬅️), `btnParar` (⏹️), `btnDerecha` (➡️)  ← en una fila
- `btnAtras` (⬇️)

### Velocidad (opcional)
- 2 **Button**: `btnMas` (Text "+") y `btnMenos` (Text "−").

---

## Paso 3 — Lógica (vista Blocks)

Cambia a **Blocks** (arriba a la derecha). Arrastra los bloques así:

### 3.1 Conectar al HC-05
```
when selectorBT.BeforePicking
do  set selectorBT.Elements to  BluetoothClient1.AddressesAndNames
```
```
when selectorBT.AfterPicking
do  if  call BluetoothClient1.Connect address (selectorBT.Selection)
    then set estado.Text to "Conectado"
    else set estado.Text to "Falló la conexión"
```
> `BeforePicking` llena la lista con los dispositivos emparejados.
> `AfterPicking` intenta conectarse al que elegiste.

### 3.2 Botones de modo (un toque = un carácter)
Para cada botón de modo, el mismo patrón cambiando la letra:
```
when btnDetener.Click  do  call BluetoothClient1.SendText text ("0")
when btnManual.Click   do  call BluetoothClient1.SendText text ("1")
when btnEvasion.Click  do  call BluetoothClient1.SendText text ("2")
when btnLinea.Click    do  call BluetoothClient1.SendText text ("3")
```

### 3.3 Flechas: mantener presionado para avanzar
Usa **TouchDown** (al presionar) y **TouchUp** (al soltar). Así el robot se
mueve mientras mantienes el dedo y se detiene al soltar — más intuitivo:
```
when btnAdelante.TouchDown   do  call BluetoothClient1.SendText text ("F")
when btnAdelante.TouchUp     do  call BluetoothClient1.SendText text ("S")

when btnAtras.TouchDown      do  call BluetoothClient1.SendText text ("B")
when btnAtras.TouchUp        do  call BluetoothClient1.SendText text ("S")

when btnIzquierda.TouchDown  do  call BluetoothClient1.SendText text ("L")
when btnIzquierda.TouchUp    do  call BluetoothClient1.SendText text ("S")

when btnDerecha.TouchDown    do  call BluetoothClient1.SendText text ("R")
when btnDerecha.TouchUp      do  call BluetoothClient1.SendText text ("S")

when btnParar.Click          do  call BluetoothClient1.SendText text ("S")
```

### 3.4 Velocidad
```
when btnMas.Click    do  call BluetoothClient1.SendText text ("+")
when btnMenos.Click  do  call BluetoothClient1.SendText text ("-")
```

> El bloque `SendText` está dentro del componente `BluetoothClient1` (clic en él
> en la columna izquierda de Blocks). El bloque de texto `" "` está en
> **Built-in → Text**.

---

## Paso 4 — Probar la app

1. En tu Android instala **MIT AI2 Companion** (Play Store).
2. Enciende el robot → empareja el **HC-05** desde
   Ajustes → Bluetooth (PIN **1234** o **0000**). Esto se hace **una sola vez**.
3. En App Inventor: **Connect → AI Companion** → escanea el QR con la app
   Companion. La app aparece en vivo en tu celular.
4. Toca **"Conectar HC-05"** → elige HC-05 → debe decir "Conectado".
5. Toca **Manual** (`1`) y luego mantén **⬆️** → el robot avanza. 🎉

---

## Paso 5 — Generar el APK (instalar de forma permanente)

Cuando todo funcione:
- **Build → Android App (.apk)** → escanea el QR o descarga el archivo e
  instálalo en el celular. Así usas la app sin depender de la PC.

---

## Mejora futura (opcional)

Para que un **slider** controle la velocidad con un número exacto (no solo
+/−), habría que añadir al firmware un comando tipo `V120`. Es un cambio
pequeño; cuando lo quieras, se documenta aparte.

---

## Relacionado
- [getting-started.md](getting-started.md) — armado y carga de sketches
- [firmware/robot_car](../firmware/robot_car/robot_car.ino) — firmware que recibe los comandos
