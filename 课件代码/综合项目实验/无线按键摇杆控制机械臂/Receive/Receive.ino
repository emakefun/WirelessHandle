#include "Emakefun_MotorDriver.h"
#include "ProtocolParser.h"
#include "debug.h"
Emakefun_MotorDriver mMotorDriver = Emakefun_MotorDriver();
Emakefun_Sensor *Nrf;
Emakefun_Servo *mServo1 = mMotorDriver.getServo(1);
Emakefun_Servo *mServo2 = mMotorDriver.getServo(2);
Emakefun_Servo *mServo3 = mMotorDriver.getServo(3);
Emakefun_Servo *mServo4 = mMotorDriver.getServo(4);
Emakefun_Sensor *buzzer = mMotorDriver.getSensor(E_BUZZER_MODE);
ProtocolParser *mProtocol = new ProtocolParser();

static int RightServo = 90;
static int LeftServo = 90;
static int PliersServo = 90;
static byte TurnServo = 90;

void setup()
{
  Serial.begin(9600);
  Nrf = mMotorDriver.getSensor(E_NRF24L01);
  mMotorDriver.SetControlMode(E_NRF24L01_CONTROL);//E_BLUETOOTH_CONTROL//E_NRF24L01_CONTROL
  Nrf->GetNrf24L01((char *)"WirelessHandle");
  mMotorDriver.begin(50);
  delay(2000);
}


void BLEControl(bool recv_flag)
{
  if (recv_flag) {
    switch (mProtocol->GetRobotControlFun()) {
      case E_ROBOT_CONTROL_DIRECTION:
        {
          byte angle = mProtocol->GetRobotDegree();
          if ((90 < angle) && (angle <= 270))
          {
            if (TurnServo < 180) {
              TurnServo += 3;
            } else {
              TurnServo = 180;
            }
          } else if ((270 < angle <= 360) || (0 <= angle <= 90)) {
            if (TurnServo != 0) {
              TurnServo -= 3;
            } else {
              TurnServo = 0;
            }
          }
          mServo1->writeServo(TurnServo);
        }
        break;
      case E_BUTTON:
        switch (mProtocol->GetBluetoothButton())
        {
          case BT_PAD_UP:
            if ((RightServo < 140)) {
              RightServo += 3;
            }
            break;
          case BT_PAD_DOWN:
            if (((RightServo != 0) && (100 < LeftServo)) && (RightServo > 60)) {
              RightServo -= 3;
            } else if (RightServo == 0) {
              RightServo = 0;
            } else {
              buzzer->Sing(S_connection);
            }
            break;
          case BT_PAD_LEFT:
            if (LeftServo < 180) {
              LeftServo += 3;
            } else {
              LeftServo = 180;
            }
            break;
          case BT_PAD_RIGHT:
            if (LeftServo >= 92) {
              LeftServo -= 3;
            } else {
              LeftServo = 90;
            }
            break;
          case BT_L:
            if (PliersServo < 145) {
              PliersServo += 5;
            }
            break;
          case BT_R:
            if (PliersServo > 60) {
              PliersServo -= 5;
            }
            break;
        }
        mServo2->writeServo(RightServo);
        mServo3->writeServo(LeftServo);
        mServo4->writeServo(PliersServo);
        break;
      default:
        break;
    }
  }
}

void NRF24L01Control(bool recv_flag)
{
  byte value[12] = {0};
  if (Nrf->mNRF24L01->dataReady())
  {
    Nrf->mNRF24L01->getData(value);
        for (int i = 0; i < 12; i++) {
          Serial.print(value[i], HEX); Serial.print(" ");
        }
        Serial.println("");
    mProtocol->RecevData(value, 12);
  }
  if (recv_flag) {
    switch (mProtocol->GetRobotControlFun()) {
      case E_ROBOT_CONTROL_DIRECTION:
        {
          byte angle = mProtocol->GetRobotDegree();
          if ((90 < angle) && (angle <= 270))
          {
            if (TurnServo < 180) {
              TurnServo += 2;
            } else {
              TurnServo = 180;
            }
          } else if ((270 < angle <= 360) || (0 <= angle <= 90)) {
            if (TurnServo != 0) {
              TurnServo -= 2;
            } else {
              TurnServo = 0;
            }
          }
          mServo1->writeServo(TurnServo);
        }
        break;
      case E_BUTTON:
        switch (mProtocol->GetBluetoothButton())
        {
          case BT_PAD_UP:
            if ((RightServo < 140)) {
              RightServo += 3;
            }
            break;
          case BT_PAD_DOWN:
            if (((RightServo != 0) && (100 < LeftServo)) && (RightServo > 60)) {
              RightServo -= 3;
            } else if (RightServo == 0) {
              RightServo = 0;
            } else {
              buzzer->Sing(S_connection);
            }
            break;
          case BT_PAD_LEFT:
            if (LeftServo < 180) {
              LeftServo += 4;
            } else {
              LeftServo = 180;
            }
            break;
          case BT_PAD_RIGHT:
            if (LeftServo >= 92) {
              LeftServo -= 4;
            } else {
              LeftServo = 90;
            }
            break;
          case BT_L:
            if (PliersServo < 145) {
              PliersServo += 5;
            }
            break;
          case BT_R:
            if (PliersServo > 60) {
              PliersServo -= 5;
            }
            break;
        }
        mServo2->writeServo(RightServo);
        mServo3->writeServo(LeftServo);
        mServo4->writeServo(PliersServo);
        break;
      default:
        break;
    }
  }
}


void loop()
{
  static byte mode;
  static bool recv_flag;
  mProtocol->RecevData();
  if (recv_flag = mProtocol->ParserPackage()) {
    if (mProtocol->GetRobotControlFun() == E_CONTROL_MODE) {
      mMotorDriver.SetControlMode(mProtocol->GetControlMode());
      return;
    }
  }
  switch (mMotorDriver.GetSendMode()) {
    case  E_NRF24L01_CONTROL:
      NRF24L01Control(recv_flag);
      break;
    case E_BLUETOOTH_CONTROL:
      BLEControl(recv_flag);
      break;
  }
}
