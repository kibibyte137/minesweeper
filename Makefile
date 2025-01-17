all:
	cc -o minesweeper main.c plansza.c gra.c wypisz.c wyniki.c

clean:
	rm -f program *.o