/*
    File:   DecodeStage.cpp
    Desc:   Decode Stage implementation for Y86-64 simulator

    Author: Javier Romero Hayden Fravel
    
*/
#include "Forward.h"
#include "Y86.h"
#include "DecodeStage.h"

/*
 * Decode Stage Reset method to set Pipeline Registers to their defaults.
 */
void DecodeStage::reset(ExecuteStage *eStage, ProgRegisters *pReg, Forward *frwrd)
{
    executeStage = eStage;
    regs = pReg;
    forward = frwrd;
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    rA.setInput(RNONE);
    rB.setInput(RNONE);
    valC.setInput(0UL);
    valP.setInput(0UL);
}

/*
 * updateDRegister method to update pipeline registers with signals generated in Fetch Stage.
 */
void DecodeStage::updateDRegister(uint64_t f_stat, uint64_t f_icode, uint64_t f_ifun, uint64_t f_rA, uint64_t f_rB, uint64_t f_valC, uint64_t f_valP)
{
    stat.setInput(f_stat);
    icode.setInput(f_icode);
    ifun.setInput(f_ifun);
    rA.setInput(f_rA);
    rB.setInput(f_rB);
    valC.setInput(f_valC);
    valP.setInput(f_valP);
}

/*
 * clockP0 method to clock all Registers.
 */
void DecodeStage::clockP0()
{
    /*
     * Test for a stall or bubble before clocking registers.
     */
    if (stageStall){
        stageStall = false;
        return;
    }
    if (stageBubble){
        stageBubble = false;
        updateDRegister(SBUB, INOP, 0UL, RNONE, RNONE, 0UL, 0UL);
    }
    stat.clock();
    icode.clock();
    ifun.clock();
    rA.clock();
    rB.clock();
    valC.clock();
    valP.clock();
}

/*
 * clockP1 method to link decode stage to Execute Stage using updateERegister method.
 */
void DecodeStage::clockP1()
{
    
    d_stat = stat.getState();
    d_icode = icode.getState();
    d_ifun = ifun.getState();
    d_valC = valC.getState();
    
    /*
     * Produce signals to be used in next Stage.
     */
    d_srcA = getSrcA();
    d_srcB = getSrcB();
    selectFwdA();
    forwardB();

    d_dstE = getDstE();
    d_dstM = getDstM();
    
    executeStage->updateERegister(d_stat, d_icode, d_ifun, d_valC, d_valA, d_valB, d_dstE, d_dstM, d_srcA, d_srcB);
}

/*
 * getSrcA method to determine SrcA.
 */
uint64_t DecodeStage::getSrcA()
{
    switch(d_icode)
    {
        case IPUSHQ:
        case IRRMOVQ:
        case IRMMOVQ:
        case IOPX:
            return rA.getState();
            break;
        case IPOPQ:
        case IRET:
            return RSP;
            break; 
        default:
            return RNONE;
    }
}

/*
 * getSrcB method to determine SrcB.
 */
uint64_t DecodeStage::getSrcB()
{
    switch(d_icode)
    {
        case IRMMOVQ:
        case IMRMOVQ:
        case IOPX:
            return rB.getState();
            break;
        case IPUSHQ:
        case IPOPQ:
        case ICALL:
        case IRET:
            return RSP;
            break;
        default:
            return RNONE;
    }
}

/*
 * getDstE method to determine DstE.
 */
uint64_t DecodeStage::getDstE()
{
    switch(d_icode)
    {
        case IRRMOVQ:
        case IIRMOVQ:
        case IOPX:
            return rB.getState();
            break;
        case IPUSHQ:
        case IPOPQ:
        case ICALL:
        case IRET:
            return RSP;
            break;
        default:
            return RNONE;
    }
}

/*
 * getDstM method to determine DstM.
 */
uint64_t DecodeStage::getDstM()
{
    switch(d_icode)
    {
        case IMRMOVQ:
        case IPOPQ:
            return rA.getState();
            break;
        default:
            return RNONE;
    }
}

/*
 * selectFwdA method to determine valA.
 */
void DecodeStage::selectFwdA()
{
    if (d_icode == IJXX || d_icode == ICALL)
    {
        d_valA = valP.getState();
    }
    else if (getSrcA() == forward->gete_dstE())
    {
        d_valA = forward->gete_valE(); 
    }
    else if (getSrcA() == forward->getM_dstM())
    {
        d_valA = forward->getm_valM(); 
    }
    else if (getSrcA() == forward->getM_dstE())
    {
        d_valA = forward->getM_valE(); 
    }
    else if (getSrcA() == forward->getW_dstM())
    {
        d_valA = forward->getW_valM();
    }
    else if (getSrcA() == forward->getW_dstE())
    {
        d_valA = forward->getW_valE(); 
    }
    else
    {
        d_valA = regs->getReg(d_srcA);
    }

}

/*
 * forwardB method to determine valB.
 */
void DecodeStage::forwardB()
{
    if (getSrcB() == forward->gete_dstE())
    {
        d_valB = forward->gete_valE(); 
    }
    else if (getSrcB() == forward->getM_dstM())
    {
        d_valB = forward->getm_valM(); 
    }
    else if (getSrcB() == forward->getM_dstE())
    {
        d_valB = forward->getM_valE(); 
    }
    else if (getSrcB() == forward->getW_dstM())
    {
        d_valB = forward->getW_valM();
    }
    else if (getSrcB() == forward->getW_dstE())
    {
        d_valB = forward->getW_valE(); 
    }
    else
    {
        d_valB = regs->getReg(d_srcB);
    }

}

