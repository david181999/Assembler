/* this file initalize the instruction */
#include "data.h" 

/*initalize instruction list*/
void initOP()
{
    strcpy(list[0].name, "add");
    list[0].opc = 0;
    list[0].funct = 1;
    list[0].type = R;

    strcpy(list[1].name, "sub");
    list[1].opc = 0;
    list[1].funct = 2;
    list[1].type = R;

    strcpy(list[2].name, "and");
    list[2].opc = 0;
    list[2].funct = 3;
    list[2].type = R;

    strcpy(list[3].name, "or");
    list[3].opc = 0;
    list[3].funct = 4;
    list[3].type = R;

    strcpy(list[4].name, "nor");
    list[4].opc = 0;
    list[4].funct = 5;
    list[4].type = R;

    strcpy(list[5].name, "move");
    list[5].opc = 1;
    list[5].funct = 1;
    list[5].type = R;

    strcpy(list[6].name, "mvhi");
    list[6].opc = 1;
    list[6].funct = 2;
    list[6].type = R;

    strcpy(list[7].name, "mvlo");
    list[7].opc = 1;
    list[7].funct = 3;
    list[7].type = R;

    strcpy(list[8].name, "addi");
    list[8].opc = 10;
    list[8].funct = 0;
    list[8].type = I;

    strcpy(list[9].name, "subi");
    list[9].opc = 11;
    list[9].funct = 0;
    list[9].type = I;

    strcpy(list[10].name, "andi");
    list[10].opc = 12;
    list[10].funct = 0;
    list[10].type = I;

    strcpy(list[11].name, "ori");
    list[11].opc = 13;
    list[11].funct = 0;
    list[11].type = I;

    strcpy(list[12].name, "nori");
    list[12].opc = 14;
    list[12].funct = 0;
    list[12].type = I;

    strcpy(list[13].name, "bne");
    list[13].opc = 15;
    list[13].funct = 0;
    list[13].type = I;

    strcpy(list[14].name, "beq");
    list[14].opc = 16;
    list[14].funct = 0;
    list[14].type = I;

    strcpy(list[15].name, "blt");
    list[15].opc = 17;
    list[15].funct = 0;
    list[15].type = I;

    strcpy(list[16].name, "bgt");
    list[16].opc = 18;
    list[16].funct = 0;
    list[16].type = I;

    strcpy(list[17].name, "lb");
    list[17].opc = 19;
    list[17].funct = 0;
    list[17].type = I;

    strcpy(list[18].name, "sb");
    list[18].opc = 20;
    list[18].funct = 0;
    list[18].type = I;

    strcpy(list[19].name, "lw");
    list[19].opc = 21;
    list[19].funct = 0;
    list[19].type = I;

    strcpy(list[20].name, "sw");
    list[20].opc = 22;
    list[20].funct = 0;
    list[20].type = I;

    strcpy(list[21].name, "lh");
    list[21].opc = 23;
    list[21].funct = 0;
    list[21].type = I;

    strcpy(list[22].name, "sh");
    list[22].opc = 24;
    list[22].funct = 0;
    list[22].type = I;

    strcpy(list[23].name, "jmp");
    list[23].opc = 30;
    list[23].funct = 0;
    list[23].type = J;

    strcpy(list[24].name, "la");
    list[24].opc = 31;
    list[24].funct = 0;
    list[24].type = L;

    strcpy(list[25].name, "call");
    list[25].opc = 32;
    list[25].funct = 0;
    list[25].type = L;

    strcpy(list[26].name, "stop");
    list[26].opc = 63;
    list[26].funct = 0;
    list[26].type = N;

 }

 /*return pointer to instruction if it is inside the list return null otherwise*/
 intruction *search_instruction(const char *to_check)
{
     int i = 0;
     for(; i < INSTRUCTIONS_NUM; i++)
        if(strcmp(to_check, list[i].name) == 0) return list + i;

     return NULL;
}
