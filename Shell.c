#include "Shell.h"

char* read_line() {
  int buffer_size = BUFFER_SIZE;
	int i = 0;
	char* buffer = malloc(sizeof(char) * BUFFER_SIZE);
	int c; //for reading symbols
	if(!buffer) {
		printf("THE MEMORY IS NOT ALOCCATED\n");
		exit(EXIT_FAILURE);
	}
	while(1) {
		c = getchar();
		if(c == EOF || c == '\n') {
			buffer[i] = '\0';
			return buffer;
		}
		else {
			buffer[i] = c;
		}
		++i;
		if(i >= buffer_size){
		  buffer_size += 1024;
      buffer = realloc(buffer, buffer_size);
			if(!buffer) {
				printf("THE MEMORY IS NOT ALOCCATED\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	
}

char** split_line(char* line) {
	if(line == NULL){
    printf("ERROR");
    return NULL;
  }
	int tok_size = TOK_BUFFER_SIZE, position = 0;
	char** tokens = malloc(sizeof(char*) * tok_size);
	char* token;
	if(!tokens) {
		printf("THE MEMORY IS NOT ALOCCATED\n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, TOK_DELIM);
	while(token != NULL) {
		tokens[position] = token;
		position++;
		if(position >= tok_size) {
			tok_size += 100;
			tokens = realloc(tokens, tok_size * sizeof(char *));
			if(!tokens) {
				printf("THE MEMORY IS NOT ALOCCATED\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

int execute(char ** args) {
  pid_t cpid;
  if (strcmp(args[0], "exit") == 0)
  {
  	return 0;
  }
  
  cpid = fork();

  if (cpid == 0) {
    if (execvp(args[0], args) < 0) {
      printf("dash: command not found: %s\n", args[0]);
      exit(EXIT_FAILURE);
    }  

  } 
  else if (cpid < 0) {
    printf( "Error forking");
  }
  else {
    wait(&cpid);
  }
  return 1;
}

void inf_loop() {
  char* line;
  char** args;
  int status = 1;
	do {
		printf(">");
		line = read_line();
		args = split_line(line);
		status = execute(args);
    free(line);
    free(args);
	} while(status);
}
