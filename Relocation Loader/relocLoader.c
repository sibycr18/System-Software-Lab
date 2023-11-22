// Relocation Loader ~ Siby C.R.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char bitmask[12], bit[12] = {0};

void hexToBinary(char h[12]){
    int i, l;
    strcpy(bit, "");
    l = strlen(h);
    for (i = 0; i < l; i++){
        switch (h[i]){
        case '0': strcat(bit, "0");    break;
        case '1': strcat(bit, "1");    break;
        case '2': strcat(bit, "10");   break;
        case '3': strcat(bit, "11");   break;
        case '4': strcat(bit, "100");  break;
        case '5': strcat(bit, "101");  break;
        case '6': strcat(bit, "110");  break;
        case '7': strcat(bit, "111");  break;
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

void main(){
    char add[6], length[10], input[10], binary[12], relocbit, ch, pn[5];
    int start, inp, len, i, address, opcode, addr, actualadd, tlen;
    FILE *fp1, *fp2;
    printf("\nEnter the actual starting address: ");
    scanf("%X", &start);
    fp1 = fopen("RLIN.txt", "r");
    fp2 = fopen("RLOUT.txt", "w");
    fscanf(fp1, "%s", input);

    fprintf(fp2, "Address \tObjCode\n");
    fprintf(fp2, "-------------------");
    while (strcmp(input, "E") != 0){
        if (strcmp(input, "H") == 0){
            fscanf(fp1, "%s", pn);
            fscanf(fp1, "%X", add);
            fscanf(fp1, "%X", length);
            fscanf(fp1, "%s", input);
        }
        if (strcmp(input, "T") == 0){
            fscanf(fp1, "%X", &address);
            fscanf(fp1, "%X", &tlen);
            fscanf(fp1, "%s", bitmask);

            address += start;
            hexToBinary(bitmask);
            len = strlen(bit);

            if (len >= 11)
                len = 10;
            for (i = 0; i < len; i++){
                fscanf(fp1, "%X", &opcode);
                fscanf(fp1, "%X", &addr);
                relocbit = bit[i];
                if (relocbit == '0')
                    actualadd = addr;
                else
                    actualadd = addr + start;

                fprintf(fp2, "\n00%X\t\t%X%X", start, opcode, actualadd);
                start += 3;
            }
            fscanf(fp1, "%s", input);
        }
    }
    fclose(fp1);
    fclose(fp2);
    printf("\nOutput file created successfully...\n");
}
