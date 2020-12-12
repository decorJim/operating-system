/*
 * Clone Lab - part2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include "libprocesslab.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include <sys/wait.h> 
#include <sys/types.h> 

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------

// -------------------------------------------------

#define PART2_OUTPUT_FILE_PATH "part2Output.txt"



void part2() {
    // Ouverture du fichier de sortie pour la question 2.3
    FILE* part2OutputFile = fopen(PART2_OUTPUT_FILE_PATH, "a");
    
    // TODO
    int pid=getpid();
    char buffer[20];
 
    sprintf(buffer,"%d",pid);
    fprintf(part2OutputFile, "Root process has pid %d (message from process %s", pid,"level0\n");
   
    if(fork()==0) {
       
       if(fork()==0) {
         registerProc(2,1, getpid(), getppid());
         execl("./part2/level2.1","level2.1",buffer,NULL);
         pid=getpid(); 
         fprintf(part2OutputFile, "Root process has pid %d (message from process %s", pid,"level2.1\n");
         
         exit(0);
       }
       if(fork()==0) {
         registerProc(2, 2, getpid() , getppid());
         execl("./part2/level2.2","level2.2",buffer,NULL);

         pid=getpid();
    
         exit(0);
       }
       registerProc(1,1, getpid(), getppid());
       execl("./part2/level1.1","level1.1",buffer,NULL);

       pid=getpid();
    
       fprintf(part2OutputFile, "Root process has pid %d (message from process %s\n", pid,"level1.1");
       wait(NULL);
       wait(NULL);
       exit(0);
    }
    if(fork()==0) {
     
     if(fork()==0) {
        registerProc(2, 3, getpid(), getppid());
        

        execl("./part2/level2.3","level2.3",buffer,NULL);

        pid=getpid();
    
       

       exit(0);
     }
     if(fork()==0) {
        registerProc(2, 4, getpid(), getppid());
        execl("./part2/level2.4","level2.4",buffer,NULL);

        pid=getpid();
    
        fprintf(part2OutputFile, "Root process has pid %d (message from process %s", pid,"level2.4\n");
        exit(0);
     }
     registerProc(1,2, getpid(), getppid());
     execl("./part2/level1.2","level1.2",buffer,NULL);
     pid=getpid();
    
    
     wait(NULL);
     wait(NULL);
     exit(0);
     
    }
    if(fork()==0) {
      
     if(fork()==0) {
       registerProc(2, 5, getpid(), getppid());
        execl("./part2/level2.5","level2.5",buffer,NULL);
        pid=getpid();
    
        

       exit(0);
     }
     if(fork()==0) {
       registerProc(2, 6, getpid(), getppid());
       execl("./part2/level2.6","level2.6", buffer,NULL);
       pid=getpid();
    
       fprintf(part2OutputFile, "Root process has pid %d (message from process %s", pid,"level2.6\n");

       exit(0);
     }
      registerProc(1,3, getpid(), getppid());
      execl("./part2/level1.3","level1.3",buffer,NULL);
      pid=getpid();
    
      fprintf(part2OutputFile, "Root process has pid %d (message from process %s", pid,"level1.3\n");
      wait(NULL);
      wait(NULL);
      exit(0);
    }
    fclose(part2OutputFile);
    while(wait(NULL)>0);

    return;
    
}
