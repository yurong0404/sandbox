#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lib.h"

#define MAX_COMMAND_LEN 10


int main(int argc, char *argv[])
{
	int flag_p=0, flag_d=0, flag_cmd=0;
	int opt;
	char **sandboxarg = malloc(MAX_COMMAND_LEN*sizeof(char *)); 
	char **commarg = malloc(MAX_COMMAND_LEN*sizeof(char *));		//must remember to free the address
	memset(sandboxarg, 0, MAX_COMMAND_LEN);
	memset(commarg, 0, MAX_COMMAND_LEN);
	
	if (split_command(sandboxarg, commarg, argc, argv) == -1)
		exit(EXIT_FAILURE);

	for (int i=0; sandboxarg[i]!=0; i++)	
		printf("%s\n", sandboxarg[i]);
	printf("==================\n");
	for (int i=0; commarg[i]!=0; i++)	
		printf("%s\n", commarg[i]);


	/*while ((opt = getopt(argc, argv, "pd--")) != -1) {
        switch (opt) {
        case 'p':
			printf("p\n");
            flag_p = 1;
            break;
        case 'd':
			printf("d\n");
			flag_d = 1;
            break;
		case '-':
			printf("-\n");
			flag_cmd = 1;
			break;
        default:
            fprintf(stderr, "Usage: %s [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n", argv[0]);
			fprintf(stderr, "\t-p: set the path to sandbox.so, default = ./sandbox.so\n");
			fprintf(stderr, "\t-d: restrict directory, default = .\n");
			fprintf(stderr, "\t--: seperate the arguments for sandbox and for the executed command\n");
            exit(EXIT_FAILURE);
        }
		printf("%d\n", optind);
    }
	*/
	return 0;
}
