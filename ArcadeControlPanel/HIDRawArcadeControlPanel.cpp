#include "HIDRawArcadeControlPanel.h"

HIDRawArcadeControlPanel::HIDRawArcadeControlPanel()
{

}

void HIDRawArcadeControlPanel::update()
{
	// update the control panel status and build an accurate HID Raw message which captures the state of the panel

	// treat shift differently since it's a modifier key. Modifier key code goes in position 1, and does not count against our 6 total USB key presses
	// because shift is different, we will be sure to access that with button position 0
	buttons[0].process();
	// if anything changed since the last time the CP status was read, flag it
	_wasChanged = _wasChanged || buttons[0].stateChanged(false);
	// HIDCode must contain {0xFD, modifier_code, 0x00, key1, key2, key3, key4, key5, key6}
	if (buttons[0].isPressed(false))
	{
		HIDCode[1] = _HIDRawKeyCodes[0]; // modifier keys always go in the second entry of the HID raw code array (first entry must be 0xFD)
	}
	else
	{
		HIDCode[1] = 0x00;
	}
	// now build message to bluetooth module for up to 6 other keypresses (Shift does not count)
	// we can add key presses to the message in positions 3-8
	byte msg_position = 3;
	// iterate over each button, skipping the first because that was Shift
	for (size_t i = 1; i<ARCADE_NUM_SWITCHES; i++) {
		// read the button value
		buttons[i].process();
		// if anything changes since the last time the CP status was read, flag it
		_wasChanged = _wasChanged || buttons[i].stateChanged(false);
		if (buttons[i].isPressed(false)) {
			HIDCode[msg_position] = _HIDRawKeyCodes[i];
			msg_position++;
			// Bluetooth module command messages for HID Raw codes are no more than 8 bytes   
			if (msg_position > HID_CODES_SIZE-1) break;
		}
	}
	// now that we've iterated over every button, the rest of the HID Raw codes should be filled with zeros
	for (size_t i = msg_position; i<HID_CODES_SIZE; i++) HIDCode[i] = 0x00;

}

// returns true if any switch position has changed since the last time this function was called
bool HIDRawArcadeControlPanel::wasChanged(bool proc)
{
	if (proc) update();
	bool _output = _wasChanged;
	_wasChanged = false;
	return _output;
}
