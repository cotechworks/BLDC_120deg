#pragma once

#include <Arduino.h>

class BLDC120
{
private:
    int energize[6][2];
    int electrical_cycle;

public:
    BLDC120(int u_h, int v_h, int w_h, int u_l, int v_l, int w_l, int electrical_cycle);
    ~BLDC120();
    float calcAngleDegrees(float angle, float change_val);
    int calcAngledigit(int angle, int change_val);
    void updatePhase(float angle);
};