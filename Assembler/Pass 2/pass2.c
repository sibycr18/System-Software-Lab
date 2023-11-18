#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File pointers
FILE *objectFile, *opcodeTable, *symbolTable, *intermediateFile, *lengthFile;

// Variables
int totalLength, locationCounter, instructionLength;
char currentLabel[10] = {' '}, currentOperand[10] = {' '}, currentOpcode[10] = {' '},
     opcodeTableOpcode[10] = {' '}, opcodeTableMnemonic[10] = {' '},
     symbolTableOperand[10] = {' '}, symbolTableOpcode[10] = {' '}, machineInstruction[10] = {' '};

// Function to convert operand to hexadecimal
void convertToHexadecimal() {
    fprintf(objectFile, "^");
    for (int i = 2; i < strlen(currentOperand) - 1; i++) {
        fprintf(objectFile, "%x", currentOperand[i]);
    }
}

// Function to get symbol from symbol table
void getSymbolFromTable() {
    symbolTable = fopen("symtab.dat", "r");
    fscanf(symbolTable, "%s%s", symbolTableOperand, symbolTableOpcode);
    while (!feof(symbolTable)) {
        if (strcmp(symbolTableOperand, currentOperand) == 0) {
            strcat(machineInstruction, symbolTableOpcode);
            break;
        }
        fscanf(symbolTable, "%s%s", symbolTableOperand, symbolTableOpcode);
    }
    rewind(symbolTable);
}

// Function to get opcode from opcode table
void getOpcodeFromTable() {
    opcodeTable = fopen("optab.dat", "r");
    fscanf(opcodeTable, "%s%s", opcodeTableMnemonic, opcodeTableOpcode);
    while (!feof(opcodeTable)) {
        machineInstruction[0] = '\0';

        if (strcmp(opcodeTableMnemonic, currentOpcode) == 0) {
            strcpy(machineInstruction, opcodeTableOpcode);
            if (strcmp(currentOperand, "-") != 0) {
                getSymbolFromTable();
                fprintf(objectFile, "^%s", machineInstruction);
            } else {
                strcat(machineInstruction, "0000");
                fprintf(objectFile, "^%s", machineInstruction);
            }
            break;
        }
        // Additional conditions for special cases
        else if (currentOpcode[0] == '+') {
            strcpy(machineInstruction, "0C");
            getSymbolFromTable();
            fprintf(objectFile, "^%s", machineInstruction);
            break;
        } else if (strcmp(currentOpcode, "BYTE") == 0 || strcmp(currentOpcode, "WORD") == 0) {
            if (strcmp(currentOpcode, "WORD") == 0) {
                strcpy(machineInstruction, "00000");
                strcat(machineInstruction, currentOperand);
                fprintf(objectFile, "^%s", machineInstruction);
                break;
            } else {
                convertToHexadecimal();
                break;
            }
        } else if (strcmp(currentOpcode, "RESW") == 0 || strcmp(currentOpcode, "RESB") == 0) {
            break;
        }
        fscanf(opcodeTable, "%s%s", opcodeTableMnemonic, opcodeTableOpcode);
    }
    rewind(opcodeTable);
}

int main() {
    // Open files
    intermediateFile = fopen("intermediate.dat", "r");
    objectFile = fopen("object.dat", "w");
    lengthFile = fopen("length.dat", "r");

    // Read the first line of the intermediate file
    fscanf(intermediateFile, "%s%s%s%s%x", currentOperand, opcodeTableOpcode, currentLabel, currentOpcode, &locationCounter);
    int startAddress = locationCounter;
    fscanf(lengthFile, "%x", &totalLength);

    // Check for START opcode
    if (strcmp(currentOpcode, "START") == 0) {
        fprintf(objectFile, "H^%s^00%x^0000%x\n", currentLabel, locationCounter, totalLength + locationCounter);
    }

    // Write text record length to object file
    fprintf(objectFile, "T^%x", totalLength - 3);

    // Read the next line from the intermediate file
    fscanf(intermediateFile, "%x%x%s%s%s", &locationCounter, &instructionLength, currentLabel, currentOpcode, currentOperand);
    
    // Main loop processing intermediate file records
    while (strcmp(currentOpcode, "END") != 0) {
        getOpcodeFromTable();
        fscanf(intermediateFile, "%x%x%s%s%s", &locationCounter, &instructionLength, currentLabel, currentOpcode, currentOperand);
    }

    // Write end record to object file
    fprintf(objectFile, "\n");
    fprintf(objectFile, "E^00%x", locationCounter);

    // Close files
    fclose(opcodeTable);
    fclose(symbolTable);
    fclose(intermediateFile);
    fclose(lengthFile);
    fclose(objectFile);
    return 0;
}