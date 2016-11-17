/*
Name:		HIDRawArcadeControlPanel.h
Created:	4/4/2016 8:53:32 PM
Author:	Ryan Hamilton, ryan.hamilton@gmail.com
This class defines an interface for a collection of switches intended to control a MAME cabinet via bluetooth.
Switches are debounced using https://github.com/t3db0t/Button
HID Raw keyboard codes corresponding to active switches are assembled in cp.HIDCode
Usage:
cp.update()
if (cp.wasChanged()) {
sendToBlueTooth(cp.HIDCode);
}
*/

#ifndef _HIDRAWARCADECONTROLPANEL_h
#define _HIDRAWARCADECONTROLPANEL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// using the debouncing forl of the button library from
// https://github.com/t3db0t/Button
// This is NOT the standard button library from the arduiono code playground
#include <Button\Button.h>

#define ARCADE_NUM_SWITCHES 17 // does not include modifier key
#define ARCADE_DEBOUNCE_DURATION 15 // milliseconds
#define HID_CODES_SIZE 9

class HIDRawArcadeControlPanel
{
private:
	uint8_t _numSwitches = ARCADE_NUM_SWITCHES;
	bool _wasChanged = false; // whether _HIDRawKeyCodes changed since the last call to HIDRawCodes()
	Button _ModifierButton = Button(9, BUTTON_PULLUP_INTERNAL, true, ARCADE_DEBOUNCE_DURATION); // must be the modifier (Shift) button
	Button buttons[ARCADE_NUM_SWITCHES] = {
		Button(6,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // menu / volume
		Button(5,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), //  save / slot-
		Button(3,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // load / slot+
		Button(2,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), //  pause / exit
		Button(11,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // coin
		Button(10,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // player 1
		Button(21,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // Y
		Button(22,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // X
		Button(15,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // LB
		Button(19,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // A
		Button(20,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // B
		Button(23,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // C
		Button(16,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // D
		Button(12,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // Up
		Button(14,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // Down
		Button(0,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // Left
		Button(13,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION) // Right
	};
	// codes corresponding to each switch being activated. keyboard key | MAME function I want to use
	// codes from http://www.freebsddiary.org/APC/usb_hid_usages.php
	uint8_t _HIDRawKeyCodes[ARCADE_NUM_SWITCHES] = {
		0x10, // m | menu
		0x16, // s | save
		0x12, // o | load
		0x13, // p | pause
		0x22, // 5 | coin
		0x1E, // 1 | player 1
		0x1C, // y | Y
		0x1B, // x | X
		0x0F, // l | LB
		0x04, // a | A
		0x05, // b | B
		0x06, // c | C
		0x07, // d | D
		0x52, // UP | Up
		0x51, // DOWN | Down
		0x50, // LEFT | Left
		0x4F // RIGHT | Right
	};
	// if _ModifierButton.isPressed then send the ALT code below instead
	uint8_t _HIDRawKeyCodesALT[ARCADE_NUM_SWITCHES] = {
		0x19, // v | volume
		0x2D, // - | slot-
		0x2E, // = | slot+
		0x08, // e | exit
		0x22, // 5 | coin
		0x1E, // 1 | player 1
		0x1C, // y | Y
		0x1B, // x | X
		0x0F, // l | LB
		0x04, // a | A
		0x05, // b | B
		0x06, // c | C
		0x07, // d | D
		0x52, // UP | Up
		0x51, // DOWN | Down
		0x50, // LEFT | Left
		0x4F // RIGHT | Right
	};

public:
	HIDRawArcadeControlPanel();
	void update(); // check all switch states
				   // default HID code is no buttons pressed. Note 0xFD is the message start for HID Raw codes.
	uint8_t HIDCode[HID_CODES_SIZE] = { 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // HID Raw command array shift key first, then up to six buttons pressed
																								// return true if HIDRawCodes changed since the last time wasChanged was called. If proc (process) is true, first update the control panel before returning results.
	bool wasChanged(bool proc = true);
};

#endif
