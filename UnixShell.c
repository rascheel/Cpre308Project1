#include <stdio.h>
#include <unistd.h>
#include "UnixShell.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	char * promptVal = processArgs(argc, argv);
	char input[MAX_LINE_LENGTH]; //User input has a limit of 100 characters, can be expanded later.

	//printf("pVal = %s\n", promptVal);
	while(1)
	{
		char * argv[MAX_ARGS];
		printf("%s ", promptVal);
		fgets(input, MAX_LINE_LENGTH, stdin);
		history(NULL, input);
		int lastIndex = parseCmd(input, argv);
		if(lastIndex != 0)
		{
			if(!strcmp(argv[0], "exit"))
			{
				break;
			}
			else if(!strcmp(argv[0], "cd"))
			{
				cd(argv);
			}	
			else if(!strcmp(argv[0], "cwd"))
			{
				cwd();
			}	
			else if(!strcmp(argv[0], "history"))
			{
				history(argv, NULL);
			}	
			else if(!strcmp(argv[0], "pid"))
			{
				selfPid();
			}	
			else if(!strcmp(argv[0], "ppid"))
			{
				selfPPid();
			}	
			else if(!strcmp(argv[lastIndex-1], "&"))
			{
				argv[lastIndex-1] = NULL; //Must strip the & so we don't send it to the process
				runCmdNoBlocking(argv);
			}
			else
			{
				runCmdBlocking(argv);
			}
		}
		int status;
		pid_t possibleChild = waitpid(-1, &status, WNOHANG);
		if(possibleChild > 0)
		{
			printf("process %i exited\n", possibleChild);
			processStatus(possibleChild, status);
		}
	}
	return 0;
}

void cd(char * argv[])
{
	if(argv[1] == NULL)
	{
		printf("No paramaters supplied to cd\n");
	}
	else
	{
		if(chdir(argv[1]) == -1)
		{
			printf("Cd failed - %s\n", strerror(errno));
		}
	}
}

void cwd()
{
	printf("%s\n", getcwd(NULL, 100));
}

void history(char * argv[], char * input)
{
	static char previousCmds[MAX_COMMAND_HISTORY][MAX_LINE_LENGTH];
	static int histSize = 0; //stores the size of the previousCmd array
	if(argv == NULL)//if argv is NULL then we are simply adding to the command history list
	{
		strcpy(previousCmds[histSize], input);
		histSize++;
	}
	else//else we are printing the history
	{
		if(*(argv[1]) != '-')//if the first character of the argument is not '-' then this doesnt match the valid parameters
		{
			printf("invalid paramaters for history no dash\n");
		}
		else
		{
			int x = atoi((argv[1]+1)); //returns 0 if string is not a number
			if(x == 0)//if string is not a number then does not match valid parameter
			{
				printf("invalid paramaters for history no number\n");
			}
			else//parameters are valid. Print history
			{
				if(x > histSize)
				{
					x = histSize;
				}
				int i;
				for(i = 0; i < x-1; i++)
				{
					printf("%s", previousCmds[i]);
				}
			}
		}
	}
		
}

void selfPid()
{
	printf("PID = %i\n", getpid());
}

void selfPPid()
{
	printf("PPID = %i\n", getppid());
}

void processStatus(pid_t childPid, int status)
{
	if(WIFEXITED(status))
	{
		printf("Child %i exited with return code %d\n", childPid, WEXITSTATUS(status));
	}
	else if(WIFSIGNALED(status))
	{
		printf("Child %i terminated because it didn't catch signal number %d\n", childPid, WTERMSIG(status));
	}
}

void runCmdBlocking(char * argv[])
{
	pid_t pid = fork();
	if(pid == 0)
	{
		//This is the child process.
		//printf("child process reporting in\n");
		printf("pid: %i\n", getpid());
		execvp(*argv, argv);
		printf("Unknown command.\n");
		exit(0);
	}
	else
	{
		int status;
		//printf("parent waiting for child\n");
		waitpid(pid, &status, 0);
		//printf("parent done waiting\n");
		processStatus(pid, status);
	}
		
}

void runCmdNoBlocking(char * argv[])
{
	pid_t pid = fork();
	if(pid == 0)
	{
		//This is the child process.
		//printf("child process reporting in\n");
		printf("\npid: %i\n", getpid());
		execvp(*argv, argv);
		printf("Pid: %i failed - %s\n", getpid(), strerror(errno));
	}
}

int parseCmd(char * cmd, char * argv[])
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
		//printf("%s\n", argv[i-1]);
		//fflush(stdout);
		argv[i] = strtok(NULL, " \n");
                i++;
	}
	
	return i-1;
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
