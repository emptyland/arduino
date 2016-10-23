/*

This is a modified combination of the excellent TrinketKeyboard library from Adafruit 
and the old DigiKeyboard library From Digistump based on AVRUSB by Obdev

With contributions by Digistump (Erik Kettenburg), Bluebie, and the Digistump community


Obdev license maintained in the usbdrv folder

Adafruit license below

Digistump makes its modifications/additions under the same LGPL

Many Thanks to Obdev and Adafruit

===============================================

This is the part of the TrinketKeyboard code that is exposed to the user

Copyright (c) 2013 Adafruit Industries
All rights reserved.

TrinketKeyboard is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

TrinketKeyboard is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with TrinketKeyboard. If not, see
<http://www.gnu.org/licenses/>.
*/

#ifndef __DigiKeyboard_h__
#define __DigiKeyboard_h__

#include <stdint.h>
#include <Print.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

class DigiKeyboardDevice : public Print
{
	private:
	public:
		DigiKeyboardDevice(); // empty constructor, ignore me
		void begin(); // BREAKING CHANGE: starts the USB driver, causes re-enumeration
		void poll(); // this (or "press" something) must be called at least once every 10ms
		void update(); // for compatability
		void delay(long milli); // poll enabled delay function
		char isConnected(); // checks if USB is connected, 0 if not connected


		// presses up to 6 keys, and modifiers (modifiers are keys like shift, CTRL, etc)
		void pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3, uint8_t keycode4, uint8_t keycode5, uint8_t keycode6);
		void pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3, uint8_t keycode4, uint8_t keycode5);
		void pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3, uint8_t keycode4);
		void pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3);
		void pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2);
		void pressKey(uint8_t modifiers, uint8_t keycode1);
		void sendKeyStroke(uint8_t keycode1, uint8_t modifiers);
		void sendKeyStroke(uint8_t keycode1);

		// presses a list of keys
		void pressKeys(uint8_t modifiers, uint8_t* keycodes, uint8_t sz);

		// type out a single ASCII character
		void typeChar(uint8_t ascii);

		// returns the state of the three LEDs on a keyboard (caps/num/scroll lock)
		uint8_t getLEDstate();
		char capsLock(); //0 if off, 1 if on
		char numLock();
		char scrollLock();

		// inherit from "Print", these two write functions are implemented
		virtual size_t write(uint8_t);
		using Print::write;
		// other "print" and "println" functions are automatically available
		using Print::print;
		using Print::println;
};

// an instance that the user can use
extern DigiKeyboardDevice DigiKeyboard;

// LED state masks
#define KB_LED_NUM			0x01
#define KB_LED_CAPS			0x02
#define KB_LED_SCROLL		0x04

//BOTH Adafruit and old DigiKeyboard defines are included for compatability 

#define MOD_CONTROL_LEFT    (1<<0)
#define MOD_SHIFT_LEFT      (1<<1)
#define MOD_ALT_LEFT        (1<<2)
#define MOD_GUI_LEFT        (1<<3)
#define MOD_CONTROL_RIGHT   (1<<4)
#define MOD_SHIFT_RIGHT     (1<<5)
#define MOD_ALT_RIGHT       (1<<6)
#define MOD_GUI_RIGHT       (1<<7)

#define KEY_A       4
#define KEY_B       5
#define KEY_C       6
#define KEY_D       7
#define KEY_E       8
#define KEY_F       9
#define KEY_G       10
#define KEY_H       11
#define KEY_I       12
#define KEY_J       13
#define KEY_K       14
#define KEY_L       15
#define KEY_M       16
#define KEY_N       17
#define KEY_O       18
#define KEY_P       19
#define KEY_Q       20
#define KEY_R       21
#define KEY_S       22
#define KEY_T       23
#define KEY_U       24
#define KEY_V       25
#define KEY_W       26
#define KEY_X       27
#define KEY_Y       28
#define KEY_Z       29
#define KEY_1       30
#define KEY_2       31
#define KEY_3       32
#define KEY_4       33
#define KEY_5       34
#define KEY_6       35
#define KEY_7       36
#define KEY_8       37
#define KEY_9       38
#define KEY_0       39

#define KEY_ENTER   40

#define KEY_SPACE   44

#define KEY_F1      58
#define KEY_F2      59
#define KEY_F3      60
#define KEY_F4      61
#define KEY_F5      62
#define KEY_F6      63
#define KEY_F7      64
#define KEY_F8      65
#define KEY_F9      66
#define KEY_F10     67
#define KEY_F11     68
#define KEY_F12     69

#define KEY_ARROW_RIGHT		0x4F
#define KEY_ARROW_LEFT		0x50
#define KEY_ARROW_DOWN		0x51
#define KEY_ARROW_UP		0x52

// some convenience definitions for modifier keys
#define KEYCODE_MOD_LEFT_CONTROL	0x01
#define KEYCODE_MOD_LEFT_SHIFT		0x02
#define KEYCODE_MOD_LEFT_ALT		0x04
#define KEYCODE_MOD_LEFT_GUI		0x08
#define KEYCODE_MOD_RIGHT_CONTROL	0x10
#define KEYCODE_MOD_RIGHT_SHIFT		0x20
#define KEYCODE_MOD_RIGHT_ALT		0x40
#define KEYCODE_MOD_RIGHT_GUI		0x80

// some more keycodes
#define KEYCODE_LEFT_CONTROL	0xE0
#define KEYCODE_LEFT_SHIFT		0xE1
#define KEYCODE_LEFT_ALT		0xE2
#define KEYCODE_LEFT_GUI		0xE3
#define KEYCODE_RIGHT_CONTROL	0xE4
#define KEYCODE_RIGHT_SHIFT		0xE5
#define KEYCODE_RIGHT_ALT		0xE6
#define KEYCODE_RIGHT_GUI		0xE7
#define KEYCODE_1				0x1E
#define KEYCODE_2				0x1F
#define KEYCODE_3				0x20
#define KEYCODE_4				0x21
#define KEYCODE_5				0x22
#define KEYCODE_6				0x23
#define KEYCODE_7				0x24
#define KEYCODE_8				0x25
#define KEYCODE_9				0x26
#define KEYCODE_0				0x27
#define KEYCODE_A				0x04
#define KEYCODE_B				0x05
#define KEYCODE_C				0x06
#define KEYCODE_D				0x07
#define KEYCODE_E				0x08
#define KEYCODE_F				0x09
#define KEYCODE_G				0x0A
#define KEYCODE_H				0x0B
#define KEYCODE_I				0x0C
#define KEYCODE_J				0x0D
#define KEYCODE_K				0x0E
#define KEYCODE_L				0x0F
#define KEYCODE_M				0x10
#define KEYCODE_N				0x11
#define KEYCODE_O				0x12
#define KEYCODE_P				0x13
#define KEYCODE_Q				0x14
#define KEYCODE_R				0x15
#define KEYCODE_S				0x16
#define KEYCODE_T				0x17
#define KEYCODE_U				0x18
#define KEYCODE_V				0x19
#define KEYCODE_W				0x1A
#define KEYCODE_X				0x1B
#define KEYCODE_Y				0x1C
#define KEYCODE_Z				0x1D
#define KEYCODE_COMMA			0x36
#define KEYCODE_PERIOD			0x37
#define KEYCODE_MINUS			0x2D
#define KEYCODE_EQUAL			0x2E
#define KEYCODE_BACKSLASH		0x31
#define KEYCODE_SQBRAK_LEFT		0x2F
#define KEYCODE_SQBRAK_RIGHT	0x30
#define KEYCODE_SLASH			0x38
#define KEYCODE_F1				0x3A
#define KEYCODE_F2				0x3B
#define KEYCODE_F3				0x3C
#define KEYCODE_F4				0x3D
#define KEYCODE_F5				0x3E
#define KEYCODE_F6				0x3F
#define KEYCODE_F7				0x40
#define KEYCODE_F8				0x41
#define KEYCODE_F9				0x42
#define KEYCODE_F10				0x43
#define KEYCODE_F11				0x44
#define KEYCODE_F12				0x45
#define KEYCODE_APP				0x65
#define KEYCODE_ENTER			0x28
#define KEYCODE_BACKSPACE		0x2A
#define KEYCODE_ESC				0x29
#define KEYCODE_TAB				0x2B
#define KEYCODE_SPACE			0x2C
#define KEYCODE_INSERT			0x49
#define KEYCODE_HOME			0x4A
#define KEYCODE_PAGE_UP			0x4B
#define KEYCODE_DELETE			0x4C
#define KEYCODE_END				0x4D
#define KEYCODE_PAGE_DOWN		0x4E
#define KEYCODE_PRINTSCREEN		0x46
#define KEYCODE_ARROW_RIGHT		0x4F
#define KEYCODE_ARROW_LEFT		0x50
#define KEYCODE_ARROW_DOWN		0x51
#define KEYCODE_ARROW_UP		0x52

#endif