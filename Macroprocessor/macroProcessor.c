// MacroProcessor ~ Siby C.R.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main() {
    FILE *inputFile, *nameTable, *definitionTable, *argumentTable, *outputFile;
    int len, i, pos = 1;
    char label[20], mnemonic[20], operand[20], macroName[20], macroMnemonic[20],
        macroOperand[20], position1[10], position2[10], argument[20];

    // Open input file in read mode (input file)
    inputFile = fopen("input.txt", "r");
    // Open other files in write and read mode (output files)
    nameTable = fopen("namTab.txt", "w+");
    definitionTable = fopen("defTab.txt", "w+");
    argumentTable = fopen("argTab.txt", "w+");
    outputFile = fopen("outputProgram.txt", "w+");

    // Read the first line of the input file
    // Order of contents in input file: label, mnemonic, operand
    fscanf(inputFile, "%s%s%s", label, mnemonic, operand);

    // Process the input file until the mnemonic is "END"
    while (strcmp(mnemonic, "END") != 0) {
        // Check if the mnemonic is "MACRO"
        if (strcmp(mnemonic, "MACRO") == 0) {
            // Write label to name table
            fprintf(nameTable, "%s\n", label);

            // Move the file pointer to the beginning of the name table
            fseek(nameTable, SEEK_SET, 0);
            
            // Write label and operand to the definition table
            fprintf(definitionTable, "%s\t%s\n", label, operand);

            // Read the next line from the input file
            fscanf(inputFile, "%s%s%s", label, mnemonic, operand);

            // Process lines until the mnemonic is "MEND"
            while (strcmp(mnemonic, "MEND") != 0) {
                // Check if the operand starts with '&' (macro argument)
                if (operand[0] == '&') {
                    // Replace the argument with its position
                    sprintf(position1, "%d", pos);
                    strcpy(position2, "?");
                    strcpy(operand, strcat(position2, position1));
                    pos = pos + 1;
                }

                // Write mnemonic and operand to the definition table
                fprintf(definitionTable, "%s\t%s\n", mnemonic, operand);

                // Read the next line from the input file
                fscanf(inputFile, "%s%s%s", label, mnemonic, operand);
            }

            // Write the last line (MEND) to the definition table
            fprintf(definitionTable, "%s", mnemonic);
        } else {
            // Check if the mnemonic matches any macro name in the name table
            fscanf(nameTable, "%s", macroName);
            if (strcmp(mnemonic, macroName) == 0) {
                // Process macro invocation
                len = strlen(operand);
                for (i = 0; i < len; i++) {
                    if (operand[i] != ',')
                        fprintf(argumentTable, "%c", operand[i]);
                    else
                        fprintf(argumentTable, "\n");
                }

                // Move the file pointers to the beginning of the definition and argument tables
                fseek(definitionTable, SEEK_SET, 0);
                fseek(argumentTable, SEEK_SET, 0);

                // Read the first line from the definition table
                fscanf(definitionTable, "%s%s", macroMnemonic, macroOperand);

                // Write macro invocation to the output file
                fprintf(outputFile, "-\t%s\t%s\n", macroMnemonic, operand);

                // Read the next line from the definition table
                fscanf(definitionTable, "%s%s", macroMnemonic, macroOperand);

                // Process lines until the mnemonic is "MEND"
                while (strcmp(macroMnemonic, "MEND") != 0) {
                    // Check if the macro operand is a parameter
                    if ((operand[0] == '?')) {
                        // Read the corresponding argument from the argument table
                        fscanf(argumentTable, "%s", argument);

                        // Write macro line to the output file with the argument
                        fprintf(outputFile, "-\t%s\t%s\n", macroMnemonic, argument);
                    } else {
                        // Write macro line to the output file with the operand
                        fprintf(outputFile, "-\t%s\t%s\n", macroMnemonic, macroOperand);
                    }

                    // Read the next line from the definition table
                    fscanf(definitionTable, "%s%s", macroMnemonic, macroOperand);
                }
            } else {
                // Write non-macro line to the output file
                fprintf(outputFile, "%s\t%s\t%s\n", label, mnemonic, operand);
            }
        }

        // Read the next line from the input file
        fscanf(inputFile, "%s%s%s", label, mnemonic, operand);
    }

    // Write the last line of the input file to the output file
    fprintf(outputFile, "%s\t%s\t%s", label, mnemonic, operand);

    // Close all open files
    fclose(inputFile);
    fclose(nameTable);
    fclose(definitionTable);
    fclose(argumentTable);
    fclose(outputFile);

    // Print a success message
    printf("Output files created successfully...\n");
}
