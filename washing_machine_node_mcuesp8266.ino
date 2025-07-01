#include <Servo.h>

// Define servo objects
Servo servo1;
Servo servo2;
Servo servo3;

// Define timing variables
unsigned long cycleDuration = 10000; // 10 seconds for each cycle
unsigned long delayDuration = 5000;  // 5 seconds delay between cycles
unsigned long servo3OnDuration = 12000; // 12 seconds for servo3

unsigned long startTime;
unsigned long cycleStartTime;
int currentCycle = 0;

void setup() {
  // Attach the servos to their respective pins
  servo1.attach(9); // Connect servo1 to pin 9
  servo2.attach(10); // Connect servo2 to pin 10
  servo3.attach(11); // Connect servo3 to pin 11

  // Initialize the servos
  servo1.write(0); // Ensure servo1 is off initially
  servo2.write(0); // Ensure servo2 is off initially
  servo3.write(0); // Ensure servo3 is off initially

  // Record the start time
  startTime = millis();
  cycleStartTime = startTime;

  // Turn on servo3 before the first cycle
  servo3.write(90); // Assume 90 degrees means 'on' for servo3
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  unsigned long cycleTime = currentTime - cycleStartTime;

  // Handle servo3 timing
  if (elapsedTime <= servo3OnDuration) {
    // Keep servo3 on for 12 seconds
    servo3.write(90); // Assume 90 degrees means 'on' for servo3
  } else {
    // Turn off servo3 after 12 seconds
    servo3.write(0);
  }

  // Handle the cycles and delays
  if (cycleTime >= (cycleDuration + delayDuration) * (currentCycle + 1) - delayDuration) {
    // Transition between cycles
    currentCycle++;
    cycleStartTime = currentTime;
  }

  if (currentCycle >= 3) {
    // Stop the loop after the 3 cycles
    return;
  }

  // Control servo1 and servo2 based on the cycle number and delay
  if (currentCycle < 2) {
    // During the first two delays
    if (cycleTime >= cycleDuration * currentCycle && cycleTime < cycleDuration * (currentCycle + 1)) {
      // During the active cycle, turn off servos 1 and 2
      servo1.write(0);
      servo2.write(0);
    } else {
      // During the delay period
      servo1.write(90); // Assume 90 degrees means 'on' for servo1
      servo2.write(0);  // Servo2 remains off
    }
  } else if (currentCycle == 2) {
    // After the 3rd cycle and delay completion
    if (cycleTime >= cycleDuration * currentCycle && cycleTime < cycleDuration * (currentCycle + 1)) {
      // During the active cycle, turn off servos 1 and 2
      servo1.write(0);
      servo2.write(0);
    } else {
      // During the delay period after the 3rd cycle
      servo1.write(0);  // Servo1 remains off
      servo2.write(90); // Assume 90 degrees means 'on' for servo2
    }
  }
}
