#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *objectFile;

char programName[10], record[100], location[4], relocBits[3], bitmask[30], objectCode[6], instructionLengthStr[2];

int i, j, k = 0, flag = 0, recordLength, startLocation, currentIndex, newLocation, loadAddress, objectCodeCount = 0;
long relativeAddress;

void convertHexToBinary() {
    i = 0;
    while (relocBits[i]) {
        switch (relocBits[i]) {
            case '0':
                strcat(bitmask, "0000");
                break;
            case '1':
                strcat(bitmask, "0001");
                break;
            case '2':
                strcat(bitmask, "0010");
                break;
            case '3':
                strcat(bitmask, "0011");
                break;
            case '4':
                strcat(bitmask, "0100");
                break;
            case '5':
                strcat(bitmask, "0101");
                break;
            case '6':
                strcat(bitmask, "0110");
                break;
            case '7':
                strcat(bitmask, "0111");
                break;
            case '8':
                strcat(bitmask, "1000");
                break;
            case '9':
                strcat(bitmask, "1001");
                break;
            case 'A':
            case 'a':
                strcat(bitmask, "1010");
                break;
            case 'B':
            case 'b':
                strcat(bitmask, "1011");
                break;
            case 'C':
            case 'c':
                strcat(bitmask, "1100");
                break;
            case 'D':
            case 'd':
                strcat(bitmask, "1101");
                break;
            case 'E':
            case 'e':
                strcat(bitmask, "1110");
                break;
            case 'F':
            case 'f':
                strcat(bitmask, "1111");
                break;
        }
        i++;
    }
}

int main() {
    printf("\nEnter the name of the program to be loaded: ");
    scanf("%s", programName);

    objectFile = fopen("obj2.txt", "r");
    fscanf(objectFile, "%s", record);

    // Validate program name
    for (i = 0; i < strlen(programName); i++) {
        if (record[i + 2] != programName[i]) {
            flag = 1;
            break;
        }
    }

    if (flag == 1) {
        printf("Invalid program name!\n");
    } else {
        printf("\nEnter load address: ");
        scanf("%x", &loadAddress);

        // Read start location from text record
        for (i = strlen(programName) + 5, k = 0; k < 4, i <= strlen(programName) + 8; k++, i++) {
            location[k] = record[i];
        }

        while (record[0] != 'E') {
            bitmask[0] = '\0';
            
            if (record[0] == 'T') {
                // Read start location from text record
                for (j = 4, k = 0; j < 8, k < 4; k++, j++) {
                    location[k] = record[j];
                }

                sscanf(location, "%x", &startLocation);
                newLocation = startLocation;

                // Read relocating bits
                for (i = 12, j = 0; i < 15, j < 3; i++, j++) {
                    relocBits[j] = record[i];
                }

                // Get instruction length
                for (i = 9, k = 0; i < 11, k < 2; i++, k++) {
                    instructionLengthStr[k] = record[i];
                }

                sscanf(instructionLengthStr, "%x", &objectCodeCount);

                objectCodeCount = objectCodeCount / 3; // Dividing by 3 to get total number of object codes

                convertHexToBinary(); // Get the bit mask

                currentIndex = 16;

                for (i = 0; i < 10; i++) {
                    while (objectCodeCount > 0) {
                        for (j = 0, k = currentIndex; j < 6, k < currentIndex + 6; j++, k++) {
                            objectCode[j] = record[k];
                        }
                        currentIndex += 7;

                        if (bitmask[i] == '1') { // Relocation required
                            sscanf(objectCode, "%lx", &relativeAddress);
                            printf("00%x\t  %lx\n", newLocation + loadAddress, relativeAddress + loadAddress);
                            newLocation += 3;
                            objectCodeCount--;
                            break;
                        } else { // Relocation not required
                            sscanf(objectCode, "%lx", &relativeAddress);
                            printf("00%x\t  %lx\n", newLocation + loadAddress, relativeAddress);
                            newLocation += 3;
                            objectCodeCount--;
                            break;
                        }
                    }
                }
            }
            fscanf(objectFile, "%s", record);
        }
    }

    return 0;
}
