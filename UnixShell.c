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
	char argv[MAX_ARGS][MAX_LINE_LENGTH];
	//printf("before malloc\n");
	//fflush(stdout);
	//char ** argv = malloc(sizeof(char)*MAX_ARGS*MAX_LINE_LENGTH);
	//printf("after malloc\n");
	//fflush(stdout);
	parseCmd(input, argv);
	int i = 0;
	while(argv[i] != NULL)
	{
		printf("2: %s\n", argv[i]);
		fflush(stdout);
		i++;
	}



	fflush(stdout);

	//int pid = fork();
	//if(pid == 0)
	//{
		//This is the child process.

		//execvp(*argv, argv);
	//free(argv);	
}

void parseCmd(char * cmd, char ** argv)
{
	//printf("in parseCmd\n");
	//fflush(stdout);
	int i = 1;
	//printf("befre strtok 0\n");
	//fflush(stdout);
        argv[0] = strtok(cmd, " \n");
	//printf("after strtok 0\n");
	//fflush(stdout);
        
	while(argv[i-1] != NULL && i < (MAX_ARGS - 1))
	{
		printf("%s\n", argv[i-1]);
		fflush(stdout);
		argv[i] = strtok(NULL, " \n");
                i++;
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
