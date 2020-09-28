/*
    File:   DecodeStage.h 
    Desc:   Declares DecodeStage class and associated constants
 */
#ifndef DECODESTAGE_H
#define DECODESTAGE_H


#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "ExecuteStage.h"
#include "Forward.h"


class DecodeStage : public PipeStage
{
    Register <uint64_t> rA;
    Register <uint64_t> rB;
    Register <uint64_t> valC;
    Register <uint64_t> valP; 

    ExecuteStage *executeStage;
    ProgRegisters *regs;
    Forward *forward;

    uint64_t d_stat;
    uint64_t d_icode;
    uint64_t d_ifun;
    //uint64_t d_rA;
    //uint64_t d_rB;
    uint64_t d_valC;
    uint64_t d_valA;
    uint64_t d_valB;
    uint64_t d_dstE;
    uint64_t d_dstM;
    uint64_t d_srcA;
    uint64_t d_srcB;

    uint64_t D_rA;
    uint64_t D_rB;

    
	public:
    void reset(ExecuteStage *, ProgRegisters *, Forward *);
    void updateDRegister(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
    uint64_t getSrcA();
    uint64_t getSrcB();
    uint64_t getDstE();
    uint64_t getDstM();
    void selectFwdA();
    void forwardB();
    
    /* (Virtual) Functions of superclass */
	void clockP0();
	void clockP1();
	void trace();
};
#endif
