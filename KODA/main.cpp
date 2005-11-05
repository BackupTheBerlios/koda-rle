#include "bitarray.h"
void testArray(bitArray * array);


/*
* UWAGA: bity liczymy od 1 nie od 0, wiec funcja getBit(1) oznacza zwrocenie 1-go current bitu z arraya. 
*/

int main(int argc, char *argv[]){
   bitArray *array = new bitArray(5); // create array of 5*4 = 20 bytes
   ULONG bnr = array->readFromFile(0,"test.txt"); //read file from 10 bit, actually it's read from second byte, so we set bitOffset to 10mod8 = 2. 
   printf ("nr of bits read: %d\n",bnr); // print nr of interesting bits read
   array->printArray();
   testArray (array);
   return 0;
}

   
void testArray(bitArray *array){
   
   printf("\n\n *************************************************************\n");
   printf("Changing bit 3 (mind the offset) \n");
   array->setBit(3,1);
   array->printArray();
   printf("\nbit 1 is: %d\n",array->getBit(1));
   printf("bit 2 is: %d\n",array->getBit(2));
   printf("bit 3 is: %d\n",array->getBit(3));
   printf("bit 4 is: %d\n",array->getBit(4));
   printf ("\n");
   printf("Changing bit 3 back to 0 (mind the offset) \n");
   array->setBit(3,0);
   array->printArray();
   printf("\nbit 1 is: %d\n",array->getBit(1));
   printf("bit 2 is: %d\n",array->getBit(2));
   printf("bit 3 is: %d\n",array->getBit(3));
   printf("bit 4 is: %d\n",array->getBit(4));
      
   printf("\n\n*************************************************************\n");
   printf("Changing bit 61\n");
   array->setBit(61,1);
   array->printArray();
   
   printf("\n\n*************************************************************\n");
   printf("Changing bit 127\n");
   array->setBit(127,1);
   array->printArray();
   printf("\nbit 126 is: %d\n",array->getBit(126));
   printf("bit 127 is: %d\n",array->getBit(127));
   printf("bit 128 is: %d\n",array->getBit(128));

   
   printf("\n\n*************************************************************\n");
   printf("Changing bit 345 - out of bound? \n\n");
   array->setBit(345,1);

   printf("\n\n*************************************************************\n");
   printf("test for save\n");
   array->saveToFile(2,8,"testwrite.txt");

   getc(stdin);
}
