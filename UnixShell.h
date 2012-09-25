#define MAX_LINE_LENGTH 100
#define MAX_ARGS 100
#define MAX_COMMAND_HISTORY 100
char * processArgs(int argc, char ** argv);
int parseCmd(char * cmd, char * argv[]);
void runCmdBlocking(char * argv[]);
void runCmdNoBlocking(char * argv[]);
void cd(char * argv[]);
void cwd();
void history(char * argv[], char * input);
void selfPid();
void selfPPid();
void processStatus(pid_t childPid, int status);
