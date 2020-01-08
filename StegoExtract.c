/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * StegoExtract.c: A program for manipulating images                    *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#define BYTETOBINARY8(byte)  \
  (byte & 0x80 ? 1 : 0), \
    (byte & 0x40 ? 1 : 0), \
    (byte & 0x20 ? 1 : 0), \
    (byte & 0x10 ? 1 : 0), \
    (byte & 0x08 ? 1 : 0), \
    (byte & 0x04 ? 1 : 0), \
    (byte & 0x02 ? 1 : 0), \
    (byte & 0x01 ? 1 : 0)//turns value into 8 bits
#define GNUMBER {0,1,0,8,5,3,8,0} //Gnumber as an Array
#define HEADINFO 64 //Required Image Header space 

// randperm function
void randperm(int a[], int n){ //void function takes in two values
  int i = 0; //initializes i
  int j = 0; //initializes j
  int TempVal = 0; //initia`lizes TempVal
  for (i = n - 1; i > 0; i--){ //for loop that goes through arrayLength
    j = random() % (i + 1); //sets j to random value
    TempVal = a[i];  //sets temp val to current array val
    a[i] = a[j]; //switches array values
    a[j] = TempVal; //switches array values
  }
}

void InitializeArray(int *numArray, const int arrayLength){ //void function takes in two values
  int i = 0; //initializes i
  for (i = 0; i < arrayLength; i++){ //for loop that goes through the length of the array
      numArray[i] = i; //adds 1-N into the array
  }
}

// unscramble function
void unscramble(unsigned char *ub, unsigned char *b, int size, int a[]){ //void function takes in 4 values
  int i = 0; //initializes i
  for (i = 0; i < size; i++){ //For Loop from 0 - payload size
    ub[a[i]] = b[i]; //Unscrambles values from b.data into ub.data
  }
}

int main(int argc, char *argv[]){
  int i = 0; //initializes i
  int j = 0; //initializes j
  int k = 0; //initializes k
  int key = 0; //initializes key 
  int cover_bits = 0; //initializes cover_bits
  int bits = 0; //initializes bits
  int byte0 = 0; //initializes byte0
  int shif = 0; //initializes shif
  unsigned char  b0 = 0; //initializes b0
  struct Buffer b = {NULL, 0, 0}; //initializes Struct b
  struct Buffer ub = {NULL, 0, 0}; //initializes Struct ub
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0}; //initializes Struct img

  if (argc != 4){ //Checks if 4 arguments were entered
      printf("\n%s <key> <stego_file> <file_to_extract> \n", argv[0]); //Clearification message
      exit(1); //Exits program
    }

  // convert key from string to integer
  sscanf(argv[1],"%d",&key);
  // use key to initialize random number generator
  srandom(key);

  ReadImage(argv[2],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros

  // hidden information
  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else
    cover_bits = 3*img.NofC*img.NofR;

  b.size = 0;//change later
  // extract four size bytes for the Buffer's size field
  // Set this to b.size
  for(i = 0; i < (HEADINFO / 2); i++){ //For Loop from 0 - 32
    unsigned char tempByte[8]={BYTETOBINARY8(GetGray(i))}; //Sets image byte to binary
    if (tempByte[7] == 1){ //Checks if lsb is a 1
      b.size = b.size|(1<<i-1); //If its a 1, it is | with a shift
    }
  }
  printf("Payload Size: %d\n", b.size); // Prints msg
  
  b.data = malloc(sizeof(char) * b.size); // Allocates room for the output data file
  if (b.data == NULL){ //checks malloc
    printf("Error: allocating memory!\n"); //prints error message
    exit(1); //exits if malloc fails
  }
  
  int *a = malloc(sizeof(int) * b.size); //Allocates room for the num array
  if (a == NULL){ //checks malloc
    printf("Error: allocating memory!\n"); //prints error message
    exit(1); //exits if malloc fails
  }  
  
  InitializeArray(a, b.size); //Calls IntializeArray function
  randperm(a, b.size); //Calls randperm function
  
  ub.data = malloc(sizeof(char) * b.size); //Allocates room for the unscrambled data
  if (ub.data == NULL){ //checks malloc
    printf("Error: allocating memory!\n"); //prints error message
    exit(1); //exits if malloc fails
  }  
  ub.size = b.size; //Sets ub.size to b.size
  
  // extract the eight digits of your G# using 4 bits per digit
  printf("GNumber: "); //Prints msg
  for(i = (HEADINFO / 2); i < HEADINFO; i++){ //For Loop from 32 - 64
    unsigned char bin[8] = {BYTETOBINARY8(GetGray(i + 1))}; //Sets image byte to binary
    if (bin[7] == 1){ //Checks if lsb is 1
      byte0 = byte0|(1<<shif); //If its a 1, it is | with a shift
      }
    shif++; //increments shift
    if (shif == 4){ //checks if 4 binary have been saved
      printf("%d", byte0); //Prints int
      byte0 = 0; //Resets byte0 
      shif = 0; //Resets shift
      }
    }
  //extracts payload
  for(i = HEADINFO; i < (b.size * 8) + HEADINFO;){ //For Loop from 64 - (payload size * 8) +64
    for (k = 0; k < b.size; k++){ //For Loop from 0 - payload size
      b0 = 0; //Initializes b0
      for(j = 0; j < 8;  j++){ //For Loop from 0-8
        i++; //Increments to get next image byte
        unsigned char bin[8] = {BYTETOBINARY8(GetGray(i))}; //Sets Image byte to binary array
        if (bin[7] == 1){ //Checks if lsb is 1
          b0 = b0|(1<<j); //If its a 1, it is | with a shift
          }
        }
      SetByte(k, b0); //Sets byte
	}
   }

  unscramble(ub.data, b.data, b.size,a); //Calls unscramble function
  
  free(a); //Frees memory from a
  a = NULL; //Prevents dangling error

  free(b.data); //Frees memory from b.data
  b.data = NULL; //Prevents dangling error
 
 /** for (i=0; i<b.size; i++)
    {
      // here you extract information from the image one byte at the time
      // note that you should extract only the least significant bits of the image
    }**/
  // Unscramble the data:   b to ub

  WriteBinaryFile(argv[3],ub);  // output payload file
  
  free(ub.data); //Frees memeory from ub.data
  ub.data = NULL; //Prevents dangling error
}
