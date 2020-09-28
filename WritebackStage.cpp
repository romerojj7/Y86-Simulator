/*
    File:   WritebackStage.cpp
    Desc:   Writeback Stage implementation for Y86-64 simulator

    Author: Javier Romero Hayden Fravel
    
*/
#include "Forward.h"
#include "Y86.h"
#include "WritebackStage.h"

/*
 * Writeback Stage Reset method to set Pipeline Registers to their defaults.
 */
void WritebackStage::reset(ProgRegisters *pReg, Forward *frwrd)
{
    regs = pReg;
    forward = frwrd;
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    valE.setInput(0UL);
    valM.setInput(0UL);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
}

/*
 * updateWRegister method to update pipeline registers with signals generated in Memory Stage.
 */
void WritebackStage::updateWRegister(uint64_t m_stat, uint64_t m_icode, uint64_t m_ifun, uint64_t m_valE, uint64_t m_valM, uint64_t m_dstE, uint64_t m_dstM)
{
    stat.setInput(m_stat);
    icode.setInput(m_icode);
    ifun.setInput(m_ifun);
    valE.setInput(m_valE);
    valM.setInput(m_valM);
    dstE.setInput(m_dstE);
    dstM.setInput(m_dstM);
    
}

/*
 * clockP0 method to clock all Registers and update forwarding paths.
 */
void WritebackStage::clockP0()
{
    /*
     * Test for a stall.
     */
    if (stageStall){
        stageStall = false;
        return;
    }
    stat.clock();
    icode.clock();
    ifun.clock();
    valE.clock();
    valM.clock();
    dstE.clock();
    dstM.clock();
    
    /*
     * Write to forwarding paths to use signals in earlier stage.
     */
    forward->setW_dstE(dstE.getState());
    forward->setW_valE(valE.getState());
    forward->setW_dstM(dstM.getState());
    forward->setW_valM(valM.getState());
    forward->setW_stat(stat.getState());
    forward->setW_icode(icode.getState());
}

/*
 * clock P1 method to update Register file.
 */
void WritebackStage::clockP1()
{
    regs->setReg(dstE.getState(), valE.getState());
    regs->setReg(dstM.getState(), valM.getState());
}
