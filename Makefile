all:
	cc -o minesweeper main.c plansza.c gra.c wypisz.c wyniki.c wczytaj.c tryby.c

clean:
	rm -f minesweeper *.o