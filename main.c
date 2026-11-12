#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TINYSHELL_TOK_BUFSIZE 64
#define TINYSHELL_TOK_DELIM " \t\r\n\a"
#define MAX_LINE 1024
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

int tinyshell_launch(char **args){
	pid_t pid = fork();
	if (pid<0){
		perror("tinyshell: fork failed💀");
		return 1;
	}
	else if (pid==0){
		if (execvp(args[0], args)==-1){
			perror("tinyshell💀");
		}
		exit(EXIT_FAILURE);
	}
	if (waitpid(pid, NULL, 0)==-1){
		perror("tinyshell: waitpid failed💀");
	}
	return 1;
}

int tinyshell_execute(char **args){
	if (args[0]==NULL) return 1;
	if (strcmp("exit",args[0])==0){
		return 0;
	}
	if (strcmp("cd", args[0])==0){
		if (args[1]==NULL){
			fprintf(stderr, "tinyshell: expected argument to \"cd\"\n");
			return 1;
		}
		if (chdir(args[1])!=0){
			perror("tinyshell");
		}
		return 1;
	}
	return tinyshell_launch(args);
}
		

int main(void){
	char line[MAX_LINE];
	int status;
	while(status){
		printf("🧑‍💻tinyshell> ");
		fflush(stdout);

		if (fgets(line, sizeof(line),stdin)==NULL){
			printf("\nExiting tinyshell💀\n");
			break;
		}
		char ** args = tinyshell_split_line(line);
		status = tinyshell_execute(args);
		free(args);		
	}
	return 0;
}

		
	
