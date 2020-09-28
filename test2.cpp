#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>   
#include <sstream>  
#include <cstdlib>  
#include <fstream>  
#include <iterator> 

//#include "Sim.h"
//#include "ProgRegisters.h"
//#include "Y86.h"
//#include "Memory.h"
using namespace std;

//static void isError(string, int);
//void storeBytes(string, int);
//void storeData(string, int, int);
static int hasValidData(string);
//static bool hasData(string);
//bool isBlankLine(string);
//bool isCommentLine(string);
static bool hasSpace(string line, int, int);

//Y86 y86;
int main()
{
    string line = "0x060: 30f90100000000000000 | irmovq  $1,%r9         # Constant 1";
    cout << line << "\n";
    cout << hasValidData(line) << "\n";
    //isError(line, 5);
    //cout << line << endl;
    //string address = line.substr(2, 3);
    //int addr = stoi(address, 0, 16);
    //storeData(line, 2, addr);
   //else
       //cout << "nope" << endl;
    
}


static int hasValidData(string line)
{
    int x = 7;
    /**if (hasData(line))
    {   
        if (hasHex(line, 7, 27) && hasSpace(line, 27, 28))
            return 10;
        else if (hasHex(line, 7, 25) && hasSpace(line, 25, 26))
            return 9;
        else if (hasHex(line, 7, 23) && hasSpace(line, 23, 24))
            return 8;
        else if (hasHex(line, 7, 11) && hasSpace(line, 11, 12))
            return 2;
        else if (hasHex(line, 7, 9) && hasSpace(line, 9, 10))
            return 1;
          */
           while (line[x] != ' ')
           {
               if ((line[x] >= '0' && line[x] <= '9' )|| (line[x] >= 'a' && line[x] <= 'f') && hasSpace(line, 27, 28))
               {
                    x++; 
               }
               else
               {
                   return 0;
               }
           }
           if ((x - 7)/2 != 0)
           {
               return 0;
           }
           return (x - 7) / 2;
    //}
    return 0;
}

static bool hasSpace(string line, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if (line[i] != ' ') return false;
    }
    return true;
}
//static void isError(string line, int lc)
//{
  //  cout << "Error on line " << lc << "\n";
  //  cout << line << "\n";
//}
/**void Y86::storeByte(string byte, int addr)
{
    Memory m = y86.getMemory();
    int numbyte = stoi(byte, 0, 16);
    m.putByte(addr, numbyte);
}

void Y86::storeData(string line, int numbytes, int addr)
{
    static string databytes = line.substr(7, numbytes * 2);
    for (int x = 0; x < numbytes * 2; x + 2)
    {
        static string byte =  databytes.substr(x, 2);
        storeByte(byte, addr);
        addr++;
    }
    //cout << databytes << "\n";
}


static int hasValidData(string line)
{
    int x = 7;
    while ((line[x] >= '0' && line[x] <= '9' )|| (line[x] >= 'a' && line[x] <= 'f'))
    {
        x++;
    }
    if (line[x] != ' ' || x > 27)
    {
        return 0;
    }
    if (x == 9)
    {
        return 1;
    }
    else if (x == 11)
    {
        return 2;
    }
    else if (x == 25)
    {
        return 9;
    }  
    else if (x == 27)
    {
        return 10;
    }
    else
        return 0;
}
/**
static bool hasData(string line)
{
    if ((line[7] >= '0' && line[7] <= '9' )|| (line[7] >= 'a' && line[7] <= 'f'))
    {
        return true; 
    }
    return false;
}

bool isBlankLine(string line)
{
    const char *l = line.c_str();
    int x = 0;
    while (x < line.length())
    {
        if (l[x] != ' ')
        {
            return false;
        }
        x++;
    }
    return true;
}

bool isCommentLine(string line)
{
    const char *l = line.c_str();
    //cout << l;
    //for (int x = 0; x < line.length(); x++)
    //{
    int x = 0;
        while (l[x] != '|' && x < line.length())
        {
            if (l[x] != ' ')
                return false;
                else
                    x++;
        }
        if (l[x] == '|')
            return true;
    //}
    return false;
}
*/
