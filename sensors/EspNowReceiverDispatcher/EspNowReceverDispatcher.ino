/*
  ESP-NOWreceiver ... echo

  JTM, 03/2026
*/

#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    float b;
    float c;
    bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Aktionen

void vorwaerts() {}
void rueckwaerts() {}
void rechtsrumdrehen() {}
void linksrumdrehen() {}
void stop() {}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Char: ");
  // Serial.println(myData.a);
  // Serial.print("Float: ");
  // Serial.println(myData.b);
  // Serial.print("Float: ");
  // Serial.println(myData.c);
  // Serial.print("Bool: ");
  // Serial.println(myData.d);
  // Serial.println();

  float xAxis = myData.b;
  float yAxis = myData.c;

  if (yAxis > 0.8f) {
    vorwaerts();
  } else {
    if (yAxis < -0.8f) {
      rueckwaerts();
    } else {
      if (xAxis > 0.8f) {
        rechtsrumdrehen();
      } else {
        if (xAxis < -0.8f) {
          linksrumdrehen();
        } else {
          stop();
        }
      }
    }
  }
}

void setupESPNOW() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("Booting up Receiver");

  setupESPNOW();
  
  Serial.println("ready");
  delay(2000);
  Serial.println("running...");
}
 
void loop() {
  // do something
  delay(200);
}