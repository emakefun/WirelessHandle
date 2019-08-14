#include "Hummerbot.h"
#include "BluetoothHandle.h"
#include "ProtocolParser.h"
#include "KeyMap.h"
#include "debug.h"

#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN3_PIN 5   // DIRA  ---  left
#define IN4_PIN 9   // PWMA

ProtocolParser *mProtocol = new ProtocolParser();
Hummerbot hbot(mProtocol, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);

void setup()
{
  Serial.begin(9600);
  hbot.init();
  hbot.SetControlMode(E_NRF24L01_CONTROL);
  hbot.SetNrf24L01Pin();
  hbot.mRgbUltrasonic->SetServoDegree(90);
  hbot.SetSpeed(0);
}

void HandleNrf24L01Remote()
{
  byte value[12] = {0};
  bool godata = false;
  if ( hbot.mNrf24L01->dataReady()) { //When the program is received, the received data is output from the serial port
    hbot.mNrf24L01->getData(value);
    for (int i = 0; i < 12; i++) {
      Serial.print(value[i], HEX); Serial.print(" ");
    }
    Serial.println("");
    mProtocol->RecevData(value, 12);
  }
  if (mProtocol->ParserPackage())
  {
    Serial.println("test");
    switch (mProtocol->GetRobotControlFun()) {
      case E_ROBOT_CONTROL_DIRECTION:
        DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ROBOT_CONTROL_DIRECTION \n");
        hbot.Drive(mProtocol->GetRobotDegree());
        break;
      case E_ROBOT_CONTROL_SPEED:
        DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ROBOT_CONTROL_SPEED \n");
        hbot.SetSpeed(mProtocol->GetRobotSpeed());
        break ;
     case E_BUTTON:
        Serial.println("button: ");
        Serial.println(mProtocol->GetBluetoothButton());   
        switch (mProtocol->GetBluetoothButton())
        {
              case BT_PAD_UP:
 
              break;
        }
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
  switch (hbot.GetControlMode())
  {
    case E_NRF24L01_CONTROL:
      HandleNrf24L01Remote();
      // DEBUG_LOG(DEBUG_LEVEL_INFO, "E_NRF24L01_CONTROL \n");
      break;
    default:
      break;
  }
}
