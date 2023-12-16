// Relocation Loader ~ Siby C.R.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Global variables for bitmask and binary representation
char bitmask[12], bit[12] = {0};

// Function to convert hexadecimal to binary
void hexToBinary(char h[12]) {
    int i, l;
    strcpy(bit, "");
    l = strlen(h);
    for (i = 0; i < l; i++) {
        switch (h[i]) {
            case '0': strcat(bit, "0"); break;
            case '1': strcat(bit, "1"); break;
            case '2': strcat(bit, "10"); break;
            case '3': strcat(bit, "11"); break;
            case '4': strcat(bit, "100"); break;
            case '5': strcat(bit, "101"); break;
            case '6': strcat(bit, "110"); break;
            case '7': strcat(bit, "111"); break;
            case '8': strcat(bit, "1000"); break;
            case '9': strcat(bit, "1001"); break;
            case 'A': strcat(bit, "1010"); break;
            case 'B': strcat(bit, "1011"); break;
            case 'C': strcat(bit, "1100"); break;
            case 'D': strcat(bit, "1101"); break;
            case 'E': strcat(bit, "1110"); break;
            case 'F': strcat(bit, "1111"); break;
        }
    }
}

void main() {
    // Variable declarations
    char add[6], length[10], input[10], relocbit, ch, programName[5];
    int start, len, i, address, opcode, addr, actualadd, tlen;
    FILE *inputFile, *outputFile;

    // User input: actual starting address
    printf("\nEnter the actual starting address: ");
    scanf("%X", &start);

    // Open input file for reading and output file for writing
    inputFile = fopen("RLIN.txt", "r");
    outputFile = fopen("RLOUT.txt", "w");

    // Read the first input token
    fscanf(inputFile, "%s", input);

    // Write headers to the output file
    fprintf(outputFile, "Address \tObjCode\n");
    fprintf(outputFile, "-------------------");

    // Main loop to process input records
    while (strcmp(input, "E") != 0) {
        if (strcmp(input, "H") == 0) {
            // Read program name, start address, program length
            fscanf(inputFile, "%s", programName);
            fscanf(inputFile, "%X", add);
            fscanf(inputFile, "%X", length);
            fscanf(inputFile, "%s", input);
        }
        if (strcmp(input, "T") == 0) {
            // Read address, record length, and bitmask
            fscanf(inputFile, "%X", &address);
            fscanf(inputFile, "%X", &tlen);
            fscanf(inputFile, "%s", bitmask);

            // Adjust address based on the starting address
            address += start;

            // Convert bitmask to binary
            hexToBinary(bitmask);
            
            // Limit the length for display purposes
            len = strlen(bit);
            if (len >= 11)
                len = 10;

            // Process object codes and write to the output file
            for (i = 0; i < len; i++) {
                // Read opcode and address from the input file
                fscanf(inputFile, "%X", &opcode);
                fscanf(inputFile, "%X", &addr);

                // Determine if relocation is needed based on the bitmask
                relocbit = bit[i];
                if (relocbit == '0')
                    actualadd = addr;   // No relocation needed
                else
                    actualadd = addr + start;   // Relocation required

                // Write the formatted output to the output file
                fprintf(outputFile, "\n%X\t\t%X%X", start, opcode, actualadd);

                // Move to the next address position
                start += 3;
            }
            // Read the next input record
            fscanf(inputFile, "%s", input);
        }
    }

    // Close the input and output files
    fclose(inputFile);
    fclose(outputFile);

    // Display completion message
    printf("\nOutput file created successfully...\n");
}
