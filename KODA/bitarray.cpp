#include "bitarray.h"
//
using namespace std;

 
ULONG bitArray::readFromFile(ULONG bitStart,char *fileName)
{
   //solution based on http://cplusplus.com/doc/tutorial/files.html
   char * memblock;
   ULONG byteOffset; 
   //calculate starting byte offset basen on bit nr. 
   if (bitStart == 0)
   {
      byteOffset = 0;
      bitOffset = 0; //set the bit offset class variable 
   }
   else
   {
      byteOffset = (bitStart - 1) / 8;
      bitOffset =  bitStart % 8; 
   }
   ifstream file (fileName, ios::in|ios::binary|ios::ate);
   ifstream::pos_type fileSize;
   if (file.is_open())
   {
      fileSize = file.tellg(); //file size in bytes
      if (byteOffset > fileSize)
      {
         printf("\nERROR: required byte offset is after EOF. File not read.  \n");
         return 0;
      }
      memblock = new char [size*4];
      memset(memblock,0,size*4);
      file.seekg(byteOffset, ios::beg);
      file.read (memblock, size*4);
      file.close();
      
      //copy char array to our 32 bit buffer
      for (ULONG i=1; i<=size; i++)
      {
         ULONG tmp1 = (ULONG) memblock[i*4-4];
         ULONG tmp2 = (ULONG) memblock[i*4-3];
         ULONG tmp3 = (ULONG) memblock[i*4-2];
         ULONG tmp4 = (ULONG) memblock[i*4-1];
         buff[i-1] = ( (tmp1 << 24) | (tmp2 << 16) | (tmp3 << 8) | tmp4);
      }
      delete [] memblock;
   }
   else
      printf("\nERROR: Unable to open file\n");
   
   //calculate number of interesting bits read from file
   printf ("File size: %d bits.\nStarting Offset: %d (byte nr)\n Buffer Size: %d bits.\n",(ULONG)fileSize*8, byteOffset, size*32);
   if ((ULONG)fileSize - byteOffset < size)
   {
     return ((ULONG)fileSize - byteOffset) * 8 - bitOffset; 
   }
   else
      return size * 32 - bitOffset;
} //returns nr of bits read

ULONG bitArray::saveToFile(ULONG bitStart, ULONG bitLength, char *fileName)
{ 
  ULONG bufSize = ((bitLength + bitStart) >> 5) + 1;
  ULONG charBufSize = ((bitLength + bitStart) >> 3) + 1;
  
  bitArray *bitArr = new bitArray(bufSize);
  bitArr->readFromFile(bitStart,fileName);   

  for(ULONG i = 1;i<=bitLength;i++)
  {
    bitArr->setBit(i,getBit(i));
  }

  bitArr->saveChars(0,charBufSize,fileName,bitStart>>3);

  delete bitArr;

  return 0;
} //returns nr of bits written to a file

ULONG bitArray::saveChars(ULONG charStart, ULONG charLength, char *fileName, ULONG fileOffset)
{
  ofstream file (fileName, ios::out|ios::binary|ios::ate);
  char tmp;
  
  if (!file.is_open())
  {
    printf("\nERROR: Unable to open file\n");
  }

  //eliminates the problem of the reverse order of the bytes in the ULONG value
  for(ULONG i = 0;i<size;i++)
  {
    tmp = ((char*)buff)[i*4];
    ((char*)buff)[i*4] = ((char*)buff)[i*4 + 3];
    ((char*)buff)[i*4 + 3] = tmp;

    tmp = ((char*)buff)[i*4 + 1];
    ((char*)buff)[i*4 + 1] = ((char*)buff)[i*4 + 2];
    ((char*)buff)[i*4 + 2] = tmp;
  }
  
  file.seekp(fileOffset,ios::beg);
  file.write(((char*)buff)+charStart,charLength);
  file.close();
  
  return 0;
}

bitArray::bitArray() 
{
  bitArray(64);
}


bitArray::bitArray(ULONG arraySize) 
{
  buff = new ULONG [arraySize];
  if ( buff == NULL ) 
    printf ("buff allocation error in bitArray.cpp\n");
  size = arraySize;
  bitOffset = 0;
  memset(buff,0,arraySize*4);
}

bitArray::~bitArray()
{
   delete(buff);
}


void bitArray::printArray()
{
 printf("Bit Array:\nStarting bit is bit nr: %d\n",bitOffset);

 ULONG bitNr = 1; //used to print the starting bit in a row
 for (ULONG i=0; i<size; i++)
 {
   ULONG tmp = buff[i];
   //printf("ULONG: %d\n",tmp);
   printf("%10d   ",bitNr);
   bitNr = bitNr + 32; // 32 bits printed in a row
 
   for (int j=0; j<32; j++){
     if (j%8 == 0)
       printf(" ");
     if ((tmp&BIT_MASK) != 0)
       printf("1");
     else 
       printf("0");
     //printf("anding: %d\n",tmp&mask);
     tmp = tmp<<1;
   }  
   printf("\n");
 }// end for
}

 
bool bitArray::setBit(ULONG index, bool value)
{
  if (value)
    return upBit(index);
  else 
    return downBit(index);
}

bool bitArray::getBit(ULONG index)
{
  if(!checkIndex(index))return false;

  if ((buff[buffOffset(index)]&getMask(index)) == 0)
   return false;
  else 
   return true;
}

bool bitArray::flipBit(ULONG index)
{
  if(!checkIndex(index))return false;

  return setBit(index,!getBit(index));
}

ULONG bitArray::buffOffset(ULONG index)
{
  return (ULONG) (index + bitOffset -1)>>5;
}

ULONG bitArray::getMask(ULONG index)
{
  return BIT_MASK >> index + bitOffset -1;
}

bool bitArray::upBit(ULONG index)
{
  if(!checkIndex(index))return false;

  buff[buffOffset(index)] |= getMask(index);
  return true;
}

bool bitArray::downBit(ULONG index)
{
  if(!checkIndex(index))return false;

  buff[buffOffset(index)] &= ~getMask(index);
  return true;
}

bool bitArray::checkIndex(ULONG index)
{
  if (index + bitOffset < 1)
  {
    printf ("ERROR: setBit index can not be less than 1\n");
    return false;
  }
  
  if (index + bitOffset > size<<5) //if index > buffer size 
  {
    printf ("ERROR: setBit index outside of buffer\n");
    return false;
  }

  return true;
}
