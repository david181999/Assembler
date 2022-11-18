assembler: assembler.c checker.c evaluate_line.c instruction.c label.c line.c evaluate_line_pass2.c outputs.c
	gcc -Wall -ansi -pedantic assembler.c checker.c evaluate_line.c instruction.c label.c line.c evaluate_line_pass2.c outputs.c -o assembler
