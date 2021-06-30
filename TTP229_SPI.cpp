//
// Created by bratoff on 27.06.21
// based on TTP229 by kiryanenko on 14.09.19.
//

#include "TTP229_SPI.h"


int findKey(uint16_t bits) {
	if(bits) {
    int i = 0;
    for (uint16_t mask = 0x0001; mask; mask <<= 1) {
        if (mask & bits) {
            return i;
        }
        ++i;
    }
	} else {
    return -1;
	}
}

void TTP229_SPI::begin(void) {
	if(_sckPin < 0) {
		SPI.begin();
		SPI.beginTransaction(SPISettings(14000000, LSBFIRST, SPI_MODE3));
	} else {
		pinMode(_sckPin, OUTPUT);
		digitalWrite(_sckPin, HIGH);
		pinMode(_misoPin, INPUT);
	}
}

uint16_t TTP229_SPI::readSPI(void) {
	if(_sckPin < 0 || _misoPin < 0) {
		return(SPI.transfer16(0));
	} else {
		uint16_t bits = 0;
		for(int i=0; i<16; i++) {
			bits >>= 1;
			digitalWrite(_sckPin, LOW);
			if(digitalRead(_misoPin))
				bits |= 0x8000;
			digitalWrite(_sckPin, HIGH);
		}
		return(bits);
	}
}

uint16_t TTP229_SPI::readKeys(void) {
    _prev_state = _keys;

		_keys = readSPI();
		if(_invert)
			_keys ^= 0xffff;

		uint16_t changed = _prev_state ^ _keys;
		if(changed) {
			if(_cb_change != NULL)
				_cb_change(_keys);

		
			if(_keys & changed) {
				if(_cb_down != NULL)
					_cb_down(findKey(changed));
			}	else {
				if(_cb_up != NULL)
					_cb_up(findKey(changed));
			}
		}		
    return _keys;
}

int TTP229_SPI::getKey(void) {
	return findKey(_keys);
}

bool TTP229_SPI::isKeyPress(byte key) {
    return bitRead(_keys, key);
}

bool TTP229_SPI::isKeyDown(byte key) {
    return !bitRead(_prev_state, key) && bitRead(_keys, key);
}

bool TTP229_SPI::isKeyUp(byte key) {
    return bitRead(_prev_state, key) && !bitRead(_keys, key);
}
