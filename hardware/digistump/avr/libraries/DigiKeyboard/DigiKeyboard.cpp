/*
This is a modified combination of the excellent TrinketKeyboard library from Adafruit 
and the old DigiKeyboard library From Digistump based on AVRUSB by Obdev

With contributions by Digistump (Erik Kettenburg), Bluebie, and the Digistump community


Obdev license maintained in the usbdrv folder

Adafruit license below

Digistump makes its modifications/additions under the same LGPL

Many Thanks to Obdev and Adafruit

=============================================================

This is the part of the TrinketKeyboard code that is exposed to the user
See the header file for more comments on the functions

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

#include "cmdline_defs.h"
#include "DigiKeyboard.h"
#include "TrinketKeyboardC.h"
#include <Arduino.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "scancode-ascii-table.h"
// create an instance that the user can use
DigiKeyboardDevice DigiKeyboard;


// empty constructor
DigiKeyboardDevice::DigiKeyboardDevice()
{
	// nothing to do
}

// starts the USB driver, causes re-enumeration
void DigiKeyboardDevice::begin()
{
	usbBegin();
}

// this must be called at least once every 10ms
void DigiKeyboardDevice::poll()
{
	usbPollWrapper();
}

void DigiKeyboardDevice::update()
{
	usbPollWrapper();
}

void DigiKeyboardDevice::delay(long milli) {
	unsigned long last = millis();
  while (milli > 0) {
    unsigned long now = millis();
    milli -= now - last;
    last = now;
    usbPollWrapper();
  }
}

void DigiKeyboardDevice::pressKey(uint8_t modifiers, uint8_t keycode1)
{
	pressKey(modifiers, keycode1, 0, 0, 0, 0, 0);
}

void DigiKeyboardDevice::pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2)
{
	pressKey(modifiers, keycode1, keycode2, 0, 0, 0, 0);
}

void DigiKeyboardDevice::pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3)
{
	pressKey(modifiers, keycode1, keycode2, keycode3, 0, 0, 0);
}

void DigiKeyboardDevice::pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3, uint8_t keycode4)
{
	pressKey(modifiers, keycode1, keycode2, keycode3, keycode4, 0, 0);
}

void DigiKeyboardDevice::pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3, uint8_t keycode4, uint8_t keycode5)
{
	pressKey(modifiers, keycode1, keycode2, keycode3, keycode4, keycode5, 0);
}

void DigiKeyboardDevice::pressKey(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3, uint8_t keycode4, uint8_t keycode5, uint8_t keycode6)
{
	// construct the report, follow the standard format as described
	// this format is compatible with "boot protocol"
	report_buffer[0] = modifiers;
	report_buffer[1] = 0; // reserved
	report_buffer[2] = keycode1;
	report_buffer[3] = keycode2;
	report_buffer[4] = keycode3;
	report_buffer[5] = keycode4;
	report_buffer[6] = keycode5;
	report_buffer[7] = keycode6;
	usbReportSend();
}

void DigiKeyboardDevice::sendKeyStroke(uint8_t keycode1) {
    pressKey(0, keycode1);
    pressKey(0, 0);
}

void DigiKeyboardDevice::sendKeyStroke(uint8_t keycode1, uint8_t modifiers)
{
	pressKey(modifiers, keycode1, 0, 0, 0, 0, 0);
	pressKey(0, 0);
}


void DigiKeyboardDevice::pressKeys(uint8_t modifiers, uint8_t* keycodes, uint8_t sz)
{
	report_buffer[0] = modifiers;
	report_buffer[1] = 0; // reserved
	uint8_t i;
	for (i = 0; i < sz; i++)
	{
		report_buffer[2 + i] = keycodes[i];
	}
	usbReportSend();
}

void DigiKeyboardDevice::typeChar(uint8_t ascii)
{
	uint8_t data = pgm_read_byte_near(ascii_to_scan_code_table + (ascii - 8));
	pressKey(data >> 7 ? MOD_SHIFT_RIGHT : 0, data & 0b01111111);
	pressKey(0, 0); // immediately release the key after
}

size_t DigiKeyboardDevice::write(uint8_t ascii)
{
	typeChar(ascii);
	return 1;
}


uint8_t DigiKeyboardDevice::getLEDstate()
{
	return led_state;
}

char DigiKeyboardDevice::capsLock()
{
	if(led_state & KB_LED_CAPS)
		return 1;
	else
		return 0;
}

char DigiKeyboardDevice::numLock()
{
	if(led_state & KB_LED_NUM)
		return 1;
	else
		return 0;
}

char DigiKeyboardDevice::scrollLock()
{
	if(led_state & KB_LED_SCROLL)
		return 1;
	else
		return 0;
}

// checks if USB is connected, 0 if not connected
char DigiKeyboardDevice::isConnected()
{
	return usb_hasCommed;
}