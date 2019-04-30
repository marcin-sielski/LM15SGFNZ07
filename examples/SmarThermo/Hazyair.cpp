/***********************************************************************************************************************
 * Hazyair.cpp                      
 * 
 * Hazyair class implements hazyair interface (https://hazyair.github.io/).
 * 
 * Copyright (C) 2019 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/
 
#include <iostream>
#include "Hazyair.h"

using namespace std;

/**
 * Hazyair constructor.
 * 
 * address - address of hazyair service.
 */
Hazyair::Hazyair(const char *address) {
    
    this->address = address;
    this->temperature = 0;
    this->dust = 0;
    this->humidity = 0;
    
}

/**
 * Load temperature, dust, humidity data.
 * 
 * Returns:
 *     true  - if operation was successfull,
 *     false - otherwise.
 */
bool Hazyair::loadData() {
    
    if (!requestData("temperature", &(this->temperature))) return false;
    if (!requestData("dust", "concentration_pm2.5_normal", &(this->dust))) return false;
    if (!requestData("humidity", &(this->humidity))) return false;

    return true;
}

/**
 * Get temperature. 
 * 
 * Returns:
 *     Temperature.
 */
double Hazyair::getTemperature() {
    
    return this->temperature;
    
}

/**
 * Get dust. 
 * 
 * Returns:
 *     Dust.
 */
unsigned int Hazyair::getDust() {
   
   return (unsigned int)this->dust;
    
}

/**
 * Get humidity. 
 * 
 * Returns:
 *     Humidity.
 */
unsigned int Hazyair::getHumidity() {
    
   return (unsigned int)this->humidity;
   
}

/**
 * Return requested data.
 * 
 * parameter - Parameter to request,
 * name      - Name of the parameter to request,
 * result    - Requested data.
 * 
 * Returns:
 *     true  - if operation was successfull,
 *     false - otherwise.
 */
bool Hazyair::requestData(const char *parameter, const char *name, double *result) {

    char url[128];
    Buffer response(256 * 1024);
    json_error_t error;
    json_t *jsonResponse = NULL;
    json_t *jsonObject = NULL;
    unsigned int timeout = 10000;

    if (this->address == NULL) return false;

    snprintf(url, 128, this->webEndpoint, this->address, parameter);
    while(timeout && !this->endpoint.request(url, &response)) timeout--;
    if (!timeout || response.getSize() <= 2) {
        return false;
    }
    response.getData()[response.getSize()] = 0;
    jsonResponse = json_loads(response.getData(), 0, &error);
    if (!jsonResponse) {
        cerr << "Error: " << error.text << "." << endl;
        return false;
    }
    jsonObject = json_object_get(jsonResponse, name);
    jsonObject = json_object_get(jsonObject, "value");
    if (json_is_real(jsonObject)) {
        *result = json_real_value(jsonObject);
    } else {
        *result = (double)json_integer_value(jsonObject);
    }
    json_decref(jsonResponse);
    
    return true;
    
}

/**
 * Return requested data.
 * 
 * parameter - Parameter to request,
 * result    - Reqested data.
 * 
 * Returns:
 *     true  - if operation was successfull,
 *     false - otherwise.
 */
bool Hazyair::requestData(const char *parameter, double *result) {
    
    return requestData(parameter, parameter, result);
    
}
