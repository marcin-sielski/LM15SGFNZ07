/***********************************************************************************************************************
 * 
 * SmarThermo.cpp                      
 * 
 * Main application.
 * 
 * Copyright (C) 2017 Marcin Sielski
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
	char temperature[8];
	char buffer[18];
	double temp = 0;
	unsigned int counter = 0;
	time_t rawtime = 0;
	struct tm *timeinfo = NULL;
	unsigned short bitmap[2500];
	int opt = 0;
	const char *apiId = NULL;
	const char *location = NULL;
	const char *config = NULL;
	unsigned int indentation = 0;

	// Parse arguments
	while ((opt = getopt(argc, argv, "k:l:c:h?")) != -1) {
        switch (opt) {
        case 'k':
            apiId = optarg;
            break;
        case 'l':
            location = optarg;
            break;
        case 'c':
        	config = optarg;
        	break;
        case 'h':
        case '?':
        default: /* '?' */
            cerr << "Usage: " << argv[0] << " [-k ApiId] [-l Location] [-c Config]" << endl;
            exit(EXIT_FAILURE);
        }
    }
    
    // Parse configuration file
    string sApiId, sLocation;
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
    				if (!key.compare("ApiId")) {
    					if (getline(issLine, sApiId)) {
    						apiId = sApiId.c_str();
    					}
    				}
    				if (!key.compare("Location")) {
    					if (getline(issLine, sLocation)) {
    						location = sLocation.c_str();
    					}
    				}
    			}
    		}
    	}
    }
    if (apiId == NULL || location == NULL) {
        cerr << "Usage: " << argv[0] << " [-k ApiId] [-l Location] [-c Config]" << endl;
        exit(EXIT_FAILURE);
    }
	if (signal(SIGINT, signalHandler) == SIG_ERR) {
    	exit(EXIT_FAILURE);	
    }
    if (signal(SIGTERM, signalHandler) == SIG_ERR) {
    	exit(EXIT_FAILURE);	
    }
    
	OneWire thermometer(65);
	OWM service(apiId, location);
	
	lcd.initialize();
	
    displayLogo();
    
	thermometer.initialize();
	temp = thermometer.read();
	while(1) {
		if (!(counter % 60)) {
			service.loadForecastData();
			if (counter == 0) {
				lcd.clearAll(BACKGROUND_COLOR);
			}
			snprintf(temperature, 8, "%.1f C", service.getTemperatureNight());
			service.getImageBitmapNight(BACKGROUND_COLOR, bitmap);
			lcd.drawBitmap(0,31,50,50, bitmap);
			lcd.drawString(temperature, 13, 71, YELLOW, BACKGROUND_COLOR);

			snprintf(temperature, 8, "%.1f C", service.getTemperatureDay());
			service.getImageBitmapDay(BACKGROUND_COLOR, bitmap);
			lcd.drawBitmap(51,31,50,50, bitmap);
			lcd.drawString(temperature, 64, 71, YELLOW, BACKGROUND_COLOR);
			
			lcd.drawLine(0, 39, 100, 39, YELLOW);
			lcd.drawLine(50, 40, 50, 79, YELLOW);
		}
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		if (counter % 2) {
			strftime(buffer, 18, "%H:%M  %d/%m/%Y", timeinfo);
		} else {
			strftime(buffer, 18, "%H %M  %d/%m/%Y", timeinfo);
		}
		lcd.printString(buffer, 0, 0, 1, YELLOW, BACKGROUND_COLOR);
		snprintf(temperature, 8, "%.1f C", temp);
		if (((temp > -10) && (temp < 0)) || (temp >= 10)) {
			indentation = 1;
		} else if (temp >= 0) {
			indentation = 2;
		}
		lcd.printString(temperature, indentation, 1, 2, YELLOW, BACKGROUND_COLOR);
		lcd.printString("O", 11, 2, 1, YELLOW, BACKGROUND_COLOR);
		temp = thermometer.read();
		counter++;
	}
	return 0;
}
