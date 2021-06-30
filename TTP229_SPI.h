//
// Created by Bruce Ratoff on 27.06.21
//   to support TTP229-BSF and 8229BSF, which are SPI, not I2C.
//
// Based on TTP229 by Alexander Kiryanenko on 14.09.19.
//
// Changes as of 28-06-21:
//	Convert from I2C to SPI interface (most boards now being sold are SPI)
//	Add constructor parameter for inverted data (default true)
//	Add callbacks for key change, key down and key up
//	Support either true SPI or bitbang based on constructor
//	Move I/O and SPI initialization into a begin() function with companion end() function
//

//////////////////////////////////////////////////////////////////
// Connections
//
// - Uno:   
//     - SDO <-> 12 (MISO)
//     - SCL <-> 13 (SCK)
// 
// - Mega:  
//     - SDO <-> 50 (MISO)
//     - SCL <-> 52 (SCK)
// 
// - Leo:
//     - SDO <-> ICSP-1 (MISO)
//     - SCL <-> ICSP-3 (SCK)
// 
// - ATMegaZero:
//     - SDO <-> 14 (MISO)
//     - SCL <-> 15 (SCK)
//
//////////////////////////////////////////////////////////////////


#ifndef TTP229_SPI_H
#define TTP229_SPI_H

#include <Arduino.h>
#include <SPI.h>

class TTP229_SPI {
private:
    uint16_t _keys = 0;											// Result of latest scan
    uint16_t _prev_state = 0;								// Result of previous scan
		bool _invert = true;										// True if pad is jumpered active-low (default)
		void (* _cb_change)(uint16_t) = NULL;	// Pointer to callback on key change
		void (* _cb_down)(int) = NULL;					// Pointer to callback on key down
		void (* _cb_up)(int) = NULL;						// Pointer to callback on key up
		int _sckPin = -1;												// Clock pin if bitbang interface, else -1
		int _misoPin = -1;											// Data pin if bitbang interface, else -1
		
		uint16_t readSPI(void);									// Helper function that reads a 16 bit word from keypad

public:
    /// constructors:
		/// for true SPI		
    explicit TTP229_SPI(bool invert = true) : _invert(invert) {
			_sckPin = -1;
			_misoPin = -1;
		}
		/// for bitbang i/o - REMEMBER TO INIT SCK and MISO pins in calling program!!
		explicit TTP229_SPI(int sckPin, int misoPin, bool invert = true) : _invert(invert), _sckPin(sckPin), _misoPin(misoPin) {}
		/// Initialization function
		void begin(void);
		/// De-init - for true SPI only
		void end(void) {if(_sckPin < 0) SPI.end();}
    /// Read data from TTP229. Should call before other methods.
    /// \return States of 16 keys in bitmap representation
		/// Trigger callbacks if any are set
    uint16_t readKeys(void);
    /// This function don't read data from ttp229. Only return previous read data.
    /// \return States of 16 keys in bitmap representation (bit 15 is key 0)
    uint16_t getKeys(void) { return _keys; }
    /// Return number of pressed key or return -1 if no one key is pressed
    /// \return Number of pressed key or -1 if no one key pressed
    int getKey(void);
    /// Return true if key is pressed
    /// \param key Number of key
    /// \return True if key is pressed
    bool isKeyPress(byte key);
    /// Return true if key is pressed
    /// \param key Number of key
    /// \return True if key is pressed
    bool isKeyDown(byte key);
    /// Return true if key is pressed
    /// \param key Number of key
    /// \return True if key is pressed
    bool isKeyUp(byte key);
		void setInvert(bool inv) {_invert = inv;}
		void setCbChange(void(*f)(uint16_t)) {_cb_change = f;}
		void setCbDown(void(*f)(uint16_t)) {_cb_down = f;}
		void setCbUp(void(*f)(uint16_t)) {_cb_up = f;}
};


#endif //TTP229_H
