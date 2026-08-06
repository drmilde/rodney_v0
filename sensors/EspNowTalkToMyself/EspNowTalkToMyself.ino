#include <esp_now.h>
#include <WiFi.h>

// Replace with your own device's MAC address
uint8_t selfAddress[] = {0xEC, 0xDA, 0x3B, 0x8C, 0x94, 0xC0}; // Replace with your MAC
//ec:da:3b:8c:94:c0

typedef struct struct_message {
    char a[32];
} struct_message;

struct_message myData;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Message: ");
  Serial.println(myData.a);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callbacks
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  // Register peer (yourself)
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, selfAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  strcpy(myData.a, "Hello to myself");
  esp_now_send(selfAddress, (uint8_t *) &myData, sizeof(myData));
  delay(2000);
}
