/***********************************************************************************************************************
 * 
 * LM15SGFNZ07.h                      
 * 
 * Header file for LM15SGFNZ07 library.
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#ifndef LM15SGFNZ07_H
#define LM15SGFNZ07_H

#include <wiringPi.h>

// Colors definitions
enum {
    BLACK        = 0x000,
    DARK_BLUE    = 0x008,
    MEDIUM_BLUE  = 0x00C,
    BLUE         = 0x00F,
    GREEN        = 0x080,
    DARK_CYAN    = 0x088,
    LIME         = 0x0F0,
    AQUA         = 0x0FF,
    DARK_RED     = 0x800,
    DARK_MAGENTA = 0x808,
    VIOLET       = 0x80F,
    OLIVE        = 0x880,
    GRAY         = 0x888,
    BU_RED       = 0xC00,
    DEEP_MAGENTA = 0xC0C,
    VIVID_ORCHID = 0xC0F, 
    SILVER       = 0xCCC,
    RED          = 0xF00,
    MAGENTA      = 0xF0F,
    DARK_ORANGE  = 0xF80,
    ORANGE       = 0xFA0,
    PINK         = 0xFCC,
    YELLOW       = 0xFF0,
    WHITE        = 0xFFF,
    NONE         = -1
};


// Line inverse options
enum {
    LCD_INVERSE_OFF   = 0x00,
    LCD_INVERSE_ON    = 0x01,
    LCD_INVERSE_BLINK = 0x03
};


class LM15SGFNZ07 {
    
    private:
        unsigned char LCD_SDATA;
        unsigned char LCD_SCLK;
        unsigned char LCD_RS;
        unsigned char LCD_RESET;
        unsigned char LCD_CS;
    
    public:
        LM15SGFNZ07(unsigned char SDATA, unsigned char SCLK, unsigned char RS, unsigned char RESET, unsigned char CS);
        void clearAll(unsigned int color);
        void setContrast(unsigned char contrast);
        void initialize(void);
        void turn(bool onoff);
        void inverseLines(unsigned char y, unsigned char height, unsigned char inverseMode);
        void inverseAll(bool inverse);
        void drawDot(unsigned char x, unsigned char y, unsigned int color);
        void drawLine(unsigned char x1, unsigned char y1, unsigned x2, unsigned char y2, unsigned int color);
        void drawRectangle(unsigned char x, unsigned char y, unsigned char width, unsigned char height,
                           unsigned int color);
        void drawFrame(unsigned char x,unsigned char y, unsigned char width, unsigned char height, unsigned char size,
                       unsigned int color);
        void drawArea(unsigned char x,unsigned char y, unsigned char width, unsigned char height, unsigned int color);
        void drawBitmap(unsigned char x,unsigned char y, unsigned char width, unsigned char height,
                        unsigned short *bitmap);
        void drawBitmap(unsigned char x,unsigned char y, unsigned char width, unsigned char height,
                        const unsigned short *bitmap);
        void drawString(const char *str, unsigned char x, unsigned char y, unsigned int color,
                        unsigned int background);
        void printChar(char chr, unsigned char x, unsigned char y, unsigned int size, unsigned int color,
                      unsigned int background);
        void printString(const char *str, unsigned char x, unsigned char y, unsigned int size, unsigned int color,
                         unsigned int background);
    
    private:
        void sendData(unsigned char dat);
        void setWindow(unsigned char x, unsigned char y, unsigned char width, unsigned char height);
        void drawPixel(unsigned char x, unsigned char y,unsigned int color);
        
};

#endif
