#include "AS5048A.h"

static const uint16_t AS5048A_CLEAR_ERROR_FLAG = 0x0001;
static const uint16_t AS5048A_PROGRAMMING_CONTROL = 0x0003;
static const uint16_t AS5048A_OTP_REGISTER_ZERO_POS_HIGH = 0x0016;
static const uint16_t AS5048A_OTP_REGISTER_ZERO_POS_LOW = 0x0017;
static const uint16_t AS5048A_DIAG_AGC = 0x3FFD;
static const uint16_t AS5048A_MAGNITUDE = 0x3FFE;
static const uint16_t AS5048A_ANGLE = 0x3FFF;

static const uint8_t AS5048A_AGC_FLAG = 0xFF;
static const uint8_t AS5048A_ERROR_PARITY_FLAG = 0x04;
static const uint8_t AS5048A_ERROR_COMMAND_INVALID_FLAG = 0x02;
static const uint8_t AS5048A_ERROR_FRAMING_FLAG = 0x01;

static const uint16_t AS5048A_DIAG_COMP_HIGH = 0x2000;
static const uint16_t AS5048A_DIAG_COMP_LOW = 0x1000;
static const uint16_t AS5048A_DIAG_COF = 0x0800;
static const uint16_t AS5048A_DIAG_OCF = 0x0400;

AS5048A::AS5048A(byte cs, bool debug /*=false*/) {
  this->_cs = cs;
}

void AS5048A::begin() {
  this->settings = SPISettings(3000000, MSBFIRST, SPI_MODE1);
  pinMode(this->_cs, OUTPUT);
  digitalWrite(this->_cs, HIGH);
  SPI.begin();
}

uint8_t AS5048A::calcEvenParity(uint16_t value) {
  uint8_t cnt = 0;

  for (uint8_t i = 0; i < 16; i++) {
    if (value & 0x1) {
      cnt++;
    }
    value >>= 1;
  }
  return cnt & 0x1;
}

uint16_t AS5048A::readRegister(uint16_t address) {
  // コマンドの作成
  uint16_t command = 0x4000;
  command = command | address;
  command |= static_cast<uint16_t>(calcEvenParity(command) << 0xF);

  // SPI通信開始
  SPI.beginTransaction(this->settings);

  // コマンド送信
  digitalWrite(this->_cs, LOW);
  SPI.transfer16(command);
  digitalWrite(this->_cs, HIGH);
  delay(5);

  // データ受信
  digitalWrite(this->_cs, LOW);
  uint16_t response = SPI.transfer16(0x00);
  digitalWrite(this->_cs, HIGH);

  // SPI通信終了
  SPI.endTransaction();

  // エラー確認
  if (response & 0x4000) {
    Serial.println("Setting error bit: " + getErrors());
    return 0;
  }

  // エラービットとパリティを省いて返す
  return response & ~0xC000;
}

uint16_t AS5048A::getAngleDigit() {
  return AS5048A::readRegister(AS5048A_ANGLE);
}

float AS5048A::getAngleDegrees() {
  return static_cast<float>(AS5048A::getAngleDigit()) * 360.0 / 16384.0;
}

float AS5048A::getAngleRadians() {
  return static_cast<float>(AS5048A::getAngleDigit()) * 2.0 * PI / 16384.0;
}

String AS5048A::getErrors() {
  uint16_t error = AS5048A::readRegister(AS5048A_CLEAR_ERROR_FLAG);
  if (error & AS5048A_ERROR_PARITY_FLAG)
	{
		return "Parity Error";
	}
	if (error & AS5048A_ERROR_COMMAND_INVALID_FLAG)
	{
		return "Command invalid";
	}
	if (error & AS5048A_ERROR_FRAMING_FLAG)
	{
		return "Framing error";
	}
	return "No Error";
}
