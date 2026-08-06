#include <BLESerial.h>

BLESerial<> SerialBLE;

// Pin-Definitionen für die Buttons (Taster)
const int PIN_BTN_FOTO = 4;
const int PIN_BTN_CHAT = 5;
const int PIN_BTN_STOP = 18;

// Pin-Definitionen für die LEDs
const int PIN_LED_FOTO = 19;
const int PIN_LED_CHAT = 21;
const int PIN_LED_STOP = 22;

void setup() {
  Serial.begin(9600);
  SerialBLE.begin("ESP32-Fernbedienung");
  // Buttons als Eingang mit internem Pull-Up-Widerstand definieren
  // (Der Pin ist HIGH, wenn nicht gedrückt, und wird beim Drücken LOW)
  pinMode(PIN_BTN_FOTO, INPUT_PULLUP);
  pinMode(PIN_BTN_CHAT, INPUT_PULLUP);
  pinMode(PIN_BTN_STOP, INPUT_PULLUP);

  // LEDs als Ausgang definieren
  pinMode(PIN_LED_FOTO, OUTPUT);
  pinMode(PIN_LED_CHAT, OUTPUT);
  pinMode(PIN_LED_STOP, OUTPUT);

  // Alle LEDs zu Beginn ausschalten
  digitalWrite(PIN_LED_FOTO, LOW);
  digitalWrite(PIN_LED_CHAT, LOW);
  digitalWrite(PIN_LED_STOP, LOW);
}

void loop() {
  // 1. Befehl "foto" prüfen
  if (digitalRead(PIN_BTN_FOTO) == LOW) {
    char *myData = "foto\n";
    SerialBLE.write((uint8_t *)myData, 5);
    SerialBLE.flush();
    Serial.println("Gesendet: foto");

    digitalWrite(PIN_LED_FOTO, HIGH);  // LED an
    delay(300);

    // Kurze Pause (entprellt auch den Taster)
    digitalWrite(PIN_LED_FOTO, LOW);
    // LED aus
  }

  // 2. Befehl "chat" prüfen
  if (digitalRead(PIN_BTN_CHAT) == LOW) {
    char *myData = "chat\n";
    SerialBLE.write((uint8_t *)myData, 5);
    SerialBLE.flush();

    Serial.println("Gesendet: chat");

    digitalWrite(PIN_LED_CHAT, HIGH);
    delay(300);
    digitalWrite(PIN_LED_CHAT, LOW);
  }

  // 3. Befehl "stop" prüfen
  if (digitalRead(PIN_BTN_STOP) == LOW) {
    char *myData = "stop\n";
    SerialBLE.write((uint8_t *)myData, 5);
    SerialBLE.flush();

    Serial.println("Gesendet: stop");
    digitalWrite(PIN_LED_STOP, HIGH);
    delay(300);
    digitalWrite(PIN_LED_STOP, LOW);
  }

  // Optional: Weiterhin Daten vom Handy im Serial Monitor anzeigen
  if (SerialBLE.available() > 0) {
    String empfangen = SerialBLE.readStringUntil('\n');
    Serial.print("Vom Handy empfangen: ");
    Serial.println(empfangen);
  }
}