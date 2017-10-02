/***********************************************************************************************************************
 * OneWire.cpp                      
 * 
 * OneWire class implements 1-Wire interface.
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#include <dirent.h>
#include <cstring>
#include <wiringPi.h>
#include <ds18b20.h>

#include "OneWire.h"

/**
 * OneWire constructor.
 * 
 * pin - Pin number where 1-Wire device sends the data.
 */
OneWire::OneWire(int pin) {
    this->pin = pin;    
}

/**
 * Initializes 1-Wire interface.
 */
void OneWire::initialize() {

    DIR *dir;
    struct dirent *dirent;
    char dev[16];      // Dev ID
    wiringPiSetupGpio();
    dir = opendir("/sys/bus/w1/devices");
    if (dir != NULL) {
        while ((dirent = readdir (dir))) {
            // 1-wire devices are links beginning with 28-, find first
            if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-") != NULL) { 
                strcpy(dev, dirent->d_name);
                break;
            }
        }
        (void) closedir (dir);
    } else {
        return;
    }
    ds18b20Setup(this->pin, (const char *)(dev + 3));
}

/**
 * Retrives the data from 1-Wire device.
 * 
 * Returns:
 *     Data from 1-Wire device.
 */
double OneWire::read() {

    return ((double)analogRead(this->pin))/10;

}
