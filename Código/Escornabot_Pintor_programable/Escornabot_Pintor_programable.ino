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
double ang_arriba;
double ang_abajo;
double nmov_max;
double espera;
double go_pulsado;
double contador;
double i;
Servo servo_11;
EscornabotDIY escornabot;
char escornabot_program[100];



void setup(){
    servo_11.attach(11);dist = 10;
    ang = 90;
    dist_eje_punta = 6;
    ang_arriba = 40;
    ang_abajo = 5;
    nmov_max = 50;
    espera = 0.2;
    servo_11.write(ang_arriba);
    escornabot.beep(4699);
    
}

void loop(){
    
    go_pulsado = 0;
    contador = 0;
    while(!(((go_pulsado) > (0)) || ((contador) > (nmov_max))))
    {
        _loop();
        if(escornabot.isButtonPressed("FORWARD")){
            escornabot_program[int(contador)] = 1;
            contador += 1;
            escornabot.beep(2637);
            _delay(espera);
        }
        if(escornabot.isButtonPressed("TURN LEFT")){
            escornabot_program[int(contador)] = 2;
            contador += 1;
            escornabot.beep(2217);
            _delay(espera);
        }
        if(escornabot.isButtonPressed("GO")){
            go_pulsado = 1;
            escornabot.beep(4699);
            _delay(espera);
        }
        if(escornabot.isButtonPressed("TURN RIGHT")){
            escornabot_program[int(contador)] = 3;
            contador += 1;
            escornabot.beep(4434);
            _delay(espera);
        }
        if(escornabot.isButtonPressed("BACKWARD")){
            escornabot_program[int(contador)] = 4;
            contador += 1;
            escornabot.beep(3520);
            _delay(espera);
        }
    }
    i = 0;
    for(int __i__=0;__i__<contador;++__i__)
    {
        if((escornabot_program[int(i)]==1)){
            escornabot.beep(2637);
            servo_11.write(ang_abajo);
            _delay(espera);
            escornabot.move(dist);
            servo_11.write(ang_arriba);
            _delay(espera);
        }
        if((escornabot_program[int(i)]==2)){
            escornabot.beep(2217);
            servo_11.write(ang_arriba);
            _delay(espera);
            escornabot.move(-dist_eje_punta);
            escornabot.turn(-ang);
            escornabot.move(dist_eje_punta);
        }
        if((escornabot_program[int(i)]==3)){
            escornabot.beep(4434);
            servo_11.write(ang_arriba);
            _delay(espera);
            escornabot.move(-dist_eje_punta);
            escornabot.turn(ang);
            escornabot.move(dist_eje_punta);
        }
        if((escornabot_program[int(i)]==4)){
            escornabot.beep(3520);
            servo_11.write(ang_abajo);
            _delay(espera);
            escornabot.move(-dist);
            servo_11.write(ang_abajo);
            _delay(espera);
        }
        i += 1;
    }
    _delay(0.25);
    escornabot.playNote(1568, 125);
    escornabot.playNote(1175, 250);
    
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
    
}

