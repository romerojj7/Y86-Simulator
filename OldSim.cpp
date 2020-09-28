/*
    File:   main.cpp
    Desc:   The main program. Accepts and verifies parameters and executes simulator
            on the specified object file. Other parameters are used to specify the stage or
            stages where trace outputs will be printed. Trace outputs may include the 
            stage register contents, memory contents, and/or register contents. Trace outputs
            are printed at the end of a cycle.
            
            Best way to capture internal state during simulation?
            Previous simulator used a special dump instruction. This requires
            adding this instruction to the instruction set. Dump routines were
            provided that allowed the dumping of program state during WRITEBACK stage.
            
*/
#include <iostream>
#include <iomanip>
#include <string>
#include "Sim.h"
#include "Y86.h"
#include "Memory.h"
#include "ProgRegisters.h"




Y86 y86;  // Declare global Y86 object. 

int main(int argc, char *argv[])
{


   y86.reset();
   Memory m = y86.getMemory();
   ProgRegisters reg = y86.getProgRegisters();

    // Test your Memory (m) and ProgRegisters (reg) functions...


    std::cout << "\nAOK.\n";
    
}


