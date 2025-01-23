all:
	cc -o minesweeper main.c plansza.c gra.c wypisz.c wyniki.c wczytaj.c tryby.c
test_g: all
	@./minesweeper -t || echo "test_g zakonczony niepowodzeniem" 

test_wl: all
	@./minesweeper -f test_wygrana_latwy || echo "test_wl zakonczony niepowodzeniem."
	
test_nl: all
	@./minesweeper -f test_nieukonczona_latwy || echo "test_nl zakonczony niepowodzeniem."
	
test_pl: all
	@./minesweeper -f test_przegrana_latwy || echo "test_pl zakonczony niepowodzeniem."

test_pn: all
	@./minesweeper -f test_przegrana_normalny || echo "test_pn zakonczony niepowodzeniem."

test_nt: all
	@./minesweeper -f test_nieukonczona_trudny || echo "test_nt zakonczony niepowodzeniem."
	
test: test_g test_wl test_nl test_pl test_pn test_nt

clean:
	rm -f minesweeper *.o