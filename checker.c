/* this file check things for the first passing */
#include "data.h"

/*if found register return one more then its value return 0 otherwise*/
char is_reg(const char *to_check)
{
    if (strcmp(to_check, "$0") == 0)  return 1;
    if (strcmp(to_check, "$1") == 0)  return 2;
    if (strcmp(to_check, "$2") == 0)  return 3;
    if (strcmp(to_check, "$3") == 0)  return 4;
    if (strcmp(to_check, "$4") == 0)  return 5;
    if (strcmp(to_check, "$5") == 0)  return 6;
    if (strcmp(to_check, "$6") == 0)  return 7;
    if (strcmp(to_check, "$7") == 0)  return 8;
    if (strcmp(to_check, "$8") == 0)  return 9;
    if (strcmp(to_check, "$9") == 0)  return 10;
    if (strcmp(to_check, "$10") == 0) return 11;
    if (strcmp(to_check, "$11") == 0) return 12;
    if (strcmp(to_check, "$12") == 0) return 13;
    if (strcmp(to_check, "$13") == 0) return 14;
    if (strcmp(to_check, "$14") == 0) return 15;
    if (strcmp(to_check, "$15") == 0) return 16;
    if (strcmp(to_check, "$16") == 0) return 17;
    if (strcmp(to_check, "$17") == 0) return 18;
    if (strcmp(to_check, "$18") == 0) return 19;
    if (strcmp(to_check, "$19") == 0) return 20;
    if (strcmp(to_check, "$20") == 0) return 21;
    if (strcmp(to_check, "$21") == 0) return 22;
    if (strcmp(to_check, "$22") == 0) return 23;
    if (strcmp(to_check, "$23") == 0) return 24;
    if (strcmp(to_check, "$24") == 0) return 25;
    if (strcmp(to_check, "$25") == 0) return 26;
    if (strcmp(to_check, "$26") == 0) return 27;
    if (strcmp(to_check, "$27") == 0) return 28;
    if (strcmp(to_check, "$28") == 0) return 29;
    if (strcmp(to_check, "$29") == 0) return 30;
    if (strcmp(to_check, "$30") == 0) return 31;
    if (strcmp(to_check, "$31") == 0) return 32;
    else return 0;
}

/* return true if the string is "extern" */
bool is_ex(const char *to_check)
{
    if((strcmp(to_check, "extern") == 0)) return true;
    else return false;
}

/* return true if the string is "entry" */
bool is_en(const char *to_check)
{
    if((strcmp(to_check, "entry") == 0)) return true;
    else return false;
}
/* return true if the string is "data instruction" (asciz\dh\dw\db) */
bool is_directive(const char *to_check)
{
    if((strcmp(to_check, "dh") == 0) || (strcmp(to_check, "asciz") == 0)
    || (strcmp(to_check, "dw") == 0) || (strcmp(to_check, "db")    == 0)) return true;
    else return false;
}

/* check if the string is valid */
bool is_reserved(const char *to_check)
{
    if(   is_reg(to_check) || is_directive(to_check) || is_en(to_check)
        || is_ex(to_check) || (search_instruction(to_check) != NULL)) return true;
    else return false;
}

/* check if the imm have a valid value */
enum error_type check_imm(char* first, char* temp)
{
    /*if first char is not digit or the number is not integer*/
    if ((strcmp(temp, first) == 0) || (isgraph(*temp)))
    {
        return invalid_imm;
    }

    /*check to see if read the whole parameter*/
    while (isspace(*temp))
    {
        temp++;
    }
    if ((*temp) != '\0') return invalid_imm;

    return valid;
}

/* check if the register is valid */
char check_reg(char* first)
{
    char temp_str[LINE_LENGTH] ,*temp = strchr(first, '$');
    bool single = true;

    if (temp == NULL) return UNKNOWN;
    else
    {
        while ((isspace(temp[2])) || isdigit(temp[2]))
        {
            if(isdigit(temp[2])) single = false;
            temp++;
        }
        if (temp[2] != '\0')   return UNKNOWN;

        temp = strchr(first, '$');
        strcpy(temp_str, temp);
        if(single)
            temp_str[2] = '\0';
        else
            temp_str[3] = '\0';
    }

    if (!is_reg(temp_str)) return UNKNOWN;
    else return is_reg(temp_str);
}
