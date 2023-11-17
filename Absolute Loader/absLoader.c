// Absolute Loader ~ Siby C.R.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *objectFile;
char record[100], location[4], instruction[2];
int i, j, k=0, flag=0, recordLength, startAddress, currentIndex, newLocation;

void main(){
    // Open object program file
    objectFile = fopen("objectProgram.dat", "r");
    // Read the first record from the file
    fscanf(objectFile, "%s", record);

    printf("Location\tObject Code\n");
    printf("----------------------------\n");

    // Loop until end of record is reached
    while(record[0] != 'E'){
        currentIndex = 12;

        // Check if it's a text record
        if (record[0] == 'T'){
            // Get the start address from the record
            for (k=0; k<4; k++)
                location[k] = record[k+4];
            startAddress = atoi(location);
            newLocation = startAddress;

            // Loop till end of record is reached
            while(record[currentIndex] != '$'){
                if(record[currentIndex] == '^'){
                    // Ignore the '^' character
                    currentIndex++;
                }
                else{
                    printf("00%d  \t   %c%c\n", newLocation, record[currentIndex], record[currentIndex+1]);
                    // Move to the next object code byte
                    currentIndex += 2;
                    newLocation += 1;
                }
            }
        }
        fscanf(objectFile, "%s", record);
    }
}
