#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "struktury.h"
#include "kolory.h"
#include "makra.h"

#include "plansza.h"
#include "gra.h"
#include "wypisz.h"
#include "wyniki.h"
#include "wczytaj.h"

int main(){
    Plansza* plansza = wczytaj_plansze("przyklad");
    if (!plansza) {
        printf("Nie udało się wczytać planszy.\n");
        return 1;
    }

    wypisz_plansze_logiczna(plansza);

    // Zwolnienie pamięci
    zwolnij_plansze(plansza);
    return 0;
	/*
	bool koniec_gry = false;
	
	Plansza* plansza = NULL;
	int trudnosc, mnoznik, wiersze, kolumny, miny;
	
	wypisz_plik("logo", "");
	
	printf("Witaj w grze Minesweeper!\n");
	printf("Wybierz poziom trudnosci:\n");
	printf(GREEN "Latwy\t\t- 1\n" YELLOW "Normalny\t- 2\n" RED "Trudny\t\t- 3\n" CYAN "Wlasna plansza\t- 4\n" RESET);
	
	scanf("%d", &trudnosc);
	
	switch(trudnosc){
		case 1:
			printf("Wybrany poziom trudnosci to: Latwy.\n");
			mnoznik = 1;
			wiersze = 9;
			kolumny = 9;
			miny = 10;
			break;
		case 2:
			printf("Wybrany poziom trudnosci to: Normalny.\n");
			mnoznik = 2;
			wiersze = 16;
			kolumny = 16;
			miny = 40;
			break;
		case 3:
			printf("Wybrany poziom trudnosci to: Trudny.\n");
			mnoznik = 3;
			wiersze = 16;
			kolumny = 30;
			miny = 99;
			break;
		case 4:
			printf("Wybrany poziom trudnosci to: Wlasna plansza.\n");
			
			printf("Podaj liczbe wierszy planszy:\n");
			scanf("%d", &wiersze);
			if(wiersze < 0){
				printf("Niepoprawna liczba wierszy.");
				return 2;
			}
			
			printf("Podaj liczbe kolumn planszy:\n");
			scanf("%d", &kolumny);
			if(kolumny  < 0){
				printf("Niepoprawna liczba kolumn.");
				return 3;
			}
			
			printf("Podaj liczbe min na planszy:\n");
			scanf("%d", &miny);
			if(miny <= 0 || miny >= (wiersze * kolumny)){
				printf("Niepoprawna liczba min.");
				return 4;
			}
			
			printf("Tworze plansze %d x %d o liczbie min: %d.\n", wiersze, kolumny, miny);
			break;
		default:
			printf("Nieprawidlowy wybor poziomu trudnosci.\n");
			return 1;
	}
	
	plansza = alokuj_plansze(wiersze, kolumny);
	if(plansza != NULL){
		inicjuj_plansze(plansza, miny);
		printf("Plansza zostala poprawnie utworzona. Powodzenia!\n\n");
		while(!plansza->koniec_gry){
			if(trudnosc!=4){
				printf("\nAktualny wynik:" GREEN "%d\n\n" RESET, oblicz_wynik(plansza, mnoznik));
			} else {
				printf(GREEN "\nTryb wlasnej planszy, wynik nie jest liczony.\n\n" RESET);
			}
			wypisz_plansze_widoczna(plansza);
			odczytaj_polecenie(plansza);
			
			if(plansza->koniec_gry){
				if(plansza->wygrana){
					wypisz_plik("wygrana", GREEN);
				} else {
					wypisz_plik("przegrana", RED);
				}
			}
		}
		wypisz_plansze_logiczna(plansza);
		if(trudnosc!=4){
			printf("\nWynik koncowy: " GREEN "%d\n" RESET, plansza->wynik);
			printf("\nWpisz swoje imie:\n");
			char imie[16];
			scanf("%15s", imie);
			zapisz_wynik("wyniki", imie, plansza->wynik);
			posortuj_wyniki_w_pliku("wyniki");
			wypisz_plik("podium","");
			wypisz_top5("wyniki");
			printf("\nDziekuje za gre, %s. Gratuluje wyniku! :)\n", imie);
		}
		zwolnij_plansze(plansza);
	}
	return 0;
	*/
}