#include "wczytaj.h"
#include "plansza.h"
#include "struktury.h"
#include "wypisz.h"
#include "gra.h"
#include "wyniki.h"
#include "kolory.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
	Funkcja dodaj_ruch
	Dodaje nowy ruch do struktury Ruchy poprzez dynamiczne alokowanie pamieci.
	Parametry:
		ruchy - wskaznik na strukture Ruchy, przechowujaca historie ruchow
		x - wspolrzedna wiersza ruchu (indeksowana od 0)
		y - wspolrzedna kolumny ruchu (indeksowana od 0)
	Dzialanie:
		- Alokuje pamiec dla nowego ruchu i dodaje go do listy ruchow.
		- W przypadku bledu alokacji wypisuje komunikat i przerywa dzialanie.
*/

void dodaj_ruch(Ruchy* ruchy, int x, int y){
	/*Zwiekszenie rozmiaru tablicy wskaznikow*/
	ruchy->ruchy = realloc(ruchy->ruchy, (ruchy->liczba_ruchow + 1) * sizeof(Punkt*));
	if(ruchy->ruchy == NULL){ /*Obsluga bledu alokacji pamieci*/
		printf("Blad alokacji pamieci.\n");
		return;
	}

	/*Alokacja pamieci dla nowego ruchu*/
	ruchy->ruchy[ruchy->liczba_ruchow] = malloc(sizeof(Punkt));
	if(ruchy->ruchy[ruchy->liczba_ruchow] == NULL){ /*Obsluga bledu alokacji pamieci*/
		printf("Blad alokacji pamieci dla punktu.\n");
		return;
	}

	/*Inicjalizacja nowego ruchu*/
	ruchy->ruchy[ruchy->liczba_ruchow]->x = x; /*Ustawienie wspolrzednej x*/
	ruchy->ruchy[ruchy->liczba_ruchow]->y = y; /*Ustawienie wspolrzednej y*/

	ruchy->liczba_ruchow++; /*Zwiekszenie licznika ruchow*/
}

/*
	Funkcja inicjuj_ruchy
	Przetwarza liste ruchow, sprawdza ich poprawnosc i aktualizuje wynik gry.
	Parametry:
		plansza - wskaznik na strukture Plansza, zawierajaca informacje o planszy gry
		ruchy - wskaznik na strukture Ruchy, przechowujaca historie ruchow
	Dzialanie:
		- Sprawdza poprawnosc ruchow i aktualizuje licznik poprawnych ruchow.
		- Wykonuje ruchy na planszy i wypisuje ich szczegoly.
		- Sprawdza status gry (wygrana/przegrana/niezakonczona).
*/

void inicjuj_ruchy(Plansza* plansza, Ruchy* ruchy){
	int wiersze = plansza->wiersze; /*Liczba wierszy planszy*/
	int kolumny = plansza->kolumny; /*Liczba kolumn planszy*/
	int miny = plansza->miny; /*Liczba min na planszy*/
	
	int mnoznik = 0;  /*Inicjalizacja mnoznika wyniku*/
	if(wiersze == 9 && kolumny == 9 && miny == 10){
		mnoznik = 1;  /*Latwy poziom trudnosci*/
	} else if(wiersze == 16 && kolumny == 16 && miny == 40){
		mnoznik = 2;  /*Normalny poziom trudnosci*/
	} else if(wiersze == 16 && kolumny == 30 && miny == 99){
		mnoznik = 3; /*Trudny poziom trudnosci*/
	}
		
	for(int i = 0; i < ruchy->liczba_ruchow; i++){
		
		int x = ruchy->ruchy[i]->x; /*Pobranie wspolrzednej x ruchu*/
		int y = ruchy->ruchy[i]->y; /*Pobranie wspolrzednej y ruchu*/
		
		/*Sprawdzenie poprawnosci ruchu*/
		if(x < wiersze && y < kolumny && (plansza->pola[x][y].zakryte) && (!plansza->pola[x][y].mina)){
			ruchy->poprawne++; /*Zwiekszenie licznika poprawnych ruchow*/
		}
		
		printf("Ruch %d: (%d, %d)\n", i + 1, x, y); /*Wypisanie szczegolow ruchu*/
		
		odkryj(plansza, ruchy->ruchy[i]->x, ruchy->ruchy[i]->y, true); /*Odkrycie pola*/
	}
	
	/*Sprawdzenie czy gra zostala ukonczona*/
	if(!plansza->koniec_gry){
		printf("\nWynik gry: " YELLOW "? - Gra nie zostala ukonczona.\n" RESET); 
	} else {
		if(plansza->wygrana){
			printf("\nWynik gry: " GREEN "1 - Gra zakonczyla sie powodzeniem.\n" RESET); /*Wygrana*/
		} else {
			printf("\nWynik gry: " RED "0 - Gra zakonczyla sie niepowodzeniem.\n" RESET); /*Przegrana*/
		}
	}
	
	printf("Wynik gracza: " GREEN "%d\n" RESET, oblicz_wynik(plansza, mnoznik)); /*Wypisanie wyniku gracza*/
}

/*
	Funkcja wczytaj_plansze
	Wczytuje plansze gry oraz ruchy gracza z pliku tekstowego.
	Parametry:
		nazwa_pliku - nazwa pliku zawierajacego dane planszy i ruchow
		ruchy - wskaznik na strukture Ruchy, do ktorej zostana zapisane wczytane ruchy
	Zwraca:
		Wskaznik na strukture Plansza zainicjalizowana danymi z pliku lub NULL w przypadku bledu.
	Dzialanie:
		- Otwiera plik i wczytuje wymiary planszy, liczbe min oraz jej zawartosc.
		- Wczytuje kolejne ruchy gracza i zapisuje je w strukturze Ruchy.
		- Oblicza liczbe min sasiednich dla kazdego pola planszy.
*/

Plansza* wczytaj_plansze(const char* nazwa_pliku, Ruchy* ruchy){
    FILE* plik = fopen(nazwa_pliku, "r");
    if(!plik){ /*Obsluga bledu otwarcia pliku*/
        printf("Nie mozna otworzyc pliku: %s\n", nazwa_pliku);
        return NULL;
    }
	
	 /*Wczytanie wymiarow planszy i liczby min*/
	int wiersze, kolumny, liczba_min;
	if(fscanf(plik, "%d %d %d\n", &wiersze, &kolumny, &liczba_min) != 3){
		printf("Blad odczytu wymiarow planszy i liczby min.\n"); /*Blad wczytywania danych*/
		fclose(plik);
		return NULL;
	}

	printf("Wymiary planszy: %d x %d, liczba min: %d\n\n", wiersze, kolumny, liczba_min); /*Wypisanie szczegolow planszy*/

    /*Alokacja planszy*/
    Plansza* plansza = alokuj_plansze(wiersze, kolumny);
    if(!plansza){ /*Obsluga bledu alokacji planszy*/
        printf("Blad alokacji pamieci dla planszy.\n");
        fclose(plik);
        return NULL;
    }

    inicjuj_plansze(plansza, liczba_min); /*Inicjalizacja planszy*/
	
	char linia[256]; /*Bufor do wczytywania linii z pliku*/
	ruchy->liczba_ruchow = 0; /*Inicjalizacja licznika ruchow*/
    ruchy->ruchy = NULL; /*Inicjalizacja tablicy ruchow*/
	
	int licznik = 0; /*Licznik wierszy planszy*/
	char typ_ruchu; /*Typ wczytywanego ruchu*/
	int x, y; /*Wspolrzedne ruchu*/
	
	while(fgets(linia, sizeof(linia), plik) != NULL){ /*Czytanie kolejnych linii z pliku*/
		if(linia[0] == '0' || linia[0] == '1'){ /*Wczytywanie zawartosci planszy*/
			for(int i = 0; i < kolumny; i++){
				plansza->pola[licznik][i].mina = (linia[i] == '1'); /*Ustawienie min na planszy*/
			}
		} else if(linia[0] == 'r'){ /*Wczytywanie ruchow*/
			if(sscanf(linia, "%c %d %d\n", &typ_ruchu, &x, &y) == 3){
				dodaj_ruch(ruchy, x, y); /*Dodanie ruchu*/
			} else {
				printf("Blad odczytu ruchu."); /*Obsluga bledu wczytywania ruchu*/
			}	
		} else {
			printf("Niepoprawny format pliku."); /*Nieznany format linii*/
		}
		licznik ++; /*Zwiekszenie licznika wierszy planszy*/
	}

	fclose(plik); /*Zamkniecie pliku*/
	oblicz_sasiednie_miny(plansza); /*Obliczenie min sasiednich*/
	return plansza; /*Zwrocenie wskaznika na plansze*/
}