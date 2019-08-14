#ifndef _Emakefun_MotorDriver_h_
#define _Emakefun_MotorDriver_h_

#include <inttypes.h>
#include <Wire.h>
#include "SPI.h"
#include "Emakefun_MS_PWMServoDriver.h"
#include "IRremote.h"
#include "PS2X_lib.h"  //for v1.6
#include "Buzzer.h"
#include "RGBLed.h"
#include "Sounds.h"
#include "nRF24L01.h"
#include "ProtocolParser.h"
//#define MOTORDEBUG

#define MICROSTEPS 16         // 8 or 16

#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4

#define RGB_PIN A1
#define ECHO_PIN A3
#define TRIG_PIN A2
#define IR_PIN 8
#define BUZZER_PIN 9

#define PS2_DAT   12
#define PS2_CMD   11
#define PS2_SEL   10
#define PS2_CLK   13

#define NRF24L01_CE 10
#define NRF24L01_CSN 8

#define NRF_NAME "emakefun"
#define NRF_DATA_LEN 12

#define BT_RB          0b100000001
#define BT_MODE        0b100001000
#define BT_SELECT      0b00000001
#define BT_L           0b10000000
#define BT_R           0b01000000
#define BT_PAD_DOWN    0b00100000
#define BT_PAD_LEFT    0b00010000
#define BT_PAD_UP      0b00001000
#define BT_PAD_RIGHT   0b00000100

typedef enum
{
    E_RGB_ALL = 0,
    E_RGB_RIGHT = 1,
    E_RGB_LEFT = 2
} E_RGB_INDEX;

typedef enum
{
    E_RGB = 0,
    E_IR = 1,
    E_BUZZER_MODE = 2,
    E_PS2X = 3,
    E_NRF24L01 = 4,
	  E_ULTRASONIC_MODE = 5,
    E_EXTERN_PIN = 6,
    E_SENSOR_MAX,
} E_SENSOR_INDEX;

class Emakefun_MotorDriver;

class Emakefun_Sensor
{
public:
  Emakefun_Sensor(void);
  friend class Emakefun_MotorDriver;
  IRremote  *mIrRecv;
  PS2X *mPs2x;
  Buzzer *mBuzzer;
  RGBLed *mRgb;
  Nrf24l *mNRF24L01;
  void SetRgbColor(E_RGB_INDEX index, long Color);
  void Sing(byte songName);
  uint16_t GetUltrasonicDistance(void);
  byte  GetNrf24L01(char *RaddrName);
  void sendNrf24l01(char *TaddrName,int SendNrfData);
private:
  uint8_t IrPin;      // Infrared remoter pin
  uint8_t BuzzerPin;  // Buzzer pin
  uint8_t RgbPin;     // Rgb pin
  uint8_t EchoPin,TrigPin;
  uint8_t Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin;    // for Ps2 remoter
  uint8_t P1, P2, P3, P4, P5;    // for Infrared tracing pin
  int GetNrfData,SendNrfData;
  Emakefun_MotorDriver *MC;
};

class Emakefun_DCMotor
{
 public:
  Emakefun_DCMotor(void);
  friend class Emakefun_MotorDriver;
  void run(uint8_t);
  void setSpeed(uint8_t);
  
 private:
  uint8_t PWMpin, IN1pin, IN2pin;
  Emakefun_MotorDriver *MC;
  uint8_t motornum;
};

class Emakefun_EncoderMotor {
 public:
  Emakefun_EncoderMotor(void);
  friend class Emakefun_MotorDriver;
  void run(uint8_t);
  void setSpeed(uint8_t);
  void release(void);

 private:
  uint8_t IN1pin, IN2pin, PWMpin , ENCODER1pin, ENCODER2pin;
  uint8_t pluse;
  Emakefun_MotorDriver *MC;
  uint8_t encodernum;
};

class Emakefun_StepperMotor {
 public:
  Emakefun_StepperMotor(void);
  friend class Emakefun_MotorDriver;
  void step(uint16_t steps, uint8_t dir,  uint8_t style = SINGLE);
  void setSpeed(uint16_t);
  uint8_t onestep(uint8_t dir, uint8_t style);
  void release(void);
  uint32_t usperstep;

 private:
  uint8_t PWMApin, AIN1pin, AIN2pin;
  uint8_t PWMBpin, BIN1pin, BIN2pin;
  uint16_t revsteps; // # steps per revolution
  uint8_t currentstep;
  Emakefun_MotorDriver *MC;
  uint8_t steppernum;
};

class Emakefun_Servo
{
 public:
  Emakefun_Servo(void);
  friend class Emakefun_MotorDriver;
  void setServoPulse(double pulse);
  void writeServo(uint8_t angle);
  uint8_t readDegrees();
 
 private:
  uint8_t PWMpin;
  Emakefun_MotorDriver *MC;
  uint8_t servonum,currentAngle;
};

class Emakefun_MotorDriver
{
 public:
    Emakefun_MotorDriver(uint8_t addr = 0x60);
    friend class Emakefun_DCMotor;
    void begin(uint16_t freq = 1600);
    void setPWM(uint8_t pin, uint16_t val);
    void setPin(uint8_t pin, boolean val);
    Emakefun_DCMotor *getMotor(uint8_t n);
    Emakefun_StepperMotor *getStepper(uint16_t steps, uint8_t n);
    Emakefun_EncoderMotor *getEncoderMotor(uint8_t num);
    Emakefun_Servo *getServo(uint8_t n);
    Emakefun_Sensor *getSensor(E_SENSOR_INDEX n);
    void SetControlMode(E_SMARTCAR_CONTROL_MODE mode);
    E_SMARTCAR_CONTROL_MODE GetSendMode(void);
  private:
    uint8_t _addr;
    uint16_t _freq;
    Emakefun_DCMotor dcmotors[4];
    Emakefun_EncoderMotor encoder[2];
    Emakefun_StepperMotor steppers[2];
    Emakefun_MS_PWMServoDriver _pwm;
    Emakefun_Servo servos[4];
    Emakefun_Sensor sensors[E_SENSOR_MAX];
    E_SMARTCAR_CONTROL_MODE mSendMode;
};

#endif
