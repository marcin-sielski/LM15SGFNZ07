/***********************************************************************************************************************
 * Hazyair.h                      
 * 
 * Header file file Hazyair class.
 * 
 * Copyright (C) 2019 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/
 
#ifndef HAZYAIR_H
#define HAZYAIR_H

#include <jansson.h>
#include "WebService.h"

class Hazyair {
    
    private:
        const char *webEndpoint = "http://%s/hazyair/%s/current";
        WebService endpoint;
        const char *address;
        double temperature;
        double dust;
        double humidity;
        
    public:
        Hazyair(const char *address);
        bool loadData();
        double getTemperature();
        unsigned int getDust();
        unsigned int getHumidity();
        
    private:
        bool requestData(const char *parameter, const char *name, double *result);
        bool requestData(const char *parameter, double *result);
};

#endif

