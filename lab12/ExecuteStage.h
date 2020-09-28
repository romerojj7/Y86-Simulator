/*
    File:   ExecuteStage.h 
    Desc:   Declares ExecuteStage class and associated constants
 */
#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H


#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "MemoryStage.h"
#include "Forward.h"
#include "ProgRegisters.h"


class ExecuteStage : public PipeStage
{
    Register <uint64_t> valC;
    Register <uint64_t> valA;
    Register <uint64_t> valB;
    Register <uint64_t> dstE;
    Register <uint64_t> dstM;
    Register <uint64_t> srcA;
    Register <uint64_t> srcB;


    MemoryStage *memoryStage;
    Forward *forward;
    ProgRegisters *regs;

    uint64_t e_stat;
    uint64_t e_icode;
    uint64_t e_ifun;
    //uint64_t e_valC;
    bool e_Cnd;
    uint64_t e_valE;
    uint64_t e_valA;
    uint64_t e_dstE;
    uint64_t e_dstM;
    //uint64_t e_srcA;
    //uint64_t e_srcB;

    uint64_t E_valE;
    uint64_t E_valC;
    uint64_t E_dstE;
    
	public:
    void reset(MemoryStage *, ProgRegisters *, Forward *);
    void updateERegister(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
    uint64_t getALUA();
    uint64_t getALUB();
    bool isSetCC();
    uint64_t getALUFunction();
    uint64_t selectDstE();
    void updateCC();
    uint64_t getDstM(){return dstM.getState();}
    bool getCnd(){return e_Cnd;}

    /* (Virtual) Functions of superclass */
	void clockP0();
	void clockP1();
	void trace();
};
#endif
