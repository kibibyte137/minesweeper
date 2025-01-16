#include "wypisz.h"
#include "struktury.h"
#include "kolory.h"
#include "makra.h"

#include <stdio.h>
#include <stdlib.h>

/*	Funkcja widoczny_symbol_pola
	Zwraca widoczny symbol reprezentujacy stan pola w grze.
	Parametry:
		pole - wskaznik na strukture Pole, ktora zawiera informacje o stanie pola
	Dzialanie:
		- Zwraca rozne symbole w zaleznosci od stanu pola:
			- Symbol flagi, jesli pole jest oznaczone flaga.
			- Symbol zakrytego pola, jesli pole jest zakryte.
			- Symbol miny, jesli pole zawiera mine.
			- Liczba sasiednich min, jesli pole nie jest mina, ale sa takie w sasiedztwie.
			- Symbol pustego pola, jesli pole nie spelnia powyzszych warunkow.
*/
char widoczny_symbol_pola(const Pole* pole){
	if(pole->flaga) return FLAGA; /*Zwrocenie symbolu flagi, jesli pole posiada flage*/
	if(pole->zakryte) return ZAKRYTE; /*Zwrocenie symbolu zakrytego pola, jesli pole jest zakryte*/
	if(pole->mina) return MINA; /*Zwrocenie symbolu miny, jesli pole ja posiada*/
	if(pole->sasiednie_miny > 0 && !pole->mina) return LICZBA(pole->sasiednie_miny); /*Zwrocenie liczby min jesli pole nie jest mina, ale sa takie w sasiedztwie*/
	return PUSTE; /*Zwrocenie symbolu pustego pola, jesli pole nie spelnia powyzszych warunkow*/
}

/*	Zwraca logiczny symbol reprezentujacy stan pola w grze (niewidoczny dla gracza).
	Parametry:
		pole - wskaznik na strukture Pole, ktora zawiera informacje o stanie pola
	Dzialanie:
		- Zwraca symbol reprezentujacy stan pola:
			- Symbol miny, jesli pole zawiera mine.
			- Liczba sasiednich min, jesli pole nie jest mina, ale sa takie w sasiedztwie.
			- Symbol pustego pola, jesli pole nie spelnia powyzszych warunkow.
*/
char logiczny_symbol_pola(const Pole* pole){
	if(pole->mina) return MINA; /*Zwrocenie symbolu miny, jesli pole ja posiada*/
	if(pole->sasiednie_miny > 0 && !pole->mina) return LICZBA(pole->sasiednie_miny); /*Zwrocenie liczby min jesli pole nie jest mina, ale sa takie w sasiedztwie*/
	return PUSTE; /*Zwrocenie symbolu pustego pola, jesli pole nie spelnia powyzszych warunkow*/
}

/*	Funkcja wypisz_plansze_widoczna
	Wypisuje widoczny stan planszy gry.
	Parametry:
		plansza - wskaznik na strukture Plansza, ktora zawiera informacje o calej planszy gry
	Dzialanie:
		Wypisuje stan planszy gry w postaci symboli, ktore reprezentuja rozne stany pol.
*/
void wypisz_plansze_widoczna(Plansza* plansza){
	if(!plansza){ /*Obsluga bledu, jesli plansza nie istnieje - wypisanie komunikatu i zakonczenie dzialania funkcji*/
		printf("Blad. Plansza nie istnieje.\n");
		return;
	}
	
	const char* kolory[] = {RESET, CYAN, GREEN, RED, BLUE, YELLOW, BRIGHT_CYAN, MAGENTA, BRIGHT_BLACK}; /*Tablica kolorow dla liczby sasiednich min*/
	
	/*Wypisanie indeksow kolumn*/
	printf("     "); /*Odstep na indeksy wierszy*/
	for(int i = 0; i < plansza->kolumny; i++){
		printf("%3d", i);
	}
	printf("\n");
	
	printf("     "); /*Odstep na indeksy wierszy*/
	for(int i = 0; i < plansza -> kolumny; i++){
		printf("___"); /*Linia oddzielajaca*/
	}
	printf("\n");
	
	
	for(int i = 0; i < plansza->wiersze; i++){ 
		printf("%3d |", i); /*Wypisanie indeksu wiersza*/
		for(int j = 0; j < plansza->kolumny; j++){
			Pole* pole = &plansza->pola[i][j]; /*Pobranie wskaznika na pole*/
			char symbol = widoczny_symbol_pola(pole); /*Pobranie widocznego symbolu pola*/
			
			if(symbol >= '1' && symbol <= '8'){ /*Jesli symbol to LICZBA, uzyj odpowiedniego koloru*/
				int liczba = symbol - '0';
				printf("%s%3c%s", kolory[liczba], symbol, RESET);
			} else if (symbol == MINA){ /*Jesli symbol to MINA, uzyj odpowiedniego koloru*/
				printf(BRIGHT_RED_BG "%3c" RESET, symbol);
			} else if (symbol == FLAGA){ /*Jesli symbol to FLAGA, uzyj odpowiedniego koloru*/
				printf(BRIGHT_RED "%3c" RESET, symbol);
			} else {
				printf("%3c", symbol); /*Wyswietlenie pozostalych symboli w domyslnym kolorze*/
			}
		}
		printf("\n");
	}
}

/*	Funkcja wypisz_plansze_logiczna
	Wypisuje logiczny stan planszy gry (z uwzglednieniem informacji niewidocznych dla gracza).
	Parametry:
		plansza - wskaznik na strukture Plansza, ktora zawiera informacje o calej planszy gry
	Dzialanie:
		Wypisuje stan planszy gry w postaci symboli, ktore reprezentuja rozne stany pol.
*/
void wypisz_plansze_logiczna(Plansza* plansza){
	if(!plansza){ /*Obsluga bledu, jesli plansza nie istnieje - wypisanie komunikatu i zakonczenie dzialania funkcji*/
		printf("Blad. Plansza nie istnieje.\n");
		return;
	}
	
	const char* kolory[] = {RESET, CYAN, GREEN, RED, BLUE, YELLOW, BRIGHT_CYAN, MAGENTA, BRIGHT_BLACK}; /*Tablica kolorow dla liczby sasiednich min*/
	
	/*Wypisanie indeksow kolumn*/
	printf("     "); /*Odstep na indeksy wierszy*/
	for(int i = 0; i < plansza->kolumny; i++){
		printf("%3d", i);
	}
	printf("\n");
	
	printf("     "); /*Odstep na indeksy wierszy*/
	for(int i = 0; i < plansza -> kolumny; i++){
		printf("___"); /*Linia oddzielajaca*/
	}
	printf("\n");
	
	for(int i = 0; i < plansza->wiersze; i++){
		printf("%3d |", i); /*Wypisanie indeksu wiersza*/
		for(int j = 0; j < plansza->kolumny; j++){
			Pole* pole = &plansza->pola[i][j]; /*Pobranie wskaznika na pole*/
			char symbol = logiczny_symbol_pola(pole); /*Pobranie logicznego symbolu pola*/
			
			if(symbol >= '1' && symbol <= '8'){ /*Jesli symbol to LICZBA, uzyj odpowiedniego koloru*/
				int liczba = symbol - '0';
				printf("%s%3c%s", kolory[liczba], symbol, RESET);
			} else if (symbol == MINA){ /*Jesli symbol to MINA, uzyj odpowiedniego koloru*/
				printf(BRIGHT_RED_BG "%3c" RESET, symbol);
			} else {
				printf("%3c", symbol); /*Wyswietlenie pozostalych symboli w domyslnym kolorze*/
			}
		}
		printf("\n");
	}
}


/*	Funkcja wypisz_plik
	Wczytuje i wypisuje zawartosc pliku.
	Parametry:
		nazwa_pliku - wskaznik do ciagu znakow zawierajacy nazwe pliku
	Dzialanie:
		Wypisuje tekst znajdujacy sie w pliku o podanej nazwie linia po linii.
*/

void wypisz_plik(const char* nazwa_pliku, const char* kolor){
	FILE* plik = fopen(nazwa_pliku, "r"); /*Otworzenie pliku w trybie odczytu*/
	if(!plik){ /*Obsluga bledu, jesli plik nie istnieje - wypisanie komunikatu i przerwanie funkcji*/
		printf("Nie mozna otworzyc pliku: %s.\n", nazwa_pliku);
		return;
	}
	char linia[256]; /*Bufor na linie tekstu (maksymalnie 255 znakow)*/
	while(fgets(linia, sizeof(linia), plik)){ /*Czytanie linii, dopoki sa w pliku*/
		printf("%s%s" RESET, kolor, linia); /*Wypisanie linii na wyjscie standardowe*/
	}
	printf("\n");
	fclose(plik); /*Zamkniecie pliku*/
}

/*	Funkcja wypisz_top5
	Wypisuje piec najlepszych wynikow z pliku.
	Parametry:
		nazwa_pliku - wskaznik na nazwe pliku
*/
void wypisz_top5(const char* nazwa_pliku){
	FILE* plik = fopen(nazwa_pliku, "r"); /*Otworzenie pliku w trybie odczytu*/
	if(!plik){ /*Obsluga bledu, jesli plik nie istnieje - wypisanie komunikatu i przerwanie funkcji*/
		printf("Nie mozna otworzyc pliku do odczytu: %s.\n", nazwa_pliku);
		return;
	}
	
	char linia[256]; /*Bufor na linie tekstu (maksymalnie 255 znakow)*/
	int licznik = 1; /*Licznik pozycji dla TOP 5*/
	
	while(fgets(linia, sizeof(linia), plik) && licznik < 6){ /*Wypisanie linii na wyjscie standardowe, dopoki nie osiagnieto 5 wpisow*/
		printf("%d. %s", licznik, linia);
		licznik++;
	}
	fclose(plik); /*Zamkniecie pliku*/
}