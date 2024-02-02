const int trigPin = A0; // pin 14
const int echoPin = 13;
const int ledStartPin = 2;
const int ledEndPin = 12;

float duration, distance;
unsigned long lastChangeTime = 0;
bool isLowPowerMode = false; // Variable to remember the low-power mode state

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  // Set LED pins as OUTPUT
  for (int pin = ledStartPin; pin <= ledEndPin; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  if (isLowPowerMode) {
    // In low-power mode, delay first to reduce checks
    delay(5000);
  }

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  float newDistance = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.println(newDistance);

  if (abs(newDistance - distance) > 10.0) {  // Check if there is a significant change in distance
    if (isLowPowerMode) {
      isLowPowerMode = false; // Exiting low-power mode
    }
    lastChangeTime = millis();
    updateLEDs(newDistance);
    distance = newDistance;
  } else {
    // If there is little change for 30 seconds, turn off LEDs
    if (millis() - lastChangeTime > 30000) {
      if (!isLowPowerMode) {
        turnOffLEDs();
        isLowPowerMode = true; // Entering low-power mode
      }
      // No need to delay here as it's already at the start of the loop when in low-power mode
    }
  }

  if (!isLowPowerMode) {
    delay(100); // Only delay if not in low-power mode
  }
}

void updateLEDs(float distance) {
  int totalLEDs = ledEndPin - ledStartPin + 1;
  float stepSize = (100.0 - 20.0) / totalLEDs;

  // Calculate the LED index based on the distance
  int activeLED = int((100.0 - distance) / stepSize) + ledStartPin;

  // Ensure the LED index is within the specified range
  activeLED = constrain(activeLED, ledStartPin, ledEndPin);

  // Turn off all LEDs
  for (int pin = ledStartPin; pin <= ledEndPin; pin++) {
    digitalWrite(pin, LOW);
  }

  // Turn on LEDs representing distances lower than the measured distance
  for (int pin = ledStartPin; pin <= activeLED; pin++) {
    digitalWrite(pin, HIGH);
  }
}

void turnOffLEDs() {
  // Turn off all LEDs
  for (int pin = ledStartPin; pin <= ledEndPin; pin++) {
    digitalWrite(pin, LOW);
  }
}