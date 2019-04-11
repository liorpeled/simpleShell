#include <sys/wait.h> // needs to include libary to use wait syscall
#include <string.h> // to elimniate warning using strcmp
#include <stdlib.h> // to eliminate warning using print
#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <errno.h>

extern char **getcommand();


int commandsNoArgs(char **args){
	
	if(args[1]==NULL)
	{
		pid_t childPid;
		childPid =fork();
		if(childPid <0)
		{
			perror("child process error");
			exit(1); //check later
		}
		if(childPid==0)
		{
			execvp(args[0],args); //reading first argument from args
			
		}else
		{
			wait(0); //check later
		}
	}
	
	return 0;

}
int commandsArgs(char **args){
    
    if(args[1]!=NULL)
    {
        pid_t childPid;
		childPid =fork();
		if(childPid <0)
		{
			perror("child process error");
			exit(1); //check later
		}
		if(childPid==0)
		{
			execvp(args[0],args); //reading first argument from args
			
		}else
		{
			wait(0); //check later
		}
    }
		return 0;
}

int main() {
  int i;
  char **args;

  while(1) {
  	args = getcommand();

  	if(strcmp(args[0],"exit") == 0||strcmp(args[0],"Exit")==0){
  		
  		exit(1);
  	}
  	commandsNoArgs(args);
  	commandsArgs(args);
  	for(i = 0; args[i] != NULL; i++) {
		printf("Argument %d: %s\n", i, args[i]); 
  	}
  	
  }
}

	
