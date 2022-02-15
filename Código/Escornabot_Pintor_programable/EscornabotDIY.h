#include <Arduino.h>
#include <stdint.h>
#include "Config.h"

// calculations
// Steppers have a gear ratio of 1:63.7, with 32 steps per turn, using half-stepping -> 63.7 * 32 * 2 = 4076.8
#define STEPPERMOTOR_FULLREVOLUTION_STEPS 4076.8f //number of steps for a full revolution of the axis
#define WHEEL_CIRCUNFERENCE float(PI * WHEEL_DIAMETER)
#define ROTATION_CIRCUNFERENCE float(PI * WHEEL_DISTANCE) // rotation circunference determined by the wheel separation
#define STEPPERS_STEPS_MM float(STEPPERMOTOR_FULLREVOLUTION_STEPS / WHEEL_CIRCUNFERENCE) // how many steps to move 1 mm
#define STEPPERS_STEPS_DEG float((ROTATION_CIRCUNFERENCE/360) * STEPPERS_STEPS_MM) // how many steps to rotate the robot 1 degree

const uint8_t numPatterns = 8;
const uint8_t steppingPatterns[8] = {
	B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001
};

#define BUTTONS_TOTAL 6 // including "NONE" button
const String buttonsLabels[BUTTONS_TOTAL] = {
	"FORWARD", "TURN LEFT", "GO", "TURN RIGHT", "BACKWARD", "NONE"
};

class EscornabotDIY {
public:
	EscornabotDIY();
	virtual ~EscornabotDIY();
	void configKeypad(
		uint8_t KeypadPin,
		int16_t KeypadValue_FW,
		int16_t KeypadValue_TL,
		int16_t KeypadValue_GO,
		int16_t KeypadValue_TR,
		int16_t KeypadValue_BW,
		int16_t KeypadValue_NN);

	// Stepper motors
	void move(float cms);
	void turn(float degrees);

	// LED
	void turnLED(uint8_t state);
	void blinkLED(uint8_t times);

	// Buzzer
	void beep(uint16_t frequency);
	void playNote(uint16_t frequency, uint16_t duration);

	// Keypad
	bool isButtonPressed(String button);
	int16_t readKeypad();

	// Extra
	void fixReversed();


private:
	// Stepper motors
	void stepLeftMotor(uint8_t pattern, bool reversed);
	void stepRightMotor(uint8_t pattern, bool reversed);

	// Buzzer
	uint8_t _buzzer_pin;

	// Keypad
	int16_t _keypad_values[BUTTONS_TOTAL];
	uint8_t _keypad_pin;
	int8_t scanButtons();

	bool _isReversed = false;

};
