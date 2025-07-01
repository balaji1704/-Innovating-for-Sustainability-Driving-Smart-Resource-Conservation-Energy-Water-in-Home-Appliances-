#include <DHT.h>
#include <Servo.h>

// Define pins
const int dhtPin = 8;
const int trigPin = 10;
const int echoPin = 11;
const int ledPin = 13;
const int buttonPin = 7;
const int pwmPin = 9;
const int dirPin = 12;

// DHT11 setup
#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

// Variables for ultrasonic sensor
long duration;
int distance;

// Variables for button and servo control
bool buttonState = false;
bool lastButtonState = false;

// Initialize servo motor
Servo myServo;

void setup() {
  Serial.begin(9600);

  // Initialize DHT11 sensor
  dht.begin();

  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize LED pin
  pinMode(ledPin, OUTPUT);

  // Initialize button pin
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize servo motor pins
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Attach the servo to its control pin
  myServo.attach(pwmPin);
}

void loop() {
  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature();

  // Ensure temperature reading is valid
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print temperature to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Read ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;  // Convert to cm

  // Print distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Control the servo motor based on temperature
  if (temperature < 32) {
    digitalWrite(dirPin, LOW);  // Anticlockwise
    myServo.write(0);           // Adjust as needed
  } else {
    digitalWrite(dirPin, HIGH); // Clockwise
    myServo.write(180);         // Adjust as needed
  }

  // Control the LED based on ultrasonic distance
  if (distance < 45) {
    digitalWrite(ledPin, LOW);  // Turn LED off
  } else {
    digitalWrite(ledPin, HIGH); // Turn LED on
  }

  // Read pushbutton state
  buttonState = digitalRead(buttonPin) == LOW; // LOW means button is pressed

  // Control LED and servo based on pushbutton state
  if (buttonState != lastButtonState) {
    if (buttonState) {
      digitalWrite(ledPin, HIGH); // Turn LED on
      digitalWrite(dirPin, HIGH); // Clockwise
      myServo.write(180);         // Adjust as needed
    } else {
      digitalWrite(ledPin, LOW);  // Turn LED off
      digitalWrite(dirPin, LOW);  // Anticlockwise
      myServo.write(0);           // Adjust as needed
    }
    lastButtonState = buttonState;
  }

  // Small delay to avoid bouncing and excessive serial output
  delay(200);
}
