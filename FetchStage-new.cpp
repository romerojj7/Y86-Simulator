/*
    File:   FetchStage.cpp
    Desc:   Fetch Stage implementation for Y86-64 simulator

    Author: efb
    Last update:  3/23/2020  (Spring 2020) replaced getInstruction to use functions
                             that more closely resemble the HCL code.
    
*/
#include "Y86.h"
#include "FetchStage.h"



/*---------------------------------------------------------------------------
    reset- used to connect to other Y86 components
     
-----------------------------------------------------------------------------*/
void FetchStage::reset(DecodeStage *pdecode, Memory *pmem)
{
		decodeStage = pdecode;  // "Connect" fetch stage to decode stage
		memory = pmem;
		predPC.reset();
}

/*---------------------------------------------------------------------------
    clockP0 - (pure virtual from PipeStage)
         Performs operations of clock Phase0 for this stage
-----------------------------------------------------------------------------*/
void FetchStage::clockP0()
{
  // Must implement--declared pure-virtual 

	// In Phase0 we (will in the future):
	//     Check for STALL or BUBBLE condition, update Register variable inputs
	//     		stall/bubble come from conditions in E, M, W stages (see HCL)
	//     Call the clock method on all Register variables
	//     Perform all Phase0 actions:
	//          For fetch, since selectPC uses outputs from other stages
	//			all stage operations happen during clock Phase1 (clockP1 method)
	//	
    predPC.clock();
  
	// Initialize internal signals to default values
    f_stat = SAOK;
    f_icode = INOP;
    f_ifun = FNONE;
    f_rA = RNONE;
    f_rB = RNONE;
    f_valC = 0;
    f_valP = 0;	
}
/*---------------------------------------------------------------------------
    clockP1 - (pure virtual from PipeStage)
         Performs operations of clock Phase1 for this stage
-----------------------------------------------------------------------------*/
void FetchStage::clockP1()
{
     
   // For fetch stage, all work done here (since we only know fetch address
   // when forwarding conditions are known (Memory and Writeback stages)
   // uint64_t opcode = memory->getByte(predPC.getState());
   // check for memory error after each memory access
   // check for illegal opcode
   f_PC = selectPC();

   // Get first byte of instruction (split into f_icode and f_ifun)
   if (!getByte0()) {
     //Generate SADR error status and NOP if memory error detected, set predPC to f_PC+1
     decodeStage->updateDRegister(SADR,INOP,FNONE,RNONE,RNONE,0UL,0UL);
     predPC.setInput(f_PC+1);
	   return;
   }

   instrValid = checkInstrValid();
   if(!instrValid) {
     //Generate SINS error status and NOP if invalid instruction detected, set predPC to f_PC+1
     // Pass the bad icode on (f_icode) because it will be marked as <bad> in the trace output
     decodeStage->updateDRegister(SINS,f_icode,FNONE,RNONE,RNONE,0UL,f_PC+1);
     predPC.setInput(f_PC+1);
	   return;
   }
  
   needsRegs = checkNeedsRegs();
   
   needsValC = checkNeedsValC();
   
   // Uses needsRegs to fetch reg byte (rA and rB) and valC if needed
   if (!align()) {
     //Generate error status and NOP if memory error detected, set predPC to f_PC+1
     decodeStage->updateDRegister(SADR,INOP,FNONE,RNONE,RNONE,0UL,0UL);
     predPC.setInput(f_PC+1);
	   return;
   }

   // Determine f_valP 
   f_valP = f_PC + getPCIncrement();
   

   // Check for HALT instruction to update f_stat
   if (f_icode == IHALT)
     f_stat = SHLT;
   
   // Predict next PC and update Decode stage register
   predictPC();
	 decodeStage->updateDRegister(f_stat,f_icode,f_ifun, f_rA, f_rB, f_valC, f_valP);
	
}
/*----------------------------------------------------------------------------
   getByte0 - Gets first instruction byte and splits into icode and ifun 
------------------------------------------------------------------------------
*/
bool FetchStage::getByte0(){

   // Get first byte from memory
   unsigned char inst = memory->getByte(f_PC);
   // Get f_icode
   f_icode = (inst >> 4) & 0xF;
   // Get f_ifun
   f_ifun = inst & 0xF;
   if(memory->isError())
     return false;
   return true;
}
/*----------------------------------------------------------------------------
   checkInstrValid - returns true if valid instruction, false otherwise
------------------------------------------------------------------------------*/
bool FetchStage::checkInstrValid(){
   return f_icode <= IPOPQ;   // IPOPQ has the max icode value, and f_icode is unsigned so can't be negative.
}

/*-----------------------------------------------------------------------------------------------------
    checkNeedsRegs - returns true if instruction requires a register byte, false otherwise
-----------------------------------------------------------------------------------------------------*/
bool FetchStage::checkNeedsRegs(){
  return (f_icode == IRRMOVQ || f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ 
             || f_icode == IPOPQ || f_icode == IPUSHQ || f_icode == IOPX);
}
/*-----------------------------------------------------------------------------------------------------
    checkNeedsValC - returns true if instruction requires valC, false otherwise
-----------------------------------------------------------------------------------------------------*/
bool FetchStage::checkNeedsValC(){
  return (f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ || f_icode == IJXX || f_icode == ICALL);
}
/*-----------------------------------------------------------------------------------------------------
    align - Models the align function in the PIPE diagram. Uses needRegs and needsValC signals 
            fetch reg byte and valC as needed. Returns true if memory access OK, false if memory error
-----------------------------------------------------------------------------------------------------*/
bool FetchStage::align(){
  uint64_t pc_offset = 1; //default offset from current PC to fetch next byte(s)
  f_rA = RNONE;  // default value for f_rA
  f_rB = RNONE;  // default value for f_rB

  if(needsRegs){
    //------------------------------------------------
    // Get reg byte
    //------------------------------------------------
    unsigned char regByte = memory->getByte(f_PC + pc_offset);
    pc_offset++;
    if(memory->isError())
      return false;
    // Get f_rA
    f_rA = (regByte >> 4) & 0xF;
    // Get f_rB
    f_rB = regByte & 0xF;
  }
  //------------------------------------------------
  // Get valC if needed (set default value to 0)
  //------------------------------------------------
  f_valC = 0L;  // set default value
  if(needsValC){
    f_valC = memory->getWord(f_PC+pc_offset);
    if(memory->isError())
      return false;
  }
  return true;
}
/*-----------------------------------------------------------------------------------------------------
    getPCIncrement - returns the size of the instruction in Fetch stage
-----------------------------------------------------------------------------------------------------*/
int FetchStage::getPCIncrement(){
  return 1 + (needsRegs ? 1 : 0) + (needsValC ? 8 : 0); // 1, or 2, or 9, or 10 byte increment
}
/*----------------------------------------------------------------------------
   predictPC - updates predPC based on f_icode, f_valC and f_valP
----------------------------------------------------------------------------*/
void FetchStage::predictPC()
{
  if(f_icode == IJXX || f_icode == ICALL)
    predPC.setInput(f_valC);
  else
    predPC.setInput(f_valP);
}
/*----------------------------------------------------------------------------
   selectPC - select next PC 
----------------------------------------------------------------------------*/
uint64_t FetchStage::selectPC()
{
	// For now, selectPC returns predicted PC
  // Later will have to add logic to select from forwarded values based on JXX and RET
    return predPC.getState();
}
