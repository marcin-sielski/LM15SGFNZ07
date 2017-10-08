/***********************************************************************************************************************
 * OWM.cpp                      
 * 
 * OWM class implements Open Weather Map interface (https://openweathermap.org/).
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>
#include <cstring>
#include <png.h>
#include <iostream>
#include <unistd.h>
#include <jansson.h>

#include "OWM.h"

using namespace std;

/**
 * OWM constructor.
 * 
 * apiId    - ApiId key used to authorize the requests to the Open Weather Map service.
 * location - Location 
 */
OWM::OWM(const char *apiId, const char *location) {

    this->apiId = apiId;
    this->location = location;
    this->temperatureNight = 0;
    this->temperatureDay = 0;
    this->iconNight[0] = 0;
    this->iconDay[0] = 0;

}

/**
 * Load forecast temperature.
 * 
 * Returns:
 *     true  - if operation was successfull,
 *     false - otherwise.
 */
bool OWM::loadForecastData() {

    char url[128];
    Buffer response(256 * 1024);
    json_error_t error;
    json_t *jsonResponse = NULL;
    json_t *jsonList = NULL;
    json_t *jsonElement = NULL;
    json_t *jsonObject = NULL;
    size_t jsonListSize = 0;
    time_t dt = 0;
    struct tm *timeInfo = NULL;
    double returnValue = 0;
    int tm_yday = 0;
    int tm_year = 0;
    time_t rawTime = 0;
    size_t i = 0;
    unsigned int timeout = 10000;
    
    snprintf(url, 128, webEndpoint, this->location, this->apiId);
    while(timeout && !endpoint.request(url, &response)) timeout--;
    if (!timeout) {
        return false;
    }
    response.getData()[response.getSize()] = 0;
    jsonResponse = json_loads(response.getData(), 0, &error);
    if (!jsonResponse) {
        cerr << "Error: " << error.text << "." << endl;
        return false;
    }
    
    jsonList = json_object_get(jsonResponse, "list");
    jsonListSize = json_array_size(jsonList);
    
    rawTime = time(NULL);
    timeInfo = localtime(&rawTime);
    tm_yday = timeInfo->tm_yday;
    tm_year = timeInfo->tm_year;

    for (; i < jsonListSize; i++) {
        jsonElement = json_array_get(jsonList, i);
        jsonObject = json_object_get(jsonElement, "dt");
        dt = json_integer_value(jsonObject);
        timeInfo = localtime(&dt);
        if (((timeInfo->tm_yday > tm_yday) || (timeInfo->tm_year > tm_year)) && (timeInfo->tm_hour >= 0)) {
            break;
        }
    }
    jsonObject = json_object_get(jsonElement, "main");
    jsonObject = json_object_get(jsonObject, "temp");
    this->temperatureNight = json_number_value(jsonObject);
    jsonObject = json_object_get(jsonElement, "weather");
    jsonElement = json_array_get(jsonObject, 0);
    jsonObject = json_object_get(jsonElement, "icon");
    snprintf(this->iconNight, 4, json_string_value(jsonObject));

    for (; i < jsonListSize; i++) {
        jsonElement = json_array_get(jsonList, i);
        jsonObject = json_object_get(jsonElement, "dt");
        dt = json_integer_value(jsonObject);
        timeInfo = localtime(&dt);
        if (((timeInfo->tm_yday > tm_yday) || (timeInfo->tm_year > tm_year)) && (timeInfo->tm_hour >= 12)) {
            break;
        }
    }
    jsonObject = json_object_get(jsonElement, "main");
    jsonObject = json_object_get(jsonObject, "temp");
    this->temperatureDay = json_number_value(jsonObject);
    jsonObject = json_object_get(jsonElement, "weather");
    jsonElement = json_array_get(jsonObject, 0);
    jsonObject = json_object_get(jsonElement, "icon");
    snprintf(this->iconDay, 4, json_string_value(jsonObject));
    
    json_decref(jsonResponse);

    return true;

}

/**
 * Get temperature at night. 
 * 
 * Returns:
 *     Temperature at night.
 */
double OWM::getTemperatureNight() {
    
    return this->temperatureNight;
    
}


/**
 * Get temperature during the day.
 * 
 * Returns:
 *     Temperature during the day.
 */
double OWM::getTemperatureDay() {
    
    return this->temperatureDay;
    
}

/**
 * Return bitmap of the night image.
 * 
 * color  - Background color,
 * bitmap - Pointer to the buffer where the bitmap is to be stored.
 * 
 * Returns:
 *     true  - if operation was successfull,
 *     false - otherwise.
 */
bool OWM::getImageBitmapNight(unsigned short color, unsigned short *bitmap) {

    return this->getImageBitmap(this->iconNight, color, bitmap);

}

/**
 * Return bitmap of the day image.
 * 
 * color - Background color,
 * bitmap - Pointer to the buffer where the bitmap is to be stored.
 * 
 * Returns:
 *     true  - if operation was successfull,
 *     false - otherwise.
 */
bool OWM::getImageBitmapDay(unsigned short color, unsigned short *bitmap) {
    
    return this->getImageBitmap(this->iconDay, color, bitmap);

}


/**
 * Return bitmap of the image.
 * 
 * image  - Image to get,
 * color  - Background color,
 * bitmap - Pointer to the buffer where the bitmap is to be stored.
 * 
 * Returns:
 *     true  - if operation was successfull,
 *     false - otherwise.
 */
bool OWM::getImageBitmap(const char *image, unsigned short color, unsigned short *bitmap) {
    
    char url[128];
    Buffer response(256 * 1024);
    int fd = 0;
    int result = 0;
    char *data = NULL;
    FILE *fp = NULL;
    unsigned char header[8];
    png_structp png = NULL;
    png_infop info = NULL;
    int width = 0;
    int height = 0;
    png_byte colorType;
    png_byte bitDepth;
    png_bytep *rowPointers = NULL;
    png_bytep row = NULL;
    png_bytep px = NULL;
    png_byte alpha;
    bool returnValue = true;
    unsigned int timeout = 10000;
    
    if (!image) {
        return false;
    }
    if (!bitmap) {
        return false;
    }
    
    snprintf(url, 128, imageEndpoint, image);
    while(timeout && !endpoint.request(url, &response)) timeout --;
    if (!timeout) {
        return false;
    }
    
    do {
        fd = shm_open("icon", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if (-1 == fd) {
            cerr << "Error: Failed to open shared memory file (errno = " << errno << ")." << endl;
            returnValue = false;
            break;
        }
        result = ftruncate(fd, response.getSize());
        if (-1 == result) {
            cerr << "Error: Failed to allocate shared memory file (errno = " << errno << ")." << endl;
            returnValue = false;
            break;
        }
        data = (char *)mmap(NULL, response.getSize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (-1 == *data) {
            cerr << "Error: Failed to map shared memory file (errno = " << errno << ")." << endl;
            returnValue = false;
            break;
        }
        memcpy(data, response.getData(), response.getSize());
        fp = fdopen(fd, "rwb");
        if (!fp) {
            cerr << "Error: Failed to open file (errno = " << errno << ")." << endl;
            returnValue = false;
            break;
        }
        fread((void *)header, 1, 8, fp);
        if (png_sig_cmp((const unsigned char*)header, 0, 8)) {
            cerr << "Error: Not a PNG file." << endl;
            returnValue = false;
            break;
        }
        png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png) {
            cerr << "Error: Failed to create PNG structure." << endl;
            returnValue = false;
            break;
        }
        png_set_sig_bytes(png, 8);
        info = png_create_info_struct(png);
        if (!info) {
            cerr << "Error: Failed to create info structure." << endl;
            returnValue = false;
            break;
        }
        png_init_io(png, fp);
        png_read_info(png, info);
        width = png_get_image_width(png, info);
        height = png_get_image_height(png, info);
        colorType = png_get_color_type(png, info);
        bitDepth  = png_get_bit_depth(png, info);
        rowPointers = (png_bytep*)calloc(height, sizeof(png_bytep));
        for(int y = 0; y < height; y++) {
            rowPointers[y] = (png_byte*)calloc(png_get_rowbytes(png,info), sizeof(png_byte));
        }
        png_read_image(png, rowPointers);
        for(int y = 0; y < height; y++) {
            row = rowPointers[y];
            for(int x = 0; x < width; x++) {
                px = &(row[x * 4]);
                alpha = (px[3] >> 4);
                bitmap[x+y*height] = ((((px[0] >> 4) * alpha / 0xF) << 8) +
                                     (((0xF - alpha) * ((color & 0xF00) >> 8) / 0xF)) << 8) |
                                     ((((px[1] >> 4) * alpha / 0xF) << 4) +
                                     (((0xF - alpha) * ((color & 0xF0) >> 4) / 0xF)) << 4)|
                                     (((px[2] >> 4) * alpha / 0xF) + ((0xF - alpha) * (color & 0xF) / 0xF));
            }
        }
    }
    while(0);
    if (rowPointers) {
        for(int y = 0; y < height; y++) {
            if (rowPointers[y]) {
                free(rowPointers[y]);
            }
        }
        free(rowPointers);
    }
    if (!info) {
        if (png) {
            png_destroy_read_struct(&png, NULL, NULL);
            png = NULL;
        }
    } else if (png) {
        png_destroy_read_struct(&png, &info, NULL);
        png = NULL;
        info = NULL;
    }
    if (fp) {
        fclose(fp);
        fp = NULL;
    }
    if (-1 != *data) {
        munmap(data, response.getSize());
    }
    if (-1 != fd) {
        shm_unlink("icon");
        close(fd);
    }
    
    return returnValue;
    
}
