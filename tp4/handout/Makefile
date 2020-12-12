all:
	gcc GestionMemoire.c -Llibs -leval -o answer

handin:
	mkdir handin
	cp -r GestionMemoire.c handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f handin.tar.gz
