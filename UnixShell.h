//Constants to avoid magic numbers
#define MAX_LINE_LENGTH 100 //Max length of input to the shell
#define MAX_ARGS 100        //Max amount of args that can be passed to the shell
#define MAX_COMMAND_HISTORY 100 //Max amount of history that is remembered by the shell

//Processes args from command line to UnixShell.exe
char * processArgs(int argc, char ** argv);

//Parses user input
int parseCmd(char * cmd, char * argv[]);

//Run commands with blocking or no blocking
void runCmdBlocking(char * argv[]);
void runCmdNoBlocking(char * argv[]);

//Processes waitpid() statuses
void processStatus(pid_t childPid, int status);

//Special Commands
void cd(char * argv[]);
void cwd();
void history(char * argv[], char * input);
void selfPid();
void selfPPid();
