#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 256 //max number of characters per 256
#define MAXTEXT 81 // max number lines
#define MAXCOMM 100 // max number of commands in file

typedef enum {none, text, line} linespecification;
typedef char str[MAXCHAR];

typedef union {   // given text or a range of lines (backslash or number)
        int none;
        str contained_text;
        int linenumbers[2];
} lineSpecRange;

typedef struct {
        char op;
        linespecification SpecType;
        lineSpecRange Range;
        str text;

} editStruct;

typedef editStruct editArray[MAXCOMM];

// int isdigit(char ch){
// 	char arr[10] = {'1','2','3','4','5','6','7','8','9','0'};
// 	for(int i = 0; i < 10; i++){
// 		if(ch == arr[i]){
// 			return 1;
// 		}
// 	}
// 	return 0;
// }

editStruct TexttoEdit (str command){

	if(command[strlen(command)-1] == '\n'){
		command[strlen(command)-1] = "\0";
	}

	editStruct editStruct;
	char *term;

	if(command[0] == '/') {
		editStruct.SpecType = text;
		command = command + 1;
		term= strchr(command,'/');
		*term = '\0';
		strcpy(editStruct.Range.contained_text,command);
		command = term +1;
		editStruct.op = command[0];
		strncpy(editStruct.text, command + 1, strlen(command));

	}
	else if(isdigit(command[0])){
		editStruct.SpecType= line;
		editStruct.Range.linenumbers[0]= atoi(command);
		command = command +2;
		editStruct.Range.linenumbers[1]= atoi(command);
		term = strchr(command,'/');
		command = term +1;
		editStruct.op = command[0];
		strncpy(editStruct.text, command +1, strlen(command));
	}
	else{
		editStruct.SpecType = none;
		editStruct.Range.none = 1;
		editStruct.op = command[0];
		strncpy(editStruct.text, command +2, strlen(command));
	}

	return(editStruct);
}


int readInput(FILE* file, editArray *edits) {
	int i = 0;
	char inputLine;

	while(fgets(&inputLine, MAXCHAR, file) && (i < MAXCOMM)){
		*edits[i] = TexttoEdit(&inputLine);
		i++;
	}
	return i;
}


str* DoEdit(str currentline, editStruct editCmd){
	int perform_op = 0;
	str tempString;

	if(editCmd.SpecType == none){
		perform_op = 1;
	}
	else if(editCmd.SpecType == text){
		if(strstr(currentline, editCmd.Range.contained_text)){
			perform_op = 1;
		}
	}
	else if(editCmd.SpecType == line){
		char* space = currentline;
		str line_num;
		for(int i = 0; (i < MAXTEXT) && (currentline[i] != ' '); i++){
			line_num[i] = currentline[i];
			line_num[i + 1] = '\0';
			space++;
		}
		if(atoi(line_num) >= editCmd.Range.linenumbers[0] && 
			atoi(line_num) <= editCmd.Range.linenumbers[1]){
			perform_op = 1;
		}
	}

	if(perform_op){
		switch(editCmd.op){
			case 'A':
			{
				strcat(currentline, editCmd.text);
				break;
			}
			case 'I':
			{
				strcpy(tempString, editCmd.text);
				strcat(tempString, currentline);
				strcpy(currentline, tempString);
				break;
			}
			case 'O':
			{
				printf("%s\n",editCmd.text);
				break;
			}
			case 's':
			{
				str delim, copy_edit_dat;
				char *search, *replace, *pos;

				delim[0] = editCmd.text[0];
				delim[1] = '\0';
				strcpy(copy_edit_dat, &editCmd.text[1]);
				search = strtok(copy_edit_dat,delim);
				replace = strtok(NULL,delim);
				
				while ((pos = strstr(currentline, search)) != NULL) {
					strncpy(tempString, currentline, pos - currentline);
					tempString[pos - currentline] = '\0';
					strcat(tempString, replace);
					strcat(tempString, &currentline[(pos - currentline) + 
	strlen(search)]);
					strcpy(currentline, tempString);
				}

				// str* cmd = editCmd.text;
				
				// char *second_slash = strchr(cmd, '/');
				// str oldtext;

				// strcpy(oldtext, cmd);
				// cmd = second_slash + 1;
				// char* third_slash =strchr(cmd,'/');
				// *third_slash= '\0';
				// str newtext;
				// strcpy(newtext, cmd);
				// str* subStr = strstr(currentline, oldtext);

				// if(subStr != NULL) {
				// 	strncpy(tempString, currentline, subStr - currentline);
				// 	tempString[subStr - currentline] = '\0';
				// 	strcat(tempString, newtext);
				// 	strcat(tempString, &currentline[(subStr - currentline) + strlen(oldtext)];
				// 	strcpy(currentline,tempString);
				// }
				break;
			}
			case 'd':
				{
					strcpy(tempString, "");
					break;
				}
			default:
				{
					fprintf(stderr, "[ERROR] Operation type could not be determined\n");
					exit(EXIT_FAILURE);
					break;
				}
		}
	}
	else{
		strncpy(tempString, currentline, MAXTEXT);
	}
	return &tempString;
}
                
int main (int argc, char** argv) {
	FILE *commandlist; //Where the commands are stored
	str currentline;
	editArray edits;

	if(argc != 2) {
		fprintf(stderr, "[ERROR] Invalid input\n");
		return EXIT_FAILURE;
	}

	commandlist = fopen(argv[1], 'r');

	if(commandlist == NULL) {
		fprintf(stderr, "[ERROR] File doesn't exist\n");
		return EXIT_FAILURE;
	}

	int size = readInput(commandlist, &edits);
	fclose(commandlist);

	str output[1024];
	int output_size = 0;
	while(fgets(currentline, MAXCHAR, stdin)){
		strcpy(output[output_size++], currentline);
		for(int x = 0; x < size; x++) {
			strcpy(output[output_size], 
				DoEdit(output[output_size], edits[x]));
		}
	}
	
	return EXIT_SUCCESS;
}
