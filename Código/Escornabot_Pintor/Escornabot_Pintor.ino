#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "EscornabotDIY.h"
#include <Servo.h>

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
double dist;
double ang;
double dist_eje_punta;
double ang_abajo;
double ang_arriba;
double espera;
Servo servo_11;
EscornabotDIY escornabot;



void setup(){
    dist = 10;
    ang = 30;
    dist_eje_punta = 6;
    ang_abajo = 0;
    ang_arriba = 40;
    espera = 0.25;
    servo_11.attach(11); // init pin
    servo_11.write(ang_arriba); // write to servo
    _delay(espera);
    
}

void loop(){
    
    if(escornabot.isButtonPressed("FORWARD")){
        escornabot.beep(2637);
        servo_11.write(ang_abajo); // write to servo
        _delay(espera);
        escornabot.move(dist);
    }
    if(escornabot.isButtonPressed("BACKWARD")){
        escornabot.beep(3520);
        servo_11.write(ang_abajo); // write to servo
        _delay(espera);
        escornabot.move(-dist);
    }
    if(escornabot.isButtonPressed("TURN LEFT")){
        escornabot.beep(2217);
        servo_11.write(ang_arriba); // write to servo
        _delay(espera);
        escornabot.move(-dist_eje_punta);
        escornabot.turn(-ang);
        escornabot.move(dist_eje_punta);
        servo_11.write(ang_abajo); // write to servo
        _delay(espera);
    }
    if(escornabot.isButtonPressed("TURN RIGHT")){
        escornabot.beep(4434);
        servo_11.write(ang_arriba); // write to servo
        _delay(espera);
        escornabot.move(-dist_eje_punta);
        escornabot.turn(ang);
        escornabot.move(dist_eje_punta);
        servo_11.write(ang_abajo); // write to servo
        _delay(espera);
    }
    if(escornabot.isButtonPressed("GO")){
        escornabot.beep(4699);
        servo_11.write(ang_arriba); // write to servo
        _delay(espera);
    }
    
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
    
}

