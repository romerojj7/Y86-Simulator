/*
    File:   FetchStage.h
    Desc:   Declares FetchStage class and associated constants
    
*/
#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H


#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "DecodeStage.h"
#include "Forward.h"

class FetchStage : public PipeStage
{

    /* Register state */
    Register <uint64_t> predPC;    // Predicted PC value stored in F Register
	
    /* Pointers to Decode Stage and Memory object */
    DecodeStage *decodeStage;
    Memory 		*memory;
	Forward     *forward;
	
    /* signals produced within the stage */
    uint64_t f_PC;
    bool needsValC;
    bool needsRegs;
    bool instrValid;
    uint64_t f_stat;
    uint64_t f_icode;
    uint64_t f_ifun;
    uint64_t f_rA;
    uint64_t f_rB;
    uint64_t f_valC;
    uint64_t f_valP;

	/* Example private methods - you may implement your own, and there will be others... */
    bool getByte0();         // Internal to Fetch Stage
    bool checkNeedsRegs();   // Internal to Fetch Stage
    bool checkNeedsValC();   // Internal to Fetch Stage
    bool align();            // Internal to Fetch Stage returns true if memory access OK
    bool checkInstrValid();  
    int getPCIncrement();
    void predictPC();
    uint64_t selectPC();
	
	public:
		void reset(DecodeStage *, Memory *, Forward *);

		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
    
    
};

#endif
