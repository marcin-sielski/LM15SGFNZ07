/***********************************************************************************************************************
 * OneWire.h                      
 * 
 * Header file for OneWire class.
 * 
 * Copyright (C) 2017 Marcin Sielski
 *  
 * This work is licensed under the terms of the GNU General Public License Version 3.
 *                        
 **********************************************************************************************************************/

#ifndef ONEWIRE_H
#define ONEWIRE_H

class OneWire {
    
    private:
        int pin;
    
    public:
        OneWire(int pin);
        bool initialize();
        double read();
    
};

#endif
