
#include "outputs.h"
#include "data.h"
/* In this file there is functions to make the outputs files */

		/*decleretion */
	static void add_line (int , FILE *, int * , int );

/* this function check if its need to add another line in the .ob output file */
static void add_line (int x, FILE * file_ptr, int * counter, int ICF)
{
	if (*counter == ICF) /* if it the first DC line not need to add new line because after the last IC there is "\n" allredy */
	{
		fprintf(file_ptr,"%04d ", *counter);
		*counter += 4;	
		return;
	}
	if(x % 4 == 0)
	{
		fprintf(file_ptr,"\n%04d ", *counter);
		*counter += 4;	
	}
}

/* make the ent output file */
void make_ent (label **l_head, int file_number,char ** argv)
{
	FILE* file_ptr;	
	label *curr; 
	bool found_entry_label = false; /* if there is no entry label - this .ent file have to be removed */
	char * file_name = (char *) malloc (strlen(argv[file_number]) + 1);
	if ( file_name == NULL) 
	{
		printf("no have enough memory\n");
		return;	
	}
	
	strcpy(file_name, *(argv + file_number));
	ENT_FILE_NAME /* change the end of the name from .as to .ent and open the file*/	
	file_ptr = fopen(file_name, "w");
	curr = *l_head;
    
       while(curr != NULL) /*try to found entry label , if it found so it printed to the ent file */
       {         
	if(curr -> entry == true)
        {
		fprintf(file_ptr,"%s %ld\n",curr -> name, curr -> address);
		found_entry_label = true;
	}
           curr = curr -> next;
       }
	if (found_entry_label == false) /* if this, delete this file, else just close him */
	{
		remove(file_name);
		free(file_name);
	}
	else 
	{
		free(file_name);
		fclose(file_ptr);
	}
}

/* make ext file */
void make_ext (exernal_instruction **l_head, int file_number,char ** argv)
{
	FILE* file_ptr;	
	exernal_instruction * curr;
	char * file_name = (char *) malloc (strlen(argv[file_number]) + 1);
	if ( file_name == NULL) 
	{
		printf("no have enough memory\n");
		return;	
	}
	curr = *l_head;	
	if (curr == NULL)
	{
		free(file_name);
	        return;
	}

	/*else, there is some instructions that used in external labels, so it make the .ext file */
	strcpy(file_name, *(argv + file_number));
	EXT_FILE_NAME	/* change .as to .ext */
	file_ptr = fopen(file_name, "w");
	
	free(file_name);
	
	while(curr != NULL) /*run on the external instructions list and print it */
        {         
	   fprintf(file_ptr,"%s %ld\n",curr -> label_name, curr -> address);
           curr = curr -> next;
	}
	fclose(file_ptr);
}

/* make the .ob output file */
void make_ob (char ** argv, long int  all_ICF, long int  all_DCF,int file_number,line ** line_head)
{
	FILE* file_ptr;	
	/*unsigned char *p = NULL;*/
	line * curr = *line_head;
	char * file_name = (char *) malloc (strlen(argv[file_number]));
	int counter = 100;
	int counter_to_new_line = 0;
	unsigned char a = 0;
	unsigned char b = 0;
	unsigned char c = 0;
	unsigned char d = 0;

	strcpy(file_name, *(argv + file_number));
	OB_FILE_NAME	/* change .as to .ob and make the file  */
	file_ptr = fopen(file_name, "w");

	fprintf(file_ptr,"     %ld %ld\n",all_ICF - 100 , all_DCF - all_ICF); /* print the IC and DC size */
	while(curr != NULL)
	{
		/*if we in the IC so evry line print all 4 bytes*/
		if(counter < all_ICF) /* print every byte */
		{		
			GET_BITES	/* get evry 8 bits to a byte to a,b,c,d */
			fprintf(file_ptr,"%04d %02x ",counter, a);
			fprintf(file_ptr,"%02x ",b);
			fprintf(file_ptr,"%02x ",c);
			fprintf(file_ptr,"%02x\n",d);
		
		}
		else /* if we in the DC so it's need to count and check if it's need to add line after 4 bytes (%4 == 0) */
		{	 /* so it's check it befor print the data every byte */			
			if(curr -> num_of_bytes == 4)
			{		
				add_line(counter_to_new_line,file_ptr,&counter,all_ICF);
				GET_BITES /* get evry 8 bits to a byte to a,b,c,d  */		
				fprintf(file_ptr,"%02x ",a);
				counter_to_new_line++;
				add_line(counter_to_new_line,file_ptr,&counter,all_ICF);
				fprintf(file_ptr,"%02x ",b);
				counter_to_new_line++;
				add_line(counter_to_new_line,file_ptr,&counter,all_ICF);
				fprintf(file_ptr,"%02x ",c);
				counter_to_new_line++;
				add_line(counter_to_new_line,file_ptr,&counter,all_ICF);
				fprintf(file_ptr,"%02x ",d);
				counter_to_new_line++;
				curr = curr-> next;
				continue;
			}			
			if(curr -> num_of_bytes == 2)
			{		
				add_line(counter_to_new_line,file_ptr,&counter,all_ICF);			
				a = curr -> code & 0xff; /* get evry 8 bits to a byte */
				b = (curr -> code & 0xff00)>>8;			
				fprintf(file_ptr,"%02x ",a);
				counter_to_new_line++;
				add_line(counter_to_new_line,file_ptr,&counter,all_ICF);
				fprintf(file_ptr,"%02x ",b);
				counter_to_new_line++;
		
				curr = curr-> next;
				continue;
			}
			if(curr -> num_of_bytes == 1)
			{
				add_line(counter_to_new_line,file_ptr,&counter,all_ICF);
				a = curr -> code & 0xff;/* get 8 bits to a byte */			
				fprintf(file_ptr,"%02x ",a);
				counter_to_new_line++;
				curr = curr-> next;
				continue;
			}	
		}
		counter += 4;
		curr = curr-> next;	
	}
	free(file_name);
	fclose(file_ptr);
} 
