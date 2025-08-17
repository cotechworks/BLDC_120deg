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

const int ELECTRICAL_CYCLE = 7;   // 電気サイクル数
const float ADVANCE_TIMING = -1.0; // 進角値

// エンコーダ
AS5048A encoder(PIN_CS);
float angle = 0;

// BLDC制御
BLDC120 bldc(PIN_U_H, PIN_V_H, PIN_W_H, PIN_U_L, PIN_V_L, PIN_W_L, ELECTRICAL_CYCLE, ADVANCE_TIMING);

// =================================================

void setup()
{
  // シリアル通信の初期化
  Serial.begin(115200);

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
    angle = encoder.getAngleDegrees();    // 機械角の取得
    angle = bldc.calcAngle(angle, -6.59); // 機械角と電気角のゼロ点ズレの補正
    bldc.updatePhase(angle);              // 通電状態の更新
  }
}

void loop1()
{
  Serial.println(String(angle));
  delay(100);
}