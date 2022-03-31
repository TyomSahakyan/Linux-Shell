#ifndef MY_SHELL
#define MY_SHELL

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


#define BUFFER_SIZE 1024
#define TOK_BUFFER_SIZE 100
#define TOK_DELIM " "

char* read_line();
char** split_line(char*);
void check_pipes(char**, char**);
int find_pipe(char*, char**);
int execute(char**);
void inf_loop();

#endif
