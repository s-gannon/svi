#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define STR_MAX 256
#define COMM_MAX 100

typedef enum {A, I, O, d, s} COMM_OPERATIONS;

typedef union {
	char* str;
	int start:7;
	int end:7;	
} lrange;

typedef struct {
	unsigned int lrs: 2;	//0 text, 1 ints, 2 all
	lrange lr;
	COMM_OPERATIONS op: 3;
	char data[STR_MAX];
} edit_command;

int main(){
	int num_commands = 0;
	char svi_output[COMM_MAX][STR_MAX];
	char buffer[STR_MAX];

	edit_command commands[COMM_MAX];

	fgets(buffer, STR_MAX, stdin);
	while(strncmp("", buffer, STR_MAX)){
		char* first_slash = strstr(buffer, "/");
		char* buf = buffer + 1;
		char* sec_slash = strstr(buf, "/");
		char range[STR_MAX];

		edit_command ecom;

		if(first_slash == NULL || sec_slash == NULL){
			fprintf(stderr, "[ERROR] first_slash or sec_slash were null\n");
			exit(EXIT_FAILURE);
		}

		if(first_slash != NULL && sec_slash != NULL && first_slash != sec_slash){
			first_slash++;
			for(int i = 0; first_slash != sec_slash; first_slash++, i++){
				range[i] = *first_slash;
			}
			printf("Our range is: %s\n", range);
			ecom.lr.str = range;
			ecom.lrs = 0;
		}
		else if(first_slash == sec_slash && sec_slash != NULL){
			buf = buffer;
			for(int i = 0; buf != sec_slash; buf++, i++){
				range[i] = *buf;
			}
			printf("Our range is: %s\n", range);
			
			int switch_ints = 0;
			int first_int_size = 0;
			char first_int[10];
			int second_int_size = 0;
			char second_int[10];
			for(int i = 0; i < strlen(range); i++){
				if(range[i] != ',' && !switch_ints){
					first_int[first_int_size] = range[i];
					first_int_size++;
				}
				else if(range[i] == ','){
					switch_ints = 1;
				}
				else if(range[i] != ',' && switch_ints){
					second_int[second_int_size] = range[i];
					second_int_size++;
				}
			}
			printf("%s %s\n", first_int, second_int);

			ecom.lrs = 1;
			ecom.lr.start = atoi(first_int);
			ecom.lr.end = atoi(second_int);
		}
		else{
			ecom.lrs = 2;
		}

		if(ecom.lrs == 0 || ecom.lrs == 1){
			sec_slash++;
			switch(*sec_slash){
				case 'A':
					ecom.op = A;
					break;
				case 'I':
					ecom.op = I;
					break;
				case 'O':
					ecom.op = O;
					break;
				case 'd':
					ecom.op = d;
					break;
				case 's':
					ecom.op = s;
					break;
			}
			if(ecom.op == A || ecom.op == I || ecom.op == O){
				sec_slash++;
				char text[STR_MAX];
				for(int i = 0; *sec_slash != '\n'; sec_slash++, i++){
					text[i] = *sec_slash;
				}
				printf("Our text given is %s\n", text);
			}
		}

		fgets(buffer, STR_MAX, stdin);
		free(first_slash);
		free(buf);
		free(sec_slash);
	}

	exit(EXIT_SUCCESS);
}
