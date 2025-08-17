#include "AS5048A.h"
#include "bldc120.h"

#define PIN_CS 17
#define PIN_SCLK 18
#define PIN_MOSI 19
#define PIN_MISO 16
#define PIN_U_H 22
#define PIN_V_H 21
#define PIN_W_H 20
#define PIN_U_L 23
#define PIN_V_L 24
#define PIN_W_L 25

#define ELECTRICAL_CYCLE 7 // 電気サイクル数

int energize[][2] = {
    {PIN_U_H, PIN_W_L},
    {PIN_V_H, PIN_W_L},
    {PIN_V_H, PIN_U_L},
    {PIN_W_H, PIN_U_L},
    {PIN_W_H, PIN_V_L},
    {PIN_U_H, PIN_V_L},
};

// エンコーダ
AS5048A encoder(PIN_CS);
float angle = 0;
int mode;

// BLDC制御
BLDC120 bldc(PIN_U_H, PIN_V_H, PIN_W_H, PIN_U_L, PIN_V_L, PIN_W_L, ELECTRICAL_CYCLE);

// =================================================

void setup()
{
  // シリアル通信の初期化
  Serial.begin(115200);

  // PWMの初期化
  analogWriteFreq(25000);
  analogWriteRange(4096);
  analogReadResolution(12);

  // SPI通信の初期化
  SPI.setSCK(PIN_SCLK);
  SPI.setTX(PIN_MOSI);
  SPI.setRX(PIN_MISO);
  encoder.begin();
}

void loop()
{
  if (micros() % 100 == 0)
  {
    int angle_digit = encoder.getAngleDigit();             // 機械角の取得
    angle_digit = bldc.calcAngledigit(angle_digit, -300);  // 機械角と電気角のゼロ点ズレの補正
    int angle_ele_digit = (angle_digit * 7) % 16384;       // 電気角に換算
    float angle_ele = angle_ele_digit * 360.0 / 16384.0;   // 物理量に換算
    float angle_pow = bldc.calcAngleDegrees(angle_ele, 0); // 進角
    angle = angle_ele;

    if (angle_pow < 30.0)
      mode = 1;
    else if (angle_pow < 90.0)
      mode = 2;
    else if (angle_pow < 150.0)
      mode = 3;
    else if (angle_pow < 210.0)
      mode = 4;
    else if (angle_pow < 270.0)
      mode = 5;
    else if (angle_pow < 330.0)
      mode = 0;
    else if (angle_pow < 360.0)
      mode = 1;
    updateFET(energize[mode][0], energize[mode][1]);
    // updatePwm(100.0, -50.0, -50.0);
  }
}

void loop1()
{
  Serial.println(String(angle) + ", " + String(mode));
  delay(100);
}

// =================================================

void updateFET(int high, int low)
{
  digitalWrite(PIN_U_H, LOW);
  digitalWrite(PIN_V_H, LOW);
  digitalWrite(PIN_W_H, LOW);
  digitalWrite(PIN_U_L, LOW);
  digitalWrite(PIN_V_L, LOW);
  digitalWrite(PIN_W_L, LOW);

  digitalWrite(high, HIGH);
  digitalWrite(low, HIGH);
}

void updatePwm(float u, float v, float w)
{
  int u_d, v_d, w_d;

  u_d = static_cast<int>(u * 16384.0 / 100);
  if (u_d > 0)
  {
    analogWrite(PIN_U_H, u_d);
  }
  else
  {
    analogWrite(PIN_U_L, -1 * u_d);
  }

  v_d = static_cast<int>(v * 16384.0 / 100);
  if (v_d > 0)
  {
    analogWrite(PIN_V_H, v_d);
  }
  else
  {
    analogWrite(PIN_V_L, -1 * v_d);
  }

  w_d = static_cast<int>(w * 16384.0 / 100);
  if (w_d > 0)
  {
    analogWrite(PIN_W_H, w_d);
  }
  else
  {
    analogWrite(PIN_W_L, -1 * w_d);
  }
}