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
    int dateDebut=10000;
    for(int i=0;i<TAILLE_TLB;i++) {
      if(mem->tlb->dateCreation[i]<dateDebut) {
        dateDebut=mem->tlb->dateCreation[i];
      }
    }
   
    for(int i=0;i<TAILLE_TLB;i++) {
      if(mem->tlb->dateCreation[i]==dateDebut) {
        mem->tlb->numeroPage[i]=req->adresseVirtuelle;
        mem->tlb->numeroCadre[i]=req->adressePhysique;
      }
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
