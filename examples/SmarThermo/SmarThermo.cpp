/***********************************************************************************************************************
 * 
 * SmarThermo.cpp                      
 * 
 * Main application.
 * 
 * Copyright (C) 2019 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include <csignal>
#include <unistd.h>

#include <LM15SGFNZ07.h>
#include "OneWire.h"
#include "OWM.h"
#include "Hazyair.h"

using namespace std;

#define BACKGROUND_COLOR DARK_BLUE

LM15SGFNZ07 lcd(17, 22, 5, 6, 26);

static void displayLogo() {
    lcd.initialize();
    lcd.clearAll(BACKGROUND_COLOR);
    lcd.printString("Smar", 0, 1, 2, YELLOW, BACKGROUND_COLOR);
    lcd.printString("Thermo", 2, 2, 2, YELLOW, BACKGROUND_COLOR);
    lcd.printString("by Marcin Sielski", 0, 7, 1, RED, BACKGROUND_COLOR);
}

static void signalHandler(int signo) {
    displayLogo();
    exit(EXIT_SUCCESS);
}

int main (int argc, char *argv[]) {
    
    char url[256];
    char output[8];
    double temperature = 0;
    char buffer[18];
    unsigned int counter = 0;
    time_t rawtime = 0;
    struct tm *timeinfo = NULL;
    unsigned short bitmap[2500];
    int opt = 0;
    const char *owmApiKey = NULL;
    const char *owmLocation = NULL;
    const char *hazyairAddress = NULL;
    const char *config = NULL;
    bool external = true;
    bool result = false;
    unsigned int dust = 0;

    // Parse arguments
    while ((opt = getopt(argc, argv, "k:l:a:c:h?")) != -1) {
        switch (opt) {
        case 'k':
            owmApiKey = optarg;
            break;
        case 'l':
            owmLocation = optarg;
            break;
        case 'a':
            hazyairAddress = optarg;
            break;
        case 'c':
            config = optarg;
            break;
        case 'h':
        case '?':
        default: /* '?' */
            cerr << "Usage: " << argv[0] << " [-k OWMApiKey] [-l OWMLocation] [-a HazyairAddress] [-c Config]" << endl;
            exit(EXIT_FAILURE);
        }
    }
    
    // Parse configuration file
    string sOWMApiKey, sOWMLocation, sHazyairAddress;
    if (config != NULL) {
        ifstream configFile(config);
        if (configFile) {
            stringstream ssFile;
            ssFile << configFile.rdbuf();
            configFile.close();
            string line;
            while(getline(ssFile, line)) {
                istringstream issLine(line);
                string key;
                if (getline(issLine, key, ' ')) {
                    if (!key.compare("OWMApiKey")) {
                        if (getline(issLine, sOWMApiKey)) {
                            owmApiKey = sOWMApiKey.c_str();
                        }
                    }
                    if (!key.compare("OWMLocation")) {
                        if (getline(issLine, sOWMLocation)) {
                            owmLocation = sOWMLocation.c_str();
                        }
                    }
                    if (!key.compare("HazyairAddress")) {
                        if (getline(issLine, sHazyairAddress)) {
                            hazyairAddress = sHazyairAddress.c_str();
                        }
                    }
                }
            }
        }
    }
    if (owmApiKey == NULL || owmLocation == NULL) {
        cerr << "Usage: " << argv[0] << " [-k OWMApiKey] [-l OWMLocation] [-a HazyairAddress] [-c Config]" << endl;
        exit(EXIT_FAILURE);
    }
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        cerr << "Error: Failed to hook up SIGINT handler." << endl;
        exit(EXIT_FAILURE);    
    }
    if (signal(SIGTERM, signalHandler) == SIG_ERR) {
        cerr << "Error: Failed to hook up SIGINT handler." << endl;
        exit(EXIT_FAILURE);    
    }
    
    OneWire thermometer(65);
    OWM owm(owmApiKey, owmLocation);
    Hazyair hazyair(hazyairAddress);
    
    displayLogo();
    
    if (!thermometer.initialize()) {
        cerr << "Error: Failed to initialize thermometer." << endl;
        exit(EXIT_FAILURE);
    }

    temperature = thermometer.read();

    while(1) {
        
        if ((hazyairAddress != NULL) && !(counter % 10) && counter != 0) external = !external;
        if (external) {
            
            // forecast
            if (!(counter % 10)) {
                result = owm.loadForecastData();

                // clean
                if (hazyairAddress == NULL) {
                    if (!(counter % 3600)) {
                        lcd.clearAll(BACKGROUND_COLOR);
                    }
                } else lcd.clearAll(BACKGROUND_COLOR);
                
                if (result) {
                    if (owm.getImageBitmapNight(BACKGROUND_COLOR, bitmap)) {
                        lcd.drawBitmap(0,31,50,50, bitmap);
                    }
                    snprintf(output, 8, "%.1f C", owm.getTemperatureNight());
                    lcd.drawString(output, 13, 71, YELLOW, BACKGROUND_COLOR);

                    if (owm.getImageBitmapDay(BACKGROUND_COLOR, bitmap)) {
                        lcd.drawBitmap(51,31,50,50, bitmap);
                    }
                    snprintf(output, 8, "%.1f C", owm.getTemperatureDay());
                    lcd.drawString(output, 64, 71, YELLOW, BACKGROUND_COLOR);
            
                    lcd.drawLine(0, 39, 100, 39, YELLOW);
                    lcd.drawLine(50, 40, 50, 79, YELLOW);
                }
            }

            // time
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            if (counter % 2) {
                strftime(buffer, 18, "%H:%M  %d/%m/%Y", timeinfo);
            } else {
                strftime(buffer, 18, "%H %M  %d/%m/%Y", timeinfo);
            }
            lcd.printString(buffer, 0, 0, 1, YELLOW, BACKGROUND_COLOR);

            // temperature
            snprintf(output, 8, "%5.1f C", temperature);
            lcd.printString(output, 0, 1, 2, YELLOW, BACKGROUND_COLOR);
            lcd.printString("O", 11, 2, 1, YELLOW, BACKGROUND_COLOR);
            temperature = thermometer.read();

        } else if (hazyair.loadData()) {
            
            if (!(counter % 10)) lcd.clearAll(BACKGROUND_COLOR);

            // time
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            if (counter % 2) {
                strftime(buffer, 18, "%H:%M  %d/%m/%Y", timeinfo);
            } else {
                strftime(buffer, 18, "%H %M  %d/%m/%Y", timeinfo);
            }
            lcd.printString(buffer, 0, 0, 1, YELLOW, BACKGROUND_COLOR);

            
            snprintf(output, 8, "%5.1f C", hazyair.getTemperature());
            lcd.printString(output, 0, 1, 2, YELLOW, BACKGROUND_COLOR);
            lcd.printString("O", 11, 2, 1, YELLOW, BACKGROUND_COLOR);
            
            lcd.drawLine(0, 39, 100, 39, YELLOW);
            lcd.drawLine(50, 40, 50, 79, YELLOW);

            dust = hazyair.getDust();
            snprintf(output, 8, "%3d ", dust);
            lcd.printString(output, 0, 3, 2, (dust <= 13 ? 0x0C0 : (dust <= 37 ? LIME : (dust <= 61 ? YELLOW :
            (dust <= 85 ? OLIVE : (dust <= 121 ? RED : DARK_RED ))))), BACKGROUND_COLOR);
            lcd.drawString("ug/m^3", 10, 71, YELLOW, BACKGROUND_COLOR);
            
            snprintf(output, 8, "%2d", hazyair.getHumidity());
            lcd.printString(output, 5, 3, 2, YELLOW, BACKGROUND_COLOR);
            lcd.drawString("%", 70, 71, YELLOW, BACKGROUND_COLOR);
            
            usleep(800000);
               
        } else external = false;

        counter++;
    }
    return 0;
}
