/***********************************************************************************************************************
 * Buffer.h                      
 * 
 * Header file Buffer class.
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

class Buffer {
    
    private:
        char *buffer;
        unsigned int size;
        unsigned int maxSize;
    
    public:
        Buffer(unsigned int maxSize);
        char *getData();
        unsigned int getSize();
        void setSize(unsigned int size);
        unsigned int getMaxSize();
        ~Buffer();
    
};

#endif 
