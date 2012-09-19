#include <stdio.h>
#include <unistd.h>
#include "UnixShell.h"
#include <string.h>

int main(int argc, char ** argv)
{
	char * promptVal = processArgs(argc, argv);
	char input[MAX_LINE_LENGTH]; //User input has a limit of 100 characters, can be expanded later.

	printf("pVal = %s\n", promptVal);
	fgets(input, MAX_LINE_LENGTH, stdin);
	runCmd(input);
	return 0;
}

void runCmd(char * input)
{
	char ** argv = malloc(sizeof(char)*MAX_ARGS*MAX_LINE_LENGTH);

	parseCmd(input, argv);

	execvp(argv[0], argv);
	free(argv);	
}

void parseCmd(char * cmd, char ** argv)
{
	int i = 0;
	argv[0] = strtok(cmd, " ");
	while(argv[i] != NULL && i < (MAX_ARGS - 1))
	{
		printf("%s\n", argv[i]);
		argv[++i] = strtok(NULL, " ");
	}
}
	

char * processArgs(int argc, char ** argv)
{
	int c;
	opterr = 0;
	char * promptVal = NULL;
	while((c = getopt(argc, argv, "p:")) != -1)
	{
		switch(c)
		{
			case 'p':
				promptVal = optarg;
				break;
			case '?':
				if(optopt == 'p')
				{
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				}
				else if(isprint(optopt))
				{
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				}
				else
				{
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				}
			default:
				abort();
		}
	}
	

	if(promptVal == NULL)
	{
		promptVal = "308sh>";
	}

	return promptVal;

}
