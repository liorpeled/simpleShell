#include <sys/wait.h> // needs to include library to use wait syscall
#include <string.h> // to elimniate warning using strcmp
#include <stdlib.h> // to eliminate warning using print
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>  // need to include library for using perror
#include <unistd.h> // need to include library for using close()
#include <fcntl.h>  //  need to include for using open()
extern char **getcommand();

int Piping(char **args){
    int numPipes=0;
    for(i = 0; args[i] != NULL; i++) {
        if(!strcmp(args[i],"|") )
        {
            numPipes++;
        }
        
        
    }
    
    int Redirect(char **args){
        int i=0;
        char ** temp_args =  (char **)malloc(0* sizeof(char)); // error
        pid_t childPid;
        int file_desc;
        
        for(i = 0; args[i] != NULL; i++) { // no need for this, can do it within for loop in main
            //can add memory to array herem, then make it equal to args[i] until if statemtn execute, at the end i can use free
            
            
            if(!strcmp(args[i],"<") )
            {
                
                childPid =fork();
                
                if(childPid==0)
                {
                    
                    file_desc = open(args[i+1],O_RDONLY, 0666  );
                    close(STDIN_FILENO);
                    dup(file_desc);
                    close(file_desc);
                    execvp(temp_args[0],temp_args);
                    // printf("%s\n", "no such command exist" );
                    exit(errno);
                }else
                {
                    waitpid(childPid, (void *)0,0);
                }
                free(temp_args);
                return 0;
                
            }
            
            if(!strcmp(args[i],">") && !strcmp(args[i+1],">"))
            {
                
                childPid =fork();
                
                if(childPid==0)
                {
                    
                    file_desc = open(args[i+2], O_CREAT | O_WRONLY | O_APPEND , 0666  );
                    close(STDOUT_FILENO);
                    dup(file_desc);
                    close(file_desc);
                    execvp(temp_args[0],temp_args);
                    printf("%s\n", "no such command exist" );
                    exit(errno);
                }else
                {
                    waitpid(childPid, (void *)0,0);
                }
                free(temp_args);
                return 0;
                
            }
            if(!strcmp(args[i],">"))
            {
                
                childPid =fork();
                
                if(childPid==0)
                {
                    
                    file_desc = open(args[i+1], O_CREAT | O_WRONLY | O_TRUNC , 0666  );
                    close(STDOUT_FILENO);
                    dup(file_desc);
                    close(file_desc);
                    execvp(temp_args[0],temp_args);
                    printf("%s\n", "no such command exist" );
                    exit(errno);
                }else
                {
                    waitpid(childPid, (void *)0,0);
                }
                //At the end the file has to be closed:
                
                
            }
            temp_args =  (char **)realloc(temp_args, i+1);
            temp_args[i] = args[i];
            //printf("%s\n", temp_args[i]);
            
            
        }
        
        free(temp_args);
        return 0;
    }
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
                printf("%s\n", "no such command exist" );
                exit(errno);
                
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
                printf("%s\n", "no such command exist" );
                exit(errno);
                
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
        int rd;
        while(1) {
            printf("%s ","#" );
            args = getcommand();
            if(args[0]==NULL)
            {
                continue;
            }
            if(strcmp(args[0],"exit") == 0||strcmp(args[0],"Exit")==0){
                
                exit(1);
            }
            for(i = 0; args[i] != NULL; i++) {
                if(!strcmp(args[i],">")||!strcmp(args[i],"<"))
                {
                    rd =1;
                }
            }
            if(rd)
            {
                Redirect(args);
                rd=0;
            }else
            {
                commandsNoArgs(args);
                commandsArgs(args);
            }
            
            
        }
    }
