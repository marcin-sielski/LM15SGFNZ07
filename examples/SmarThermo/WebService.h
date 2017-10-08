/***********************************************************************************************************************
 * 
 * WebService.h                      
 * 
 * Header file for WebService class.
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <curl/curl.h>

#include "Buffer.h"

class WebService {
    
    private:
        CURL *curlHandle;

    public:
        WebService();
        bool request(const char *url, Buffer *buffer);
        ~WebService();
	
    private:
        static size_t writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

};

#endif
