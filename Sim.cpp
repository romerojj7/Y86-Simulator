/*
    File:   Sim.cpp
    Desc:   The main program. Accepts and verifies parameters and executes simulator
            on the specified object file. Other parameters are used to specify the stage or
            stages where trace outputs will be printed. Trace outputs may include the 
            stage register contents, memory contents, and/or register contents. Trace outputs
            are printed at the end of a cycle.
            
            Best way to capture internal state during simulation?
            Previous simulator used a special dump instruction. This requires
            adding this instruction to the instruction set. Dump routines were
            provided that allowed the dumping of program state during WRITEBACK stage.
            
*/
#include <iostream>
#include <iomanip>
#include <string>
#include <unistd.h>
#include "Sim.h"
#include "Y86.h"
#include "Memory.h"


using namespace std;



Y86 y86;  // Declare global Y86 object. 

void usage(void);
char *getOptions(int, char**,bool*,bool*,bool*,bool*,bool*);

int main(int argc, char *argv[])
{
	if (argc < 2){
		usage();
		return 0;
	}
	
	
	bool f=false,d=false,e=false,m=false,w=false;
	char *new_argv = getOptions(argc,argv,&f,&d,&e,&m,&w);

	// cout << "f="<<f<<" d="<<d<<" e="<<e<<" m="<<m<<" w="<<w<<endl;
	// cout << "next argument: " << new_argv << endl;
	// cout << "argv[0] is: " << argv[0] << endl;
	

	
	y86.reset();    
	if(y86.load(new_argv)) {
		y86.setTrace(f,d,e,m,w);
		bool stop = false;
		while(!stop){
			y86.clock();
			y86.trace();
			stop = y86.getStat();
		}
	}
	y86.dumpProcessorRegisters();
	y86.dumpProgramRegisters();
	y86.dumpMemory();

    
}

char * getOptions(int argc, char **argv, bool *f, bool *d, bool *e, bool *m, bool *w)
{
	opterr = 0;
	char c;
	while((c = getopt(argc, argv, "fdemw")) != -1){
		switch(c) {
			case 'f':
				*f = true;
				break;
			case 'd':
				*d = true;
				break;
			case 'e':
				*e = true;
				break;
			case 'm':
				*m = true;
				break;
			case 'w':
				*w = true;
				break;
			default:
				usage();
				break;
		}
	}
	return argv[optind];
	
}

void usage(void)
{
    printf("Usage: yess [-fdemw] <objectfile>\n"
          "<objectfile> must be created with the Y86 assembler 'yas' (has .yo extension.)\n"
		  "[-fdemw] are optional flags that enable trace output for any or all stages.\n\n");
    exit(0);
}
