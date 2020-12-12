#include<stddef.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<stdio.h>


int main() {

    int tmpFile=open("executables/Question2/tmpfile",O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR);
    
    int pipefd[2];
    pipe(pipefd);

    if(fork()==0) {
      close(pipefd[0]);
      dup2(pipefd[1],STDOUT_FILENO);
      dup2(pipefd[1],STDERR_FILENO);
      close(pipefd[1]);
      execl("executables/Question2/prog1","prog1",NULL);
    }
    while(wait(NULL)>0);
   
    if(fork()==0) {
      close(pipefd[1]);
      dup2(pipefd[0], tmpFile);
      close(pipefd[0]);
      execl("executables/Question2/prog2","prog2",NULL);

    }
    //while(wait(NULL)>0);
    
    /*if(fork()==0) {


    }
    while(wait(NULL)>0);
    */
    
    close(tmpFile);
    exit(0);
}
