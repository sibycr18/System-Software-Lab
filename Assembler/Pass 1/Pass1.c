// Pass 1 ~ Siby C.R.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *programFile, *intermediateFile, *symTab;
int locctr;
char label[100], opcode[100], operand[100];

// Read line from program file
void readline(){
    fscanf(programFile, "%s", label);
    fscanf(programFile, "%s", opcode);
    fscanf(programFile, "%s", operand);
}

// Write line to intermediate file
void writeline(){
    fprintf(intermediateFile, "%X\t", locctr);
    fprintf(intermediateFile, "%s\t", label);
    fprintf(intermediateFile, "%s\t", opcode);
    fprintf(intermediateFile, "%s\n", operand);
}

// Add symbol to symbol table
void add_sym(){
    fprintf(symTab, "%s\t", label);
    fprintf(symTab, "%X\n", locctr);
}

void main(){
    // Open necessary files
    symTab = fopen("symtab.dat", "w");
    programFile = fopen("sicprogram.dat", "r");
    intermediateFile = fopen("intermediate.dat", "w");

    // Read first line from program file
    readline();

    // Initialize location counter
    if (strcmp(opcode, "START") == 0){
        locctr = strtol(operand, NULL, 16);
        writeline();
    }
    else
        locctr = 0;

    // Read next line from program file
    readline();
    writeline(); // since same address for START

    while (strcmp(opcode, "END") != 0){
        if (strcmp(label, "**") != 0)
            add_sym();
        if (strcmp(opcode, "WORD") == 0)
            locctr += 3;
        else if (strcmp(opcode, "RESW") == 0)
            locctr += atoi(operand) * 3;
        else if (strcmp(opcode, "RESB") == 0)
            locctr += atoi(operand);
        else if (strcmp(opcode, "BYTE") == 0)
            locctr += strlen(operand)-3;
        else
            locctr += 3;
        readline();
        writeline();
    }

    // Close files
    fclose(programFile);
    fclose(symTab);
    fclose(intermediateFile);
}