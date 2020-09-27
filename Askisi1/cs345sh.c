// KARATHANASIS DIMITRIS 3547 <csd3547@csd.uoc.gr>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int checkPipe(char ** args, int counter){
	int i;
	char * p;

	for(i=0; i < counter; i++){
		p = strchr(args[i], '|');
		if (p) return 1;
	}
	return 0;
}


int checkDaemon(char **args, int counter){
	int i;
	char * p;

	for(i=0; i < counter; i++){
		p = strchr(args[i], '&');
		if (p) return 1;
	}
	return 0;
}


void func_cd(char ** args){
	int len;
	char * temp;

	len = strlen(args[1]);
	temp = args[1];
	if (len > 0 && temp[len-1] == '\n') temp[len-1] = '\0';
	
	if(args[1] == NULL){
		printf("Error on cd, no args found!\n");
		return;
	}
	else{
		chdir(temp);
	}
}


void func_exit(){
	exit(EXIT_SUCCESS);
}


char ** tokenize_commands(char * command, char ** args){

	int max_number_of_commands = 50;
	int i;
	char * token;

	args = malloc(sizeof(char*)*max_number_of_commands);
	for(i=0; i<max_number_of_commands; i++){
		args[i] = (char*)malloc(sizeof(char)* 100);
	}

	token = strtok(command," ");
	i = 0;
	while(token != NULL){
		args[i] = token;

		// printf("args[%d] = %s\n", i, args[i]);
		i++;

		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return args;
}


char * read_command(){
	char * lineptr = NULL;
	size_t n = 0;

	getline(&lineptr, &n, stdin);
	return lineptr;
}


void fork_process(char ** args){

	pid_t pid, wpid;
	int status;
	int counter = 0;
	char * temp, * temp2;
	int len, len2;
	int hasPipe = 0;
	int isDaemon = 0;


	while(args[counter]!=NULL){
		counter++;
	}
	
	hasPipe = checkPipe(args, counter);
	if(hasPipe == 1){
		printf("brika pipe\n");
	}
	isDaemon = checkDaemon(args, counter);
	if(isDaemon == 1){
		printf("einai daemon\n");
	}
	


	len = strlen(args[0]);
	temp = args[0];
	if (len > 0 && temp[len-1] == '\n') temp[len-1] = '\0';

	if(counter > 1){
		len2 = strlen(args[1]);
		temp2 = args[1];
		if (len2 > 0 && temp2[len2-1] == '\n') temp2[len2-1] = '\0';
	}

	pid = fork();
	if(pid != 0){

		if(isDaemon){
			printf("New background job: %d\n",pid);
		}
		do{
			wpid = waitpid(pid, &status, WUNTRACED);  
		}
		while(!WIFEXITED (status) && !WIFSIGNALED(status));
	}
	else{
		if(isDaemon){
			printf("\nDaemon process...\n");
			fclose(stdin);
			fopen ("/dev/null", "r");
			execvp(*args, args);
		}
		else if (execvp(temp, args) == -1){  
			perror("exec error");
		}
	}
	return;
}


void exec(char ** args){

	int len;
	char * temp;

	temp = args[0];
	len = strlen(args[0]);
	
	if (len > 0 && temp[len-1] == '\n') temp[len-1] = '\0';

	if(strcmp(args[0],"cd") == 0){
		func_cd(args);
	}
	else if(strcmp(temp,"exit") == 0){
		func_exit();
	}
	else {
		fork_process(args);
	}
	return;
}


int main(){

	char * command;
	char ** args;
	char * buf;
	char * cwd;
	char * login;
	size_t size;

	buf = (char*)malloc(size);
	size = 100;
	login = getlogin();

	while(1){
		cwd = getcwd(buf, size);
		printf("%s@cs345sh%s $",login, cwd );

		command = read_command();
		args = tokenize_commands(command, args);

		exec(args);
	}
	return 0;
}