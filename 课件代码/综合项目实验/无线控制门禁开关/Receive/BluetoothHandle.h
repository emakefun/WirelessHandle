#ifndef _BLUETOOTH_HANDLE_H_
#define _BLUETOOTH_HANDLE_H_

#include "Protocol.h"

//These are our button constants
#define BT_SELECT     0b100000000
#define BT_MODE        0b00000010
#define BT_ENTER       0b00000001
#define BT_L           0b10000000
#define BT_R           0b01000000
#define BT_PAD_DOWN    0b00100000
#define BT_PAD_LEFT    0b00010000
#define BT_PAD_UP      0b00001000
#define BT_PAD_RIGHT   0b00000100

//These are stick values
#define PSS_RX 5
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

#define SET(x,y) (x|=(1<<y))
#define CLR(x,y) (x&=(~(1<<y)))
#define CHK(x,y) (x & (1<<y))
#define TOG(x,y) (x^=(1<<y))

class BluetoothHandle {
  public:
    boolean Button(uint16_t);                //will be TRUE if button is being pressed
    unsigned int ButtonDataByte();
    boolean NewButtonState();
    boolean NewButtonState(unsigned int);    //will be TRUE if button was JUST pressed OR released
    boolean ButtonPressed(unsigned int);     //will be TRUE if button was JUST pressed
    boolean ButtonReleased(unsigned int);    //will be TRUE if button was JUST released
    byte readType(void);
    byte config_gamepad(void);
    byte Analog(byte);
    void SendBluetoothButton(uint16_t button);
  private:
    unsigned char i;
    unsigned int last_buttons;
    unsigned int buttons;
    unsigned long last_read;
    byte read_delay;
    byte controller_type;
    boolean en_Rumble;
    boolean en_Pressures;
};

#endif
