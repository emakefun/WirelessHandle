#include "WirelessHandle.h"
#include "ProtocolParser.h"
#include "debug.h"

WirelessHandle mHandle(E_BLUTOOTH_MODE);
int count = 0;

byte readbyte(void) {
  if ((mHandle.GetControlMode() == E_BLUTOOTH_MODE) || (mHandle.GetControlMode() == E_WIFI_MODE)) {
    return Serial.read();
  } else {
    return;
  }
}

int writedata(byte buf[], int len)
{
  if ((mHandle.GetControlMode() == E_BLUTOOTH_MODE) || (mHandle.GetControlMode() == E_WIFI_MODE)) {
    Serial.write(buf, len);
    Serial.flush();
    return len;
  } else {
    mHandle.mNrf24L01->setTADDR((uint8_t *)"WirelessHandle");           //Set the receiver address
    mHandle.mNrf24L01->payload = 12;
    mHandle.mNrf24L01->send(buf);
     for (int i = 0; i < 12; i++) {
      Serial.print(buf[i], HEX); Serial.print(" ");
    }
    Serial.println("");
    while (mHandle.mNrf24L01->isSending())
      delay(2);
  }
}

ProtocolParser Package(0xAA, 0x55, readbyte, writedata);

int CalculateSpeed(float gx, float gy)
{
  return (sqrt(gx * gx + gy * gy) * 100);
}

void setup()
{
  Serial.begin(9600);
  mHandle.SetControlSelect(E_WIRELESS_CONTROL_MODE);
  mHandle.SetControlMode(E_NRF24L01_MODE);   //E_BLUETOOTH_CONTROL //E_NRF24L01_MODE //E_WIFI_MODE
  mHandle.InitNrf24l01(NRF24L01_CE, NRF24L01_CSN);
  mHandle.InitMpu6050();
  mHandle.mProtocolPackage = &Package;
}

void loop() {
  if (mHandle.ScanKeyPad()) {
    if (mHandle.ButtonPressed(BT_SELECT)) {
      static byte select_count = 0;
      if (select_count++ >= 2) select_count = 0;
      Serial.println("BT_SELECT pressed");
      mHandle.SetControlSelect((E_CONTROL_SELECT)select_count);
    } else if (mHandle.ButtonPressed(BT_MODE)) {
      static byte mode_count = 0;
      if (mode_count++ >= 3) mode_count = 0;
      Serial.println("BT_MODE pressed");
      mHandle.SetControlMode((E_CONTROL_MODE)mode_count);
    }
  }
  if (mHandle.GetControlSelect() == E_GRAVITY_CONTROL_MODE) {
    mHandle.getInclination();
    if (count++ > 30) {
      count = 0;
      Package.SendSpeed(CalculateSpeed(mHandle.GetGravity(BT_X), mHandle.GetGravity(BT_Y)));
    }
    delay(5);
  } else if (mHandle.GetControlSelect() == E_WIRELESS_CONTROL_MODE) {
    if (mHandle.ButtonDataByte()) {
      if (mHandle.Button(BT_PAD_UP)) {
        Package.SendButton(BT_PAD_UP);
      } else if (mHandle.Button(BT_PAD_DOWN)) {
        Package.SendButton(BT_PAD_DOWN);
      } else if (mHandle.Button(BT_PAD_LEFT)) {
        Package.SendButton(BT_PAD_LEFT);
      } else if (mHandle.Button(BT_PAD_RIGHT)) {
        Package.SendButton(BT_PAD_RIGHT);
      } else if (mHandle.Button(BT_R)) {
        Package.SendButton(BT_R);
      } else if (mHandle.Button(BT_L)) {
        Package.SendButton(BT_L);
      } else if (mHandle.Button(BT_ENTER)) {
        Package.SendButton(BT_ENTER);
      }
    }
    int degree = mHandle.GetDegree();
    if (degree != 0xFFFF) {
      Package.SendDirection(degree);
    }
    delay(100);
  }
}
