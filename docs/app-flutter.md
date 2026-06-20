# App de Control en código (Flutter + VS Code)

Plan para crear la app Android **desde código**, editándola en **VS Code** y
compilándola a **APK** desde la terminal. Controla el robot por Bluetooth
(HC-05) enviando los mismos caracteres que entiende
[`robot_car.ino`](../firmware/robot_car/robot_car.ino).

> Alternativa más simple sin programar: [app-control.md](app-control.md)
> (MIT App Inventor). Esta guía es para quien quiere hacerlo con código.

> Requisitos de hardware: PC (Windows/Linux/Mac) + celular **Android**
> (el HC-05 es Bluetooth Clásico; no funciona con iPhone).

---

## Paso 0 — Instalar el entorno (una sola vez)

Esto es lo que más toma; hazlo con calma.

1. **Java JDK 17** (Flutter/Android lo necesitan).
2. **Android SDK**: la forma simple es instalar **Android Studio** una vez
   (solo para que baje el SDK y las "platform-tools"); luego trabajas en VS Code.
3. **Flutter SDK**: descarga de https://flutter.dev → descomprime → agrega
   `flutter\bin` al PATH.
4. **VS Code** + extensiones: **Flutter** y **Dart** (de la marketplace).
5. Verifica todo con:
   ```
   flutter doctor
   ```
   Resuelve lo que marque con ✗ (acepta licencias con `flutter doctor --android-licenses`).

---

## Paso 1 — Crear el proyecto

En la terminal de VS Code:
```
flutter create robot_car_app
cd robot_car_app
code .
```

---

## Paso 2 — Agregar el plugin de Bluetooth

En `pubspec.yaml`, dentro de `dependencies:` agrega:
```yaml
dependencies:
  flutter:
    sdk: flutter
  flutter_bluetooth_serial: ^0.4.0
```
Luego:
```
flutter pub get
```

---

## Paso 3 — Permisos en Android

Edita `android/app/src/main/AndroidManifest.xml` y, antes de `<application>`,
agrega:
```xml
<uses-permission android:name="android.permission.BLUETOOTH" />
<uses-permission android:name="android.permission.BLUETOOTH_ADMIN" />
<!-- Android 12+ -->
<uses-permission android:name="android.permission.BLUETOOTH_CONNECT" />
<uses-permission android:name="android.permission.BLUETOOTH_SCAN" />
```
En `android/app/build.gradle` asegúrate de `minSdkVersion 21` (o mayor).

---

## Paso 4 — Código de la app (`lib/main.dart`)

Reemplaza el contenido por este esqueleto funcional. Conecta al HC-05 emparejado
y manda comandos. Está comentado en español para que lo entiendas y amplíes.

```dart
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

void main() => runApp(const RobotCarApp());

class RobotCarApp extends StatelessWidget {
  const RobotCarApp({super.key});
  @override
  Widget build(BuildContext context) {
    return const MaterialApp(home: PantallaControl());
  }
}

class PantallaControl extends StatefulWidget {
  const PantallaControl({super.key});
  @override
  State<PantallaControl> createState() => _PantallaControlState();
}

class _PantallaControlState extends State<PantallaControl> {
  BluetoothConnection? _conexion;
  String _estado = "Desconectado";

  // Conecta al primer HC-05 emparejado.
  Future<void> _conectar() async {
    final dispositivos =
        await FlutterBluetoothSerial.instance.getBondedDevices();
    final hc05 = dispositivos.firstWhere(
      (d) => d.name == "HC-05",
      orElse: () => dispositivos.first,
    );
    try {
      _conexion = await BluetoothConnection.toAddress(hc05.address);
      setState(() => _estado = "Conectado a ${hc05.name}");
    } catch (_) {
      setState(() => _estado = "Falló la conexión");
    }
  }

  // Envia un caracter al robot (los que entiende robot_car.ino).
  void _enviar(String comando) {
    if (_conexion != null && _conexion!.isConnected) {
      _conexion!.output.add(utf8.encode(comando));
    }
  }

  @override
  void dispose() {
    _conexion?.dispose();
    super.dispose();
  }

  // Boton que envia un comando al presionar (y "S" al soltar, si es flecha).
  Widget _boton(String texto, String comando, {bool mantener = false}) {
    if (mantener) {
      return Listener(
        onPointerDown: (_) => _enviar(comando),
        onPointerUp: (_) => _enviar("S"),
        child: ElevatedButton(onPressed: () {}, child: Text(texto)),
      );
    }
    return ElevatedButton(onPressed: () => _enviar(comando), child: Text(texto));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Robot Car")),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          children: [
            ElevatedButton(onPressed: _conectar, child: const Text("Conectar HC-05")),
            Text(_estado),
            const Divider(),
            // Modos
            Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children: [
              _boton("Detener", "0"),
              _boton("Manual", "1"),
              _boton("Evasión", "2"),
              _boton("Línea", "3"),
            ]),
            const Divider(),
            // Flechas (mantener presionado para mover)
            _boton("Adelante", "F", mantener: true),
            Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children: [
              _boton("Izq", "L", mantener: true),
              _boton("Parar", "S"),
              _boton("Der", "R", mantener: true),
            ]),
            _boton("Atrás", "B", mantener: true),
            const Divider(),
            Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children: [
              _boton("− vel", "-"),
              _boton("+ vel", "+"),
            ]),
          ],
        ),
      ),
    );
  }
}
```

---

## Paso 5 — Probar en el celular (modo desarrollo)

1. En el Android: activa **Opciones de desarrollador → Depuración USB**.
2. Empareja el **HC-05** desde Ajustes → Bluetooth (PIN **1234** o **0000**).
3. Conecta el celular por USB y ejecuta:
   ```
   flutter run
   ```
   La app se instala y abre en tu celular, con recarga en caliente al guardar.

---

## Paso 6 — Compilar el APK (instalable)

```
flutter build apk --release
```
El archivo queda en:
```
build/app/outputs/flutter-apk/app-release.apk
```
Cópialo al celular e instálalo (permite "orígenes desconocidos").

---

## Tabla de comandos (referencia)

| Botón | Carácter | Acción |
|-------|----------|--------|
| Detener | `0` | Idle |
| Manual / Evasión / Línea | `1` / `2` / `3` | Cambia de modo |
| Adelante / Atrás | `F` / `B` | Mover (manual) |
| Izq / Der / Parar | `L` / `R` / `S` | Mover (manual) |
| + / − | `+` / `-` | Velocidad |

---

## Mejora futura (opcional)
Un **slider** de velocidad con valor exacto requiere añadir un comando `V120`
al firmware. Pequeño cambio; se documenta aparte cuando lo quieras.

## Relacionado
- [app-control.md](app-control.md) — la versión sin código (App Inventor)
- [firmware/robot_car](../firmware/robot_car/robot_car.ino) — recibe los comandos
