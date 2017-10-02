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
 *     Forecast temperature. 
 */
bool OWM::loadForecastData() {

    char url[128];
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
    unsigned int timeout = 0;
    
    snprintf(url, 128, webEndpoint, this->location, this->apiId);
    while(!endpoint.requestJson(url, &jsonResponse) && timeout < 10000) timeout++;
    if (timeout == 10000) {
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

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
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
 */
void OWM::getImageBitmapNight(unsigned short color, unsigned short *bitmap) {

    this->getImageBitmap(this->iconNight, color, bitmap);

}

/**
 * Return bitmap of the day image.
 * 
 * color - Background color,
 * bitmap - Pointer to the buffer where the bitmap is to be stored.
 */
void OWM::getImageBitmapDay(unsigned short color, unsigned short *bitmap) {
    
    this->getImageBitmap(this->iconDay, color, bitmap);

}


/**
 * Return bitmap of the image.
 * 
 * image  - Image to get,
 * color  - Background color,
 * bitmap - Pointer to the buffer where the bitmap is to be stored.
 */
void OWM::getImageBitmap(const char *image, unsigned short color, unsigned short *bitmap) {
    
    char url[128];
    Buffer fileResponse(256 * 1024);
    int fd = 0;
    int result = 0;
    char *data = NULL;
    FILE *fp = NULL;
    png_structp png;
    png_infop info;
    int width = 0;
    int height = 0;
    png_byte colorType;
    png_byte bitDepth;
    png_bytep *rowPointers = NULL;
    png_bytep row;
    png_bytep px;
    png_byte alpha;
    
    snprintf(url, 128, imageEndpoint, image);
    while(!endpoint.requestFile(url, &fileResponse));
    do {
        fd = shm_open("icon", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if (-1 == fd) {
            cerr << "Error: Failed to open shared memory file (errno = " << errno << ")." << endl;
            break;
        }
        result = fallocate(fd, 0, 0, fileResponse.getSize());
        if (-1 == result) {
            cerr << "Error: Failed to allocate shared memory file (errno = " << errno << ")." << endl;
            break;
        }
        data = (char *)mmap(NULL, fileResponse.getSize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (-1 == *data) {
            cerr << "Error: Failed to map shared memory file (errno = " << errno << ")." << endl;
            break;
        }
        memcpy(data, fileResponse.getData(), fileResponse.getSize());
        fp = fdopen(fd, "rwb");
        if (!fp) {
            cerr << "Error: Failed to open file (errno = " << errno << ")." << endl;
            break;
        }
        png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png) {
            cerr << "Error: Failed to create PNG structure." << endl;
            break;
        }
        info = png_create_info_struct(png);
        if (!info) {
            cerr << "Error: Failed to create info structure." << endl;
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
                                     ((0xF - alpha) * ((color & 0xF00) >> 8) / 0xF)) |
                                     ((((px[1] >> 4) * alpha / 0xF) << 4) +
                                     ((0xF - alpha) * ((color & 0xF0) >> 4) / 0xF)) |
                                     (((px[2] >> 4) * alpha / 0xF) + ((0xF - alpha) * (color & 0xF) / 0xF));
            }
        }
    }
    while(0);
    for(int y = 0; y < height; y++) {
        if (!rowPointers[y]) {
            free(rowPointers[y]);
        }
    }
    if (!rowPointers) {
        free(rowPointers);
    }
    if (!fp) {
        fclose(fp);
    }
    if (-1 != *data) {
        munmap(data, fileResponse.getSize());
    }
    if (-1 != fd) {
        shm_unlink("icon");
    }
}
