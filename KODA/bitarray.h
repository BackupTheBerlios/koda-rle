#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#ifndef ULONG 
#define ULONG unsigned long
#endif

#ifndef BIT_MASK
#define BIT_MASK 0x80000000
#endif

class bitArray
{
   ULONG size; 
   ULONG *buff; 
   int bitOffset; // bit offset specyfing starting bit our buffer (due to byte reading)
   
   ULONG buffOffset(ULONG index); //gets the index in the ULONG in the buffer according to the input bit index
   ULONG getMask(ULONG index); //gets the mask to be used for changing bit given by index
   ULONG saveChars(ULONG charStart, ULONG charLength, char *fileName, ULONG fileOffset);
    //internally saves the data from the bitArray buffer
protected:
   bool checkIndex(ULONG index);
public:
   bool upBit(ULONG index);
   bool downBit(ULONG index);
   bool setBit(ULONG index, bool value);
   bool getBit(ULONG index);
   bool flipBit(ULONG index); 
   ULONG readFromFile(ULONG bitStart,char *fileName); //returns nr of bits read
   ULONG saveToFile(ULONG bitStart, ULONG bitLength, char *fileName); //returns nr of bits written to a file
   bitArray(ULONG arraySize);
   void printArray();
   bitArray();
   ~bitArray();
   
};


