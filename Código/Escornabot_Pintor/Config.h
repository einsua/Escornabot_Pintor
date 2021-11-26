// Escornabot geometry
#define WHEEL_RADIUS 37.0 //mm

// Stepper motors
#define STEPPERMOTOR_R_IN1 5
#define STEPPERMOTOR_R_IN2 4
#define STEPPERMOTOR_R_IN3 3
#define STEPPERMOTOR_R_IN4 2
#define STEPPERMOTOR_L_IN1 9
#define STEPPERMOTOR_L_IN2 8
#define STEPPERMOTOR_L_IN3 7
#define STEPPERMOTOR_L_IN4 6
#define STEPPERMOTOR_STEPS_PER_SECOND 1000 // speed

// Buzzer
#define BUZZER_PIN 10
#define NOTE_BEEP 4699      // D8
#define NOTE_FORWARD 2637   // E7
#define NOTE_TURNLEFT 2217  // C#7
#define NOTE_TURNRIGHT 4434 // C#8
#define NOTE_BACKWARD 3520  // A7

// Keypad
// https://github.com/mgesteiro/escornakeypad
#define KEYPAD_PIN A4
#define KEYVALUE_FORWARD 230
#define KEYVALUE_TURNLEFT 368
#define KEYVALUE_GO 586
#define KEYVALUE_TURNRIGHT 532
#define KEYVALUE_BACKWARD 462
#define KEYVALUE_NONE 1002

// Led
#define SIMPLELED_PIN 13
