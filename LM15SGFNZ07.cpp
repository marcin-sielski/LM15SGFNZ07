/***********************************************************************************************************************
 * 
 * LM15SGFNZ07.cpp                      
 * 
 * LM15SGFNZ07 library for Raspberry Pi.
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#include <cstdlib>
#include <wiringPi.h>
#include "LM15SGFNZ07.h"

// Signals
#define LCD_RS_ON       digitalWrite(LCD_RS,HIGH); 
#define LCD_RS_OFF      digitalWrite(LCD_RS,LOW);
#define LCD_CS_OFF      digitalWrite(LCD_CS,HIGH);
#define LCD_CS_ON       digitalWrite(LCD_CS,LOW);
#define LCD_RESET_ON    digitalWrite(LCD_RESET,LOW);
#define LCD_RESET_OFF   digitalWrite(LCD_RESET,HIGH);

const unsigned char initData[139]  =    // LCD Inicialization data
{
	0xF4, 0x90, 0xB3, 0xA0, 0xD0, 0xF0, 0xE2, 0xD4, 0x70, 0x66,
	0xB2, 0xBA, 0xA1, 0xA3, 0xAB, 0x94, 0x95, 0x95, 0x95, 0xF5,
	0x90, 0xF1, 0x00, 0x10, 0x22, 0x30, 0x45, 0x50, 0x68, 0x70,
	0x8A, 0x90, 0xAC, 0xB0, 0xCE, 0xD0, 0xF2, 0x0F, 0x10, 0x20,
	0x30, 0x43, 0x50, 0x66, 0x70, 0x89, 0x90, 0xAB, 0xB0, 0xCD,
	0xD0, 0xF3, 0x0E, 0x10, 0x2F, 0x30, 0x40, 0x50, 0x64, 0x70,
	0x87, 0x90, 0xAA, 0xB0, 0xCB, 0xD0, 0xF4, 0x0D, 0x10, 0x2E,
	0x30, 0x4F, 0x50, 0xF5, 0x91, 0xF1, 0x01, 0x11, 0x22, 0x31,
	0x43, 0x51, 0x64, 0x71, 0x86, 0x91, 0xA8, 0xB1, 0xCB, 0xD1,
	0xF2, 0x0F, 0x11, 0x21, 0x31, 0x42, 0x51, 0x63, 0x71, 0x85,
	0x91, 0xA6, 0xB1, 0xC8, 0xD1, 0xF3, 0x0B, 0x11, 0x2F, 0x31,
	0x41, 0x51, 0x62, 0x71, 0x83, 0x91, 0xA4, 0xB1, 0xC6, 0xD1,
	0xF4, 0x08, 0x11, 0x2B, 0x31, 0x4F, 0x51, 0x80, 0x94, 0xF5,
	0xA2, 0xF4, 0x60, 0xF0, 0x40, 0x50, 0xC0, 0xF4, 0x70
};

unsigned char font5x7 [][5] = 			//Font data
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x32, 0x34, 0x08, 0x16, 0x26 },   // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
};
	
const unsigned char font[576] = {
	2, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,		// Sp
	1, 0b11111010, 0b00000000, 0b00000000, 0b00000000, 0b00000000,		// !
	3, 0b11100000, 0b00000000, 0b11100000, 0b00000000, 0b00000000,		// "
	5, 0b00101000, 0b11111110, 0b00101000, 0b11111110, 0b00101000,		// #
	5, 0b00100100, 0b01010100, 0b11111110, 0b01010100, 0b01001000,		// $
	5, 0b11000110, 0b11001000, 0b00010000, 0b00100110, 0b11000110,		// %
	5, 0b01101100, 0b10010010, 0b01101010, 0b00000100, 0b00001010,		// &
	1, 0b11100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,		// '
	2, 0b01111100, 0b10000010, 0b00000000, 0b00000000, 0b00000000,		// (
	2, 0b10000010, 0b01111100, 0b00000000, 0b00000000, 0b00000000,		// )
	5, 0b00101000, 0b00010000, 0b01111100, 0b00010000, 0b00101000,		// *
	5, 0b00010000, 0b00010000, 0b01111100, 0b00010000, 0b00010000,		// +
	2, 0b00000010, 0b00000100, 0b00000000, 0b00000000, 0b00000000,		// ,
	5, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000,		// -
	1, 0b00000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000,		// .
	5, 0b00000110, 0b00001000, 0b00010000, 0b00100000, 0b11000000,		// /
	5, 0b01111100, 0b10001010, 0b10010010, 0b10100010, 0b01111100,		// 0
	3, 0b01000010, 0b11111110, 0b00000010, 0b00000000, 0b00000000,		// 1
	5, 0b01000010, 0b10000110, 0b10001010, 0b10010010, 0b01100010,		// 2
	5, 0b10000100, 0b10000010, 0b10100010, 0b11010010, 0b10001100,		// 3
	5, 0b00011000, 0b00101000, 0b01001000, 0b11111110, 0b00001000,		// 4
	5, 0b11100100, 0b10100010, 0b10100010, 0b10100010, 0b10011100,		// 5
	5, 0b00111100, 0b01010010, 0b10010010, 0b10010010, 0b00001100,		// 6
	5, 0b10000000, 0b10001110, 0b10010000, 0b10100000, 0b11000000,		// 7
	5, 0b01101100, 0b10010010, 0b10010010, 0b10010010, 0b01101100,		// 8
	5, 0b01100000, 0b10010010, 0b10010010, 0b10010100, 0b01111000,		// 9
	1, 0b00101000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,		// :
	2, 0b00000010, 0b00100100, 0b00000000, 0b00000000, 0b00000000,		// ;
	4, 0b00010000, 0b00101000, 0b01000100, 0b10000010, 0b00000000,		// <
	5, 0b00101000, 0b00101000, 0b00101000, 0b00101000, 0b00101000,		// =
	4, 0b10000010, 0b01000100, 0b00101000, 0b00010000, 0b00000000,		// >
	5, 0b01000000, 0b10000000, 0b10001010, 0b10010000, 0b01100000,		// ?
	5, 0b01111100, 0b10000010, 0b10111010, 0b10101010, 0b01111000,		// @
	5, 0b01111110, 0b10010000, 0b10010000, 0b10010000, 0b01111110,		// A
	5, 0b11111110, 0b10010010, 0b10010010, 0b10010010, 0b01101100,		// B
	5, 0b01111100, 0b10000010, 0b10000010, 0b10000010, 0b01000100,		// C
	5, 0b11111110, 0b10000010, 0b10000010, 0b10000010, 0b01111100,		// D
	5, 0b11111110, 0b10010010, 0b10010010, 0b10010010, 0b10000010,		// E
	5, 0b11111110, 0b10010000, 0b10010000, 0b10010000, 0b10000000,		// F
	5, 0b01111100, 0b10000010, 0b10010010, 0b10010010, 0b01011110,		// G
	5, 0b11111110, 0b00010000, 0b00010000, 0b00010000, 0b11111110,		// H
	3, 0b10000010, 0b11111110, 0b10000010, 0b00000000, 0b00000000,		// I
	5, 0b00000100, 0b00000010, 0b10000010, 0b11111100, 0b10000000,		// J
	5, 0b11111110, 0b00010000, 0b00101000, 0b01000100, 0b10000010,		// K
	4, 0b11111110, 0b00000010, 0b00000010, 0b00000010, 0b00000000,		// L
	5, 0b11111110, 0b01000000, 0b00110000, 0b01000000, 0b11111110,		// M
	5, 0b11111110, 0b00100000, 0b00010000, 0b00001000, 0b11111110,		// N
	5, 0b01111100, 0b10000010, 0b10000010, 0b10000010, 0b01111100,		// O
	5, 0b11111110, 0b10010000, 0b10010000, 0b10010000, 0b01100000,		// P
	5, 0b01111100, 0b10000010, 0b10001010, 0b10000100, 0b01111010,		// Q
	5, 0b11111110, 0b10010000, 0b10011000, 0b10010100, 0b01100010,		// R
	5, 0b01100010, 0b10010010, 0b10010010, 0b10010010, 0b10001100,		// S
	5, 0b10000000, 0b10000000, 0b11111110, 0b10000000, 0b10000000,		// T
	5, 0b11111100, 0b00000010, 0b00000010, 0b00000010, 0b11111100,		// U
	5, 0b11111000, 0b00000100, 0b00000010, 0b00000100, 0b11111000,		// V
	5, 0b11111100, 0b00000010, 0b00001100, 0b00000010, 0b11111100,		// W
	5, 0b11000110, 0b00101000, 0b00010000, 0b00101000, 0b11000110,		// X
	5, 0b11100000, 0b00010000, 0b00001110, 0b00010000, 0b11100000,		// Y
	5, 0b10000110, 0b10001010, 0b10010010, 0b10100010, 0b11000010,		// Z
	2, 0b11111110, 0b10000010, 0b00000000, 0b00000000, 0b00000000,		// [
	5, 0b11000000, 0b00100000, 0b00010000, 0b00001000, 0b00000110,		// \
	1, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,		// WTF!?
	2, 0b10000010, 0b11111110, 0b00000000, 0b00000000, 0b00000000,		// ]
	3, 0b01000000, 0b10000000, 0b01000000, 0b00000000, 0b00000000,		// ^
	5, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010,		// _
	2, 0b10000000, 0b01000000, 0b00000000, 0b00000000, 0b00000000,		// `
	4, 0b00011100, 0b00100010, 0b00100100, 0b00111110, 0b00000000,		// a
	4, 0b11111110, 0b00100100, 0b00100010, 0b00011100, 0b00000000,		// b
	4, 0b00011100, 0b00100010, 0b00100010, 0b00010100, 0b00000000,		// c
	4, 0b00011100, 0b00100010, 0b00100100, 0b11111110, 0b00000000,		// d
	4, 0b00011100, 0b00101010, 0b00101010, 0b00010000, 0b00000000,		// e
	4, 0b01111110, 0b10010000, 0b10000000, 0b01000000, 0b00000000,		// f
	4, 0b01100100, 0b10010010, 0b01010010, 0b11111100, 0b00000000,		// g
	4, 0b11111110, 0b00010000, 0b00100000, 0b00011110, 0b00000000,		// h
	1, 0b10111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000,		// i
	3, 0b00000010, 0b00000010, 0b10111100, 0b00000000, 0b00000000,		// j
	4, 0b11111110, 0b00001000, 0b00010100, 0b00100010, 0b00000000,		// k
	2, 0b11111100, 0b00000010, 0b00000000, 0b00000000, 0b00000000,		// l
	5, 0b00111110, 0b00100000, 0b00011000, 0b00100000, 0b00011110,		// m
	4, 0b00111110, 0b00100000, 0b00100000, 0b00011110, 0b00000000,		// n
	4, 0b00011100, 0b00100010, 0b00100010, 0b00011100, 0b00000000,		// o
	4, 0b11111110, 0b01010000, 0b10010000, 0b01100000, 0b00000000,		// p
	4, 0b01100000, 0b10010000, 0b01010000, 0b11111110, 0b00000000,		// q
	4, 0b00111110, 0b00010000, 0b00100000, 0b00010000, 0b00000000,		// r
	4, 0b00010010, 0b00101010, 0b00101010, 0b00100100, 0b00000000,		// s
	4, 0b11111100, 0b00100010, 0b00100010, 0b00000100, 0b00000000,		// t
	4, 0b00111100, 0b00000010, 0b00000010, 0b00111100, 0b00000000,		// u
	5, 0b00111000, 0b00000100, 0b00000010, 0b00000100, 0b00111000,		// v
	5, 0b00111100, 0b00000010, 0b00001100, 0b00000010, 0b00111100,		// w
	5, 0b00100010, 0b00010100, 0b00001000, 0b00010100, 0b00100010,		// x
	4, 0b11100100, 0b00010010, 0b00100010, 0b11111100, 0b00000000,		// y
	4, 0b00100110, 0b00101010, 0b00101010, 0b00110010, 0b00000000,		// z
	3, 0b00010000, 0b01101100, 0b10000010, 0b00000000, 0b00000000,		// {
	1, 0b11111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000,		// |
	3, 0b10000010, 0b01101100, 0b00010000, 0b00000000, 0b00000000,		// }
	4, 0b01000000, 0b10000000, 0b01000000, 0b10000000, 0b00000000,		// ~
	5, 0b10101010, 0b01010100, 0b10101010, 0b01010100, 0b10101010		// Block
};
	
/**
 * LM15SGFNZ07 constructor that initilizes the display interface.
 * 
 * SDATA - GPIO number where SDATA line is connected, 
 * SCLK  - GPIO number where SCLK line is connected,
 * RS    - GPIO number where RS line is connected,
 * RESET - GPIO number where RESER line is connected,
 * CS    - GPIO number where CS line is connected. 
 */
LM15SGFNZ07::LM15SGFNZ07(unsigned char SDATA, unsigned char SCLK, unsigned char RS, unsigned char RESET,
                         unsigned char CS) {
	wiringPiSetupGpio();
	this->LCD_SDATA = SDATA;
	this->LCD_SCLK = SCLK;
	this->LCD_RS = RS;
	this->LCD_RESET = RESET;
	this->LCD_CS = CS;
}

/**
 * Clean the display with the specified color.  
 * 
 * color - 12-bit color value.
 */
void LM15SGFNZ07::clearAll(unsigned int color) {
	unsigned int i;

	setWindow(0, 0, 101, 80);
	LCD_CS_ON;
	for(i = 0; i < 8080; i++)
	{
    	sendData(color >> 8);
    	sendData(color & 0x00FF);
	}
	LCD_CS_OFF;
}

/**
 * Set specified contrast.
 * 
 * contrast - Contrast setting.
 */
void LM15SGFNZ07::setContrast(unsigned char contrast) {
	LCD_RS_ON;
	LCD_CS_ON;
	sendData(0xF4);
	sendData(0xB0 | (contrast >> 4));
	sendData(0xA0 | (contrast & 0x0F));
	LCD_CS_OFF;
}

/**
 * Initialize the display. 
 */
void LM15SGFNZ07::initialize(void) {
	// Decrarating PORTS as OUTPUTS
	pinMode(LCD_SDATA, OUTPUT);
    pinMode(LCD_RS, OUTPUT);
    pinMode(LCD_CS, OUTPUT);
    pinMode(LCD_SCLK, OUTPUT);
	
	unsigned short j;
	LCD_CS_OFF;
	LCD_RESET_OFF;
	LCD_RS_OFF;
	LCD_RESET_ON;   
	delay(10);
	LCD_RESET_OFF;
	delay(10);
    LCD_RS_ON;      
    LCD_CS_ON;      

	for(j = 0; j < 139; j++) {
		sendData(initData[j]);   // Sends bytes to LCD
	}
	delay(1);
    LCD_CS_OFF;      

	delay(1);
    LCD_CS_ON;      

    sendData(0xF0);
    sendData(0x81);
    sendData(0xF4);
    sendData(0xB3);
    sendData(0xA0);

    sendData(0xF0);
    sendData(0x06);
    sendData(0x10);
    sendData(0x20);
    sendData(0x30);
    sendData(0xF5);
    sendData(0x0F);
    sendData(0x1C);
    sendData(0x2F);
    sendData(0x34);
    LCD_CS_OFF;     
    LCD_RS_OFF;  
    
    setContrast(0x1a);
    delay(100);
}

/**
 * Turn the LCD on or off. When turned off display content is preserved.
 *
 * on - Turns the LCD on when true and off when false.
 */
void LM15SGFNZ07::turn(bool onoff) {
	LCD_RS_ON;
	LCD_CS_ON;
	sendData(0xF0);
	sendData(0x80 | (onoff ? 0x01 : 0x00));
	LCD_CS_OFF;
	LCD_RS_OFF;
}

/**
 * Set a number of rows on the display to be inversed using the given inverting mode.
 *
 * y           - The row to start inverting,
 * height      - The number of rows to invert,
 * inverseMode - The inverting function to be used this can be one of the following:
 *               LCD_INVERSE_OFF:   Turns off line inversions,
 *               LCD_INVERSE_ON:    Inversed lines are shown in negative,
 *               LCD_INVERSE_BLINK: Inversed lines alternate between positive and negative.
 */
void LM15SGFNZ07::inverseLines(unsigned char y, unsigned char height, unsigned char inverseMode) {
	LCD_RS_ON;
	LCD_CS_ON;
	sendData(0xF5);
	sendData(0x40 | (y & 0x0F));
	sendData(0x50 | (y >> 4));
	sendData(0xF5);
	sendData(0x60 | ((y + height - 1) & 0x0F));
	sendData(0x70 | ((y + height - 1) >> 4));
	sendData(0xF5);
	sendData(0x80 | (inverseMode & 0x03));
	LCD_CS_OFF;
	LCD_RS_OFF;
}

/**
 * Toggle inverse display.
 *
 * inverse - Turns inverse display on when true.
 */
void LM15SGFNZ07::inverseAll(bool inverse) {
	LCD_RS_ON;
	LCD_CS_ON;
	sendData(0xF0);
	sendData(0x90 | (inverse ? 0x09 : 0x01));
	LCD_CS_OFF;
	LCD_RS_OFF;
}

/**
 * Draw a pixel on screen in the given color. Note that drawing individual pixels is slow due to having to set the 
 * active window for each pixel.
 *
 * x     - X coordinate of pixel,
 * y     - Y coordinate of pixel,
 * color - 12-bit color value.
 */
void LM15SGFNZ07::drawDot(unsigned char x, unsigned char y, unsigned int color) {
	LCD_CS_ON;
	setWindow(x, y, 1, 1);
	sendData(color >> 8 );
	sendData(color & 0xFF);
	LCD_CS_OFF;
}

/**
 * Draw a line between two points in the given color.
 *
 * x1    - X coordinate of the first point,
 * y1    - Y coordinate of the first point,
 * x2    - X coordinate of the second point,
 * y2    - Y coordinate of the second point,
 * color - 12-bit color value.
 */
void LM15SGFNZ07::drawLine(unsigned char x1, unsigned char y1, unsigned x2, unsigned char y2, unsigned int color) {
	LCD_CS_ON;
	if (y1 == y2) {
    	// Simple solution for horizontal lines.
    	setWindow(x1, y1, abs(x2 - x1) + 1, 1);
    	for (int i = 0; i <= abs(x2 - x1); i ++) {
    		sendData(color >> 8 );
    		sendData(color & 0xFF);
    	}

	} else if (x1 == x2) {
    	// Simple solution for vertical lines.
    	setWindow(x1, y1, 1, abs(y2 - y1) + 1);
    	for (int i = 0; i <= abs(y2 - y1); i ++) {
    		sendData(color >> 8 );
    		sendData(color & 0xFF);
    	}
	} else {
    	// Draw diagonal line using Bresenham's algorithm.
    	if (x2 < x1) {
    		unsigned char t = x1;
    		x1 = x2;
    		x2 = t;
    	}

    	int dx = abs(x2 - x1);
    	int sx = x1 < x2 ? 1 : -1;
    	int dy = abs(y2 - y1);
    	int sy = y1 < y2 ? 1 : -1;
    	float err = (dx > dy ? dx : -dy) / 2;
    	float e2;

    	for (;;) {
    		drawDot(x1, y1, color);
    		if (x1 == x2 && y1 == y2) break;
    		e2 = err;
    		if (e2 > -dx) {
        		err -= dy;
        		x1 += sx;
    		}
    		if (e2 < dy) {
        		err += dx;
        		y1 += sy;
    		}
    	}
	}
	LCD_CS_OFF;
}

/**
 * Draw a rectangular area on the screen with a given color.
 *
 * x      - Left coordinate,
 * y      - Top coordinate,
 * width  - Rectangle width,
 * height - Rectangle height,
 * color  - 12-bit color value.
 */
void LM15SGFNZ07::drawRectangle(unsigned char x, unsigned char y, unsigned char width, unsigned char height,
                                unsigned int color) {
	LCD_CS_ON;
	// Top
	setWindow(x, y, width, 1);
	for (int i = 0; i < width; i ++) {
    	sendData(color >> 8);
    	sendData(color & 0xFF);
	}

	// Bottom
	setWindow(x, y + height - 1, width, 1);
	for (int i = 0; i < width; i ++) {
    	sendData(color >> 8);
    	sendData(color & 0xFF);
	}

	// Left
	setWindow(x, y, 1, height);
	for (int i = 0; i < height; i ++) {
    	sendData(color >> 8);
    	sendData(color & 0xFF);
	}

	// Right
	setWindow(x + width - 1, y, 1, height);
	for (int i = 0; i < height; i ++) {
    	sendData(color >> 8);
    	sendData(color & 0xFF);
	}
	LCD_CS_OFF;
}

/**
 * Draw a frame.
 * 
 * x      - Left coordinate,
 * y      - Top coordinate,
 * width  - Frame width,
 * height - Frame height,
 * s      - Frame size,
 * color  - Frame color.
 */
void LM15SGFNZ07::drawFrame(unsigned char x, unsigned char y, unsigned char width, unsigned char height,
                            unsigned char size, unsigned int color) {
	int x2 = x + width;
	int y2 = y + height;
	unsigned int i;
	for(i=1; i<=size; i++)
	{
    	drawLine(x, y, x, y2, color);
    	drawLine(x2, y, x2, y2, color);
    	drawLine(x, y, x2, y, color);
    	drawLine(x, y2, x2, y2, color);
    	x++;
    	y++;
    	x2--;
    	y2--;
	}
}

/**
 * Fill a rectangular area of the screen with a given color.
 *
 * x      - Left coordinate,
 * y      - Top coordinate,
 * width  - Rectangle width,
 * height - Rectangle height,
 * color  - 12-bit color value.
 */
void LM15SGFNZ07::drawArea(unsigned char x, unsigned char y, unsigned char width, unsigned char height,
                           unsigned int color) {
	LCD_CS_ON;
	setWindow(x, y, width, height);
	for (int i = 0; i < width * height; i ++) {
    	sendData(color >> 8);
    	sendData(color & 0xFF);
	}
	LCD_CS_OFF;
}

/**
 * Draw a bitmap onto the screen at the given position.
 *
 * x      - Left coordinate,
 * y      - Top coordinate,
 * width  - Width of the bitmap,
 * height - Height of the bitmap,
 * bitmap - Pointer to the bitmap image.
 */
void LM15SGFNZ07::drawBitmap(unsigned char x, unsigned char y, unsigned char width, unsigned char height,
                             unsigned short *bitmap) {
	LCD_CS_ON;
	setWindow(x, y, width, height);
	for (int i = 0; i < width * height; i ++) {
    	sendData(bitmap[i] >> 8);
    	sendData(bitmap[i] & 0xFF);
	}
	LCD_CS_OFF;
}


/**
 * Draw a bitmap stored in progam memory onto the screen at the given position.
 *
 * x      - Left coordinate,
 * y      - Top coordinate,
 * width  - Width of the bitmap,
 * height - Height of the bitmap,
 * bitmap - Pointer to the bitmap image.
 */
void LM15SGFNZ07::drawBitmap(unsigned char x,unsigned char y, unsigned char width, unsigned char height,
                             const unsigned short *bitmap) {
	LCD_CS_ON;
	setWindow(x, y, width, height);
	for (int i = 0; i < width * height; i ++) {
    	unsigned int data = *(bitmap ++);
    	sendData(data >> 8);
    	sendData(data & 0xFF);
	}
	LCD_CS_OFF;
}

/**
 * Draw a string on screen.
 *
 * str        - Pointer to the string to display,
 * x          - X position of the text,
 * y          - Y position of the text,
 * color      - 12-bit text color,
 * background - 12-bit background color.
 */
void LM15SGFNZ07::drawString(const char *str, unsigned char x, unsigned char y, unsigned int color,
                             unsigned int background) {
	LCD_CS_ON;
	int strPos = 0;

	while (str[strPos] != 0) {
    	unsigned int glyphPtr   = (str[strPos] - 32) * 6;
    	unsigned int glyphWidth = font[glyphPtr ++];

    	for (int i = 0; i < glyphWidth; i ++) {
    		setWindow(x ++, y, 1, 8);
    		unsigned char glyphData = font[glyphPtr ++];

    		for (unsigned char mask = 0x80; mask; mask >>= 1) {
        		sendData(glyphData & mask ? color >> 8 : background >> 8);
        		sendData(glyphData & mask ? color & 0xFF : background & 0xFF);
    		}
    	}

    	setWindow(x ++, y, 1, 8);
    	for (unsigned char j = 0; j < 8; j ++) {
    		sendData(background >> 8);
    		sendData(background & 0xFF);
    	}
    	strPos ++;
	}
	LCD_CS_OFF;
}

/**
 * Print a character.
 *  
 * chr        - Character to print,
 * x          - Left coordinate dependent on font size,
 * y          - Top coordinate dependent on font size,
 * size       - Font size (1-4),
 * color      - Font color,
 * background - Background color or NONE.
 */
void LM15SGFNZ07::printChar(char chr, unsigned char x, unsigned char y, unsigned int size, unsigned int color,
                            unsigned int background) {

    char buf;
	unsigned int i, j, l, k, xpoz, ypoz, w, h;
	xpoz=x*6*size;
	ypoz=79-(y*8*size) - (size == 4 ? 11 : size);
   
    if(size == 1) //if the size of 1
	{
		for(i = 0; i < 5; i++)//for 5 elements character array
		{
			buf=font5x7[chr - 32][i];//buf assign one of 5 elements
			j=0;
			while(j<8)
			{
				if(buf & (1 << j))//If another element is 1
				{
					drawPixel(xpoz+i,ypoz-j, color);//draws a pixel
					j++;
				} else if(background==-1) {
					j++;
				} else {
					drawPixel(xpoz+i,ypoz-j, background);//draws a pixel
					j++;
				}
			}
		}
	} else if(size == 2) {
		l=0;
		for (i = 0; i < 5; i++) {
			k=0;
			j=0;
			l+=2;
			buf=font5x7[chr - 32][i];
	    	while(j<8) {
		
				if(buf & (1 << j)) {
					for (w = 0; w < 2; w ++) {
						for (h = 0; h < 2; h ++) {
							drawPixel(xpoz + l + w, ypoz - k + h, color);
						}
					}
					k+=2;
					j++;
				} else if(background==-1) {
					k+=2;
					j++;
				} else {
					for (w = 0; w < 2; w ++) {
						for (h = 0; h < 2; h ++) {
							drawPixel(xpoz + l + w, ypoz - k + h, background);
						}
					}
					k+=2;
					j++;
				}
			}      
    	}
	} else if(size == 3) {
		l=0;
		for (i = 0; i < 5; i++) {
			k=0;
			j=0;
			l+=2;
			buf=font5x7[chr - 32][i];
	    	while(j<8) {
		
				if(buf & (1 << j)) {
					for (w = 0; w < 3; w ++) {
						for (h = 0; h < 3; h ++) {
							drawPixel(xpoz + l + w, ypoz - k + h, color);
						}
					}
					k+=3;
					j++;
				} else if(background==-1) {
					k+=3;
					j++;
				} else {
					for (w = 0; w < 3; w ++) {
						for (h = 0; h < 3; h ++) {
							drawPixel(xpoz + l + w, ypoz - k + h, background);
						}
					}
					k+=3;
					j++;
				}
			}      
    	}
	} else if(size == 4) {
		l=0;
		for (i = 0; i < 5; i++) {
			k=0;
			j=0;
			l+=4;
			buf=font5x7[chr - 32][i];
	    	while(j<8) {
		
				if(buf & (1 << j)) {
					for (w = 0; w < 5; w ++) {
						for (h = 0; h < 11; h ++) {
							drawPixel(xpoz + l + w, ypoz - k + h, color);
						}
					}
					k+=11;
					j++;
				} else if(background==-1) {
					k+=11;
					j++;
				} else {
					for (w = 0; w < 5; w ++) {
						for (h = 0; h < 11; h ++) {
							drawPixel(xpoz + l + w, ypoz - k + h, background);
						}
					}
					k+=11;
					j++;
				}
			}      
    	}
	}	
}

/**
 * Print a string. 
 * 
 * str        - String to print,
 * x          - Left coordinate dependent on font size,
 * y          - Top coordinate dependent on font size,
 * size       - Font size (1-4),
 * color      - Font color,
 * background - Background color or NONE.
 */
void LM15SGFNZ07::printString(const char *str, unsigned char x, unsigned char y, unsigned int size,
                              unsigned int color, unsigned int background)
{
    while((*str != 0) && (*str != 10)) {	
        printChar(*str++, x, y, size, color, background);
		x++;
		if(size==1 && x>16)////If mae letters and come off the screen
		{
			y++;
			x=0;
		}
		else if(size==2 && x>7)//If large letters and come off the screen
		{		
			y++;
			x=0;
		}
		else if(size==3 && x>4)//If large letters and come off the screen
		{		
			y++;
			x=0;
		}
		else if(size==4 && x>4)//If large letters and come off the screen
		{		
			y++;
			x=0;
		}
		
    }
	
}

/**
 * Sends RAW data to LCD.
 * 
 * data - Raw data to be sent.
 */
void LM15SGFNZ07::sendData(unsigned char data) {
	unsigned short i;
	
    // Access to the LCD controller
	LCD_CS_ON;
	 
	for(i=0; i<8; i++) //Sends 8 bits
	{
	
		if((data>>(7-i)) & 0x01) //If the bit is a single room sends the SDAT, sends zero if 0
		{
            digitalWrite(LCD_SDATA,HIGH);
		} 
		else 
		{
			digitalWrite(LCD_SDATA,LOW);
		}
		
		// Clock ticks, jump to the next bit
		digitalWrite(LCD_SCLK,HIGH);
		digitalWrite(LCD_SCLK,LOW);
	}

	// Not available LCD controller
    LCD_CS_OFF; 
}

/**
 * Set the active drawing area of the LCD. The cursor will be set to the given (x, y) position.
 *
 * x      - Left coordinate,
 * y      - Top coodrinate,
 * width  - Width of the active drawing area,
 * height - Height of the active drawing area.
 */
void LM15SGFNZ07::setWindow(unsigned char x, unsigned char y, unsigned char width, unsigned char height) {
	unsigned char xStart = (x << 1) + 6;
	unsigned char xEnd = ((x + width - 1) << 1) + 7;
	unsigned char yStart = y;
	unsigned char yEnd = y + height - 1;

	LCD_RS_ON;
	sendData(0xF0);
	sendData(0x00 | (xStart & 0x0F));
	sendData(0x10 | (xStart >> 4));
	sendData(0x20 | (yStart & 0x0F));
	sendData(0x30 | (yStart >> 4));
	sendData(0xF5);
	sendData(0x00 | (xEnd & 0x0F));
	sendData(0x10 | (xEnd >> 4));
	sendData(0x20 | (yEnd & 0x0F));
	sendData(0x30 | (yEnd >> 4));
	LCD_RS_OFF;
}

/**
 * Set specified pixel with specified color.
 * 
 * x     - Left coordinate,
 * y     - Top coordinate,
 * color - Color.
 */
void LM15SGFNZ07::drawPixel(unsigned char x, unsigned char y,unsigned int color) {
	LCD_CS_ON;
	setWindow(x, 79-y, 1, 1);
	//SPI1_16bit;
	sendData((color & 0x0FFF) >> 8 );
	sendData(color & 0x00FF);
	//SPI1_8bit;
	LCD_CS_OFF;
}
