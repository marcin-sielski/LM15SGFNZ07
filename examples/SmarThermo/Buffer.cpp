/***********************************************************************************************************************
 * Buffer.cpp                      
 * 
 * Buffer class.
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#include "Buffer.h"

/**
 * Buffer constructor.
 * 
 * maxSize - Maximal buffer size.
 */
Buffer::Buffer(unsigned int maxSize) {
    this->size = 0;
    this->maxSize = maxSize;
    this->buffer = new char [maxSize];
}

/**
 * Get pointer to the data buffer.
 * 
 * Returns:
 *     Pointer to the data in the buffer. 
 */
char *Buffer::getData() {
    return this->buffer;
}

/**
 * Gets current buffer size.
 * 
 * Returns:
 *     Current buffer size. 
 */
unsigned int Buffer::getSize() {
    return this->size;
}

/**
 * Sets current buffer size to specified value.
 * 
 * size - Size to set for the buffer.
 */
void Buffer::setSize(unsigned int size) {
    this->size = size;
}

/**
 * Gets maximal buffer size. 
 * 
 * Returns:
 *     Maximal buffer size.
 */
unsigned int Buffer::getMaxSize() {
    return this->maxSize;
}

/**
 * Gets buffer data in JSON object.
 * 
 * Returns:
 *     Buffer data in JSON object. 
 */
json_t *Buffer::jsonLoads() {
    json_error_t error;
    this->buffer[this->size] = 0;
    return json_loads(this->buffer, 0, &error);
}

/**
 * Buffer destructor. 
 */
Buffer::~Buffer() {
    delete [] this->buffer;
}
