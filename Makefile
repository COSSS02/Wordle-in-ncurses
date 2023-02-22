wordle: main.o wordle.o
	gcc -o wordle *.o -lncurses

main.o: main.c wordle.h
	gcc -c main.c
wordle.o: wordle.c wordle.h
	gcc -c wordle.c



clean:
	rm -f *.o wordle
