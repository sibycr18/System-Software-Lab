// Pass 1 ~ Siby C.R.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    // Declare variables
    char buffer[50], addressStr[50], label[50], opcode[50], operand[50], mnemonic[50], symbol[50];
    int i, address, code, symbolAddress, len, actual_len;
    FILE *outputFile, *symbolTable, *intermediateFile, *opcodeTable;

    // Open necessary files
    outputFile = fopen("assmlist.dat", "w");
    symbolTable = fopen("symtab.dat", "r");
    intermediateFile = fopen("intermediate.dat", "r");
    opcodeTable = fopen("optab.dat", "r");

    // Read the first line from the intermediate file
    fscanf(intermediateFile, "%d%s%s%s", &address, label, opcode, operand);

    // Check if the opcode is START
    if (strcmp(opcode, "START") == 0) {
        // Write START record to the output file
        fprintf(outputFile, "\t%s\t%s\t%s\n", label, opcode, operand);
        // Read the next line from the intermediate file
        fscanf(intermediateFile, "%d%s%s%s", &address, label, opcode, operand);
    }

    // Main loop processing intermediate file records
    while (strcmp(opcode, "END") != 0) {
        // Check opcode type
        if (strcmp(opcode, "BYTE") == 0) {
            // Process BYTE instruction
            fprintf(outputFile, "%d\t%s\t%s\t%s\t", address, label, opcode, operand);
            len = strlen(operand);
            actual_len = len - 3;
            // Convert characters to hexadecimal and write to the output file
            for (i = 2; i < (actual_len + 2); i++) {
                itoa(operand[i], addressStr, 16);
                fprintf(outputFile, "%s", addressStr);
            }
            fprintf(outputFile, "\n");
        } else if (strcmp(opcode, "WORD") == 0) {
            // Process WORD instruction
            len = strlen(operand);
            itoa(atoi(operand), buffer, 10);
            fprintf(outputFile, "%d\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, buffer);
        } else if ((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0)) {
            // Process RESB or RESW instruction
            fprintf(outputFile, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
        } else {
            // Process other instructions
            rewind(opcodeTable);

            // Find the corresponding opcode in the opcode table
            do {
                fscanf(opcodeTable, "%s%d", mnemonic, &code);
            } while (strcmp(opcode, mnemonic) != 0);

                
            // printf("%s\n", operand);


            if (strcmp(operand, "**") == 0) {
                // If operand is **, write format 1 instruction
                fprintf(outputFile, "%d\t%s\t%s\t%s\t%d0000\n", address, label, opcode, operand, code);
            } else {
                // If operand is a symbol, find its address in the symbol table
                rewind(symbolTable);

                // Find the corresponding symbol in the symbol table
                do {
                    fscanf(symbolTable, "%s%d", symbol, &symbolAddress);
                } while (strcmp(operand, symbol) != 0);
                // printf("Hey\t%s\t%s\n", operand, symbol);

                // Write the complete machine code to the output file
                fprintf(outputFile, "%d\t%s\t%s\t%s\t%d%d\n", address, label, opcode, operand, code, symbolAddress);
                // printf("Wrote to out file\n");
            }
        }

        // printf("Read next line from intermediate file\n\n");
        // Read the next line from the intermediate file
        fscanf(intermediateFile, "%s%s%s%s", address, label, opcode, operand);
        printf("%s\t%s\t%s\t%s\n", address, label, opcode, operand);
    }
    // Write the last line (END record) to the output file
    fprintf(outputFile, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);

    printf("Finished\n");
    // Close all files
    fclose(outputFile);
    fclose(symbolTable);
    fclose(intermediateFile);
    fclose(opcodeTable);
    return 0;
}