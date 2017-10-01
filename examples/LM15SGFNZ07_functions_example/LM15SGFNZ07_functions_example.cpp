/* Siemens M55 LCD (LM15SGFNZ07) examples
 *  E
 *  Mindogas 2011 | markauskas.mindaugas at gmail.com
 *
 *  Copyright (C) 2017 Marcin Sielski
 */

#include <LM15SGFNZ07.h>
/*#include <ctime>
#include <cstdio>
*/
LM15SGFNZ07 lcd(17,22,5,6,26); //configing lcd conectivity

void setup() {
    lcd.initialize();
}

void loop() {
    lcd.clearAll(RED);
    delay(200);
    lcd.clearAll(GREEN);
    delay(200);
    lcd.clearAll(BLUE);
    delay(200);
    lcd.clearAll(BLACK);
    delay(200);
    lcd.clearAll(WHITE);
    delay(200);
    lcd.printString("TestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcd"
                    "TestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcdTestlcd",0, 0, 1,
                    BLACK, NONE);
    delay(2000);
    lcd.clearAll(WHITE);
    delay(200);
    lcd.printString("test lcd",4, 1, 1, BLACK, NONE);
    delay(2000);
    lcd.clearAll(WHITE);
    delay(200);
    lcd.printString("TEST LCD",0, 1, 2, RED, WHITE);
    delay(2000);
    lcd.clearAll(WHITE);
    delay(200);
    lcd.printString("TEST LCD",0, 0, 3, DARK_MAGENTA, WHITE);
    delay(2000);
    lcd.clearAll(WHITE);
    delay(200);
    lcd.printString("TEST",0, 0, 4, VIOLET, WHITE);
    delay(2000);
    lcd.clearAll(WHITE);
    delay(200);
    lcd.drawLine(0, 35, 101, 35,  GREEN);
    delay(2000);
    lcd.clearAll(WHITE);
    delay(200);
    lcd.drawLine(0, 37, 100, 37,  ORANGE);
    delay(2000);
    lcd.clearAll(WHITE);
    delay(200);
    lcd.drawFrame(3, 10, 40, 10, 3, BLUE);
    delay(2000);
    lcd.clearAll(WHITE);
    delay(200);
    lcd.drawArea(45, 10, 40 , 10,  DARK_ORANGE);
    delay(2000);
    lcd.clearAll(WHITE);
    delay(200);
}

int main() {
    setup();
    loop();
    return 0;
}
