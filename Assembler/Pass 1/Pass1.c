// Pass 1 ~ Siby C.R.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fptr, *inter_ptr, *sym;
int locctr;
char label[100], opcode[100], operand[100];

// Read line from program file
void readline(){
    fscanf(fptr, "%s", label);
    fscanf(fptr, "%s", opcode);
    fscanf(fptr, "%s", operand);
}

// Write line to intermediate file
void writeline(){
    fprintf(inter_ptr, "%X\t", locctr);
    fprintf(inter_ptr, "%s\t", label);
    fprintf(inter_ptr, "%s\t", opcode);
    fprintf(inter_ptr, "%s\n", operand);
}

// Add symbol to symbol table
void add_sym(){
    fprintf(sym, "%s\t", label);
    fprintf(sym, "%x\n", locctr);
}

void main(){
    // Open necessary files
    sym = fopen("symtab.dat", "w");
    fptr = fopen("sicprogram.dat", "r");
    inter_ptr = fopen("intermediate.dat", "w");

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

    while (strcmp(opcode, "END") != 0){
        if (strcmp(label, "**") != 0)
            add_sym(); // to check for duplicates in symtab
        if (strcmp(opcode, "WORD") == 0){
            writeline();
            locctr += 3;
        }
        else if (strcmp(opcode, "RESW") == 0){
            writeline();
            locctr += atoi(operand) * 3;
        }
        else if (strcmp(opcode, "RESB") == 0){
            writeline();
            locctr += atoi(operand);
        }
        else if (strcmp(opcode, "BYTE") == 0){
            writeline();
            locctr += strlen(operand)-3;
        }
        else{
            writeline();
            locctr += 3;
        }
        readline();
    }
    writeline();

    // Close files
    fclose(fptr);
    fclose(sym);
    fclose(inter_ptr);
}