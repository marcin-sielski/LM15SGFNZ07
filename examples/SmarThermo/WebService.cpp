/***********************************************************************************************************************
 * 
 * WebService.cpp                     
 * 
 * WebService class implements interface that helps with interactions with Web Services.
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#include <cstring>
       
#include "WebService.h"

/**
 * WebService contructor. 
 */
WebService::WebService() {

    curl_global_init(CURL_GLOBAL_ALL);
    this->curlHandle = curl_easy_init();

}

/**
 * Sends request to the Web Service.
 * 
 * url    - Web Service endpoint,
 * buffer - Pointer to the Buffer where the retrived data are to be stored,
 * 
 * Returns:
 *          true - if request was successful,
 *          false - otherwise.
 */
bool WebService::request(const char *url, Buffer *buffer) {

    CURLcode returnValue = CURLE_OK;

    do {
        returnValue = curl_easy_setopt(this->curlHandle, CURLOPT_URL, url);
        if (CURLE_OK != returnValue) {
            break;
        }
        returnValue = curl_easy_setopt(this->curlHandle, CURLOPT_WRITEFUNCTION, this->writeCallback);
        if (CURLE_OK != returnValue) {
            break;
        }
        returnValue = curl_easy_setopt(this->curlHandle, CURLOPT_WRITEDATA, (void *)buffer);
        if (CURLE_OK != returnValue) {
            break;
        }
        returnValue = curl_easy_setopt(this->curlHandle, CURLOPT_TIMEOUT, 20L);
        if (CURLE_OK != returnValue) {
            break;
        }
        returnValue = curl_easy_perform(this->curlHandle);
        if (CURLE_OK != returnValue) {
            break;        
        }
    }
    while(0);

    return (CURLE_OK == returnValue ? true : false);

}

/**
 * WebService destructor 
 */
WebService::~WebService() {

    curl_easy_cleanup(this->curlHandle);
    curl_global_cleanup();

}

/**
 * Callback function used to store result of the web query.
 * 
 * ptr      - Pointer to the retrieved data,
 * size     - Size of chunk of retireved data,
 * nmemb    - Number of retrieved chunks,
 * userdata - Pointer to the buffer where the retrieved data are stored,
 * 
 * Returns:
 *            Size of retrieved data.
 */
size_t WebService::writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    
    Buffer *buffer = (Buffer *)userdata;
    
    if (buffer->getSize() + size * nmemb >= buffer->getMaxSize() - 1) {
        return 0;
    }
    memcpy(buffer->getData() + buffer->getSize(), ptr, size * nmemb);
    buffer->setSize(buffer->getSize() + size * nmemb);
    
    return size * nmemb;

}
