/*
  Read joystick value, calibrate, and normalize to (-1.0,1.0)

  JTM, 03/2026
 */

#include <esp_now.h>
#include <WiFi.h>


 // REPLACE WITH YOUR RECEIVER MAC Address
//uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// Rodney V0
//uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0x98, 0x86, 0xA0}; //ec:da:3b:98:86:a0

// Rodney DDSM
uint8_t broadcastAddress[] = {0xCC, 0x7B, 0x5C, 0xBC, 0x5D, 0xB0}; //cc:7b:5c:bc:5d:b0

// C3-Oled display
//uint8_t broadcastAddress[] = {0x20, 0x6E, 0xF1, 0x6B, 0x58, 0x18}; // 20:6e:f1:6b:58:18


// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  float b;
  float c;
  bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;


// Definitionen für den Joystick

#define VRX_01_PIN  7 // ESP32 pin GPIO39 (ADC3) connected to VRX pin
#define VRY_01_PIN  17 // ESP32 pin GPIO36 (ADC0) connected to VRY pin

int valueX_01 = 0; // to store the X-axis value
int valueY_01 = 0; // to store the Y-axis valueint zeroX_01 = 0;
int zeroX_01 = 0;
int zeroY_01 = 0;


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  //Serial.println(status);
}

void setup() {
  Serial.begin(9600);

  // Set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);
  calibrate();


  // Setup Wifi/ESP-NOW
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("added peer");
  }

  Serial.println("Setup WIFI/ESP-NOW OK");
  delay(2000);

}

void calibrate() {
  int count = 512;

  Serial.println("\nCalibrating ...\n");

  for (int i = 0; i < 512; i++) {
    valueX_01 = analogRead(VRX_01_PIN);
    valueY_01 = analogRead(VRY_01_PIN);
    zeroX_01 = zeroX_01 + valueX_01;
    zeroY_01 = zeroY_01 + valueY_01;
    delay(5);
  }
  zeroX_01 = zeroX_01 / 512;
  zeroY_01 = zeroY_01 / 512;

  Serial.print("zeroX_01 = ");
  Serial.print(zeroX_01);
  Serial.print(", zeroY_01 = ");
  Serial.println(zeroY_01);
}

float normXY(int vxy, int zeroXY) {
  int diff = vxy - zeroXY;
  float prange = 4096.0 - zeroXY;
  float nrange = zeroXY * 1.0f;

  if (diff > 0) {
    return min(1.0f, (diff / prange));
  } else {
    return max(-1.0f, (diff / nrange));
  }
}

void loop() {
  Serial.println("\n\n========= READING VALUES ===========");
  // read X and Y analog values
  valueX_01 = analogRead(VRX_01_PIN);
  valueY_01 = analogRead(VRY_01_PIN);

  float nX = normXY(valueX_01, zeroX_01);
  float nY = normXY(valueY_01, zeroY_01);

  // print data to Serial Monitor on Arduino IDE
  Serial.print("old max = ");
  Serial.print(1.0);
  Serial.print(", ");
  Serial.print(-1.0);
  Serial.print(", ");
  Serial.print(nX);
  Serial.print(", ");
  Serial.print(nY);

  Serial.println("\n========= SENDING ===========");

  // Set values to send
  strcpy(myData.a, "Sending values");
  myData.b = nX;
  myData.c = nY;
  myData.d = false;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  Serial.println("\n========= END CYCLE ===========");

  delay(20);
}
