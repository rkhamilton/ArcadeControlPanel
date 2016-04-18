ArcadeControlPanel
=======

This project is a wireless [MAME](http://mamedev.org/) arcade control panel which emulates a bluetooth keyboard. A simple arduino microcontroller ([Adafruit Feather 32u4](https://www.adafruit.com/products/2771)) is wired to the panel controls, and wired to a bluetooth module that emulates a HID Keyboard ([Adafruit EZ-Key](https://www.adafruit.com/product/1535)). I was thinking about building a full cabinet, but realized I live in a small house, and I prefer sitting on my couch to standing in front of a cabinet. I wanted to have a control panel that felt like an arcade cabinet that I could use from my couch.

![ArcadeControlPanel](/images/finished.jpg)
![back side](/images/wired-all.jpg)
![controllers](/images/wired.jpg)
For this project I was focused more on the mechanicals than on the electronics, so no custom PCB here. I used a few off-the shelf components and wired them together in the simplest way I could. A cheaper, more power-efficient build would have been to use individual components to build an arduino-compatible board like I did with my shooCat project, but the small arduinos are so compact and easy to use I went for the faster route of buying an Adafruit module. A Teensy would also have been a good choice here.

I'm writing this description aimed toward the MAME / emulation enthusiast, so I'll "over explain" some concepts.

#Still Broken!
Before you read further, note there's still a significant problem that I haven't fixed. The control panel registers clicks perfectly from all of the buttons, but many of the joystick movements are not registering. They're wired and coded exactly the same, so I think it must be due to the different microswitches used in the joystick. I've ordered some ultra light touch switches, similar to the buttons. Once I try that out I'll update the project if this fixes the problem. I think it may be that the debounce tuning needs to be very different for the heavy weight switches used in the joystick.

#Control panel layout and art
This is my first MAME build of any kind, so I did a lot of reading about example cabinets. An example of visual styling that I really liked was this bartop arcade by [floriske.nl]((http://forum.arcadecontrols.com/index.php/topic,123039.0.html)). I adjusted the action button layout based on examples at [slagcoin](http://www.slagcoin.com/joystick/layout.html). I printed out several layouts until I found one that I liked, then modified it to have seven buttons. 

I did the panel layout in Adobe Illustrator. My [.ai layout file](/control_panel_art/control-panel.ai), and a [PDF version](/control_panel_art/control-panel.pdf), are in the panel design folder. I sent the design file in to [gameongrafix](http://www.gameongrafix.com/) to be printed on adhesive-backed polycarbonate. I'm really happy with the quality of the printed art, and I'd use them again. I'm running the poly artwork right to the edge of the controller, and I'm concerned with how it will wear as things catch on the edge. It would probably be more robust to have the edges protectd by a raised edge peice, or covered by plastic.

#Buttons and Joystick
I'm only planning to play classic MAME arcade games, so I didn't need analog controls. I selected the [Ultimarc Mag-stik plus](http://www.ultimarc.com/store/section.php?xSec=6) joystick because it lets you toggle from 4-way control to 8-way control by lifting up on the stick and twisting it 45 degrees. It's four times more expesive than a good 8-way non-adjustible joystick, but it seems like people like it.

For buttons I went with 30mm leaf switches from ultimarc. My plan is the bottom four are for NeoGeo games, and the right-most six are for Street Fighter style buttons. I used some [16mm black pusbuttons from adafruit](https://www.adafruit.com/products/1505) for the admin buttons. I'm taking advantage of the fact that emulating a keyboard lets me use a shift key to get double-duty out of my admin buttons.

The power toggle switch is a [simple SPST switch](https://www.sparkfun.com/products/11138).

#Enclosure
I'm a complete novice at woodworking, so I learned a lot building this, and I wouldn't necessarily do it the same way a second time. It's all 3/4" thick poplar. I started from the control panel layout and cut the top wood surface to match the needed dimensions. I printed out a [hole template](/images/hole-template.jpg) and drilled the wood so I could work on the panel components while I waited for the top artwork to be delivered. In the end the holes didn't line up perfectly, so if I were to do it over I'd be more patient and wait for the final printed art to arrive and use that as the template for drilling holes.

The joystick cutout is routed down 1/8" to make room for the mounting plate. The buttons mounted directly to the wood, but the admin buttons are not tall enough to fully penetrate the wood. I ended up routing a channel halfway through the wood to make the admin button area thinner so that I could mount the admin buttons. I wanted smaller admin buttons, and I didn't readily find small diameter buttons that would pass through 3/4" paneling. In retrospect 1/2" panels would have worked fine.
![routed channel for admin buttons](/images/routed.jpg)

I assembled the case with bronzed wood screws, sanded it all down, and applied stain and polyurathane to the edges. The top surface was painted with primer to help the adhesive artwork stick to the wood. The use of primer was recommended by the art vendor (gameongrafix).
![rough](/images/rough-enclosure.jpg)
![painting](/images/finishing-enclosure.jpg)
![finish](/images/enclosure-finish.jpg)


#Bluetooth
I'm using an [Adafruit EZ-Key](https://www.adafruit.com/product/1535) to connect to a Raspberry Pi 3 via Bluetooth 2.1. I tried using a [Adafruit Feather Bluefruit LE](https://learn.adafruit.com/adafruit-feather-32u4-bluefruit-le/overview) but I couldn't get the Pi to work with the newer Bluetooth LE device. It worked fine with the EZ-Key though. The LE device would have had less power consumption and more connectivity options to the arduino, but in the end the EZ Key works just fine.

The EZ-Key is connected to the arduino in the simplest way possible. 3.3V power to my regulator, ground, and one wire to the arduino serial hardware TX on pin 1. I'm sending keypresses using the approach for "[sending keys via serial](https://learn.adafruit.com/introducing-bluefruit-ez-key-diy-bluetooth-hid-keyboard/sending-keys-via-serial)."

#Latency
The switch debounce is 15ms (most any switch has a lag in software or hardware to damp mechanical vibrations). HID keyaboard 8 byte key commands are sent from the arduino to the bluetooth module via UART serial at 9600 baud in about 7ms. Bluetooth lag ([according to Adafruit](https://learn.adafruit.com/introducing-bluefruit-ez-key-diy-bluetooth-hid-keyboard/faq)) is about 30ms. Total latency from button press to receipt by the arcade emulator hardware is then about 52ms

#Power
The Feather has an integrated power regulator that would take battery input directly. However I couldn't tell if it was a switching regulator (power efficient) or linear regulator (uses more power and generates waste heat the higher your input voltage source). I happened to have some LM2596 switching regulators handy so I just used one of those. 
![LM2596 voltage regulator](/images/regulator.jpg)
I needed to output 3.3V from the regulator, so I needed at least 4.8V input power so it could smooth and step it down to 3.3V. I also didn't know how much power this thing would draw so I just used two 3-battery AA battery holders wired in parallel (9V, 12000mAh, AKA "too much"). The power draw seems to be around 25 mA when powered on and idle, so this should run for 20 continuous days of use before I need to replace the (rechargable) AA batteries.

I have the power regulator in series with a SPST switch to turn the whole thing off and on.

#Code
My control panel is emulating a keyboard. In order to handle multiple simultaneous key presses, one has to send HID raw USB codes. This limits you to six simultaneous key presses, plus modifiers (shift, control, etc). In my case, I'm using a shift key, plus up to six pressed buttons. You send a "keys pressed" command that indicates the active keys, and then later follow that up with a "no keys pressed" command (or a "different keys pressed" command). I.e., you don't continually send a signal when the key is pressed, you only signal on state change. This is described well by Adafruit in their [documentation](https://learn.adafruit.com/introducing-bluefruit-ez-key-diy-bluetooth-hid-keyboard) for the EZ-Key bluetooth module. I always enjoy buying from Adafruit because they provide such great documentation!

In order to accomplish a notification on state-change, I need the control panel to track when any button changes. The code consists of one class, which manages the collection of switches that make up the control panel. Each switch is controlled using a [Button class]((https://github.com/t3db0t/Button)) that includes software-based debounce. The button class has a wasChanged() method, so my control panel just sets up and traverses an array of buttons to see if any of them have changed. As the control panel class traverses this array, it also populates an array of bytes consisting of the HID raw key codes that describe the current state of the activated buttons (up to 6 buttons, per the USB HID limit). 

Within the control panel class, each button is mapped to a [HID raw code for a particular keyboard key](http://www.freebsddiary.org/APC/usb_hid_usages.php). I picked keys that are commonly used in MAME, but I still have to specifically map them to my MAME controller configuration.

```c++
class HIDRawArcadeControlPanel
{
 private:
	byte _numSwitches = ARCADE_NUM_SWITCHES;
	bool _wasChanged = false; // whether _HIDRawKeyCodes changed since the last call to HIDRawCodes()
	Button buttons[ARCADE_NUM_SWITCHES] = {
		Button(9,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // must be the Shift button
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
		Button(12,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // U
		Button(14,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // D
		Button(0,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION), // L
		Button(13,BUTTON_PULLUP_INTERNAL,true, ARCADE_DEBOUNCE_DURATION) // R
	};
	// codes corresponding to each switch being activated. keyboard key | MAME function I want to use
	// codes from http://www.freebsddiary.org/APC/usb_hid_usages.php
	uint8_t _HIDRawKeyCodes[ARCADE_NUM_SWITCHES] = { 
		0x02, // LeftShift modifier key - note this must always be the code for the shift button | must be the Shift button. Also note this is the MODIFIER, not the shift keypress (0xE1). This cost me time to realize...
		0x10, // m | menu / volume
		0x16, // s | save / slot-
		0x12, // o | load / slot+
		0x13, // p | pause / exit
		0x22, // 5 | coin
		0x1E, // 1 | player 1
		0x1C, // y | Y
		0x1B, // x | X
		0x0F, // l | LB
		0x04, // a | A
		0x05, // b | B
		0x06, // c | C
		0x07, // d | D
		0x52, // UP | U
		0x51, // DOWN | D
		0x50, // LEFT | L
		0x4F // RIGHT | R
	};

 public:
	HIDRawArcadeControlPanel();
	void update(); // check all switch states
	 // default HID code is no buttons pressed. Note 0xFD is the message start for HID Raw codes.
	uint8_t HIDCode[HID_CODES_SIZE] = { 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // HID Raw command array shift key first, then up to six buttons pressed
	// return true if HIDRawCodes changed since the last time wasChanged was called. If proc (process) is true, first update the control panel before returning results.
	bool wasChanged(bool proc = true); 
};
```

I use the shift key in my control panel so that I can easily get double-duty out of my admin buttons (press pause to pause the game, shift-pause to exit back to the front end). I lost a little time figuring out that the HID key code for shift is 0xE1, but the modifier key code for shift is 0x02. We need the latter in sending the HID codes to the computer.

In the main program the logic is very simple. The EZ-Key bluetooth device lists for serial commands that are sent via the arduino hardware UART (Serial1).

```c++
HIDRawArcadeControlPanel cp = HIDRawArcadeControlPanel();
void setup() {
	if (DEBUG_PRINT)
	{
		Serial.begin(115200);
		delay(200);
	}
	// initialize connectiont to EZ-Key via Serial1. EZ Key talks at 9600 baud.
	Serial1.begin(9600);
}
void loop() {
	// check all switches in the control panel for updates
	// if any switches have changed, send them to the bluetooth module
	if (cp.wasChanged(true))
	{
		Serial1.write(cp.HIDCode, HID_CODES_SIZE);
		Serial1.flush();
	}  
}
```

#Visual Studio / Visual Micro
I also use, and highly recommend, the [Visual Micro](http://www.visualmicro.com/) plugin for Visual Studio. It doesn't seem to get a lot of discussion, but if you know how to program at all this is a great tool. You can use the full featured free "[Visual Studio Community Edition](https://www.visualstudio.com/en-us/products/visual-studio-community-vs.aspx)" to write software, and this Visual Micro plugin brings full arduino support to the VS IDE. It even allows debugging! I don't know how people write code of any complexity with the stock Arduino editor.

#Shopping List
| Item  | Price  |
|---|---|
| wood - assorted poplar, with gunstock stain and polyurethane |   |
| 7x [30mm goldleaf pushbuttons](http://www.ultimarc.com/store/product.php?xProd=171) | $1.95/ea  |
| [30mm player 1 button](http://www.ultimarc.com/controls.html) | $1.40  |
| [Ultimarc Mag-stik plus](http://www.ultimarc.com/store/section.php?xSec=6) | $37.00  |
| [Adafruit Feather 32u4](https://www.adafruit.com/products/2771) | $19.95  |
| [Adafruit EZ-Key](https://www.adafruit.com/product/1535) | $19.95 |
| 7x [16mm black pusbuttons](https://www.adafruit.com/products/1505) | $0.95/ea  |
| [printed art from gameongrafix](http://www.gameongrafix.com/categories/control-panels) | $19.99  |
| [power switch](https://www.sparkfun.com/products/11138) | $0.50  |
| 2x [3 battery AA holders](https://www.sparkfun.com/products/9543) | $1.50/ea  |
| [LM2596-based switching voltage regulator, adjusted to 3.3V output](http://www.amazon.com/RioRand-LM2596-Converter-1-23V-30V-Pcs-LM2596) | $2 |


#Lessons Learned
* I'm running the poly artwork right to the edge of the controller, and I'm concerned with how it will wear as things catch on the edge. It would probably be more robust to have the edges protectd by a raised edge peice, or covered by plastic.
* I didn't have a router at the start of the project. Now that I know a bit how to use one, I would attach the wood edges uing a miter bit router to join wood edges. This would give it more of a "furniture" look where there are no exposed cut ends of the wood.
* Get final panel art before drilling button holes to ensure perfect alignment with printed art.
* I tried using a Feather 32u4 with integrated Bluetooth LE, but the driver support on Windows 10 and Raspbian are too sketchy and I couldn't get it to work. I ended up buying a the EZ-Key becauyse it's "only" Bluetooth 2.1, which is well supported.

#Acknowledgements
* This project uses [Ted Hayes' Button.h library](https://github.com/t3db0t/Button), an improved version of the original Arduino button library by Alexander Brevig.
* Thanks to [Adafruit](https://www.adafruit.com) for their great, maker friendly hardware, documentation, and example code.
* Thanks to floriske.nl for [posting his stylish bartop arcade](http://forum.arcadecontrols.com/index.php/topic,123039.0.html). The style of my custom graphics are heavily inspired by his design.
