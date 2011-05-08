#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CODESIZE 1024

int debug = 0;

void msg (char *msg){
	if (debug == 1) puts(msg);
}

void computing (char *codeptr, int *cellpos, int *cellsize, int *cellptr) {
	
	while(*codeptr) {
		if (debug == 1){
			printf("Cell:    %i\n", *cellpos);
			printf("Value:   %i %c\n", *(cellptr + *cellpos), *(cellptr + *cellpos));
			printf("Operand: %c\n", *codeptr);
			printf("Action: ");
		}
		switch (*codeptr) {
			case '>':
				msg("increment pointer");
				if (*cellpos == *cellsize){
					cellptr = realloc(cellptr, (*cellsize + 1)*sizeof(int));
					(*cellsize)++;
					*(cellptr + *cellsize) = 0;
				}
				(*cellpos)++;
				break;
			case '<':
				msg("decrement pointer");
				if (*cellpos - 1 >= 0){
					(*cellpos)--;
				}
				break;
			case '+':
				msg("increment value");
				(*(cellptr + *cellpos))++;
				break;
			case '-':
				msg("decrement value");
				(*(cellptr + *cellpos))--;
				break;
			case '.':
				msg("print value to stdout");
				putchar(*(cellptr + *cellpos));
				break;
			case ',':
				msg("read value from stdin");
				*(cellptr + *cellpos) = getchar();
				break;
			case '[':
				msg("begin loop");
				
				int nestinglvl = 0;
				codeptr++;
				char *startptr = codeptr;
				while (1){
					if (*codeptr == '[') {
						nestinglvl++;
					}
					else if (*codeptr == ']') {
						if (nestinglvl == 0) {
							break;
						}
						nestinglvl--;
					}
					codeptr++;
					
					if (debug) printf("\tnestinglvl: %i\n", nestinglvl);
				}
				*codeptr = 0;
				msg("Replaced ']' with '\\0'");
				
				do {
					computing(startptr, cellpos, cellsize, cellptr);
				} while (*(cellptr + *cellpos) != 0);
				
				*codeptr = ']';
				break;
			default:
				msg("ignoring as comment");
				break;
		}
		
		codeptr++;
	}
}

int main (int argc, const char * argv[]) {
	if(argc != 2){
		if ((argc == 3) && (strcmp(argv[1], "-d") == 0)) {
			debug = 1;
		}
		else{
			printf("Usage: banana [-d] [brainfuck code file]\nOnly the brainfuck operators (< > , . [ ] - +) will be interpreted.\n\"-d\" enables debugging information.\n");
			return 1;
		}
	}
	
	int i;
	
	msg("starting...");
	
	char code[CODESIZE+1];
	char* codeptr = code;
	msg("array creation succeded.");
	
	
	FILE *inputfile = fopen(argv[(debug ? 2 : 1)], "rb");
	msg("file creation succeded");
	
	while (fread(codeptr, 1, 1, inputfile)){codeptr++;}
	*codeptr = 0;
	codeptr = code;
	msg("file reading succeded");
	
	fclose(inputfile);
	msg("file closing succeded");
	
	if (debug == 1){
		puts("code mem dump:");
		for (i=0; code[i] != 0; i++){
			if (i%5 == 0) puts("");
			printf(" |%03i %c|", i, (int)code[i]);
		}
		puts("");puts("");
		puts("Parsing code:");
		puts("");
	}
	
	int cellpos = 0;
	int cellsize = 1;
	int* cellptr = malloc(sizeof(int));
	*cellptr = 0;

	computing(codeptr, &cellpos, &cellsize, cellptr);

	if (debug == 1){
		puts("");
		puts("cell mem dump:");
		for (i=0; i<=cellsize; i++){
			if (i%5 == 0) puts("");
			printf(" [%03i %03i %c]", i, *(cellptr + i), *(cellptr + i));
		}
	}
	
	free(cellptr);
	msg("");msg("");
	msg("freed mem.");
	
	msg("done.");
	
    return 0;
}
