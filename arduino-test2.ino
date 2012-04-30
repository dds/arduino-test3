#include <stdint.h>

#define SWITCH_PIN 3
#define POT_PIN 0

#define MODE_STROBE 1
#define MODE_FLASH 2
#define MODE_ON 3
#define MODE_OFF 4

#define PING_PIN 8

static uint8_t modes[] = {MODE_STROBE, MODE_FLASH, MODE_ON, MODE_OFF};
static uint8_t ledPins[] = {12, 11, 10, 9, NULL};
static uint8_t speedLedPins[] = {5, 6, 7, NULL};
static uint8_t *currentStrobePin = &ledPins[0];
static uint8_t *currentMode = &modes[0];
static uint8_t switchState = 1;
static long previousDistance = -1;
static long ticks = 0;

void setup(void) {
  pinMode(SWITCH_PIN, INPUT);
  for (uint8_t i = 0; ledPins[i] != NULL; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  for (uint8_t i = 0; speedLedPins[i] != NULL; i++) {
    pinMode(speedLedPins[i], OUTPUT);
  }
  pinMode(PING_PIN, OUTPUT);
}

void displaySpeed(void) {
  int speed = map(analogRead(POT_PIN), 0, 1023, 0, 999);
  if (*currentMode == MODE_ON || *currentMode == MODE_OFF) {
    speed = 0;
  }

  if (speed < 50) {
    digitalWrite(speedLedPins[0], HIGH);
    digitalWrite(speedLedPins[1], LOW);
    digitalWrite(speedLedPins[2], LOW);
  } else if (speed < 500) {
    digitalWrite(speedLedPins[0], LOW);
    digitalWrite(speedLedPins[1], HIGH);
    digitalWrite(speedLedPins[2], LOW);
  } else {
    digitalWrite(speedLedPins[0], LOW);
    digitalWrite(speedLedPins[1], LOW);
    digitalWrite(speedLedPins[2], HIGH);
    }
}

void displayDistance(long pulse) {
  long distance = pulse / 29 / 2;
  if (distance < 18) {
    digitalWrite(speedLedPins[0], HIGH);
    digitalWrite(speedLedPins[1], LOW);
    digitalWrite(speedLedPins[2], LOW);
  } else if (distance < 50) {
    digitalWrite(speedLedPins[0], LOW);
    digitalWrite(speedLedPins[1], HIGH);
    digitalWrite(speedLedPins[2], LOW);
  } else {
    digitalWrite(speedLedPins[0], LOW);
    digitalWrite(speedLedPins[1], LOW);
    digitalWrite(speedLedPins[2], HIGH);
    }
}

int strobeDelay(void) {
  int potVal = analogRead(POT_PIN);
  return map(potVal, 0, 1023, 400, 50);
}

void strobeStep(void) {
  digitalWrite(*currentStrobePin++, LOW);
  if (*currentStrobePin == NULL) {
    currentStrobePin = &ledPins[0];
  }
  digitalWrite(*currentStrobePin, HIGH);
}

void flash(void) {
  for (uint8_t i = 0; ledPins[i] != NULL; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(100);
  for (uint8_t i = 0; ledPins[i] != NULL; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  delay(100);
}

long ping(void) {
  pinMode(PING_PIN, OUTPUT);
  digitalWrite(PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_PIN, HIGH);
  delayMicroseconds(3);
  digitalWrite(PING_PIN, LOW);
  pinMode(PING_PIN, INPUT);
  return pulseIn(PING_PIN, HIGH);
}

void loop(void) {
  if (digitalRead(SWITCH_PIN) == LOW) {
    if (switchState == 0) {
      if (*currentMode == MODE_OFF) {
        currentMode = &modes[0];
      } else {
        currentMode++;
      }
    }
    switchState = 1;
  } else {  // SWITCH_PIN == HIGH
    switchState = 0;
  }

  switch (*currentMode) {
    case MODE_FLASH:
      flash();
      break;
    case MODE_STROBE:
      strobeStep();
      break;
    case MODE_ON:
      for (uint8_t i = 0; ledPins[i] != NULL; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
      break;
    case MODE_OFF:
    default:
      for (uint8_t i = 0; ledPins[i] != NULL; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      break;
  }
  // displaySpeed();
  if (ticks % 7 == 0) {
    displayDistance(ping());
  }
  delay(strobeDelay());
  ticks++;
}
