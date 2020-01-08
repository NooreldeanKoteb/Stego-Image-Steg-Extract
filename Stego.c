/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * Stego.c: A program for manipulating images                           *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#define BYTETOBINARY32(byte)  \
    (byte & 0x80000000 ? 1 : 0), \
    (byte & 0x40000000 ? 1 : 0), \
    (byte & 0x20000000 ? 1 : 0), \
    (byte & 0x10000000 ? 1 : 0), \
    (byte & 0x8000000 ? 1 : 0), \
    (byte & 0x4000000 ? 1 : 0), \
    (byte & 0x2000000 ? 1 : 0), \
    (byte & 0x1000000 ? 1 : 0), \
    (byte & 0x800000 ? 1 : 0), \
    (byte & 0x400000 ? 1 : 0), \
    (byte & 0x200000 ? 1 : 0), \
    (byte & 0x100000 ? 1 : 0), \
    (byte & 0x80000 ? 1 : 0), \
    (byte & 0x40000 ? 1 : 0), \
    (byte & 0x20000 ? 1 : 0), \
    (byte & 0x10000 ? 1 : 0), \
    (byte & 0x8000 ? 1 : 0), \
    (byte & 0x4000 ? 1 : 0), \
    (byte & 0x2000 ? 1 : 0), \
    (byte & 0x1000 ? 1 : 0), \
    (byte & 0x800 ? 1 : 0), \
    (byte & 0x400 ? 1 : 0), \
    (byte & 0x200 ? 1 : 0), \
    (byte & 0x100 ? 1 : 0), \
    (byte & 0x80 ? 1 : 0), \
    (byte & 0x40 ? 1 : 0), \
    (byte & 0x20 ? 1 : 0), \
    (byte & 0x10 ? 1 : 0), \
    (byte & 0x08 ? 1 : 0), \
    (byte & 0x04 ? 1 : 0), \
    (byte & 0x02 ? 1 : 0), \
    (byte & 0x01 ? 1 : 0) //turns value into 32bits
#define BYTETOBINARY8(byte)  \
    (byte & 0x80 ? 1 : 0), \
    (byte & 0x40 ? 1 : 0), \
    (byte & 0x20 ? 1 : 0), \
    (byte & 0x10 ? 1 : 0), \
    (byte & 0x08 ? 1 : 0), \
    (byte & 0x04 ? 1 : 0), \
    (byte & 0x02 ? 1 : 0), \
    (byte & 0x01 ? 1 : 0) //Turns value into 8 bits
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

void scramble(unsigned char *sb, unsigned char *b, int size, int a[]){ //void function takes in 4 values
  int i = 0; //initializes i
  for (i = 0; i < size;i++){ //For Loop from 0 - payload size
  	sb[i] = b[a[i]]; // Scrambles values and saves in sb.data
  }
}

int main(int argc, char *argv[]){
  int i = 0; //initializes i
  int j = 0; //initializes j
  int k = 0; //initializes k
  int key = 0; //initializes key 
  int cover_bits = 0; //initializes cover_bits
  int bits = 0; //initializes bits
  int gnum[8] = GNUMBER; //initializes gnum with defined array
  struct Buffer b = {NULL, 0, 0}; //initializes struct b
  struct Buffer sb = {NULL, 0, 0}; //initializes struct sb
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0}; //initializes struct img
  
  if (argc != 5){ // Checks if 4 values were inputed into the program
      printf("\n%s <key> <cover_file> <stego_file> <file_to_hide> \n", argv[0]); //sends msg for clearification
      exit(1); //exits program
    }
  // convert key from string to integer
  sscanf(argv[1],"%d",&key);
  // use key to initialize random number generator
  srandom(key);

  ReadImage(argv[2],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros
  ReadBinaryFile(argv[4],&b);    // Read binary data

  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else
    cover_bits = 3*img.NofC*img.NofR;
  bits = (8 + b.size)*8;
  if (bits > cover_bits)
    {
      printf("Cover file is not large enough %d (bits) > %d (cover_bits)\n",bits,cover_bits);
      exit(1);
    }
  // hidden information
  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  // embed four size bytes for the Buffer's size field
  // embed the eight digits of your G# using 4 bits per digit
  // here you scramble data and produce sb from b
  
  int *a = malloc(sizeof(int) * b.size); //Declares & sets memory of num array 
  if (a == NULL){ //checks malloc
    printf("Error: allocating memory!\n"); //prints error message
    exit(1); //exits if malloc fails
  }
  
  InitializeArray(a, b.size); //Calls Array Intializer function
  randperm(a, b.size); //Calls Random Permutation function
  
  sb.data = malloc(sizeof(char) * b.size); //sets memory for sb.data array
  if (sb.data == NULL){ //checks malloc
    printf("Error: allocating memory!\n"); //prints error message
    exit(1); //exits if malloc fails
  }
  
  scramble(sb.data, b.data, b.size, a); //Calls Scramble Array
  free(a); //Frees memory from a
  a = NULL; //Prevent dangling error

//Embeds payload size in bytes 0-32 of image
  for(i = 0; i < (HEADINFO / 2); i++){ //For Loop goes from 0-32
    //Declares & intializes an array with a 32 bit binary that holds payload size 
    unsigned char bin[32]={BYTETOBINARY32(b.size)};
    if (bin[(HEADINFO / 2) - i]==1){ //Checks if the bit is a 1
      SetGray(i, (GetGray(i) | 1)); //If bit is a 1 then image byte is | with 1
    }else{ //if bit is 0
      SetGray(i, (GetGray(i) & 254)); //image byte is & with 254
       }
    }

//Embeds GNumber in bytes 32-64 of image
  for(k = (HEADINFO / 2) + 1; k < HEADINFO; k++){ //For Loop goes from 32-64
    for(j = 0; j < 8; j++){ //For Loop goes from 0-8
      //Sets one number at a time from GNumber to binary 
      unsigned char bin[8]={BYTETOBINARY8(gnum[j])};
      for(i = 0; i < 4; i++){ //For Loop from 0-4 to set last 4 values of binary
        if (bin[7 - i] == 1){ //Checks if the last 4 binary have 1
          SetGray(k, (GetGray(k) | 1)); //If a 1 exists it is | with an image byte
        }else{ //If it is a 0
          SetGray(k, (GetGray(k) & 254)); //If 0 Image byte is & with 254
           }
        k++; //increments to get next image byte 
         }
      }
    }

//Embeds payload in bytes 65 - payload size of image
  for (k = HEADINFO; k < (b.size * 8) + HEADINFO;){ //For Loop from 64 - (payload size * 8)+64
    for (j = 0; j < b.size; j++){ //For Loop from 0 - payload size
      unsigned char bin[8]={BYTETOBINARY8(sb.data[j])}; //Sets scrambled byte to binary 
      for(i = 0; i < 8; i++){ //For Loop from 0 - 8
        k++; //Increments to get next image byte
        if (bin[7 - i] == 1){ //Checks through binary for 1
          SetGray(k, (GetGray(k) | 1)); //If a 1 exists it is | with an image byte
        }else{ //If it is a 0
          SetGray(k, (GetGray(k) & 254)); //If 0 Image byte is & with 254
           }
         }
      }
    }
  free(sb.data); //Frees memory from sb.data
  sb.data = NULL; //Prevents Dangling Error
  //for (i=0; i<sb.size; i++){
        // here you embed information into the image one byte at the time
        // note that you should change only the least significant bits of the image
  WriteImage(argv[3],img);  // output stego file (cover_file + file_to_hide)
}
