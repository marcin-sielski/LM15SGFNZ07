/**
 * Copyright (C) 2017 Marcin Sielski
 * 
 * Demo sketch for the LM15SGFNZ07 LCD display library. This demo consists of 3 parts to show most of the capabilities
 * of the LM15SGFNZ07 library.
 * Coded by Maarten Janssen in 2015.
 *
 * Latest version of the libaray can be downloaded from my GitHub page: http://github.com/DhrBaksteen
 * Contact me at maarten@cheerful.nl
 *
 * The LM15SGFNZ07 is an LCD with a 101x80 pixels resolution, capable of displaying 4096 colors. It is found in Siemens
 * mobile phones (models A65, M55, C60, MC60, S55, A60). Connect the LCD to your Arduino as follows:
 * LCD pin   -->   Arduino pin
 *   1 (/CS)        6
 *   2 (/RST)       5
 *   3 (RS)         4
 *   4 (Sclk)       13 (or SPI SCK)
 *   5 (Sdata)      11 (or SPI MOSI)
 *   6 (+3.3v)      3.3v
 *   7 (GND)        GND
 *   8 (LED1)       3.3v
 *   9 (GND)        GND
 *  10 (LED2)       3.3v
 */

#include <LM15SGFNZ07.h>
#include "icon.h"

LM15SGFNZ07 lcd(17, 22, 5, 6, 26);

void setup() {
	lcd.initialize();
}

void demo1() {
	lcd.clearAll(0x000);
	lcd.drawArea(1,  0, 14, 45, 0xFFF);
	lcd.drawArea(15, 0, 14, 45, 0xFF0);
	lcd.drawArea(29, 0, 14, 45, 0x0FF);
	lcd.drawArea(43, 0, 14, 45, 0x0F0);
	lcd.drawArea(57, 0, 14, 45, 0xF0F);
	lcd.drawArea(71, 0, 14, 45, 0xF00);
	lcd.drawArea(85, 0, 14, 45, 0x00F);
  
	lcd.drawArea(1, 45, 98, 15, 0xF00);
	lcd.drawString("LM15SGFNZ07", 18, 49, 0xFFF, 0xF00);
  
	for (int i = 0; i < 16; i ++) {
		lcd.drawArea(i * 6 + 1, 60, 8, 5, (i << 8) + (i << 4) + i);
    	lcd.drawArea(i * 6 + 1, 65, 8, 5, i << 8);
    	lcd.drawArea(i * 6 + 1, 70, 8, 5, i << 4);
    	lcd.drawArea(i * 6 + 1, 75, 8, 5, i);
	}
  
	delay(10000);
}

void demo2() {
	lcd.clearAll(0xFFF);
  
	for (int i = 2; i < 80; i += 5) {
    	lcd.drawLine(0, i, 100, i, 0x888);
	}
  
	for (int i = 2; i < 101; i += 5) {
    	lcd.drawLine(i, 0, i, 79, 0x888);
	}
  
	lcd.drawArea(0, 0, 10, 10, 0x00F);
	lcd.drawArea(91, 0, 10, 10, 0x0F0);
	lcd.drawArea(91, 70, 10, 10, 0xFF0);
	lcd.drawArea(0, 70, 10, 10, 0xF00);
  
	lcd.drawArea(10, 0, 81, 5, 0x0FF);
	lcd.drawArea(96, 10, 5, 60, 0x8F0);
	lcd.drawArea(10, 75, 81, 5, 0xF80);
	lcd.drawArea(0, 10, 5, 60, 0xF0F);
  
	lcd.drawBitmap(34, 24, 32, 32, icon);
	delay(10000);
}

void demo3() {
	lcd.clearAll(0x22F);
	lcd.drawRectangle(0, 0, 101, 80, 0xFFF);
  
	for (unsigned char x = 0; x <= 80; x += 10) {
    	lcd.drawLine(0, x, x, 80, 0xFFF);
    	lcd.drawLine(20 + x, 0, 100, x, 0xFFF);
	}
 
	lcd.drawString("Arduino", 28, 35, 0xFFF, 0x22F);
	lcd.drawBitmap(63, 34, 11, 9, heart);
	delay(10000);
}

void loop() {
	demo1();
	demo2();
	demo3();
}

int main() {
	setup();
	loop();
	return 0;
}