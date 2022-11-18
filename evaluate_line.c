/* this file is evaluate line in the first passing on the input file. it's  encode what it's can and return if there is error in the input file */
#include "data.h"

enum error_type evaluate(char *, label **,line **,line **,int *,int *);


/*divde each buff line to tokens in order to find if it is valid line.
its return the error (if it was)  or is valid*/
enum error_type evaluate(char *buff, label **l_head, line **line_head, line **d_head,int *IC,int *DC)
{
    char *first,*token,*temp,label_name[LINE_LENGTH], rs, rt, rd;
    int imm;
    bool insert_l = false,update_label = false,read_imm = false;
    label *curr_l;
    intruction *curr_instruction;
    enum error_type error;

    first = token = strtok(buff, " \n\t\r");
    if(first == NULL)    return empty;   /*empty line*/
    if(first[0] == ';')  return comment; /*comment line*/

     if ((temp = strchr(first, ':')) != NULL) /*label declaration found*/
     {
         /*if label before extern or entry then ignore it*/
         if ((strstr(buff, ".entry") == NULL) && (strstr(buff, ".extern") == NULL))
         {

             if (temp[1] == '\0') /*valid label*/
             {
                 strcpy(label_name, first); /*truncate ':' */
                 label_name[strlen(label_name) - 1] = '\0';

                 curr_l = search_label(l_head, label_name);
                 if (curr_l != NULL)
                 {
                     if ((curr_l->external) == true)    return external_defined_in_other_file;
                     if ((curr_l->address) == TO_BE_FOUND)  update_label = true;
                     else return already_declared;/*multiple label declaration*/
                 }
                 else insert_l = true;
                 token = strtok(NULL, " \n\t\r"); /*next word after label*/
             }
             else return invalid_label; /*stop this patterns { END : stop  END:stop }*/
         }
         else token = strtok(NULL, " \n\t\r"); /*next word after label*/
     }/*end of label declaration*/

     if (token[0] == '.')
     {
         if (is_directive(token + 1))
         {
             if (insert_l == true)
             {
                 error = insert_label(l_head, label_name, *DC, false, false, false);
                 if (error != valid) return error;
             }

             if (update_label == true)
             {
                 curr_l -> address = *DC;
                 curr_l -> code    = false;
             }

             if (strcmp(token, ".asciz") == 0)
             {
                 first = strtok(NULL, "\n\r");
                 if (first == NULL) return missing_parameters;

                 token = strchr(first, '\"');
                 temp = strrchr(first, '\"');

                 if (token == NULL)                    return missing_quote;/*there is no " */
                 if (strcmp(temp, token) == 0)         return missing_quote;/*only one " */
                 if (strcmp(temp, token + 1) == 0)     return empty_string;

                 /*check to see if read the whole parameter*/
                 while (isspace(*(temp + 1)))
                 {
                    temp++;
                 }
                 if (*(temp + 1) != '\0') return too_many_parameters;/*after last " */

                 while (token++)
                 {
                    if (strcmp(temp, token) == 0) break;
                    if (!isprint(*token)) return  invalid_string;

                    error = insert_line(d_head, 0, 0, (*DC)++, N, N, N, *token, 1, D);
                    if (error != valid) return error;
                 }

                 return insert_line(d_head, 0, 0, (*DC)++, N, N, N, '\0', 1, D);
             }
             else /*.db .dh .dw*/
             {
                 first = strtok(NULL, " ,\t\n\r");
                 if (first != NULL)
                 {
                    do
                    {
                       imm = strtol(first, &temp, 10);

                       error = check_imm(first, temp);
                       if (error != valid) return error;

                       if (strcmp(token, ".db") == 0)
                       {
                           if((imm < -128) || (imm > 127)) return out_of_range;

                           error = insert_line(d_head, 0, 0, (*DC)++, N, N, N, imm, 1, D);
                       }
                       else if (strcmp(token, ".dh") == 0)
                       {
                           if((imm < -32768) || (imm > 32767)) return out_of_range;

                           error = insert_line(d_head, 0, 0, (*DC), N, N, N, imm, 2, D);

                           *DC = *DC + 2;
                       }
                       else /* .dw */
                       {
                           if ((imm == LONG_MIN) || (imm == LONG_MAX)) return out_of_range;

                           error = insert_line(d_head, 0, 0, (*DC), N, N, N, imm, 4, D);

                           *DC = *DC + 4;
                       }

                       if (error != valid) return error;
                    }while ((first = strtok(NULL, " ,\t\n\r")) != NULL);

                    return valid;
                 }
                 else return missing_parameters;
             }
         }/*end of directive*/
         else if (is_en(token + 1) || is_ex(token + 1))
         {
            if (is_en(token + 1)) return next_line;

            first = strtok(NULL, " \n\t\r");
            if (first == NULL) return invalid_label;

            if ((strtok(NULL, " \n\t\r") != NULL)) return too_many_parameters;

            curr_l = search_label(l_head, first);
            if (curr_l != NULL)
            {
                if ((curr_l->address) == TO_BE_FOUND)
                {
                    curr_l->address = 0;
                    curr_l->external = true;
                    return valid;
                }
                else return already_declared;
            }
            else return insert_label(l_head, first, 0, false, true, false);
         }/*end of .entry or .extern*/
         else return expected_reserved;
     }

     curr_instruction = search_instruction(token);
     if (curr_instruction != NULL)
     {
         if (insert_l == true)
         {
             error = insert_label(l_head, label_name, *IC, true, false, false);
             if (error != valid) return error;
         }

         if (update_label == true)
         {
             curr_l -> address = *IC;
             curr_l -> code    = true;
         }

         if (curr_instruction->type == N)
         {
             if (strtok(NULL, " \n\t\r") != NULL) return too_many_parameters;
             else
                 error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, N, N, N, 0, 4, curr_instruction->type);
         }
         else if ((curr_instruction->type == R) || (curr_instruction->type == I))
         {
             first = strtok(NULL, ",  \t");
             if (first == NULL) return missing_operand;

             if ((rs = check_reg(first)) == UNKNOWN) return invalid_register;
             rs--;

             /* reading immediate ; addi, subi, andi, ori, nori,lb, sb, lw, sw, lh, sh */
             if ((curr_instruction->type == I) && ((curr_instruction->opc < 15) || (curr_instruction->opc > 18)))
             {
                 first = strtok(NULL, ", \t");
                 if (first == NULL) return missing_operand;

                 imm = strtol(first, &temp, 10);

                 error = check_imm(first, temp);
                 if (error != valid) return error;

                 if((imm < -32768) || (imm > 32767)) return out_of_range;

                 read_imm = true;
             }

             first = strtok(NULL, ", \t\n\r");
             if (first == NULL) return missing_operand;

             if ((rt = rd = check_reg(first)) == UNKNOWN) return invalid_register;
             rt--;
             rd--;
             
             /* move, mvhi, mvlo */
             if (curr_instruction->opc == 1)
             {
                 rt = 0;
                 error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, rs, rt, rd, 0, 4, curr_instruction->type);
             }
             else if (curr_instruction->opc == 0) /* add, sub, and, or, nor */
             {
                 first = strtok(NULL, " \t\n\r");
                 if (first == NULL) return missing_operand;

                 if ((rd = check_reg(first)) == UNKNOWN) return invalid_register;
                 rd--;

                 error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, rs, rt, rd, 0, 4, curr_instruction->type);
             }
             else
             {
                 if (read_imm == true)
                 {
                     error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, rs, rt, rd, imm, 4, curr_instruction->type);
                 }
                 else
                 {
                     first = strtok(NULL, ", \t\n\r");
                     if (first == NULL) return missing_operand;

                     curr_l = search_label(l_head, first);
                     if (curr_l == NULL)
                     {
                         error = insert_label(l_head, first, TO_BE_FOUND, false, false, false);
                         if (error != valid) return error;
                         error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, rs, rt, rd, TO_BE_FOUND, 4, curr_instruction->type);
                     }
                     else
                     {
                         if(curr_l ->address == TO_BE_FOUND)
                          error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, rs, rt, rd, TO_BE_FOUND, 4, curr_instruction->type);
                          else
                          error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, rs, rt, rd, (curr_l ->address - *IC), 4, curr_instruction->type);
                     }
                 }
             }

         }
         else /* L or J */
         {
             first = strtok(NULL, " \n\t\r");
             curr_l = search_label(l_head, first);
             if (curr_l != NULL)
             {
                 if ((curr_l->address) == TO_BE_FOUND)
                    error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, N, N, N, TO_BE_FOUND    , 4, curr_instruction->type);
                 else
                    error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, N, N, N, curr_l->address, 4, curr_instruction->type);

             }
             else
             {
                 if ((first[0] == '$') && (curr_instruction->type == J))
                 {
                     if ((rs = check_reg(first)) == UNKNOWN) return invalid_register;
                     else
                     {
                         rs--;
                         error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, N, N, N, rs, 4, curr_instruction->type);
                     }
                 }
                 else
                 {
                     error = insert_label(l_head, first, TO_BE_FOUND, false, false, false);
                     if (error != valid) return error;
                     error = insert_line(line_head, curr_instruction->opc, curr_instruction->funct, *IC, N, N, N, TO_BE_FOUND, 4, curr_instruction->type);
                 }
             }
         }


         if (error != valid) return error;
         if (strtok(NULL, " \n\t\r") != NULL) return too_many_parameters;
         *IC = *IC + 4;
     }/*end of valid instruction*/
     else return invalid_instruction;

     return valid;
}
