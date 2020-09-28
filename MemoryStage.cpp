/*
    File:   MemoryStage.cpp
    Desc:   Memory Stage implementation for Y86-64 simulator

    Author: Javier Romero Hayden Fravel
    
*/
#include "Forward.h"
#include "Y86.h"
#include "MemoryStage.h"

/*
 * Memory Stage Reset method to set Pipeline Registers to their defaults.
 */
void MemoryStage::reset(WritebackStage *wStage, Memory *mem, Forward *frwrd) 
{
    writebackStage = wStage;
    memory = mem;
    forward = frwrd;
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    cnd.setInput(false);
    valE.setInput(0UL);
    valA.setInput(0UL);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
}

/*
 * updateMRegister method to update pipeline registers with signals generated in Execute Stage.
 */
void MemoryStage::updateMRegister(uint64_t e_stat, uint64_t e_icode, uint64_t e_ifun, bool e_cnd, uint64_t e_valE, uint64_t e_valA, uint64_t e_dstE, uint64_t e_dstM)
{
    stat.setInput(e_stat);
    icode.setInput(e_icode);
    ifun.setInput(e_ifun);
    cnd.setInput(e_cnd);
    valE.setInput(e_valE);
    valA.setInput(e_valA);
    dstE.setInput(e_dstE);
    dstM.setInput(e_dstM);
}

/*
 * clockP0 method to clock all Registers.
 */
void MemoryStage::clockP0()
{
    /*
     * Test for a bubble before clocking registers.
     */
    if (stageBubble){
        stageBubble = false;
        updateMRegister(SBUB, INOP, 0UL, 0, 0UL, 0UL, RNONE, RNONE);
    }
    stat.clock();
    icode.clock();
    ifun.clock();
    cnd.clock();
    valE.clock();
    valA.clock();
    dstE.clock();
    dstM.clock();
    
    /*
     * Write to or read from memory.
     */
    if (icode.getState() == IRMMOVQ || icode.getState() == IPUSHQ || icode.getState() == ICALL)
    {
        memory->putWord(getMemAddr(), valA.getState());
        m_valM = 0UL;
    }
    else if(icode.getState() == IMRMOVQ || icode.getState() == IPOPQ || icode.getState() == IRET)
    {
        m_valM = memory->getWord(getMemAddr());
    }
    else
    {
        m_valM = 0UL;
    }
    
    if (memory->isError() == true)
    {
        m_stat = SADR;
        m_valM = 0UL;
    }
    else 
    {
        m_stat = stat.getState();
    }

    /*
     * Write to forwarding paths for use in earlier stages.
     */
    forward->setm_valM(m_valM);
    forward->setM_dstM(dstM.getState());
    forward->setM_valE(valE.getState());
    forward->setM_dstE(dstE.getState());
    forward->setm_stat(m_stat);
    forward->setM_icode(icode.getState());
    forward->setM_Cnd(cnd.getState());
    forward->setM_valA(valA.getState());
}

/*
 * clockP1 method to link Memory Stage to Writeback Stage using updateWRegister method.
 */
void MemoryStage::clockP1()
{
   
    m_icode = icode.getState();
    m_ifun = ifun.getState();
    m_valE = valE.getState();
    m_dstE = dstE.getState();
    m_dstM = dstM.getState();

    writebackStage -> updateWRegister(m_stat, m_icode, m_ifun, m_valE, m_valM, m_dstE, m_dstM);
}

/*
 * getMemAddr method to determine memory address
 */
uint64_t MemoryStage::getMemAddr()
{
    if (icode.getState() == IRMMOVQ || icode.getState() == IPUSHQ || icode.getState() == IMRMOVQ ||
        icode.getState() == ICALL)
    {
        return valE.getState();
    }
    else if (icode.getState() == IPOPQ || icode.getState() == IRET)
    {
        return valA.getState();
    }
    else
        return 0UL;
}
