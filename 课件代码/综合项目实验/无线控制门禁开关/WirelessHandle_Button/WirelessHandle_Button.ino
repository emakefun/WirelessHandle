#include "WirelessHandle.h"
#include "ProtocolParser.h"
#include "debug.h"

WirelessHandle mHandle(E_BLUTOOTH_MODE);

byte readbyte(void) {
    if ((mHandle.GetControlMode() == E_BLUTOOTH_MODE) || (mHandle.GetControlMode() == E_WIFI_MODE)) {
        return Serial.read();
    } else {
        return;
    }
}

int writedata(byte buf[], int len)
{
    int value = 10;
    if ((mHandle.GetControlMode() == E_BLUTOOTH_MODE) || (mHandle.GetControlMode() == E_WIFI_MODE)) {
        Serial.write(buf, len);
        Serial.flush();
        return len;
    } else {
        mHandle.mNrf24L01->setTADDR((uint8_t *)"WirelessSwitch");           //Set the receiver address
        mHandle.mNrf24L01->payload = sizeof(value);
        mHandle.mNrf24L01->send((byte *)&value);
        while (mHandle.mNrf24L01->isSending())
           delay(2);
    }
}

ProtocolParser Package(0xAA, 0x55, readbyte, writedata);

void setup()
{
    Serial.begin(9600);
  //  mHandle.SetControlSelect(E_WIRELESS_CONTROL_MODE);
    mHandle.SetControlMode(E_NRF24L01_MODE);   //E_BLUETOOTH_CONTROL //E_NRF24L01_MODE //E_WIFI_MODE
    mHandle.InitNrf24l01(10, 9);
    mHandle.InitMpu6050();
    mHandle.mProtocolPackage = &Package;
}

void loop() {

    if (mHandle.ScanKeyPad()) {
        if (mHandle.ButtonPressed(BT_PAD_UP)) {
            Serial.println("BT_PAD_UP Button Pressed");
            int value = 10;
            mHandle.mNrf24L01->setTADDR((uint8_t *)"WirelessSwitch");           //Set the receiver address
            mHandle.mNrf24L01->payload = sizeof(value);
            mHandle.mNrf24L01->send((byte *)&value);
            while (mHandle.mNrf24L01->isSending())
               delay(2);
        }
    }
    if (mHandle.ButtonReleased(BT_PAD_UP)) {
        Serial.println("BT_PAD_UP Button ButtonReleased");
        int value = 20;
        mHandle.mNrf24L01->setTADDR((uint8_t *)"WirelessSwitch");           //Set the receiver address
        mHandle.mNrf24L01->payload = sizeof(value);
        mHandle.mNrf24L01->send((byte *)&value);
        while (mHandle.mNrf24L01->isSending())
           delay(2);
    } 
    delay(50);
}
