/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-dc-motor-l298n-motor-driver-control-speed-direction/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.  
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/
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


void setup() {
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

  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");
}


void leftStop() {
  Serial.println("Motor 1 stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(enable1Pin, 0);   
}

void leftBackward(int speed) {
  // Move DC motor backwards at maximum speed
  Serial.println("Motor 1: Moving Backward");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  ledcWrite(enable1Pin, speed);   
}

void leftForward(int speed) {
  Serial.println("Motor 1: Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  ledcWrite(enable1Pin, speed);   
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

void rightForward(int speed) {
  Serial.println("Motor 2: Moving Forward");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); 
  ledcWrite(enable2Pin, speed);   
}

void rightBackward(int speed) {
  // Move DC motor backwards at maximum speed
  Serial.println("Motor 2: Moving Backwards");
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); 
  ledcWrite(enable2Pin, speed);   
}

void rightStop() {
  Serial.println("Motor 2 stopped");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(enable2Pin, 0);   
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
  //testMotorLeft();
  leftStop();
  rightStop();
  delay(4000);
  leftBackward(255);
  rightBackward(255);
  delay(4000);

  leftStop();
  rightStop();
  delay(4000);
  leftForward(255);
  rightBackward(255);
  delay(1500);
  //testMotorRight();
}