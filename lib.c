#include <stdio.h>
#include <string.h>
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
	
	if (argc >= MAX_COMMAND_LEN*2+1)
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
			for (int x=i;x<argc;x++)
				commarg[x-i] = argv[x+1];
			return 0;
		}
	}

	for (int i=0; i<argc; i++)
		sandboxarg[i] = argv[i];
	return 0;
}


