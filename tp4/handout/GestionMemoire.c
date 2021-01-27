#include "./libs/lib.h"
unsigned int calculerNumeroDePage(unsigned long adresse) {
	// TODO
	return adresse/1024;
}

unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
	// TODO
	return adresse%1024;
}

unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
	// TODO
	return numeroDePage*1024+deplacementDansLaPage;
}

void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
    unsigned int page=calculerNumeroDePage(req->adresseVirtuelle);
    unsigned long deplacement=calculerDeplacementDansLaPage(req->adresseVirtuelle);
    unsigned long adresse=calculerAdresseComplete(page,deplacement);

    for(int i=0;i<TAILLE_TLB;i++) {
      int numPageMem=mem->tlb->numeroPage[i];
      unsigned long dep=calculerDeplacementDansLaPage(req->adresseVirtuelle);

      if(page==numPageMem && mem->tlb->entreeValide[i]==1) {
        req->adressePhysique=calculerAdresseComplete(mem->tlb->numeroCadre[i],dep);
        mem->tlb->dernierAcces[i]=req->date;
        break;
      }
      else {
        req->adressePhysique=0;
      }
    }
}

void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
    unsigned int page=calculerNumeroDePage(req->adresseVirtuelle);
    unsigned long deplacement=calculerDeplacementDansLaPage(req->adresseVirtuelle);
     
      if(mem->tp->entreeValide[page]==1) {
        req->adressePhysique=calculerAdresseComplete(mem->tp->numeroCadre[page],deplacement);
      }
      else {
        req->adressePhysique=0;
      }
    
}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	//TODO

    unsigned long deplacement=calculerDeplacementDansLaPage(req->adresseVirtuelle);

    for(int i=0;i<TAILLE_MEMOIRE;i++) {
     
     if(mem->memoire->utilisee[i]==0) {
        unsigned long adresse=calculerAdresseComplete(i,deplacement);
        req->adressePhysique=adresse;
        mem->memoire->dateCreation[i]=req->date;
        mem->memoire->dernierAcces[i]=req->date;
        mem->memoire->utilisee[i]=1;
        break;
     }
    }
}

void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
   int positionMin=0;
   
       for(int position=0; position <TAILLE_TLB;++position){
   
               // on a une cadre tlb qui a au moins un emplacement vide
           if(mem->tlb->entreeValide[position] == 0) {
               
               mem->tlb->entreeValide[position] = 1;
               mem->tlb->dateCreation[position] = req->date;
               mem->tlb->dernierAcces[position] = req->date;
               mem->tlb->numeroPage[position] = calculerNumeroDePage(req->adresseVirtuelle);
               mem->tlb->numeroCadre[position] = calculerNumeroDePage(req->adressePhysique);
               break;
           }
   
           // cas ou la tlb est remplite.
           else if(mem->tlb->dateCreation[position] < mem->tlb->dateCreation[positionMin]){
               positionMin = position;
           }
   
           mem->tlb->entreeValide[positionMin] = 1;
           mem->tlb->dateCreation[positionMin] = req->date;
           mem->tlb->dernierAcces[positionMin] = req->date;
           mem->tlb->numeroPage[positionMin] = calculerNumeroDePage(req->adresseVirtuelle);
           mem->tlb->numeroCadre[positionMin] = calculerNumeroDePage(req->adressePhysique);
       }
}

// NE PAS MODIFIER
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}
