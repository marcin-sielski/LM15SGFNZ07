/***********************************************************************************************************************
 * OWM.h                      
 * 
 * Header file file OWM class.
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#ifndef OWM_H
#define OWM_H

#include "WebService.h"

class OWM {
	
	private:
		const char *webEndpoint = "http://api.openweathermap.org/data/2.5/forecast?q=%s&appid=%s&units=Metric";
		const char *imageEndpoint = "http://openweathermap.org/img/w/%s.png";
		WebService endpoint;
		const char *apiId; 
		const char *location;
		double temperatureNight;
		double temperatureDay;
		char iconNight[4];
		char iconDay[4];
		
	public:
		OWM(const char *apiId, const char *location);
		void loadForecastData();
		double getTemperatureNight();
		double getTemperatureDay();
		void getImageBitmapNight(unsigned short color, unsigned short *bitmap); 
		void getImageBitmapDay(unsigned short color, unsigned short *bitmap); 

	private:
		void getImageBitmap(const char *image, unsigned short color, unsigned short *bitmap); 

};

#endif