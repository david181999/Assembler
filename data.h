#ifndef ERROR_H
#define ERROR_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<limits.h>

#define LINE_LENGTH 82 /* 80 + '\n' + '\0' */
#define LABEL_LENGTH 32 /*  31 + '\0' */
#define INSTRUCTIONS_NUM 27
#define UNKNOWN -1
#define MAX_LABEL_LEN 32
#define TO_BE_FOUND 34
#define ILEGAL_DISTANCE 65536 /* 2^16, cant be ore than 16 bits */
#define BYTS_IN_LINE 4
#define IC_MINUS_4 96 /* starts in 96 and add 4 before instruction */

#define MAX_INSTRUCTION_LEN 5
#define REG_LOCATION 25
#define ILEGAL_LABEL_ADRESS 16777216 /* 2^24, cant be ore than 24 bits */
#define IC_MINUS_4 96 /* starts in 96 and add 4 before instruction */



enum error_type {valid,empty,comment,long_line,already_declared,no_memory,invalid_label, next_line,missing_operand
,expected_reserved,too_many_parameters,missing_parameters,out_of_range,invalid_imm, empty_string, invalid_string
,external_defined_in_other_file,missing_quote,invalid_operand,invalid_instruction,invalid_register,undefined_label};

enum situations {all_good,not_found_label,entry_and_extern_label,cant_find_line,this_label_cant_be_use_to_that_instruction
,ilegal_distance,no_enough_memory,ilegal_label_adress};

enum operand {R,I,L,J,N,D};/*R = register,I = immediate,L = label,J = (L || R),N = no operands,D = data */


typedef enum boolean {false, true}bool;



typedef struct intruction
{
    char name[5];
    char opc;
    char funct;
    enum operand type;
}intruction;

typedef struct label
{
    char name[LABEL_LENGTH];
    long address;
    bool code;
    bool external;
    bool entry;
    struct label *next;
}label;

typedef struct line
{
    long code;
    long address;
    char num_of_bytes;
    struct line *next;
}line;

typedef struct exernal_instruction
{
    long address;
    char label_name[MAX_INSTRUCTION_LEN];
    struct exernal_instruction * next;
}exernal_instruction;


/*global variable for the instruction list*/
intruction list[INSTRUCTIONS_NUM];

char is_reg(const char *);
bool is_ex(const char *);
bool is_en(const char *);
bool is_directive(const char *);
bool is_reserved(const char *);
enum error_type evaluate(char *, label **,line **,line **,int *,int *);
void initOP();
intruction *search_instruction(const char *);
label *search_label(label **l_head, char *);
bool is_label_well_declared(const char *);
enum error_type insert_label(label **,char *,int,bool,bool,bool);
enum error_type insert_line(line** line_head, int opc, int funct, int address, char rs, char rt, char rd
    , long immed, char num_of_bytes, enum operand code);
void update_label_list(label** l_head, int ICF);
void update_data_list(line**  d_head, int ICF);
void merge_list_of_lines(line** line_head, line** d_head);
enum error_type check_imm(char* first, char* temp);
enum situations find_situations (char *, label **, long int *, line **, exernal_instruction ** );
char check_reg(char* first);
void make_ent (label ** , int ,char **);
void make_ext (exernal_instruction **, int ,char **);
void make_ob (char **, long int , long int ,int ,line **);
line * search_line_by_address (line **, long int *);



#endif
