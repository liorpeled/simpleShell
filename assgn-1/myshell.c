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


/*>
 
 args = [ls, >, filename]
 tmp_args = [ls, -la]
 
 close(stdout)
 dup(fd)
 execvp(tmp_args)
 */


int Redirect(char **args){
    int i=0;
    char ** temp_args;
    temp_args =  (char **)malloc(0* sizeof(char)); // error
    
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
            }else
            {
                waitpid(childPid, (void *)0,0);
            }
            free(temp_args);
            return 0;
            
        }
        
        if(!strcmp(args[i],">"))
        {
            //printf("%s\n",args[i+1]);
            
            pid_t childPid;
            childPid =fork();
            
            if(childPid==0)
            {
                file_desc = open(args[i+1], O_CREAT | O_WRONLY| O_TRUNC , 0666 );
                
                close(STDOUT_FILENO);
                dup(file_desc);//                    dup2(file_desc,1);
                close(file_desc);
                   execvp(temp_args[0],temp_args); // this causes the program to be terminated, fixed with fork();
            }else
            {
                waitpid(childPid, (void *)0,0);
            }
            //At the end the file has to be closed:
            
            
        }
        temp_args =  (char **)realloc(temp_args, i+1);
        temp_args[i] = args[i];
        printf("%s\n", temp_args[i]);
        
        
    }
    
    
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
        Redirect(args);
        //commandsNoArgs(args);
        //commandsArgs(args);
        for(i = 0; args[i] != NULL; i++) {
            //printf("Argument %d: %s\n", i, args[i]);
        }
        
    }
}
