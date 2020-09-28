/*
    Name:   Memory.cpp
    Desc:   Implementation of the Memory class
    Auth:   Javier Romero Hayden Fravel 
    Date:   2/5/2020   
*/
#include <iostream>
#include <iomanip>
#include <cstdint>

#include "Memory.h"
#include "Tools.h"


/*-------------------------------------------------------------------------
  Constructor:  Memory 
  Description:  Store 64-bit value to a word address in memory. Set
                memError if address out of bounds.
--------------------------------------------------------------------------*/
Memory::Memory()
{
    reset();
}
/*-------------------------------------------------------------------------
  Function:     store 
  Description:  Store 64-bit value to a word address in memory. Set
                memError if address out of bounds.
--------------------------------------------------------------------------*/
void   Memory::store	(uint64_t waddr, uint64_t val) // takes word address
{
    if (waddr >= 1024)
    {
        memError = true;
        return;
    }
    else
        memError = false;

    mem[waddr] = val;
}
/*-------------------------------------------------------------------------
  Function:     fetch 
  Description:  Fetch a 64-bit value from a word address in memory. Set
                memError if address out of bounds.
--------------------------------------------------------------------------*/
uint64_t  Memory::fetch	(uint64_t waddr) // takes word address
{
	// your code here
    if (waddr >= 1024)
    {
        memError = true;
    }
    else
        memError = false;
        
     return mem[waddr]; 
}
/*--------------------------------------------------------------------
   Function:   getByte

   Description:  Returns a byte from the specified byte address
                 of Y86 memory array. 
                 If address is out of range the error status is set.
--------------------------------------------------------------------*/
unsigned char   Memory::getByte	(uint64_t byteAddress) // takes byte address
{
	// your code here
    if (byteAddress >= 8192)
    {
        memError = true;
        return 0;
    }
    else
        memError = false;
    
    uint64_t word = fetch(byteAddress >> 3);         
    return Tools::getByteNumber(Tools::getBits(0,2,byteAddress), word);
    
         
}
/*--------------------------------------------------------------------
   Function:     putByte

   Description:  Write a single byte into the Y86 simulated memory
                 at the byte address specified by 'address'.
                 If address is out of range the error status is set.                 
--------------------------------------------------------------------*/
void  Memory::putByte(uint64_t byteAddress, uint8_t value) // takes byte address
{
	// your code here
    if (byteAddress >= 8192)
    {
        memError = true;
        return;
    }
    else
        memError = false;
    
    uint64_t word = fetch(byteAddress >> 3);
    word = Tools::putByteNumber(Tools::getBits(0,2,byteAddress),value,word);
    store(byteAddress >> 3, word);
}
/*--------------------------------------------------------------------
   Function:   getWord

   Description:  Returns word starting from the specified byte address.
                 No part of the word must lie outside memory range.
--------------------------------------------------------------------*/
uint64_t   Memory::getWord	(uint64_t byteAddress)	
{
	// your code here
    if (byteAddress >= 8192)
    {
        memError = true;
        return 0UL;
    }
    else
        memError = false;
    
    unsigned char bytes[8];

    for (int x = 0; x < 8; x++)
    {
        bytes[x] = getByte(byteAddress + x);
    }
    uint64_t word = Tools::buildWord(bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7]);
    return word;
}
/*----------------------------------------------------------------------------------------------
   Function:   putWord

   Description:  Writes a word to memory starting at the specified byte address.
                 No part of the word must lie outside memory range. If there is
				 a memory error (out of range) the memory should not be changed.
------------------------------------------------------------------------------------------------*/
void Memory::putWord	(uint64_t byteAddress, uint64_t wordValue) 
{
	// your code here
    if (byteAddress >= 8184)
    {
        memError = true;
        return;
    }
    else
        memError = false;

    for (int x = 0; x < 8; x++)
    {
        putByte(byteAddress + x, Tools::getByteNumber(x, wordValue));
    }
}
/*--------------------------------------------------------------------
   Function:   reset

   Description:  Clears memory to all zero. Clears error status.
                 
--------------------------------------------------------------------*/
void  Memory::reset	(void) // clears memory to all zero
{
	// your code here
    for (int x = 0; x < MEMORY_SIZE; x++)
    {
        mem[x] = 0;
    }
    memError = false;
}
