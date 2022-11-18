/* the main file, it read assembly files and tranlate them to machine language, if it found error it nforms the user about that */
#include "data.h"
#include "error_print.h" /* to print the error type */
static void free_memory(label**, line**, exernal_instruction **);
static void free_all_memory (label ***, line ***, exernal_instruction ***, int);

/*read files from command line with extension .as and process them*/

int main(int argc, char* argv[])
{
	/* intelized */    
    FILE* input_file;
    int* IC = (int*)malloc(sizeof(int)), file_count, ICF;
    int* DC = (int*)malloc(sizeof(int)), line_count, DCF;
    long int * all_ICF = (long int*)malloc(sizeof(long int) * (argc - 1)); /* ICF and DCF table for all files */
    long int * all_DCF = (long int*)malloc(sizeof(long int) * (argc - 1));
    long int* IC_counter = (long int*)malloc(sizeof(long int));
    int cnt = 0, x;
    bool is_valid = true;
    enum error_type error = valid;
    char buff[LINE_LENGTH];
    char* file_name;
    label *** label_table = (label ***)malloc(sizeof(label **) * (argc - 1)); /* label list , external instruction and line list tabels for all files*/
    line *** line_table = (line ***)malloc(sizeof(line **) * (argc - 1));
    exernal_instruction *** ext_table = (exernal_instruction ***)malloc(sizeof(exernal_instruction **) * (argc - 1));
    label** l_head; 
    line** line_head; 
    line** d_head = (line**)malloc(sizeof(line*));
    exernal_instruction** ext_head;
    enum situations curren_line = all_good;
    initOP(list);
    *IC_counter = IC_MINUS_4;
    
	
   
    if (argc == 1) /* if there is no input file - stop the program */
    {
        printf("no input files\n");
        return 0;
    }
    for (cnt = 0; cnt <(argc - 1); cnt ++) /* intelized memmory for every head list (for 3 lists) */
    {
	label_table[cnt] = (label**)malloc(sizeof(label*));
	line_table[cnt] = (line**)malloc(sizeof(line*));
	ext_table[cnt] = (exernal_instruction**)malloc(sizeof(exernal_instruction*));	
	if( ext_table[cnt] == NULL || line_table[cnt] == NULL || label_table[cnt] == NULL)
	{
		printf("something wrong. no have enough memory\n");
		exit(1);
	}
    }




    for (file_count = 1; file_count < argc; file_count++) /*start run on the files in the first time */
    {
        file_name = *(argv + file_count);
        if (!(file_name[strlen(file_name) - 1] == 's' && file_name[strlen(file_name) - 2] == 'a' && file_name[strlen(file_name) - 3] == '.'))
        {
            printf("file extension not .as:%s\n", *(argv + file_count));
	    is_valid = false;
            continue;
        }


        if ((input_file = fopen(*(argv + file_count), "r")) == NULL)/*fila as shown?*/
        {
            printf("cant open %s\n", *(argv + file_count));
	    is_valid = false;
            break;
        }


        if ((IC == NULL) || (DC == NULL) || (d_head == NULL)|| (all_ICF == NULL) || (all_DCF == NULL))
        {
            printf("Not enough memory!\n");
	    is_valid = false;
            break;
        }
	
    	line_head = line_table[file_count - 1];
	l_head = label_table[file_count - 1];
	*line_head = NULL;
        *d_head = NULL;
        *l_head = NULL;
        *IC = 100;
        *DC = 0;
        line_count = 1;
        while ((fgets(buff, LINE_LENGTH, input_file) != NULL)) /*start to run on the current file */
        {
            if ((strchr(buff, '\n') != NULL) || feof(input_file))
                error = evaluate(buff, l_head, line_head, d_head, IC, DC);
            else
                error = long_line;/*line too long*/


            if ((error != valid) && (error != empty) && (error != comment) && (error != next_line))
            {
                is_valid = false; /*error found, print what and where the error */
                printf("In file: %s Line #%d: ", *(argv + file_count), line_count);
                PRINT_ERROR
            }

            line_count++;

        }/*end of cuur file*/
	if (is_valid)
        {
        	 ICF = *IC; /*save ICF DCF and merg the DC to be after IC */
       		 DCF = *DC + ICF;
		 all_ICF[file_count - 1] = ICF;
		 all_DCF[file_count - 1] = DCF;
       		 update_label_list(l_head, ICF);
        	 update_data_list(d_head, ICF);
        	 merge_list_of_lines(line_head, d_head);
   	}
	fclose(input_file); /*close the file and go to the next file */
    }
	

    if (is_valid == false) /* if eror founded stop here and free all the memory */
    {
	free(IC);
        free(all_ICF);
        free(all_DCF);
        free_all_memory (label_table, line_table,ext_table,argc);
        free(DC);        
	exit(1);
    }

    /* start the second pass on the files */
    for (file_count = 1; file_count < argc; file_count++) /* all the files opened well at the first pass so there is no erors */
    {
        input_file = fopen(*(argv + file_count), "r");
	ext_head = ext_table[file_count - 1];
        *IC_counter = IC_MINUS_4;
        line_count = 0;
	*ext_head = NULL;

        while ((fgets(buff, LINE_LENGTH, input_file) != NULL)) /* run on the current file */
        {
		        
	    curren_line = find_situations(buff, label_table[file_count - 1], IC_counter, line_table[file_count - 1], ext_head);
            line_count++;
		
            if (curren_line == all_good)
            {
		 continue;
            }
            else /*error found, print what and where the error */
            {
                is_valid = false;
                printf("In file: %s Line #%d: ", *(argv + file_count), line_count);
                PRINT_ERROR_2
            }
        }
        fclose(input_file); /* close the file and go to the next file */
    } /* the secound pass is done, check if was errors, if not so  make outputs files */

	
   if (is_valid == false)/* if eror founded stop here and free all the memory */
    {
	free(IC);
        free(all_ICF);
        free(all_DCF);
        free_all_memory (label_table, line_table,ext_table,argc);
        free(DC);        
	exit(1);
    }
	
       /* all good so make the outputs files */
    for(x = 0; x < (argc - 1); x++)
    {	
            make_ent(label_table[x], x + 1, argv);
            make_ext(ext_table[x], x + 1, argv);
	    make_ob(argv,all_ICF[x],all_DCF[x],x + 1,line_table[x]);
        
    }
    
     /*free all the allocate memory */
    free(IC);
    free(all_ICF);
    free(all_DCF);
    free_all_memory (label_table, line_table,ext_table,argc);
    free(DC);
	
    return 0;

}/* end of the main function */


/* free all tabels of data */

static void free_all_memory (label ** lable_table[], line **line_table[], exernal_instruction ** ext_table[], int argc)
{
	int x = 0;		
	for(x = 0; x < (argc - 1); x++)
	{
		free_memory (lable_table[x], line_table[x], ext_table[x]);
	}	
}

 /* free one list of label, one list of line and one list of exernal_instruction */
static void free_memory(label** l_head, line** line_head,exernal_instruction ** ext_head)

{
    label* l_temp = *l_head, * l_previous;
    line* line_temp = *line_head, * line_previous;
    exernal_instruction * ext_temp = *ext_head, * ext_previous;

    while (l_temp != NULL)
    {
        l_previous = l_temp;
        l_temp = l_temp->next;
        free(l_previous);
    }

    while (line_temp != NULL)
    {
        line_previous = line_temp;
        line_temp = line_temp->next;
        free(line_previous);
    }
    while (ext_temp != NULL)
    {
        ext_previous = ext_temp;
        ext_temp = ext_temp->next;
        free(ext_previous);
    }

}  /*end of free_memory*/
