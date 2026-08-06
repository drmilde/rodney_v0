/*********
  Main Control for Rodney

  JTM @03/2026
*********/
#include <esp_now.h>
#include <WiFi.h>

// Motor A
int motor1Pin1 = 5; 
int motor1Pin2 = 6; 
int enable1Pin = 7; 

int motor2Pin1 = 9; 
int motor2Pin2 = 10; 
int enable2Pin = 11; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle1 = 200;
int dutyCycle2 = 200;


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

// Aktionen

void leftStop() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(enable1Pin, 0);   
}

void leftBackward(int speed) {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  ledcWrite(enable1Pin, speed);   
}

void leftForward(int speed) {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  ledcWrite(enable1Pin, speed);   
}

void rightForward(int speed) {
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); 
  ledcWrite(enable2Pin, speed);   
}

void rightBackward(int speed) {
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); 
  ledcWrite(enable2Pin, speed);   
}

void rightStop() {
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(enable2Pin, 0);   
}


// Commands

void stop() {
  leftStop();
  rightStop();
}

void vorwaerts(int speed) {
  stop();
  leftForward(speed);
  rightForward(speed);
}

void rueckwaerts(int speed) {
  stop();
  leftBackward(speed);
  rightBackward(speed);
}

void rechtsrumdrehen(int speed) {
  stop();
  leftForward(speed);
  rightBackward(speed);
}

void linksrumdrehen(int speed) {
  stop();
  leftBackward(speed);
  rightForward(speed);
}

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
    vorwaerts(255);
  } else {
    if (yAxis < -0.8f) {
      rueckwaerts(255);
    } else {
      if (xAxis > 0.8f) {
        rechtsrumdrehen(255);
      } else {
        if (xAxis < -0.8f) {
          linksrumdrehen(255);
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

/// ESPNOW Ende


void setup() {
  Serial.begin(115200);

  Serial.println("\nBooting Rodney base...");

  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // configure LEDC PWM
  ledcAttachChannel(enable1Pin, freq, resolution, pwmChannel);
  ledcAttachChannel(enable2Pin, freq, resolution, pwmChannel);

  Serial.println("\nStarting ESPNOW...");
  setupESPNOW();

  delay(2000);
  // System is running and waiting for commands
  Serial.println("\nStartup ready...");
}

void testMotorLeft() {
  leftBackward(200);
  delay(2000);

  // Stop the DC motor
  leftStop();
  delay(1000);

  leftForward(200);
  delay(2000);
}


void testMotorRight() {
  // rightForward(200);
  // delay(2000);

  // // Stop the DC motor
  // rightStop();
  // delay(1000);

  // rightBackward(200);
  delay(2000);
}



void loop() {
  // leftStop();
  // rightStop();
  // delay(4000);
  // leftBackward(255);
  // rightBackward(255);
  // delay(4000);

  // leftStop();
  // rightStop();
  // delay(4000);
  // leftForward(255);
  // rightBackward(255);
  // delay(1500);
  Serial.println("running");
  delay(200);
}