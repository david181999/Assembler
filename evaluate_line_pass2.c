
/* this file is evaluating lines in the second pass on the files */

#include "data.h"


/* this function check if a J instruction need to encode in the second pass or not */
static bool needToEncode (long int code)
{
		
	long int musk = 1<<REG_LOCATION;
	  /* if the reg bit is 0 it need to be encode again, if its 1 so it not */
	return ((code & musk) == 0);
}


enum situations find_situations (char * buff, label **l_head, long int *adress, line **line_head, exernal_instruction ** exernal_instruction_head)
{
    char *first;
    char *token;
    label * curr_label = NULL;
    intruction * curr_intruction = NULL;
    line * curr_line = NULL;
    exernal_instruction * ex_instructions = NULL;
    exernal_instruction * node;
    long int distance; /* for the beq,bne,blt,bgt instruction */
    long int label_adress; /* for the call la jmp instructions */
	
    
    first = strtok(buff, " \n\t\r");
    if(first == NULL)    return all_good;   /*empty line*/
    if(first[0] == ';')  return all_good; /*comment line*/
    token = first;
	
    if (strchr(first, ':') != NULL) /* if this is a label ignor it and go the the next word */
    {
	token = strtok(NULL, " \n\t\r");
	
    }
	
    if (token[0] == '.' &&  (strcmp(token,".entry")) != 0) return all_good; /*if this is instruction but not entry, ignor it*/
	
    if ((strcmp(token,".entry")) == 0)
    {
	token = strtok(NULL, " \n\t\r"); /* now token is the name of the label (the .entry operand) */
	
	curr_label = search_label(l_head, token);
	if (curr_label == NULL)
	{
		return not_found_label; /* label not found, error! */
	}
	if(curr_label -> external == true)
	{
		return entry_and_extern_label; /* label cant be extern and entry, error! */
	}
	curr_label -> entry = true;
	return all_good;
    }
	/* else, this is a instruction line, need to encode the line. */
    		/* update the new address */
            *adress += BYTS_IN_LINE;
	   curr_intruction = (intruction *)search_instruction (token);
	  
     /* the first pass on the file succeeded, must be legal instruction */
    
		
    curr_line = (line *)search_line_by_address (line_head,adress);
	
    if(curr_line == NULL) return cant_find_line;
	
	
    if (strcmp((curr_intruction -> name),"beq") == 0 || strcmp((curr_intruction -> name),"bne") == 0 || strcmp((curr_intruction -> name),"blt") == 0 || strcmp((curr_intruction -> name),"bgt") == 0) /* if it is so it's have to encode because it's use label */
     {
	/* get the label name */
		
	 token = strtok(NULL,", \n\t\r");
         token = strtok(NULL,", \n\t\r");
         token = strtok(NULL,", \n\t\r");
         curr_label = search_label (l_head,token);
	 if(curr_label == NULL) return not_found_label; /* label not found, error! */
	 if(curr_label -> external == true) return this_label_cant_be_use_to_that_instruction; /* external label, error! */
	 distance = (curr_label -> address) - *adress;
	 if(distance > ILEGAL_DISTANCE) return ilegal_distance; /* it will be bigger than 16 bits */
	 curr_line -> code = (((curr_line -> code) & 0xffff0000) | (distance & 0xffff)); /* make the 16 bits in 0 value and then encode the distance */
      }
	
    if (strcmp((curr_intruction -> name),"jmp") == 0 || strcmp((curr_intruction -> name),"la") == 0 || strcmp((curr_intruction -> name),"call") == 0)
    {
			
	if (strcmp((curr_intruction -> name),"jmp") == 0) /* if its jmp check if the operand is label or register, if its register so not need to encode */
	{
		if (needToEncode(curr_line -> code) == false) return all_good;
	}
	/* get the label name */
		
	token = strtok(NULL,", \n\t\r");
	curr_label = search_label (l_head,token);
	if(curr_label == NULL) return not_found_label; /* label not found, error! */
	
	if(curr_label -> external == true) 
	{			
		/*if this is extern label so encode this and add the label name and line address to the list of external instructions (to .ext outputs files)*/	
		ex_instructions = (exernal_instruction *)malloc(sizeof(exernal_instruction));
    		if(ex_instructions == NULL) return no_enough_memory;
		ex_instructions -> address = *adress;
		strcpy(ex_instructions -> label_name, curr_label -> name);
		ex_instructions -> next = NULL;
		if(*exernal_instruction_head == NULL) /*check for empty list*/
    		{      		
			*exernal_instruction_head = ex_instructions;
    		}
		else 
		{
			node = *exernal_instruction_head;
			while((node -> next) != NULL)
			{
				node = node -> next;
			}
			node -> next = ex_instructions;
		}
		curr_line -> code = curr_line -> code & 0xfc000000 ; /* bit number 25 have to be '0' because it's use in a external label */
		return all_good;
	}
	label_adress = curr_label -> address; /* else, encode the label */
	if(distance > ILEGAL_LABEL_ADRESS) return ilegal_label_adress; /* it will be bigger than 24 bits */
	curr_line -> code = (curr_line -> code) | label_adress;
	return all_good;
     }
	return all_good;
}
