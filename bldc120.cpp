#include "bldc120.h"

/**
 * @brief BLDC120クラスのコンストラクタ
 * @param u_h U相のハイサイドピン
 * @param v_h V相のハイサイドピン
 * @param w_h W相のハイサイドピン
 * @param u_l U相のローサイドピン
 * @param v_l V相のローサイドピン
 * @param w_l W相のローサイドピン
 * @param electrical_cycle 電気サイクル数
 * @param advance_timing 進角値（デフォルトは0.0）
 */
BLDC120::BLDC120(int u_h, int v_h, int w_h, int u_l, int v_l, int w_l, int electrical_cycle, float advance_timing)
{
    this->u_h = u_h;
    this->v_h = v_h;
    this->w_h = w_h;
    this->u_l = u_l;
    this->v_l = v_l;
    this->w_l = w_l;

    this->energize[0][0] = this->u_h;
    this->energize[0][1] = this->w_l;
    this->energize[1][0] = this->v_h;
    this->energize[1][1] = this->w_l;
    this->energize[2][0] = this->v_h;
    this->energize[2][1] = this->u_l;
    this->energize[3][0] = this->w_h;
    this->energize[3][1] = this->u_l;
    this->energize[4][0] = this->w_h;
    this->energize[4][1] = this->v_l;
    this->energize[5][0] = this->u_h;
    this->energize[5][1] = this->v_l;

    pinMode(this->u_h, OUTPUT);
    pinMode(this->v_h, OUTPUT);
    pinMode(this->w_h, OUTPUT);
    pinMode(this->u_l, OUTPUT);
    pinMode(this->v_l, OUTPUT);
    pinMode(this->w_l, OUTPUT);

    digitalWrite(this->u_h, LOW);
    digitalWrite(this->v_h, LOW);
    digitalWrite(this->w_h, LOW);
    digitalWrite(this->u_l, LOW);
    digitalWrite(this->v_l, LOW);
    digitalWrite(this->w_l, LOW);

    this->electrical_cycle = electrical_cycle;
    this->advance_timing = advance_timing;
}

/**
 * @brief BLDC120クラスのデストラクタ
 * ピンをLOWに設定して通電を停止します。
 */
BLDC120::~BLDC120()
{
    digitalWrite(u_h, LOW);
    digitalWrite(v_h, LOW);
    digitalWrite(w_h, LOW);
    digitalWrite(u_l, LOW);
    digitalWrite(v_l, LOW);
    digitalWrite(w_l, LOW);
}

/**
 * @brief 角度を計算する関数
 * @param angle 現在の角度
 * @param change_val 変更値（進角など）
 * @return 更新された角度
 */
float BLDC120::calcAngle(float angle, float change_val)
{
    angle += change_val;
    if (angle >= 360.0)
        angle -= 360.0;
    else if (angle < 0.0)
        angle += 360.0;

    return angle;
}

/**
 * @brief フェーズを更新する関数
 * @param angle 電気角
 */
void BLDC120::updatePhase(float angle)
{
    int phase = 0;
    float angle_ele = fmod(angle * electrical_cycle, 360.0);   // 電気角に換算
    float angle_pow = calcAngle(angle_ele, advance_timing); // 電気角に進角を加える

    // 電気角に基づいてフェーズを決定
    if (angle_pow < 30.0)
        phase = 0;
    else if (angle_pow < 90.0)
        phase = 1;
    else if (angle_pow < 150.0)
        phase = 2;
    else if (angle_pow < 210.0)
        phase = 3;
    else if (angle_pow < 270.0)
        phase = 4;
    else if (angle_pow < 330.0)
        phase = 5;
    else
        phase = 0;

    digitalWrite(u_h, LOW);
    digitalWrite(v_h, LOW);
    digitalWrite(w_h, LOW);
    digitalWrite(u_l, LOW);
    digitalWrite(v_l, LOW);
    digitalWrite(w_l, LOW);

    digitalWrite(energize[phase][0], HIGH);
    digitalWrite(energize[phase][1], HIGH);
}