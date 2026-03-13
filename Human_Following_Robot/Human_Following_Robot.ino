// Human Following Robot using Arduino

#include <NewPing.h>   // Library for ultrasonic sensor
#include <Servo.h>     // Library for servo motor
#include <AFMotor.h>   // Library for motor shield

// Pin definitions
#define RIGHT A2          // Right IR sensor connected to A2
#define LEFT A3           // Left IR sensor connected to A3
#define TRIGGER_PIN A1    // Ultrasonic trigger pin
#define ECHO_PIN A0       // Ultrasonic echo pin
#define MAX_DISTANCE 200  // Maximum measurable distance (cm)

// Variables to store sensor values
unsigned int distance = 0;
unsigned int Right_Value = 0;
unsigned int Left_Value = 0;

// Initialize ultrasonic sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Create motor objects for motor shield
AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);
AF_DCMotor Motor3(3, MOTOR34_1KHZ);
AF_DCMotor Motor4(4, MOTOR34_1KHZ);

// Create servo object
Servo myservo;
int pos = 0;   // Variable to store servo position

void setup() {

  Serial.begin(9600);       // Start serial communication
  myservo.attach(10);       // Attach servo to pin 10

  // Sweep servo from center → right → left → center
  for (pos = 90; pos <= 180; pos++) {
    myservo.write(pos);
    delay(15);
  }

  for (pos = 180; pos >= 0; pos--) {
    myservo.write(pos);
    delay(15);
  }

  for (pos = 0; pos <= 90; pos++) {
    myservo.write(pos);
    delay(15);
  }

  pinMode(RIGHT, INPUT);    // Set right IR sensor as input
  pinMode(LEFT, INPUT);     // Set left IR sensor as input
}

void loop() {

  delay(50);                           // Delay between ultrasonic readings
  distance = sonar.ping_cm();          // Measure distance using ultrasonic sensor

  Serial.print("Distance: ");
  Serial.println(distance);

  // Read IR sensor values
  Right_Value = digitalRead(RIGHT);
  Left_Value  = digitalRead(LEFT);

  Serial.print("RIGHT: ");
  Serial.println(Right_Value);

  Serial.print("LEFT: ");
  Serial.println(Left_Value);

  // Move forward if object detected within 1–15 cm
  if ((distance > 1) && (distance < 15)) {

    Motor1.setSpeed(130);
    Motor1.run(FORWARD);

    Motor2.setSpeed(130);
    Motor2.run(FORWARD);

    Motor3.setSpeed(130);
    Motor3.run(FORWARD);

    Motor4.setSpeed(130);
    Motor4.run(FORWARD);
  }

  // Turn left when right IR detects object
  else if ((Right_Value == 0) && (Left_Value == 1)) {

    Motor1.setSpeed(150);
    Motor1.run(FORWARD);

    Motor2.setSpeed(150);
    Motor2.run(FORWARD);

    Motor3.setSpeed(150);
    Motor3.run(BACKWARD);

    Motor4.setSpeed(150);
    Motor4.run(BACKWARD);

    delay(150);
  }

  // Turn right when left IR detects object
  else if ((Right_Value == 1) && (Left_Value == 0)) {

    Motor1.setSpeed(150);
    Motor1.run(BACKWARD);

    Motor2.setSpeed(150);
    Motor2.run(BACKWARD);

    Motor3.setSpeed(150);
    Motor3.run(FORWARD);

    Motor4.setSpeed(150);
    Motor4.run(FORWARD);

    delay(150);
  }

  // Stop motors if object is far away
  else if (distance > 15) {

    Motor1.setSpeed(0);
    Motor1.run(RELEASE);

    Motor2.setSpeed(0);
    Motor2.run(RELEASE);

    Motor3.setSpeed(0);
    Motor3.run(RELEASE);

    Motor4.setSpeed(0);
    Motor4.run(RELEASE);
  }
}