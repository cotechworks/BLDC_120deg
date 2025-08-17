#include "bldc120.h"

BLDC120::BLDC120(int u_h, int v_h, int w_h, int u_l, int v_l, int w_l, int electrical_cycle)
{
    this->energize[0][0] = u_h;
    this->energize[0][1] = w_l;
    this->energize[1][0] = v_h;
    this->energize[1][1] = w_l;
    this->energize[2][0] = v_h;
    this->energize[2][1] = u_l;
    this->energize[3][0] = w_h;
    this->energize[3][1] = u_l;
    this->energize[4][0] = w_h;
    this->energize[4][1] = v_l;
    this->energize[5][0] = u_h;
    this->energize[5][1] = v_l;

    pinMode(u_h, OUTPUT);
    pinMode(v_h, OUTPUT);
    pinMode(w_h, OUTPUT);
    pinMode(u_l, OUTPUT);
    pinMode(v_l, OUTPUT);
    pinMode(w_l, OUTPUT);

    digitalWrite(u_h, LOW);
    digitalWrite(v_h, LOW);
    digitalWrite(w_h, LOW);
    digitalWrite(u_l, LOW);
    digitalWrite(v_l, LOW);
    digitalWrite(w_l, LOW);

    this->electrical_cycle = electrical_cycle;
}

BLDC120::~BLDC120()
{
    // Destructor implementation
}

float BLDC120::calcAngleDegrees(float angle, float change_val)
{
    angle += change_val;
    if (angle >= 360.0)
        angle -= 360.0;
    else if (angle < 0.0)
        angle += 360.0;

    return angle;
}

int BLDC120::calcAngledigit(int angle, int change_val)
{
    angle += change_val;
    if (angle >= 16384)
        angle -= 16384;
    else if (angle < 0)
        angle += 16384;

    return angle;
}

void BLDC120::updatePhase(float angle)
{
    float angle_ele = fmot(angle * electrical_cycle, 360.0); // 電気角に換算
    float angle_pow = fmod(angle, 360.0);
    if (angle_pow < 30.0)
        mode = 0;
    else if (angle_pow < 90.0)
        mode = 1;
    else if (angle_pow < 150.0)
        mode = 2;
    else if (angle_pow < 210.0)
        mode = 3;
    else if (angle_pow < 270.0)
        mode = 4;
    else if (angle_pow < 330.0)
        mode = 5;
    else
        mode = 0;

    updateFET(energize[mode][0], energize[mode][1]);
}