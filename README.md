TTP229_SPI Arduino Library
============================================

An Arduino library for TTP229-BSF module for controlling a 16 capacitive touch keys via SPI. 

Derived from I2C version by Alexander Kiryanenko



## Connection

- Uno:   
    - SDO <-> 12 (MISO)
    - SCL <-> 13 (SCK)

- Mega:  
    - SDO <-> 50 (MISO)
    - SCL <-> 52 (SCK)

- Leo:
    - SDO <-> ICSP-1 (MISO)
    - SCL <-> ICSP-3 (SCK)

- ATMegaZero:
    - SDO <-> 14 (MISO)
    - SCL <-> 15 (SCK)

- Any other board with SPI capability will work if the MISO and SCK pins are known



## Methods:

- `TTP229_SPI(bool invert)` - The constructor takes an optional boolean to indicate whether the
	keypad is jumpered for inverted logic (0 for key pressed).  The default is TRUE.
	Use this constructor if your board and core support SPI and the keypad is connected to
	the predefined SPI pins for that board.
	
- `TTP229_SPI(int sckPin, int misoPin, bool invert)` - Alternate constructor for bitbang i/o on any
	two digital pins.  The optional invert parameter is supported and defaults to TRUE.
	
- `void begin()` - Initializes SPI subsystem or bitbang pins.

- `void end()` - Releases SPI subsystem (so another device can use it)

- `uint16_t readKeys()` - Read data from TTP229 and return states of 16 keys 
  in bitmap representation. This method should call before other methods.
  Put it into `loop` function.

- `int getKey()` - Return number of pressed key or return `-1` if no one key is pressed.

- `bool isKeyPress(byte key)` - Return true if key is pressed.

- `bool isKeyDown(byte key)` - Return true if key is down.

- `bool isKeyUp(byte key)` - Return true if key is up.

- `void setInvert(bool inverted)` - Set inverted-logic flag (same as in constructor)

The following callbacks (if set) may be triggered by calling readKeys():

- `void setCbChange(void (*cbFunc)(uint16_t keys))` - sets a callback function for every change.
	cbFunc is called with the current bit string (result of the last call to readkeys).
	You can call getkey from inside the callback function to return the actual key.

- `void setCbDown(void (*cbFunc)(int key))` - sets a callback on key pressed.
	cbFunc is called with the key number that was just pressed.

- `void setCbUp(void (*cbFunc)(int key))` - sets a callback on key released.
	cbFunc is called with the key number that was just released.

One or more of the above callbacks will be called if any keys have been pressed or released between the
most recent and preceding calls to readKeys().  The basic idea is to call readKeys() frequently
in your loop() function and then do all the work in the callbacks.  Since interrupts are not used,
it is possible to miss a key change if readKeys() is not called frequently enough.

To disable a callback after setting it, set it to NULL.



## Examples

`TTP229_SPI` - strictly polled example

`TTP229_SPI_cb` - example with all 3 callbacks



## Known limitations:

- Since these keypads do not support CS, no other device can use the SPI bus unless
	some sort of external buffering and enable/disable is implemented. If you need the SPI bus
	for something else (eg. SD card), it's easier to use the bitbang mode (with different pins)
	for the keypad.

- The callback mechanism is not reentrant, so take care not to do anything in a callback
	function that would cause readKeys to be called again before all callbacks have returned.

 
 # License
 Copyright (c) 2021 Bruce Ratoff. Licensed under the MIT license.