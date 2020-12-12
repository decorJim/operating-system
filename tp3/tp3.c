#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int NbL = 0;
sem_t S;
sem_t tour;
sem_t T;
sem_t U;
char buffer[100];

struct Params {
   int boucleId;
   int tid;
   sem_t* sem;
};

void* acces_bd(int id_boucle,int tid,sem_t* sem) {
  sem_wait(sem);
  sprintf(buffer, "%s", "Acces a la base de donnee");
  printf(" valeur dans access_bd du thread %d \n", tid); 
  sem_post(sem);
  return NULL;
}

// A verifier
void* acces_bd_lecture(void* id) {
  while(1) {
  sem_wait(&tour);  // attend pour son tour
  sem_wait(&U);  // pour l'acces a la variable NbL
  if(NbL == 0) sem_wait(&T);  // pour l'acces a la base de donne
  sem_post(&tour);  // libere jeton pour le tour
  NbL++;        // incrementer le nb de lecteurs
  sem_post(&U); // libere le jeton pour acces de la variable NbL
  printf("%d va lire\n",id);  // va lire
  sprintf(buffer, "%s", "Acces en mode lecture");
  sem_wait(&U); // pour le jeton de la variable NbL
  NbL--;  // decrementer NbL
  if(NbL == 0) sem_post(&T);  // si il n'y a plus de lecteurs liberer le jeton d'acces a la base de donnees
  sem_post(&U); // liberer le jeton d'acces a NbL
  }
  return NULL;
}

//A verifier
void* acces_bd_ecriture(void* id) {
  while(1) {
  sem_wait(&tour); // attend pour son tour
  sem_wait(&T); // attend pour l'acces a la base de donnees
  sem_post(&tour); // libere jeton pour le tour
  printf("%d va ecrire\n",id); // va ecrire
  sprintf(buffer, "%s", "Acces en mode ecriture");
  sem_post(&T); // libere jeton pour l'acces a la base de donnes
  }
  return NULL;
}

void* wrapper(void* args) {
  struct Params* params=(struct Params*)args;
  return acces_bd(params->boucleId,params->tid,params->sem);
}


sem_t* init_sem() {
   sem_init(&S,0,1);
   return &S;
};

void detruire_sem(sem_t* sem) {
  sem_destroy(sem);
}

// dans une base de données, une seule accès simultané peut se faire, car pour éviter d'avoir des résultats inconhérents il faut que l'accès en écriture soit en exclusion mutuelle

int main() {

    // initialisation des semaphores
    init_sem();
    sem_init(&tour,0,1);
    sem_init(&T,0,1);
    sem_init(&U,0,1);

    pthread_t tids[10];
    pthread_t tids2[10];
    int i;

    // allocation pour 10 objets Params
    struct Params *mesParams=calloc(10,10*sizeof(struct Params));

    // creation des objets
    for(i=0;i<10;i++) {
        mesParams[i].boucleId=i;
        mesParams[i].tid=tids[i];
        mesParams[i].sem=&S;
        pthread_create(&tids[i],NULL,wrapper,(void*)&mesParams[i]);
    }
    // attente de la fin d'execution des chaque threads
    for(i=0;i<10;i++) {
        pthread_join(tids[i],NULL);
    }

    // liberation de l'espace alloue des 10 objets
    free(mesParams);

    //Partie C 
   
    for(i=0;i<10;i++) {
        pthread_create(&tids[i],NULL,acces_bd_lecture,(void*)&tids[i]);
        pthread_create(&tids2[i],NULL,acces_bd_ecriture,(void*)&tids2[i]);
    }
    for(i=0;i<10;i++) {
        pthread_join(tids[i],NULL);
        pthread_join(tids2[i],NULL);
    }

    detruire_sem(&S);
    detruire_sem(&tour);
    detruire_sem(&T);
    detruire_sem(&U);
 
    return 0;
    
}
