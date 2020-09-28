/*
    File:   ExecuteStage.cpp
    Desc:   Execute Stage implementation for Y86-64 simulator

    Author: Javier Romero Hayden Fravel
    
*/
#include "Forward.h"
#include "Y86.h"
#include "ExecuteStage.h"
#include "ProgRegisters.h"
#include <iostream>

/*
 * Execute Stage Reset method to set Pipeline Registers to their defaults.
 */
void ExecuteStage::reset(MemoryStage *mStage, ProgRegisters *pregs,Forward *frwrd)
{
    memoryStage = mStage;
    forward = frwrd;
    regs = pregs;
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    valC.setInput(0UL);
    valA.setInput(0UL);
    valB.setInput(0UL);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
    srcA.setInput(RNONE);
    srcB.setInput(RNONE);
}

/*
 * updateERegister method to update pipeline registers with signals generated in Decode Stage.
 */
void ExecuteStage::updateERegister(uint64_t d_stat, uint64_t d_icode, uint64_t d_ifun, uint64_t d_valC, uint64_t d_valA, uint64_t d_valB, uint64_t d_dstE, uint64_t d_dstM, uint64_t d_srcA, uint64_t d_srcB)
{
    stat.setInput(d_stat);
    icode.setInput(d_icode);
    ifun.setInput(d_ifun);
    valC.setInput(d_valC);
    valA.setInput(d_valA);
    valB.setInput(d_valB);
    dstE.setInput(d_dstE);
    dstM.setInput(d_dstM);
    srcA.setInput(d_srcA);
    srcB.setInput(d_srcB);
}

/*
 * clockP0 method to clock all Registers and implement IRMOVQ and OPQ instructions.
 */
void ExecuteStage::clockP0()
{
     /*
     * Test for a bubble before clocking registers.
     */
    if (stageBubble){
        stageBubble = false;
        updateERegister(SBUB, INOP, 0UL, 0UL, 0UL, 0UL, RNONE, RNONE, RNONE, RNONE);
    }
    stat.clock();
    icode.clock();
    ifun.clock();
    valC.clock();
    valA.clock();
    valB.clock();
    dstE.clock();
    dstM.clock();
    srcA.clock();
    srcB.clock();
    
    /*
     * capture CC flag values to update them in clock P1.
     */
    bool overF = regs->getCC(OF);
    bool signF = regs->getCC(SF);
    bool zeroF = regs->getCC(ZF);
    
    /*
     * set Cnd flags.
     */
    switch(ifun.getState())
    {
        case ALWAYS:
            e_Cnd = 1; 
            break;
        case LEQ:
            e_Cnd = (signF ^ overF) || zeroF;
            break;
        case LT:
            e_Cnd = signF ^ overF;
            break;
        case EQ:
            e_Cnd = zeroF;
            break;
        case NEQ:
            e_Cnd = !zeroF;
            break;
        case GTEQ:
            e_Cnd = !(signF  ^ overF);
            break;
        case GT:
            e_Cnd = !(signF ^ overF) && !zeroF;
            break;      
    }

    /*
     * Determine the value of e_valE.
     */
    if (icode.getState() == IOPX)
    {
        switch(ifun.getState())
        {
            case FADDQ:
                e_valE = getALUA() + getALUB();
                break;

            case FSUBQ:
                e_valE = getALUB() - getALUA();
                break;

            case FANDQ:
                e_valE = getALUA() & getALUB();
                break;

            case FXORQ:
                e_valE = getALUA() ^ getALUB();
                break;
        
        }
    }
    else if (icode.getState() == IIRMOVQ || icode.getState() == IRRMOVQ)
    {
        e_valE = getALUA();
    }
    else if (icode.getState() == IRMMOVQ)
    {
        e_valA = getALUA();
        e_valE = getALUB() + valC.getState();
    }
    else if(icode.getState() == IMRMOVQ || icode.getState() == IPOPQ || icode.getState() == IPUSHQ ||
            icode.getState() == ICALL || icode.getState() == IRET)
    {
        e_valE = getALUB() + getALUA();
        e_dstM = dstM.getState();
    }
    else 
    {
        e_valE = 0UL;
    }

    /*
     * Write to forward so signals can be used in earlier stages if need be. 
     */
    forward->sete_valE(e_valE);
    forward->sete_dstE(selectDstE());    
}

/*
 * clockP1 method to link Execute stage to Memory Stage using updateMRegister method.
 */
void ExecuteStage::clockP1()
{
        
    e_stat = stat.getState();
    e_icode = icode.getState();
    e_ifun = ifun.getState();
    e_valA = valA.getState();
    e_dstE = selectDstE(); 
    e_dstM = dstM.getState();
    
    /*
     * Test for exceptions.
     */
    if (forward->getm_stat() == SADR || forward->getm_stat() == SINS || forward->getm_stat() == SHLT ||
         forward->getW_stat() == SADR || forward->getW_stat() == SINS || forward->getW_stat() == SHLT)
    {    
        memoryStage->updateMRegister(SBUB, INOP, e_ifun, 0, 0, 0, RNONE, RNONE);
    }
    else
    {
        if (icode.getState() == IOPX)
        {
            updateCC();
        }
        memoryStage->updateMRegister(e_stat, e_icode, e_ifun, e_Cnd, e_valE, e_valA, e_dstE, e_dstM);
    }
}

/*
 * getALUA method to determine ALUA output.
 */
uint64_t ExecuteStage::getALUA()
{
    if (icode.getState() == IOPX || icode.getState() == IRRMOVQ)
    {
        return valA.getState();
    }
    else if (icode.getState() == IIRMOVQ || icode.getState() == IMRMOVQ || icode.getState() == IRMMOVQ)
    {
        return valC.getState();
    }
    else if (icode.getState() == IPOPQ || icode.getState() == IRET)
    {
        return 8;
    }
    else if (icode.getState() == IPUSHQ || icode.getState() == ICALL)
    {
        return -8;
    }
    return 0UL;
}

/*
 * getALUB method to determine ALUB output.
 */
uint64_t ExecuteStage::getALUB()
{
    if (icode.getState() == IOPX || icode.getState() == IRMMOVQ || icode.getState() == IMRMOVQ ||
        icode.getState() == IPUSHQ || icode.getState() == IPOPQ || icode.getState() == ICALL ||
        icode.getState() == IRET)
    {
        return valB.getState();
    }
    else
    {
        return 0UL;
    }
    return 0UL;
}

/*
 * isSetCC method to determine if the current instruction sets CC flags.
 */
bool ExecuteStage::isSetCC()
{
    if (icode.getState() == IOPX)
    {
        return true;
    }
    else return false;
}


/*
 * getALUFunction method to determine ALUFUN output.
 */
uint64_t ExecuteStage::getALUFunction()
{
    if (icode.getState() == IOPX)
    {
        return ifun.getState();
    }
    return 0UL;
}


/*
 * selectDstE method to get dstE value.
 */
uint64_t ExecuteStage::selectDstE()
{
    if (icode.getState() == IRRMOVQ && !e_Cnd)
    {
        return RNONE;
    }
    else
    {
        return dstE.getState();
    }
}

/*
 * updateCC method to update condition flags in clockP1.
 */
void ExecuteStage::updateCC()
{
    if (e_valE == 0)
    {
        regs->setCC(ZF, 1);
        regs->setCC(SF, 0);
    }
    else 
    {
        regs->setCC(ZF, 0);
        regs->setCC(SF, Tools::getBits(63,63,e_valE));
    }
    
    uint64_t e_valESign;
    if (ifun.getState() == FADDQ)
    {
        e_valESign = Tools::getBits(63,63,e_valE);
        if ((e_valESign != Tools::getBits(63,63,getALUA())) && (e_valESign != Tools::getBits(63,63,getALUB())))
        {
            regs->setCC(OF,1);
        }
        else
        {
            regs->setCC(OF,0);
        }
    }
    else if (ifun.getState() == FSUBQ)
    {
        e_valESign = Tools::getBits(63,63,e_valE);
        if ((e_valESign == Tools::getBits(63,63,getALUA())) && (e_valESign != Tools::getBits(63,63,getALUB())))
        {
            regs->setCC(OF,1);
        }
        else
        {
            regs->setCC(OF,0);
        }
    }
}
