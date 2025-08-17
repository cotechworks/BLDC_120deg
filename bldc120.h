#pragma once

#include <Arduino.h>

class BLDC120
{
private:
    int energize[6][2];
    int electrical_cycle;
    int u_h, v_h, w_h, u_l, v_l, w_l;
    float advance_timing;

public:
    BLDC120(int u_h, int v_h, int w_h, int u_l, int v_l, int w_l, int electrical_cycle, float advance_timing = 0.0);
    ~BLDC120();
    float calcAngle(float angle, float change_val);
    void updatePhase(float angle);
};