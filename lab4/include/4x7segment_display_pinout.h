#ifndef 4X7SEGMENT_DISPLAY_PINOUT_H
#define 4X7SEGMENT_DISPLAY_PINOUT_H

#include <stdint.h>

/* PORT 1 */

/*
 * Define segments in one 7-segment digit + dot
 *    DDD
 *   A   C
 *   A   C
 *    GGG
 *   F   D
 *   F   D
 *    EEE   HH  <- dot
 */

#define A 0x01
#define B 0x02
#define C 0x04
#define D 0x08
#define E 0x10
#define F 0x20
#define G 0x40
#define H 0x80

/* PORT 2 */

/*
 * Define active digits
 * 0 - digit is active; 1 - digit disabled
 */
#define DIGIT1 0x01;
#define DIGIT2 0x02;
#define DIGIT3 0x04;
#define DIGIT4 0x08;

#define ENABLE_DIGIT1 0xFE;
#define ENABLE_DIGIT2 0xFD;
#define ENABLE_DIGIT3 0xFC;
#define ENABLE_DIGIT4 0xFB;

/* Digit definition */

const uint8_t LCDHexChar[] = {
  A | B | C | D | E | F,      // '0'
  C | D,                      // '1'
  B | C | G | F | E,          // '2'
  B | C | G | D | E,          // '3'
  A | G | C | D,              // '4'
  B | A | G | D | E,          // '5'
  B | A | G | D | E | F,      // '6'
  B | C | D,                  // '7'
  A | B | C | D | E | F | G,  // '8'
  A | B | C | G | D | E,      // '9'
  F | A | B | C | D | G,      // 'A'
  A | F | E | D | G,          // 'B'
  E | F | A | B,              // 'C'
  G | F | E | D | C,          // 'D'
  E | F | G | A | B,          // 'E'
  F | G | A | B               // 'F'
};

const uint8_t LCDrChar = F | G;              // 'r'
const uint8_t LCDoChar = F | G | D | E;      // 'o'
const uint8_t LCDtChar = A | G | F | F | E;  // 't'
const uint8_t LCDpChar = F | A | B | C | G;  // 'p'

#endif /* 4X7SEGMENT_DISPLAY_PINOUT_H */
