#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *objectFile;

char programName[10], record[100], location[4], instruction[2];

int i, j, k=0, flag=0, recordLength, startAddress, currentIndex, newLocation;

void main(){

    // printf("\nEnter the name of the program to be loaded: ");
    // scanf("%s", programName);

    objectFile = fopen("objectProgram.dat", "r");
    fscanf(objectFile, "%s", record);

    // 
    // for (i = 0; i < strlen(record); i++)
    // {
    //     printf("%c", record[i]);
    // }
    // printf("\n");
    // 

    // // Check given name = program name in record
    // for(i=0; i<strlen(programName); i++){
    //     if(record[i+2] != programName[i]){
    //         printf("Invalid program name!\n");
    //         return;
    //     }
    // }

    printf("\nLocation\tObject Code\n");
    printf("----------------------------\n");
    while(record[0] != 'E'){
        currentIndex = 12;
        if (record[0] == 'T'){
            for (j=4, k=0; j<8, k<4; k++, j++){
                location[k] = record[j];
            }

            startAddress = atoi(location);
            newLocation = startAddress;

            while(record[currentIndex] != '$'){
                if(record[currentIndex] == '^'){
                    currentIndex++;
                }
                else{
                    printf("00%d  \t   %c%c\n", newLocation, record[currentIndex], record[currentIndex+1]);
                    currentIndex +=2;
                    newLocation += 1;
                }
            }
        }
        fscanf(objectFile, "%s", record);
    }
}
