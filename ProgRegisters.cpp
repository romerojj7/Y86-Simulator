/*
    Name:       ProgRegisters.cpp
    
    Desc:       Implements functions for the program register file.
    
    Authors:    Javier Romero Hayden Fravel    
    
    Date:       2/9/2020       
*/
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>

#include "ProgRegisters.h"
#include "Tools.h"


/*-------------------------------------------------------------------------------------------------
    ProgRegisters constructor  
--------------------------------------------------------------------------------------------------*/
ProgRegisters::ProgRegisters()
{
    reset();
}

/*-------------------------------------------------------------------------------------------------
    clock  - calls clock method of each register and also the CC register
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: clock()
{
	// your code here
    CC.clock();
    for (int x = 0; x < NUM_REGISTERS; x++)
    {
        reg[x].clock();
    }
} 

/*-------------------------------------------------------------------------------------------------
    setReg - sets the INPUT of the specified register
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: setReg(unsigned regNum, uint64_t rval)
{
	// your code here
    assert(regNum < 16);
    if (regNum == 15)
    {
        return;
    }
    reg[regNum].setInput(rval); 
}
/*-------------------------------------------------------------------------------------------------
    getReg - returns STATE of specified register
--------------------------------------------------------------------------------------------------*/
uint64_t ProgRegisters:: getReg(unsigned regNum)
{
	// your code here
    assert(regNum < 16);
    if (regNum == 15)
    {
        return 0;
    }
    else
        return reg[regNum].getState(); 
}       
/*-------------------------------------------------------------------------------------------------
    setCC - Sets the INPUT of the specified CC bit to the specified value (0 or 1)
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: setCC(unsigned bitNumber, unsigned val)
{
	// your code here
    assert(bitNumber < 8 && (val == 0 || val == 1));
    unsigned flags = CC.getInput();
    flags = Tools::assignOneBit(bitNumber,val,flags);
    CC.setInput(flags);
}

/*-------------------------------------------------------------------------------------------------
    getCC - returns the STATE of the specified CC bit
--------------------------------------------------------------------------------------------------*/
unsigned ProgRegisters:: getCC(unsigned bitNumber)
{
	// your code here
    assert(bitNumber < 8);  
    return Tools::getBits(bitNumber, bitNumber, CC.getState()); 
}
          
/*-------------------------------------------------------------------------------------------------
    reset - resets registers and CC flags
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: reset(void)
{
	// your code here
    for (int x = 0; x < NUM_REGISTERS; x++)
    {
        reg[x].reset();
    }
    CC.reset(4);
} 
