#include<sys/types.h>
#include<stddef.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>


const char* WR_PATHA="/tmp/pipe_a";
const char* WR_PATHB="/tmp/pipe_b";

int main() {


   mkfifo("/tmp/pipe_a", 0666);
   mkfifo("/tmp/pipe_b", 0666);
   
   char buffer[100];

   if(fork()==0) {
     
     sprintf(buffer,"%d",getppid());
      execl("executables/Question1/johnDoe","johnDoe",buffer,NULL); 

   }
   if(fork()==0) {
     sprintf(buffer,"%d",getppid());
     execl("executables/Question1/uncharted","uncharted",buffer,NULL);
   }
   if(fork()==0) {
  
         int fdWrite1=open(WR_PATHA,O_WRONLY);
    
         write(fdWrite1,"b061658b83ab74f6",strlen("b061658b83ab74f6"));
         close(fdWrite1);
   
         exit(0);
   }
   if(fork()==0) {
       
         int fdWrite2=open(WR_PATHB,O_WRONLY);
         write(fdWrite2,"84613ab6d3076bcd",strlen("84613ab6d3076bcd"));
         close(fdWrite2);
         exit(0);
   }
   
   while(wait(NULL)>0);
   exit(0);
    
}
