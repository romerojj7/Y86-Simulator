/*
    File:   Y86.h
    Desc:   Y86 class declaration 
    
    Author: efb
*/

#ifndef Y86_H
#define Y86_H

#include <string>

#include "Memory.h"
#include "ProgRegisters.h"



class Y86 {

    Memory              memory;          
    ProgRegisters       regs;
    
    /* Private member functions */
    
	/* Public interface */
    public:
        Y86();
        void reset();
	void clockP0();
	void clockP1();
        Memory& getMemory(){return memory;}  			// used to test memory
        ProgRegisters& getProgRegisters(){return regs;} // used to test registers
};

#endif
