all:
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o processlab libprocesslab.o q1/libq1.o q1.c part2.c processlab.c

handin:
	mkdir handin
	cp q1.c handin/
	cp part2.c handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f initlab
	rm -f handin.tar.gz
