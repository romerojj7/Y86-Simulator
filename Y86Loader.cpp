/**
 * File: Y86Loader.cpp
 * Desc: Program to load YESS programs into our simulated memory.
 * Date: 2/15/2020
 * Auth: Javier Romero Hayden Fravel 
 */
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>   
#include <sstream>  
#include <cstdlib>  
#include <fstream>  
#include <iterator> 

#include "Y86.h"
using namespace std;

static void isError(string, int);
static bool isBlankLine(string line);
static bool IsValidFileName(string filename);
static bool isCommentLine(string line);
static string getAddress(string line);
static bool hasValidAddress(string address, int nextAddr);
static bool hasHex(string, int, int);
static bool hasSpace(string, int, int);
static bool hasData(string);
static int hasValidData(string);

/**
 * Loads a Y86 file and returns true if file exists and has no errors.
 * File must be a <filename>.yo;
 * File contents must be formatted correctly
 */
bool Y86::load(char *filename)
{
    string loadfile = filename;
    ifstream inf(loadfile);
    if (filename == 0) loadfile = ""; 
    if (!inf || !IsValidFileName(filename))
    {
        cerr << "File open failed: " << loadfile << "\n";
        cerr << "Usage: yess <filename>.yo\n";
        exit(1);
    }
    return readFile(inf);
}

/**
 * Takes a pointer to and ifstream object as input.
 * Reads all lines of file: Returns true if all lines are formatted correctly.
 * If a line is formattted incorectly, returns false.
 * If it makes it through the whole file with no errors, returns true.
 */
bool Y86::readFile(ifstream& inf)
{
    string line;
    int lc = 0;
    string prevAddr = ""; 
    int nextAddr = 0;
    while (getline(inf, line))
    {
        lc++;
        if (!checkLine(line, lc, nextAddr))
        {
            isError(line, lc);
            return false;
        }
        if (hasValidAddress(line, nextAddr))
        {
            prevAddr = getAddress(line);
            nextAddr = stoi(prevAddr.substr(2, prevAddr.length() - 2), 0, 16) + hasValidData(line);
        }
    }
    return true;
}

/**
 * Takes a string filename as input.
 * Ensures filename ends with a .yo extension.
 * Returns true if the file has a valid name.
 */
static bool IsValidFileName(string filename)
{
    return filename.find(".yo") > 0;
}

/**
 * Takes a string representation of the current line, current line count, and the predicted next address as input.
 * Check current line to make sure it is a valid instruction.
 * Return true if is a valid line, else return false.
 */
bool Y86::checkLine(string line, int lc, int nextAddr)
{
    if (isCommentLine(line)) return true;
    else if (hasValidAddress(line, nextAddr) && (line.find('|', 28) == getAddress(line).length() + 23))
    {
        if (isBlankLine(line)) return true;
        else if (hasData(line))
        {
            if (hasValidData(line) != 0)
            {
                storeData(line);
                return true;
            }
        }
    }
    return false;
}

/**
 * Takes a string representation of a byte and a byte address as input.
 * Stores a byte at its corresponding byte address in memory
 */
void Y86::storeByte(string byte, int addr)
{
    int numbyte = stoi(byte, 0, 16);
    Y86::memory.putByte(addr, numbyte);
}

/**
 * Takes a string representation of the current line as input if it has valid data.
 * Stores data in YESS memory.
 */
void Y86::storeData(string line)
{
    int numbytes = hasValidData(line) * 2;
    string addrStr = getAddress(line);
    int addr = stoi(addrStr.substr(2, addrStr.length() - 2), 0, 16);
    string databytes = line.substr(7, numbytes);
    for (int x = 0; x < numbytes; x += 2)
    {
        string byte =  databytes.substr(x, 2);
        storeByte(byte, addr);
        addr++;
    }
}

/**
 * Takes a string representation of the current line and current line count as input.
 * Prints error message and the line containing the error. 
 */
static void isError(string line, int lc)
{
    cout << "Error on line " << lc << "\n";
    cout << line << "\n";
}

/**
 * Takes a string representation of the current line as input.
 * Finds and returns the address.
 * If no colon at position 5 or 6 returns and empty string.
 * If colon is found: returns first 5 or 6 characters of line.
 * Must run hasValidAddress in order to see if the address is valid.
 */
static string getAddress(string line)
{
    int colonLoc = line.find(':');
    if (colonLoc < 5 || colonLoc > 6 ) return "";
    return line.substr(0, colonLoc);
}

/**
 * Takes a string representation of the current line and the predicted next address as input.
 * Determines whether there is a valid address.
 * Elements the address must have in order to be valid:
 * Starting with 0x;
 * Address length is only 3 or 4: 0x123 or 0x1234;
 * Only consisting of the characters 0-9 and a-f;
 */
static bool hasValidAddress(string line, int nextAddr)
{
    string address = getAddress(line);
    if (!address.compare("")) return false;
    if (address[0] != '0' || address[1] != 'x') return false;
    if (stoi(address.substr(2, address.length() -2), 0, 16) < nextAddr) return false;
    return hasHex(address, 2, address.length());
}

/*
 * Takes a string representation of the current line as input.
 * Determines if the line contains only a comment.
 * The first character in a line should be |. If it is, return true.
 * If the first character is anything else, return false.
 */
static bool isCommentLine(string line)
{
    int barLoc = line.find('|',28);
    //barLoc will be -1 if there is no '|'
    if (barLoc < 0) return false;
    return hasSpace(line, 0, barLoc);
}

/**
 * Takes a string representation of the current line as input.
 * returns true if the line only contains an address, otherwise returns false.
 */ 
static bool isBlankLine(string line)
{
    int barLoc = line.find('|', 0);
    return hasSpace(line, getAddress(line).length() + 1, barLoc);
}


/**
 * Takes a string representation of the current line, a start index, and end index as input.
 * Returns true if all characters from start to end are hex characters.
 */
bool hasHex(string line, int start, int end)
{
    for (int i = start; i < end; i++){
        if (line[i] < '0' || (line[i] > '9' && line[i] < 'a') || line[i] > 'f') return false;
    }
    return true;
}

/**
 * Takes a string representation of the current line, a start index, and end index as input.
 * Returns true if all characters from start to end are a space ' '.
 */
static bool hasSpace(string line, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if (line[i] != ' ') return false;
    }
    return true;
}

/*
 * Takes a string representation of the current line as input.
 * returns true if the line contains any data.
 */
static bool hasData(string line)
{
    return !hasSpace(line, 7, 8);
}

/**
 * Takes a string representation of the current line as input.
 * returns number of data bytes if the data is valid.
 * returns 0 if the data is invalid.
 */
static int hasValidData(string line)
{
    if (hasData(line))
    {
        for (int i = 27; i > 7; i -=2)
        {
            if (hasHex(line, 7, i) && hasSpace(line, i, i+1))
                return (i - 7) / 2;
        }
    }
    return 0;
}
