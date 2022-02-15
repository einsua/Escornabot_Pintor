#include <Arduino.h>
#include "EscornabotDIY.h"

////////////////////////////////////////
// Initialization

// constructor
EscornabotDIY::EscornabotDIY() {
	// Initial setup
	// Stepper motors
	pinMode(STEPPERMOTOR_L_IN1, OUTPUT);
	pinMode(STEPPERMOTOR_L_IN2, OUTPUT);
	pinMode(STEPPERMOTOR_L_IN3, OUTPUT);
	pinMode(STEPPERMOTOR_L_IN2, OUTPUT);
	pinMode(STEPPERMOTOR_R_IN1, OUTPUT);
	pinMode(STEPPERMOTOR_R_IN2, OUTPUT);
	pinMode(STEPPERMOTOR_R_IN3, OUTPUT);
	pinMode(STEPPERMOTOR_R_IN4, OUTPUT);
	// On-board LED
	pinMode(SIMPLELED_PIN, OUTPUT);
	// Buzzer
	pinMode(BUZZER_PIN, OUTPUT);
	// Keypad
	configKeypad(
		KEYPAD_PIN,
		KEYVALUE_FORWARD,
		KEYVALUE_TURNLEFT,
		KEYVALUE_GO,
		KEYVALUE_TURNRIGHT,
		KEYVALUE_BACKWARD,
		KEYVALUE_NONE);
}

// destructor
EscornabotDIY::~EscornabotDIY() {
}

////////////////////////////////////////
// Stepper motors

void EscornabotDIY::move(float cms)
{
	// fixReversed
	if (_isReversed) cms = -cms;
	// calculations
	int16_t steps = cms * 10 * STEPPERS_STEPS_MM; // implicit trunc()
	int8_t increment = steps > 0 ? -1 : 1;
	uint16_t wait = 1000000 / STEPPERMOTOR_STEPS_PER_SECOND; //implicit trunc()
	bool reversed = cms < 0;

	// do the steps
	uint8_t current_pattern = 0;
	while (steps != 0)
	{
		// both motors advance in the same direction
		stepLeftMotor(steppingPatterns[current_pattern], reversed);
		stepRightMotor(steppingPatterns[current_pattern], reversed);
		delayMicroseconds(wait);
		current_pattern ++;
		if (current_pattern >= numPatterns) current_pattern = 0;
		steps += increment;
	}

	// disable motors
	stepLeftMotor(0, reversed);
	stepRightMotor(0, reversed);
}

void EscornabotDIY::turn(float degrees)
{
	// fixReversed
	if (_isReversed) degrees = -degrees;
	// calculations
	int16_t steps = STEPPERS_STEPS_DEG * degrees;
	int8_t increment = steps > 0 ? -1 : 1;
	uint16_t wait = trunc(1000000 / STEPPERMOTOR_STEPS_PER_SECOND);
	bool reversed = degrees < 0;

	// do the steps
	uint8_t current_pattern = 0;
	while (steps != 0)
	{
		// motors turn in opposite directions
		stepLeftMotor(steppingPatterns[current_pattern], reversed);
		stepRightMotor(steppingPatterns[current_pattern], !reversed);
		delayMicroseconds(wait);
		current_pattern ++;
		if (current_pattern > 7) current_pattern = 0;
		steps += increment;
	}

	// disable motors
	stepLeftMotor(0, reversed);
	stepRightMotor(0, reversed);
}

void EscornabotDIY::stepLeftMotor(uint8_t pattern, bool reversed)
{
	if (reversed) {
		digitalWrite(STEPPERMOTOR_L_IN4, bitRead(pattern, 0));
		digitalWrite(STEPPERMOTOR_L_IN3, bitRead(pattern, 1));
		digitalWrite(STEPPERMOTOR_L_IN2, bitRead(pattern, 2));
		digitalWrite(STEPPERMOTOR_L_IN1, bitRead(pattern, 3));
	} else {
		digitalWrite(STEPPERMOTOR_L_IN1, bitRead(pattern, 0));
		digitalWrite(STEPPERMOTOR_L_IN2, bitRead(pattern, 1));
		digitalWrite(STEPPERMOTOR_L_IN3, bitRead(pattern, 2));
		digitalWrite(STEPPERMOTOR_L_IN4, bitRead(pattern, 3));
	}
}

void EscornabotDIY::stepRightMotor(uint8_t pattern, bool reversed)
{
	if (reversed) {
		digitalWrite(STEPPERMOTOR_R_IN1, bitRead(pattern, 0));
		digitalWrite(STEPPERMOTOR_R_IN2, bitRead(pattern, 1));
		digitalWrite(STEPPERMOTOR_R_IN3, bitRead(pattern, 2));
		digitalWrite(STEPPERMOTOR_R_IN4, bitRead(pattern, 3));
	} else {
		digitalWrite(STEPPERMOTOR_R_IN4, bitRead(pattern, 0));
		digitalWrite(STEPPERMOTOR_R_IN3, bitRead(pattern, 1));
		digitalWrite(STEPPERMOTOR_R_IN2, bitRead(pattern, 2));
		digitalWrite(STEPPERMOTOR_R_IN1, bitRead(pattern, 3));
	}
}


////////////////////////////////////////
// LED

void EscornabotDIY::turnLED(uint8_t state)
{
	digitalWrite(SIMPLELED_PIN, state);
}

void EscornabotDIY::blinkLED(uint8_t times)
{
	while (times > 0)
	{
		digitalWrite(SIMPLELED_PIN, HIGH);
		delay(200);
		digitalWrite(SIMPLELED_PIN, LOW);
		delay(200);
		times--;
	}
}


////////////////////////////////////////
// Buzzer

void EscornabotDIY::beep(uint16_t frequency)
{
	tone(BUZZER_PIN, frequency, 100);
}

void EscornabotDIY::playNote(uint16_t frequency, uint16_t duration)
{
	tone(BUZZER_PIN, frequency, duration);
	delay(duration);
}


////////////////////////////////////////
// Keypad
void EscornabotDIY::configKeypad(
	uint8_t KeypadPin,
	int16_t KeypadValue_FW,
	int16_t KeypadValue_TL,
	int16_t KeypadValue_GO,
	int16_t KeypadValue_TR,
	int16_t KeypadValue_BW,
	int16_t KeypadValue_NN)
{
	// Keypad
	_keypad_pin = KeypadPin;
	pinMode(_keypad_pin, INPUT_PULLUP); // 2-wires
	_keypad_values[0] = KeypadValue_FW;
	_keypad_values[1] = KeypadValue_TL;
	_keypad_values[2] = KeypadValue_GO;
	_keypad_values[3] = KeypadValue_TR;
	_keypad_values[4] = KeypadValue_BW;
	_keypad_values[5] = KeypadValue_NN;
}

int16_t EscornabotDIY::readKeypad()
{
	return analogRead(_keypad_pin);
}

bool EscornabotDIY::isButtonPressed(String label)
{
	int8_t buttonPressed = scanButtons();
	Serial.println(buttonPressed);

	if (label == buttonsLabels[buttonPressed]) return true;

	return false;
}

int8_t EscornabotDIY::scanButtons()
{
	int8_t result, i;
	int16_t value, diff, minor_diff;

	// defaults
	minor_diff = 1023; // maximun
	result = BUTTONS_TOTAL - 1; // <- last button = NONE

	// check buttons
	value = readKeypad();
	Serial.println(value);
	for (i=0; i<BUTTONS_TOTAL; i++)
	{
		diff = abs(_keypad_values[i] - value);
		if (diff < minor_diff) {
			minor_diff = diff;
			result = i;
		}
	}

	return result;
}

////////////////////////////////////////
// Extra
void EscornabotDIY::fixReversed()
{
	_isReversed = true;
}

