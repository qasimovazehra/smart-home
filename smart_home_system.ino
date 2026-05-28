#include <Servo.h>

// SERVO DEFINITIONS
Servo garageServo;
Servo homeServo;
Servo clothesServo;

// PIN CONFIGURATIONS
// Garage System
const int garageTrig = 9;
const int garageEcho = 10;
const int garageServoPin = 11;

// Home Door System
const int homeTrig = 4;
const int homeEcho = 3;
const int homeServoPin = 7;

// Rain and Clothes Rack System
const int rainPin = 6; // DO (Digital Output) pin of the rain sensor
const int clothesServoPin = 5;

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Attach servos to their respective pins
  garageServo.attach(garageServoPin);
  homeServo.attach(homeServoPin);
  clothesServo.attach(clothesServoPin);

  // Configure sensor pin modes (Input/Output)
  pinMode(garageTrig, OUTPUT);
  pinMode(garageEcho, INPUT);
  
  pinMode(homeTrig, OUTPUT);
  pinMode(homeEcho, INPUT);
  
  pinMode(rainPin, INPUT); // Rain sensor is configured as INPUT

  // INITIAL STATE (Ensuring everything is closed/safe on startup)
  garageServo.write(0);  // Garage door is closed
  homeServo.write(0);    // Home door is closed
  clothesServo.write(0); // Clothes rack is outside (no rain)
}

void loop() {
  // 1. GARAGE DOOR SYSTEM (Distance measurement and control)
  long garageDuration, garageDistance;
  digitalWrite(garageTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(garageTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(garageTrig, LOW);
  garageDuration = pulseIn(garageEcho, HIGH);
  garageDistance = garageDuration * 0.034 / 2;

  if (garageDistance > 0 && garageDistance <= 25) { // If car is closer than 25 cm
    garageServo.write(90); // Open the garage door
    delay(3000);           // Keep open for 3 seconds
  } else {
    garageServo.write(0);  // Otherwise, close the door
  }

  // 2. HOME DOOR SYSTEM (Distance measurement and control)
  long homeDuration, homeDistance;
  digitalWrite(homeTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(homeTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(homeTrig, LOW);
  homeDuration = pulseIn(homeEcho, HIGH);
  homeDistance = homeDuration * 0.034 / 2;

  if (homeDistance > 0 && homeDistance <= 15) { // If person is closer than 15 cm
    homeServo.write(90); // Open the home door
    delay(3000);         // Keep open for 3 seconds
  } else {
    homeServo.write(0);  // Otherwise, close the door
  }

  // 3. RAIN AND CLOTHES RACK SYSTEM
  int rainStatus = digitalRead(rainPin);

  // Note: Rain sensors typically output LOW (0) when water is detected
  if (rainStatus == LOW) { 
    clothesServo.write(90); // If it rains, retract the rack inside (90 degrees)
    Serial.println("Rain detected! Clothes rack retracted inside.");
  } else {
    clothesServo.write(0);  // When rain stops, extend the rack outside (0 degrees)
  }

  // Small delay to prevent MCU overload and maintain stability
  delay(100);
}
