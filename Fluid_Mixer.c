#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the pin connections
const int pump1Pin1 = 2;
const int pump1Pin2 = 3;
const int pump2Pin1 = 4;
const int pump2Pin2 = 5;
const int c1ButtonPin = 6;
const int c2ButtonPin = 7;
const int mixerPin = 8;
const int valvePin = 9;
const int emptyLevelSensorPin = 10;
const int fullLevelSensorPin = 11;

// Initialize the LCD
LiquidCrystal_I2C lcd(32, 16, 2);

// Define the states of the system
enum State {
  INITIAL,
  CONFIRM_CYCLE,
  MIXING_CYCLE,
  INTERRUPTED,
  EMPTYING_CONTAINER
};

// Initialize the state
State state = INITIAL;

// Define the timers
unsigned long timer = 0;
unsigned long delayPeriod = 0;

// Define current step in mixing cycle
int currentStep = 0;

void setup() {
  // Initialize the pins
  pinMode(pump1Pin1, OUTPUT);
  pinMode(pump1Pin2, OUTPUT);
  pinMode(pump2Pin1, OUTPUT);
  pinMode(pump2Pin2, OUTPUT);
  pinMode(c1ButtonPin, INPUT_PULLUP);
  pinMode(c2ButtonPin, INPUT_PULLUP);
  pinMode(mixerPin, OUTPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(emptyLevelSensorPin, INPUT);
  pinMode(fullLevelSensorPin, INPUT);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initial State");
}

void loop() {
  // Read the button states
  int c1ButtonState = digitalRead(c1ButtonPin);
  int c2ButtonState = digitalRead(c2ButtonPin);

  // Handle the states
  switch (state) {
    case INITIAL:
      digitalWrite(valvePin, LOW);
      if (c1ButtonState == LOW) {
        state = CONFIRM_CYCLE;
        timer = millis();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Start cycle?");
        if (c2ButtonState == LOW) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Initial State");
       	break;
    	}	
        lcd.setCursor(0, 1);
        lcd.print("C1 to confirm.");
      }
      break;
	
      
    case CONFIRM_CYCLE:
      if (c1ButtonState == LOW && digitalRead(valvePin) == LOW) {
        state = MIXING_CYCLE;
        currentStep = 1; // Start with step 1
        timer = millis();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Mixing started");
      } else if (millis() - timer > 10000 && digitalRead(valvePin) == LOW) {
        state = INITIAL;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Initial State");
        
      } else if (c2ButtonState == LOW) {
		state = INTERRUPTED;

      }
    	
      break;

    case MIXING_CYCLE:
      switch (currentStep) {
        case 1: // Step 1: Activate pump B1 for 5 seconds
          if (millis() - timer < 5000 && digitalRead(fullLevelSensorPin) == 1) {
            digitalWrite(pump1Pin1, HIGH);
            digitalWrite(pump1Pin2, LOW);
            lcd.setCursor(0, 1);
            lcd.print("Pump B1 active");
          } else {
            digitalWrite(pump1Pin1, LOW);
            digitalWrite(pump1Pin2, LOW);
            currentStep = 2; // Move to next step
            timer = millis(); // Reset the timer
          }
          break;

        case 2: // Step 2: Activate pump B2 until full-level sensor is triggered
          if (digitalRead(fullLevelSensorPin) == HIGH) {
            digitalWrite(pump2Pin1, HIGH);
            digitalWrite(pump2Pin2, LOW);
            lcd.setCursor(0, 1);
            lcd.print("Pump B2 active");
          } 
          else{
            digitalWrite(pump2Pin1, LOW);
            digitalWrite(pump2Pin2, LOW);
            currentStep = 3; // Move to next step
            timer = millis(); // Reset the timer
          }
          break;
        case 3: // Step 3: Turn on the mixer for 5 seconds
          if (millis() - timer < 5000) {
            digitalWrite(mixerPin, HIGH);
            lcd.setCursor(0, 1);
            lcd.print("Mixer active");
          } else {
            digitalWrite(mixerPin, LOW);
            currentStep = 4; // Move to next step
          }
          break;

        case 4: // Step 4: Activate the solenoid valve until the container is emptied
          if (digitalRead(emptyLevelSensorPin) == HIGH) {
            digitalWrite(valvePin, HIGH);
            lcd.setCursor(0, 1);
            lcd.print("Valve active");
          } else {
            digitalWrite(valvePin, LOW);
            state = INITIAL;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Cycle complete");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Initial State");
          }
          break;
      }

      // Interrupt the cycle if C2 is pressed
      if (c2ButtonState == LOW) {
        state = INTERRUPTED;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cycle");
        lcd.setCursor(0,1);
        lcd.print("Interrupted");
        stopAllOutputs();
      }
      break;

    case INTERRUPTED:
      if (c2ButtonState == LOW) {
        state = EMPTYING_CONTAINER;

        if (digitalRead(emptyLevelSensorPin) == HIGH) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Emptying");
          lcd.setCursor(0, 1);
          lcd.print("container");
          digitalWrite(emptyLevelSensorPin, HIGH);
        } else {
          digitalWrite(emptyLevelSensorPin, LOW);
          state = INITIAL;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Initial State");
        }
      }
      break;
// Empties if float sensor is 1
    case EMPTYING_CONTAINER:
      if (digitalRead(emptyLevelSensorPin) == HIGH) {
        digitalWrite(valvePin, HIGH);
      } else {
        digitalWrite(valvePin, LOW);
        state = INITIAL;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Initial State");
      }
      break;
  }
}

// Stop all functions
void stopAllOutputs() {
  digitalWrite(pump1Pin1, LOW);
  digitalWrite(pump1Pin2, LOW);
  digitalWrite(pump2Pin1, LOW);
  digitalWrite(pump2Pin2, LOW);
  digitalWrite(mixerPin, LOW);
  digitalWrite(valvePin, LOW);
}
