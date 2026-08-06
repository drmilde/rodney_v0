/*
speed loop ctrl for DDSM115.
*/

#include <esp_now.h>
#include <WiFi.h>
#include <ddsm_ctrl.h>

DDSM_CTRL dc;

// device settings.
#define DDSM_RX 18
#define DDSM_TX 19

#define LEFT_WHEEL_ID 1
#define RIGHT_WHEEL_ID 2

/// ESPNOW

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

// Start: Rodney bas actions


void leftStop(int time) {
	dc.ddsm_ctrl(LEFT_WHEEL_ID, 1, 3); // going down to 2 rpm
}

void leftBackward(int speed) {
	dc.ddsm_ctrl(LEFT_WHEEL_ID, speed, 3); // speed rpm
}

void leftForward(int speed) {
	dc.ddsm_ctrl(LEFT_WHEEL_ID, -speed, 3); // -spee rpm
}

void rightStop(int time) {
	dc.ddsm_ctrl(RIGHT_WHEEL_ID, -1, 3); // going down to 2 rpm
}

void rightBackward(int speed) {
	dc.ddsm_ctrl(RIGHT_WHEEL_ID, -speed, 3); // -speed rpm
}

void rightForward(int speed) {
	dc.ddsm_ctrl(RIGHT_WHEEL_ID, speed, 3); // speed rpm
}

// Commands

void stop(int time) {
	leftStop(time);
	rightStop(time);
}

void vorwaerts(int speed) {
  stop(100);
  leftForward(speed);
  rightForward(speed);
}

void rueckwaerts(int speed) {
  stop(100);
  leftBackward(speed);
  rightBackward(speed);
}

void rechtsrumdrehen(int speed) {
  stop(100);
  leftForward(speed);
  rightBackward(speed);
}

void linksrumdrehen(int speed) {
  stop(100);
  leftBackward(speed);
  rightForward(speed);
}


// End: Rodney base actions

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
    vorwaerts(200);
  } else {
    if (yAxis < -0.8f) {
      rueckwaerts(200);
    } else {
      if (xAxis > 0.8f) {
        rechtsrumdrehen(50);
      } else {
        if (xAxis < -0.8f) {
          linksrumdrehen(50);
        } else {
          stop(100);
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

/// ESPNOW Ende



void setup() {
	Serial.begin(115200);
  Serial.println("\nBooting Rodney base...");

  Serial.println("\nStarting ESPNOW...");
  setupESPNOW();

  delay(2000);
  // System is running and waiting for commands

	// ddsm init.
  Serial.println("\nSetting up DDSM115...");
	Serial1.begin(DDSM_BAUDRATE, SERIAL_8N1, DDSM_RX, DDSM_TX);
	dc.pSerial = &Serial1;

	// config the type of ddsm. 
	dc.set_ddsm_type(115);

	// clear ddsm serial buffer.
	dc.clear_ddsm_buffer();

  Serial.println("\nStartup ready...");
}

void loop() {
	// ACC_TIME:
	// Acceleration Time: This parameter is effective in speed loop mode. 
	// It defines the acceleration time per 1 rpm in units of 0.1 ms. 
	// For instance, if set to 1, the acceleration time is 0.1 ms per 1 rpm; 
	// if set to 10, the acceleration time is 1 ms per 1 rpm (10 * 0.1 ms). 
	// If set to 0, the default value is 1, 
	// meaning the acceleration time is 0.1 ms per 1 rpm. 
	// This parameter only applies to speed loop mode.

	// args: ddsm_ctrl(DDSM_ID, CMD, ACC_TIME)
	// dc.ddsm_ctrl(1, 200, 3); // 50 rpm
	// delay(3000);

	// // args: ddsm_ctrl(DDSM_ID, CMD, ACC_TIME)
	// dc.ddsm_ctrl(1, -200, 3); // -50 rpm
	// delay(3000);

	// dc.ddsm_ctrl(2, 20, 3); // 50 rpm
	// delay(3000);

	// // args: ddsm_ctrl(DDSM_ID, CMD, ACC_TIME)
	// dc.ddsm_ctrl(2, -20, 3); // -50 rpm


}