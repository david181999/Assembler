#ifndef OUTPUTS
#define OUTPUTS


#define DOT_AS_LEN 3/* the length of the end name of the file, ".as" */

/* get evry 8 bits to a byte */
#define GET_BITES {\
	a = curr -> code & 0xff;\
	b = (curr -> code & 0xff00)>>8;\
	c = (curr -> code & 0xff0000)>>16;\
	d = (curr -> code & 0xff000000)>>24;\
}

/* to change the end of the file name from .as to .ent\.ext\.ob */
#define ENT_FILE_NAME {\
	file_name[strlen(argv[file_number]) - 2] = 'e';\
	file_name[strlen(argv[file_number]) - 1] = 'n';\
	file_name[strlen(argv[file_number])] = 't';\
	file_name[strlen(argv[file_number]) + 1] = '\0';\
}

#define EXT_FILE_NAME {\
	file_name[strlen(argv[file_number]) - 2] = 'e';\
	file_name[strlen(argv[file_number]) - 1] = 'x';\
	file_name[strlen(argv[file_number])] = 't';\
	file_name[strlen(argv[file_number]) + 1] = '\0';\
}

#define OB_FILE_NAME {\
	file_name[strlen(argv[file_number]) - 2] = 'o';\
	file_name[strlen(argv[file_number]) - 1] = 'b';\
	file_name[strlen(argv[file_number])] = '\0';\
}

#endif
