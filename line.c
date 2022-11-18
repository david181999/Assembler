/* this file is some functions for the struct "line" */

#include "data.h"

/*insret line to list depends on its type
this enum is insert line data and encode at the first pass on the files*/
enum error_type insert_line(line **line_head,int opc,int funct,int address,char rs,char rt,char rd
                                          ,long imm, char num_of_bytes, enum operand code)
{
    line *node = (line *)malloc(sizeof(line));
    if(node == NULL) return no_memory;
    /*initialize new line node*/
    node -> address      = address;
    node -> num_of_bytes = num_of_bytes;
    node -> next         = NULL;

    /* .db */
    if (num_of_bytes == 1)
    {
        node->code = (imm & 0xff);
    }

    /* .dh */
    if (num_of_bytes == 2)
    {
        node->code = (imm & 0xffff);
    }

    if (num_of_bytes == 4)
    {
        /* .dw */
        if (code == D)
        {
            node->code = imm;
        }

        /* stop */
        if (code == N)
        {
            node->code = (opc << 26);
        }

        /* add, sub, and, or, nor, move, mvhi, mvlo */
        if (code == R)
        {
            node->code = (opc << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (funct << 6);
        }

        /* addi, subi, andi, ori, nori, beq, bne, blt, bgt, lb, sb, lw, sw, lh, sh */
        if (code == I)
        {     
		node->code = (opc << 26) | (rs << 21) | (rt << 16) | (imm & 0xffff);
        }

        /* jmp, la, call */
        if ((code == L) || (code == J))
        {
            if (imm != (imm & 0x1ffffff)) return invalid_imm;

            if (imm < TO_BE_FOUND)
                node->code = (opc << 26) | (1 << 25) | (imm & 0x1ffffff);
            else
                node->code = (opc << 26) | (0 << 25);
        }
    }


    if(*line_head == NULL) /*check for empty list*/
    {
        *line_head = node;
    }
    else
    {
        line *temp = *line_head;

        while((temp -> next) != NULL)
            temp = temp -> next;

        temp -> next = node;
    }
    return valid;
}

/*  update address of data lines */
void update_data_list(line **d_head, int ICF)
{
    if(*d_head != NULL)
    {
        line *temp = *d_head;

        do
        {
            temp ->address += ICF;
            temp = temp -> next;

        }while(temp != NULL);
    }
}

/* combine the list of instructions such that they are first in order and after them come the data lines */
void merge_list_of_lines(line **line_head,line **d_head)
{
    if(*line_head == NULL)
    {
        *line_head = *d_head;
    }
    else
    {
        line *temp = *line_head;

        while((temp -> next) != NULL)
            temp = temp -> next;

        temp -> next = *d_head;
    }
}

/* serch the currnet line by the address in the second run on the file */
line * search_line_by_address (line **line_head, long int *curr_address)
{
	line *temp = *line_head;	
	if(*line_head == NULL) return NULL;
	

        while((temp -> next) != NULL)
	{
		if (temp -> address ==   *curr_address) return temp;          
		temp = temp -> next;
	}
	return NULL;
}
