//
// Created by bratoff on 27.06.21.
// Based on TTP229 by kiryanenko.
//
// New example to illustrate callbacks

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


#include <SPI.h>
#include <TTP229_SPI.h>

#define SCK_PIN 4
#define MISO_PIN 5

// Uncomment for true SPI
//TTP229_SPI ttp229;
// Uncomment for bitbang on SCK_PIN and MISO_PIN (defined above)
TTP229_SPI ttp229(SCK_PIN, MISO_PIN);

void keyChangeCb(uint16_t) {
    Serial.print("Key states: ");
    for (int i = 0; i < 16; ++i) {
        if (ttp229.isKeyPress(i)) {
            Serial.print("1 ");
        } else {
            Serial.print("0 ");
        }
    }

    int key = ttp229.getKey();
    Serial.print("Pressed key: ");
    Serial.print(key);
}

void keyDownCb(int key) {
  Serial.print("    Key ");
  Serial.print(key);
  Serial.println(" is down");
}

void keyUpCb(int key) {
  Serial.print("   Key ");
  Serial.print(key);
  Serial.println(" is up");
}

void setup()
{
		ttp229.begin();
    ttp229.setCbChange(&keyChangeCb);
    ttp229.setCbDown(&keyDownCb);
    ttp229.setCbUp(&keyUpCb);
    Serial.begin(115200);
}

void loop()
{
    ttp229.readKeys();
    delay(100);
}
