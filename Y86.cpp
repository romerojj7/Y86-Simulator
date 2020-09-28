/*
    Name:    Y86.cpp
    Desc:    Starting implementation of Y86 class
    Authors: Javier Romero Hayden Fravel
    Date:    2/11/2020
*/
#include <cstdint>
#include "Y86.h"


/*-------------------------------------------------------------------------
  Name:  Y86()
  Desc:  Constructor
---------------------------------------------------------------------------*/
Y86::Y86()
{
    //your code
    reset();
}
/*-------------------------------------------------------------------------
  clock - calls clock.
  
---------------------------------------------------------------------------*/
void Y86::clock()
{
    regs.clock();
    for (int i =0; i < 5; i++)
    {
        stage[i]->clockP0();
    }
    for (int i=0; i < 5; i++)
    {
        stage[i]->clockP1();
    }
    pipeControl();
}
/*-------------------------------------------------------------------------
  reset - resets Y86 object to initial state.
  
---------------------------------------------------------------------------*/
void Y86::reset()
{
	// your code here
    stage[0] = &fetchStage;
    stage[1] = &decodeStage;
    stage[2] = &executeStage;
    stage[3] = &memoryStage;
    stage[4] = &writebackStage;

    fetchStage.reset(&decodeStage, &memory, &forward);
    decodeStage.reset(&executeStage, &regs, &forward);
    executeStage.reset(&memoryStage, &regs, &forward);
    memoryStage.reset(&writebackStage, &memory, &forward);
    writebackStage.reset(&regs, &forward);
    
    cycles = 0;

    memory.reset();
    regs.reset();
}

/*-------------------------------------------------------------------------
  clockP0 - calls clock function of ProgRegisters class
  
---------------------------------------------------------------------------*/
void  Y86::clockP0()
{
	// your code here
    regs.clock();
}
/*-------------------------------------------------------------------------
  clockP1 - implements functions that run on second half of clock cycle
  
---------------------------------------------------------------------------*/
void  Y86::clockP1()
{
	return;  // nothing for now (Lab 4)
}
/*-------------------------------------------------------------------------
  
---------------------------------------------------------------------------*/
uint64_t Y86::getStat(void)
{
    uint64_t writebackStat = writebackStage.getStat();
    if (writebackStat == SAOK || writebackStat == SBUB)
        return 0;
    else
        return writebackStat;
}

void Y86::pipeControl()
{
    //Conditions for fetch, decode, execute stage
    bool cond0, cond1, cond2;
    cond0 = (stage[2]->getIcode() == IMRMOVQ || stage[2]->getIcode() == IPOPQ) &&
            (executeStage.getDstM() == decodeStage.getSrcA() || executeStage.getDstM() == decodeStage.getSrcB());
    cond1 = IRET == stage[1]->getIcode() || IRET == stage[2]->getIcode() || IRET == stage[3]->getIcode();
    cond2 = stage[2]->getIcode() == IJXX && !executeStage.getCnd();
    
    //Fetch stage
    stage[0]->setStall(cond0 || cond1);
    stage[0]->setBubble(0);
    //Decode stage
    stage[1]->setStall(cond0);
    stage[1]->setBubble(cond2 || (!cond0 && cond1));
    //Execute stage
    stage[2]->setStall(0);
    stage[2]->setBubble(cond2 || cond0);
    
    //Conditions for memory and writeback stage
    bool condm, condw;
    condm = stage[3]->getStat() == SADR || stage[3]->getStat() == SINS || stage[3]->getStat() == SHLT;
    condw = stage[4]->getStat() == SADR || stage[4]->getStat() == SINS || stage[4]->getStat() == SHLT;
    
    //Memory stage
    stage[3]->setStall(0);
    stage[3]->setBubble(condm || condw);
    //Writeback stage
    stage[4]->setStall(condw);
    stage[4]->setBubble(0);
}
