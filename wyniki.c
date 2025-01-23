#include "wyniki.h"
#include "kolory.h"
#include "makra.h"
#include "struktury.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*	Funkcja oblicz_wynik
	Oblicza wynik gracza na podstawie liczby odkrytych pol i mnoznika.
	Parametry:
		plansza - wskaznik na strukture Plansza, ktora zawiera informacje o stanie gry
		mnoznik - mnoznik uzywany do obliczenia wyniku
	Zwraca:
		Wyliczony wynik gracza.
*/

int oblicz_wynik(Plansza* plansza, int mnoznik){
	plansza->wynik=mnoznik*plansza->odkryte_pola; /*Obliczenie wyniku jako iloczyn mnoznika i liczby odkrytych pol*/
	return plansza->wynik; /*Zwrocenie wynik*/
}

/*	Funkcja porownaj_wyniki
	Porownuje dwa wyniki graczy na potrzeby sortowania.
	Parametry:
		a - wskaznik na pierwszy wynik (ciag znakow w formacie "imie wynik")
		b - wskaznik na drugi wynik (ciag znakow w formacie "imie wynik")
	Zwraca:
		Wartosc ujemna, zero lub dodatnia w zaleznosci od porzadku:
		- Ujemna, jesli pierwszy wynik powinien byc za drugim.
		- Zero, jesli wyniki sa rowne.
		- Dodatnia, jesli pierwszy wynik powinien byc przed drugim.
*/

int porownaj_wyniki(const void* a, const void* b){
    int wynik_a, wynik_b;
	char imie_a[MAX_IMIE], imie_b[MAX_IMIE];
	
    sscanf((const char*)a, "%s %d", imie_a, &wynik_a); /*Wyodrebnienie imienia i wyniku z pierwszego ciagu*/
    sscanf((const char*)b, "%s %d", imie_b, &wynik_b); /*Wyodrebnienie imienia i wyniku z drugiego ciagu*/
	
	if(wynik_a != wynik_b){ /*Zwrocenie roznicy w odwrotnej kolejnosci, jesli wyniki sa rozne*/
		return wynik_b - wynik_a;
	}
	
	return strcmp(imie_a, imie_b); /*Alfabetyczne porownanie imion, jesli wyniki sa rowne*/
}

/*	Funkcja zapisz_wynik
	Zapisuje wynik gracza do pliku.
	Parametry:
		nazwa_pliku - wskaznik na nazwe pliku, do ktorego wynik zostanie zapisany
		imie - wskaznik na imie gracza
		wynik - wynik gracza
	Dzialanie:
		Otwiera plik w trybie dopisywania i zapisuje wynik w formacie "imie wynik".
		Jesli operacja zapisu sie nie powiedzie, wyswietla komunikat o bledzie.
*/

void zapisz_wynik(const char* nazwa_pliku, const char* imie, int wynik){
	FILE* plik = fopen(nazwa_pliku, "a"); /*Otworzenie pliku w trybie dopisywania */
	if(!plik){ /*Obsluga bledu otwarcia pliku*/
		printf("Nie mozna otworzyc pliku: %s.\n", nazwa_pliku);
		return;
	}
	if(fprintf(plik, "%-15s %d\n", imie, wynik) < 0) { /*Obsluga bledu zapisu do pliku*/
		printf("Blad podczas zapisu do pliku: %s", nazwa_pliku);
	}
	fclose(plik); /*Zamkniecie pliku*/
}

/*	Funkcja posortuj_wyniki_w_pliku
	Sortuje wyniki w pliku wedlug ich wartosci (w przypadku rownych wartosci - wedlug kolejnosci alfabetycznej imion graczy) i zapisuje je w kolejnosci od najwyzszego do najnizszego.
	Parametry:
		nazwa_pliku - wskaznik na nazwe pliku z wynikami
	Dzialanie:
		- Wczytuje wyniki z pliku do bufora.
		- Sortuje wyniki za pomoca funkcji porownaj_wyniki.
		- Nadpisuje plik posortowanymi wynikami.
*/

void posortuj_wyniki_w_pliku(const char* nazwa_pliku){
	char wyniki[MAX_WYNIKOW][MAX_IMIE + 10]; /*Bufor na linie z imieniem i wynikiem*/
	int liczba_wynikow = 0;
	
	int najwyzszy_wynik_przed = -1; /*Przechowuje najwyzszy wynik przed sortowaniem*/
	int najwyzszy_wynik_po = -1; /*Przechowuje najwyzszy wynik po sortowaniu*/
	char najwyzsze_imie_przed[MAX_IMIE] = ""; /*Przechowuje imie gracza z najwyzszym wynikiem przed sortowaniem*/
	char najwyzsze_imie_po[MAX_IMIE] = ""; /*Przechowuje imie gracza z najwyzszym wynikiem po sortowaniu*/

	FILE* plik = fopen(nazwa_pliku, "r"); /*Otworzenie pliku w trybie odczytu*/
	if(!plik){ /*Obsluga bledu otwarcia pliku*/
		printf("Nie mozna otworzyc pliku do odczytu: %s.\n", nazwa_pliku);
		return;
	}

	while(fgets(wyniki[liczba_wynikow], sizeof(wyniki[0]), plik)){ /*Wczytanie kazdej linii z pliku do bufora*/
		if (liczba_wynikow == 0) {
			sscanf(wyniki[0], "%s %d", najwyzsze_imie_przed, &najwyzszy_wynik_przed); /*Odczytanie najwyzszego wyniku przed sortowaniem (pierwszy w pliku)*/
		}
		liczba_wynikow++; /*Zwiekszenie liczby wynikow o 1*/
		if(liczba_wynikow >= MAX_WYNIKOW){ /*Przerwanie, jesli osiagnieto maksymalna liczbe wynikow*/
			break;
		}
	}
	fclose(plik); /*Zamkniecie pliku po odczycie*/

	qsort(wyniki, liczba_wynikow, sizeof(wyniki[0]), porownaj_wyniki); /*Sortowanie wynikow w buforze*/
	
	if (liczba_wynikow > 0) {
		sscanf(wyniki[0], "%s %d", najwyzsze_imie_po, &najwyzszy_wynik_po); /*Odczytanie najwyzszego wyniku po sortowaniu (pierwszy w buforze)*/
	}

	/*Nadpisanie pliku posortowanymi wynikami*/
	plik = fopen(nazwa_pliku, "w"); /*Otworzenie pliku w trybie zapisu*/
	if(!plik){ /*Obsluga bledu otwarcia pliku*/
		printf("Nie mozna otworzyc pliku do zapisu: %s.\n", nazwa_pliku);
		return;
	}

	for(int i = 0; i < liczba_wynikow; i++){
		fprintf(plik, "%s", wyniki[i]); /*Zapisanie kazdego wyniku z bufora do pliku*/
	}
	fclose(plik); /*Zamkniecie pliku po zapisie*/
	
	if (najwyzszy_wynik_po != najwyzszy_wynik_przed) { 	/*Wyswietlenie komunikatu, jesli najwyzszy wynik zmienil sie*/
		printf(YELLOW "\nGratulacje! Osiagnieto nowy najwyzszy wynik!\nGracz:" RED " %s\n" YELLOW "Wynik:" RED " %d\n" YELLOW "Poprzedni najlepszy gracz: %s z wynikiem %d.\n" RESET, najwyzsze_imie_po, najwyzszy_wynik_po, najwyzsze_imie_przed, najwyzszy_wynik_przed);
	}
}