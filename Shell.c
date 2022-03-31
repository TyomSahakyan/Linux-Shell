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

void check_pipes(char** parsed, char** parsedpipe) {
  int pipe_fd[2];
  pid_t p1, p2;
  if(pipe(pipe_fd) < 0) {
    printf("PIPE COULD NOT BE INITIALIZED\n");
    return;
  }
  p1 = fork();
  if(p1 < 0) {
    printf("COULD NOT CREATE PROCESS\n");
    return;
  }
  if(p1 == 0) {
    //First Child executing
    close(pipe_fd[0]);
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);
    
    if(execvp(parsed[0], parsed) < 0) {
      printf("COULD NOT EXECUTE COMMAND\n");
      exit(0);
    }
  }
  else {
    //Parnet executing
    p2 = fork();
    if(p2 < 0) {
      printf("Could not fork");
      return;
    }
    //Second child executing
    if(p2 == 0) {
      close(pipe_fd[1]);
      dup2(pipe_fd[0], STDIN_FILENO);
      close(pipe_fd[0]);
      if(execvp(parsedpipe[0], parsedpipe) < 0) {
        printf("COULD NOT EXECUTE COMMAND\n");
        exit(0);
      }
    }
    else {
      //Waiting for Children
      wait(NULL);
      wait(NULL);
    }
  }
}

int find_pipe(char* str, char** strpipe) {
  for(int i = 0; i < 2; ++i) {
    strpipe[i] = strsep(&str, "|");
    if(strpipe[i] == NULL)
      break;
  }
  if(strpipe[1] == NULL)
    return 0; //pipe not found
  else
    return 1;
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
  int pipe = 0;
  int status = 1;
	do {
		printf(">");
		line = read_line();
		args = split_line(line);
    pipe = find_pipe(line, args);
    if(pipe)
    {
      check_pipes(args, args);
      continue;
    }
		status = execute(args);
    free(line);
    free(args);
	} while(status);
}
