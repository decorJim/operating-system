all:
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o redirections libredirections.o redirections.c

handin:
	mkdir handin
	cp TubesNommes.c LigneDeCommande.c handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f redirections
	rm -f handin.tar.gz
