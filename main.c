#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lib.h"

#define MAX_COMMAND_LEN 10

int main(int argc, char *argv[])
{
	char d_path[60];
	char p_path[60];
	char **sandboxarg = malloc(MAX_COMMAND_LEN*sizeof(char *)); 
	char **commarg = malloc(MAX_COMMAND_LEN*sizeof(char *));		//must remember to free the address
	char comm[100];
	char setenv[70] = "DPATH=";
	char temp[100];
	
	memset(sandboxarg, 0, MAX_COMMAND_LEN);
	memset(commarg, 0, MAX_COMMAND_LEN);
	memset(comm, '\0', 20);
	realpath(".", d_path);	
	realpath("./sandbox.so", p_path);

	if (split_command(sandboxarg, commarg, argc, argv) == -1)
		exit(EXIT_FAILURE);

	if (parse_sandboxarg(sandboxarg, d_path, p_path)==-1)
		exit(EXIT_FAILURE);
	
	strcat(setenv, d_path);
	putenv(setenv);

	if (commname(argc, argv, comm)==-1)
		exit(EXIT_FAILURE);


	strcpy(temp, comm);
	strcpy(comm, "LD_PRELOAD=");
	strcat(comm, p_path);
	strcat(comm, " ");
	strcat(comm, temp);
	for (int i=0; i<arraylen_pointer(commarg); i++)
	{
		strcat(comm, " ");
		strcat(comm, commarg[i]);
	}

	system(comm);

	free(sandboxarg);
	free(commarg);
	
	return 0;
}
