#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lib.h"

#define MAX_COMMAND_LEN 10
#define MAX_ARG_LEN 60

int split_command(char *sandboxarg[], char *commarg[], int argc, char *argv[])
{
	if (argc <= 1)
	{
		fprintf(stderr, "no command given\n");
		return -1;
	}
	
	if (argc > MAX_COMMAND_LEN*2)
	{
		fprintf(stderr, "command too long\n");
		return -1;
	}

	for (int i=0; i<argc; i++)
	{
		if (strlen(argv[i]) > MAX_ARG_LEN)
		{
			fprintf(stderr, "arguments too long\n");
			return -1;
		}
	}

	for (int i=0; i<argc; i++)
	{
		if (strcmp(argv[i], "--")==0)
		{
			for (int x=0;x<i;x++)
				sandboxarg[x] = argv[x];
			for (int x=i+2;x<argc;x++)
				commarg[x-i-2] = argv[x];
			return 0;
		}
	}

	for (int i=0; i<argc; i++)
		sandboxarg[i] = argv[i];
	return 0;
}

int arraylen_pointer(char **arr)
{
	int len = 0;
	while(arr[len]!=NULL)
	{
		len++;
	}
	return len;
}

int parse_sandboxarg(char **sandboxarg, char *d_path, char *p_path)
{
	int opt;
	while ((opt = getopt(arraylen_pointer(sandboxarg), sandboxarg, "p:d:")) != -1) {
        switch (opt) {
        case 'p':
			realpath(optarg, p_path);
			//strcpy(p_path, optarg);
            break;
        case 'd':
			realpath(optarg, d_path);
			//strcpy(d_path, optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n", sandboxarg[0]);
			fprintf(stderr, "\t-p: set the path to sandbox.so, default = ./sandbox.so\n");
			fprintf(stderr, "\t-d: restrict directory, default = .\n");
			fprintf(stderr, "\t--: seperate the arguments for sandbox and for the executed command\n");
			return -1;
        }
    }
	return 0;
}


/* 
get the command name from argv
return 0, if successful
return -1, if fail
*/
int commname(int argc, char *argv[], char *comm)
{
	int opt;
	int p_idx = -1;
	int d_idx = -1;
	optind = 0;

	while ((opt = getopt(argc, argv, "p:d:")) != -1) {
        switch (opt) {
        case 'p':
			p_idx = optind-1;
            break;
        case 'd':
			d_idx = optind-1;
            break;
        default:
            fprintf(stderr, "lib.c: In function 'commname'");
			return -1;
        }
    }

	for (int i=1; i<argc; i++)
	{
		if (i==p_idx || i==d_idx || strncmp("-", argv[i], 1)==0)
			continue;
		else
		{
			strcpy(comm, argv[i]);
			return 0;
		}
	}
	
	fprintf(stderr, "no command given\n");
	return -1;
}

