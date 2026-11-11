#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TINYSHELL_TOK_BUFSIZE 64
#define TINYSHELL_TOK_DELIM " \t\r\n\a"

char **tinyshell_split_line(char *line){
	size_t bufsize = TINYSHELL_TOK_BUFSIZE;
	size_t position = 0;
	char **tokens = malloc(bufsize * sizeof (char *));
	if (tokens==NULL){
		perror("tinyshell: allocation error");
		exit(EXIT_FAILURE);
	}
	char *token = strtok(line, TINYSHELL_TOK_DELIM);
	while (token!=NULL){
		tokens[position]=token;
		position++;
		if (position >= bufsize){
			char **tokens_backup = tokens;
			bufsize *= 2;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (tokens == NULL){
				free(tokens_backup);
				perror("tinyshell: reallocation error");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, TINYSHELL_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}
