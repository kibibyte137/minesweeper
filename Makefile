all:
	cc -o minesweeper main.c plansza.c gra.c wypisz.c wyniki.c wczytaj.c

clean:
	rm -f program *.o