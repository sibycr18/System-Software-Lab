// MacroProcessor ~ Siby C.R.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main() {
    FILE *inputFile, *nameTable, *definitionTable, *argumentTable, *outputFile;
    int len, i, pos = 1;
    char label[20], mnemonic[20], operand[20], macroName[20], macroMnemonic[20],
        macroOperand[20], position1[10], position2[10], argument[20];

    inputFile = fopen("input.txt", "r");
    nameTable = fopen("namTab.txt", "w+");
    definitionTable = fopen("defTab.txt", "w+");
    argumentTable = fopen("argTab.txt", "w+");
    outputFile = fopen("outputProgram.txt", "w+");

    fscanf(inputFile, "%s%s%s", label, mnemonic, operand);

    while (strcmp(mnemonic, "END") != 0) {
        if (strcmp(mnemonic, "MACRO") == 0) {
            fprintf(nameTable, "%s\n", label);
            fseek(nameTable, SEEK_SET, 0);
            fprintf(definitionTable, "%s\t%s\n", label, operand);
            fscanf(inputFile, "%s%s%s", label, mnemonic, operand);

            while (strcmp(mnemonic, "MEND") != 0) {
                if (operand[0] == '&') {
                    sprintf(position1, "%d", pos);
                    strcpy(position2, "?");
                    strcpy(operand, strcat(position2, position1));
                    pos = pos + 1;
                }
                fprintf(definitionTable, "%s\t%s\n", mnemonic, operand);
                fscanf(inputFile, "%s%s%s", label, mnemonic, operand);
            }
            fprintf(definitionTable, "%s", mnemonic);
        } else {
            fscanf(nameTable, "%s", macroName);
            if (strcmp(mnemonic, macroName) == 0) {
                len = strlen(operand);
                for (i = 0; i < len; i++) {
                    if (operand[i] != ',')
                        fprintf(argumentTable, "%c", operand[i]);
                    else
                        fprintf(argumentTable, "\n");
                }

                fseek(definitionTable, SEEK_SET, 0);
                fseek(argumentTable, SEEK_SET, 0);
                fscanf(definitionTable, "%s%s", macroMnemonic, macroOperand);
                fprintf(outputFile, ".\t%s\t%s\n", macroMnemonic, operand);

                fscanf(definitionTable, "%s%s", macroMnemonic, macroOperand);

                while (strcmp(macroMnemonic, "MEND") != 0) {
                    if ((operand[0] == '?')) {
                        fscanf(argumentTable, "%s", argument);
                        fprintf(outputFile, "-\t%s\t%s\n", macroMnemonic, argument);
                    } else {
                        fprintf(outputFile, "-\t%s\t%s\n", macroMnemonic, macroOperand);
                    }

                    fscanf(definitionTable, "%s%s", macroMnemonic, macroOperand);
                }
            } else {
                fprintf(outputFile, "%s\t%s\t%s\n", label, mnemonic, operand);
            }
        }

        fscanf(inputFile, "%s%s%s", label, mnemonic, operand);
    }
    fprintf(outputFile, "%s\t%s\t%s", label, mnemonic, operand);
    fclose(inputFile);
    fclose(nameTable);
    fclose(definitionTable);
    fclose(argumentTable);
    fclose(outputFile);
    printf("Output files created successfully...\n");
}
