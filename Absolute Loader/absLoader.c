#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *objectFile;

char record[100], location[4], instruction[2];

int i, j, k=0, flag=0, recordLength, startAddress, currentIndex, newLocation;

void main(){
    objectFile = fopen("objectProgram.dat", "r");
    fscanf(objectFile, "%s", record);

    printf("Location\tObject Code\n");
    printf("----------------------------\n");
    while(record[0] != 'E'){
        currentIndex = 12;
        if (record[0] == 'T'){
            for (k=0; k<4; k++)
                location[k] = record[k+4];

            startAddress = atoi(location);
            newLocation = startAddress;

            while(record[currentIndex] != '$'){
                if(record[currentIndex] == '^'){
                    currentIndex++;
                }
                else{
                    printf("00%d  \t   %c%c\n", newLocation, record[currentIndex], record[currentIndex+1]);
                    currentIndex += 2;
                    newLocation += 1;
                }
            }
        }
        fscanf(objectFile, "%s", record);
    }
}
