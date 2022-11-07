#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define STR_MAX 256
#define COMM_MAX 100

typedef enum {A, I, O, d, s} COMM_OPERATIONS;

typedef union {
	int all;
	char str[STR_MAX];
	int range[2];
} lrange;

typedef struct {
	unsigned int lrs: 2;	//0 text, 1 ints, 2 all
	lrange lr;
	COMM_OPERATIONS op: 3;
	char data[STR_MAX];
} edit_command;

int determine_op(char op_char){
	switch(op_char){
		case 'A':
			return A;
			break;
		case 'I':
			return I;
			break;
		case 'O':
			return O;
			break;
		case 'd':
			return d;
			break;
		case 's':
			return s;
			break;
		default:
			fprintf(stderr, "[ERROR] operation type could not be determined\n");
			exit(EXIT_FAILURE);
			return 0;
	}
}

int main(int argc, char** argv){
	int num_commands = 0;
	int num_input_lines = 0;
	int num_output_lines = 0;
	char svi_input[COMM_MAX][STR_MAX];
	char svi_output[COMM_MAX][STR_MAX];
	char buffer[STR_MAX];
	FILE* file;

	// if(argc != 2 && argc != 3){
	// 	fprintf(stderr, "[ERROR] Too %s inputs\n", 
	// 		(argc < 2 ? "few" : "many"));
	// 	exit(EXIT_FAILURE);
	// }
	// file = fopen(argv[1], "r");

	edit_command commands[COMM_MAX];

	fgets(buffer, STR_MAX, stdin);
	while(strncmp("\n", buffer, STR_MAX)){
		char range[STR_MAX];
		char* end;

		edit_command ecom;

		int single_char = 1;
		int digit_char =  0;
		int slash_char =  0;

		switch(buffer[0]){
			case 'A':
			case 'I':
			case 'O':
			case 's':
			case 'd':
				break;
			default:
				single_char = 0;
				break;
		};
		for(int i = 48; i < 58; i++){
			if(buffer[0] == i){
				digit_char = 1;
			}
		}
		if(buffer[0] == '/'){
			slash_char = 1;
		}

		if(!digit_char && !slash_char && !single_char){
			fprintf(stderr, "[ERROR] invalid input\n");
			exit(EXIT_FAILURE);
		}

		if(single_char){
			ecom.lrs = 2;
			ecom.lr.all = 1;
			ecom.op = determine_op(buffer[0]);
		}
		else if(slash_char){
			char* end = strstr(buffer + 1, "/");
			char* point = buffer + 1;

			for(int i = 0; point != end; point++, i++){
				range[i] = *point;
			}
			printf("Our text range is: %s\n", range);
			//segfaults around here
			strncpy(ecom.lr.str, range, STR_MAX);
			ecom.lrs = 0;
			ecom.op = determine_op(*(end + 1));
		}
		else if(digit_char){
			char* end = strstr(buffer, "/");
			char* point = buffer;

			for(int i = 0; point != end; point++, i++){
				range[i] = *point;
			}
			printf("Our int range is: %s\n", range);
			
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
			ecom.lr.range[0] = atoi(first_int);
			ecom.lr.range[1] = atoi(second_int);
			ecom.op = determine_op(*(end + 1));
		}
		else{
			fprintf(stderr, "[ERROR] Invalid input encountered\n");
			exit(EXIT_FAILURE);
		}

		if(ecom.op == A || ecom.op == I || ecom.op == O){
			if(ecom.lrs != 2)
				end++;
			else
				end = buffer + 1;
			char text[STR_MAX];
			for(int i = 0; *end != '\n'; end++, i++){
				text[i] = *end;
			}
			printf("Our text given is %s\n", text);
		}
		else if(ecom.op == s){
			end += 2;	//points after the s/ 
			char* thrd_slash = strstr(end, "/");
			char old_text[STR_MAX];
			char new_text[STR_MAX];

			for(int i = 0; end != thrd_slash; end++, i++){
				old_text[i] = *end;
			}

			end++;	//after s/old_text/
			char* frth_slash = strstr(end, "/");

			for(int i = 0; end != frth_slash; end++, i++){
				new_text[i] = *end;
			}

			printf("Our old text is %s\n", old_text);
			printf("Our new text is %s\n", new_text);
		}
		else if(ecom.op == d){
			//todo
		}
		else{
			fprintf(stderr, "[ERROR] Encountered invalid operation\n");
			exit(EXIT_FAILURE);
		}
		commands[num_commands++] = ecom;

		for(int i = 0; i < STR_MAX; i++){
			buffer[i] = ' ';
		}
		fgets(buffer, STR_MAX, stdin);
	}
	//get the text and perform the commands
	if(argc == 3){
		FILE* input_file = fopen(argv[2], "r");
		char* buffer;
		while(fgets(buffer, STR_MAX, input_file) != NULL){
			strncpy(svi_input[num_input_lines], buffer, STR_MAX);
			num_input_lines++;
		}
	}
	else{
		//user input
	}

	for(int i = 0; i < num_input_lines; i++){
		for(int j = 0; j < num_commands; j++){
			if(commands[j].lrs == 2){
				//performs command on all text
			}
			else if(commands[j].lrs == 1){
				if(i + 1 >= commands[j].lr.range[0] && i + 1 <= commands[j].lr.range[1]){
					//if its in the range
				}
			}
			else if(commands[j].lrs == 0){
				if(strstr(svi_input[i], commands[j].lr.str) != NULL){
					//if it exists in the input line
				}
			}
			else{
				fprintf(stderr, "[ERROR] Ran into an invalid lrs\n");
			}
		}
	}

	for(int i = 0; i < num_output_lines; i++){
		printf("%s\n", svi_output[i]);
	}

	exit(EXIT_SUCCESS);
}
