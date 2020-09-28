/*
    File:   MemoryStage.h 
    Desc:   Declares MemoryStage class and associated constants
 */
#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H


#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "WritebackStage.h"
#include "Forward.h"


class MemoryStage : public PipeStage
{
    Register <bool> cnd;
    Register <uint64_t> valA;
    Register <uint64_t> valE;
    Register <uint64_t> dstM;
    Register <uint64_t> dstE;


    WritebackStage *writebackStage;
    Memory *memory;
    Forward *forward;

    uint64_t m_stat;
    uint64_t m_icode;
    uint64_t m_ifun;
    //uint64_t m_cond;
    //uint64_t m_valA;
    uint64_t m_valE;
    uint64_t m_valM;
    uint64_t m_dstM;
    uint64_t m_dstE;
    
	public:
    void reset(WritebackStage *, Memory *, Forward *);
    void updateMRegister(uint64_t, uint64_t, uint64_t, bool, uint64_t, uint64_t, uint64_t, uint64_t);
    uint64_t getMemAddr();
    
    /* (Virtual) Functions of superclass */
	void clockP0();
	void clockP1();
	void trace();
};
#endif
