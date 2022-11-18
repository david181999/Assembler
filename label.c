/* this file is some functions for the struct "label" */

#include "data.h"


/*return pointer to label if it is inside the list return null otherwise*/
label *search_label(label **l_head, char *to_find)
{
    label *curr = *l_head;
    
    while(curr != NULL)
    {
        if((strcmp(to_find,curr -> name)) == 0) return curr;
        else curr = curr -> next;
    }

    return NULL;
}

/*check if the label name is valid*/
bool is_label_well_declared(const char *to_check)
{
    int i = 0;
    if((strlen(to_check)) > LABEL_LENGTH) return false;/*label too long*/
    if(!isalpha(to_check[0])) return false;/*illegal first char of label*/

    for(; i < strlen(to_check) ; i++)
        if((!isalpha(to_check[i])) && (!isdigit(to_check[i]))) return false;

    if(is_reserved(to_check)) return false;
    else return true;
}

/*add new labek to list*/
enum error_type insert_label(label **l_head,char *name,int address,bool code,bool external,bool entry)
{
    label *node = (label *)malloc(sizeof(label)), *temp;
    if(node == NULL) return no_memory; 
    if(!is_label_well_declared(name)) return invalid_label;

    strcpy(node -> name,name); /*initialize new label node*/
    node -> address     = address;
    node -> code        = code;
    node -> external    = external;
    node -> entry       = entry;
    node -> next        = NULL;

    if(*l_head == NULL) /*check for empty list*/
    {
        *l_head = node;
    }
    else
    {
        temp = *l_head;

        while((temp -> next) != NULL)
        temp = temp -> next;

        temp -> next = node;
    }
    return valid;
}

/*  only update labels of data that are not external */	


void update_label_list(label **l_head, int ICF)
{
    if(*l_head != NULL)
    {
        label *temp = *l_head;

        do
        {
            if((!(temp->code)) && (!(temp->external))) /* its not external and not code, so it's data*/
                temp ->address += ICF;

            temp = temp -> next;

        }while(temp != NULL);
    }
}
