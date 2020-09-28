/*
    File:   Y86.h
    Desc:   Y86 constants 
    
    Author: efb
*/

#ifndef Y86_H
#define Y86_H

#include <iostream>
#include <cstdint>
#include <string>
#include "Memory.h"
#include "ProgRegisters.h"
#include "FetchStage.h"
#include "Forward.h"
#include "Tools.h"

class PipeStage;  // Forward declare because of PipeStage reference array

class Y86 {

    Memory              memory;          
    ProgRegisters       regs;

	Forward             forward;
    FetchStage          fetchStage;
    DecodeStage         decodeStage;
    ExecuteStage        executeStage;
    MemoryStage         memoryStage;
    WritebackStage      writebackStage;

    PipeStage *stage[5];
    int cycles;


    /* Private member functions */
	bool 		readFile		(std::ifstream& infile);
	int 		writeMemory		(std::string inst, uint64_t address);
    void 		getLine			(uint64_t *, uint64_t);
    void        storeByte       (std::string byte, int addr);
    void        storeData       (std::string line);
    bool	    checkLine       (std::string line, int, int);
	std::string getFlagsString	(void); // in Y86trace.cpp
	
    /* Public member functions */
    public:
        Y86();
        void reset();
        void clock();
		void clockP0();
		void clockP1();

        void setTrace(bool f, bool d, bool e, bool m, bool w);
        void trace(void);
        uint64_t getStat();
        int getCycles(){return cycles;};
		bool load(char *fname);  // takes name of a file as a C string
		void dumpMemory();			
		void dumpProgramRegisters();
		void dumpProcessorRegisters();
		void pipeControl();

        Memory& getMemory(){return memory;}  
        ProgRegisters& getProgRegisters(){return regs;}
};

#endif
