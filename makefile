CXX=g++
CXXFLAGS = -Wall -g -std=c++11

# Put object files in next line
OBJS = Memory.o Tools.o ProgRegisters.o Sim.o Y86.o Y86Loader.o Y86trace.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o

# The OBJTEST macro is same as 'OBJS' above except replace Sim.o with testSim.o 
OBJTEST = Memory.o Tools.o ProgRegisters.o testSim.o Y86.o Y86Loader.o Y86trace.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o

#complete the following targets with appropriate dependencies and commands
all: yess ytest
yess:	$(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o yess

ytest:  $(OBJTEST)
	$(CXX) $(CXXFLAGS) $(OBJTEST) -o ytest

#your object file targets below here...
Memory.o: Memory.h Memory.cpp Tools.h 

Tools.o: Tools.cpp Tools.h

ProgRegisters.o: ProgRegisters.h ProgRegisters.cpp Register.h Sim.h Tools.h

Y86.o: Y86.cpp Y86.h

Y86Loader.o: Y86Loader.cpp Y86.h

Sim.o: Sim.h Y86.h Memory.h ProgRegisters.h

Y86trace.o: Y86trace.cpp Y86.h Sim.h PipeStage.h FetchStage.h DecodeStage.h ExecuteStage.h MemoryStage.h WritebackStage.h 

FetchStage.o: FetchStage.cpp FetchStage.h Y86.h Sim.h PipeStage.h DecodeStage.h Register.h

DecodeStage.o: DecodeStage.cpp DecodeStage.h Sim.h PipeStage.h Register.h ExecuteStage.h Y86.h

ExecuteStage.o: ExecuteStage.cpp ExecuteStage.h Sim.h PipeStage.h Register.h MemoryStage.h Y86.h

MemoryStage.o: MemoryStage.cpp MemoryStage.h Sim.h PipeStage.h Register.h WritebackStage.h Y86.h

WritebackStage.o: WritebackStage.cpp WritebackStage.h Sim.h PipeStage.h Register.h Y86.h
# clean target
# Remeber, you don't want to delete your testSim.o file
# The following target preserves testSim.o
clean:
	touch yess
	cp testSim.o testSim-tmp
	rm *.o yess
	cp testSim-tmp testSim.o
