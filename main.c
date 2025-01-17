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

int main(){
	bool koniec_gry = false; /*Oznaczenie, ze gra nie skonczyla sie*/
	
	Plansza* plansza = NULL;
	int trudnosc, mnoznik, wiersze, kolumny, miny;
	
	wypisz_plik("logo", ""); /*Wypisanie logo gry*/
	
	printf("Witaj w grze Minesweeper!\n"); /*Ekran wyboru trudnosci*/
	printf("Wybierz poziom trudnosci:\n");
	printf(GREEN "Latwy\t\t- 1\n" YELLOW "Normalny\t- 2\n" RED "Trudny\t\t- 3\n" CYAN "Wlasna plansza\t- 4\n" RESET);
	
	scanf("%d", &trudnosc); /*Pobranie trudnosci wybranej przez gracza*/
	
	switch(trudnosc){
		case 1: /*Wprowadzenie danych potrzebnych do inicjalizacji planszy dla poziomu latwego*/
			printf("Wybrany poziom trudnosci to: Latwy.\n");
			mnoznik = 1;
			wiersze = 9;
			kolumny = 9;
			miny = 10;
			break;
		case 2: /*Wprowadzenie danych potrzebnych do inicjalizacji planszy dla poziomu normalnego*/
			printf("Wybrany poziom trudnosci to: Normalny.\n");
			mnoznik = 2;
			wiersze = 16;
			kolumny = 16;
			miny = 40;
			break;
		case 3: /*Wprowadzenie danych potrzebnych do inicjalizacji planszy dla poziomu trudnego*/
			printf("Wybrany poziom trudnosci to: Trudny.\n");
			mnoznik = 3;
			wiersze = 16;
			kolumny = 30;
			miny = 99;
			break;
		case 4: /*Tryb tworzenia wlasnej planszy*/
			printf("Wybrany poziom trudnosci to: Wlasna plansza.\n");
			
			printf("Podaj liczbe wierszy planszy:\n");
			scanf("%d", &wiersze); /*Pobranie liczby wierszy planszy*/
			if(wiersze < 0){ /*Obsluga bledu niepoprawnej liczby wierszy*/
				printf("Niepoprawna liczba wierszy.");
				return 2;
			}
			
			printf("Podaj liczbe kolumn planszy:\n");
			scanf("%d", &kolumny); /*Pobranie liczby kolumn planszy*/
			if(kolumny  < 0){ /*Obsluga bledu niepoprawnej liczby kolumn*/
				printf("Niepoprawna liczba kolumn.");
				return 3;
			}
			
			printf("Podaj liczbe min na planszy:\n");
			scanf("%d", &miny); /*Pobranie liczby min na planszy*/
			if(miny <= 0 || miny >= (wiersze * kolumny)){ /*Obsluga bledu niepoprawnej liczby min na planszy*/
				printf("Niepoprawna liczba min.");
				return 4;
			}
			
			printf("Tworze plansze %d x %d o liczbie min: %d.\n", wiersze, kolumny, miny); /*Komunikat potwierdzajacy tworzenie planszy o podanych parametrach*/
			break;
		default:
			printf("Nieprawidlowy wybor poziomu trudnosci.\n"); /*Obsluga bledu wyboru nieprawidlowego poziomu trudnosci*/
			return 1;
	}
	
	plansza = alokuj_plansze(wiersze, kolumny); /*Alokacja pamieci dla planszy o podanych wymiarach*/
	if(plansza != NULL){
		inicjuj_plansze(plansza, miny); /*Inicjalizacja planszy o odpowiedniej liczbie min*/
		printf("Plansza zostala poprawnie utworzona. Powodzenia!\n\n"); /*Komunikat potwierdzajacy poprawne utworzenie planszy*/
		while(!plansza->koniec_gry){ /*Petla gry, trwajaca dopoki gra sie nie zakonczy*/
			if(trudnosc!=4){ /*Wynik nie jest obliczany dla trybu wlasnej planszy*/
				printf("\nAktualny wynik:" GREEN "%d\n\n" RESET, oblicz_wynik(plansza, mnoznik)); /*Wyswietlenie aktualnego wyniku gracza*/
			} else {
				printf(GREEN "\nTryb wlasnej planszy, wynik nie jest liczony.\n\n" RESET); /*Komunikat o nieobliczaniu wyniku dla trybu wlasnej planszy*/
			}
			
			wypisz_plansze_widoczna(plansza); /*Wypisanie planszy widocznej dla gracza*/
			odczytaj_polecenie(plansza); /*Odczytanie polecenia gracza*/
			
			if(plansza->koniec_gry){ /*Sprawdzenie, czy gra zakonczyla sie*/
				if(plansza->wygrana){
					wypisz_plik("wygrana", GREEN); /*Wypisanie ekranu wygranej*/
				} else {
					wypisz_plik("przegrana", RED); /*Wypisanie ekranu przegranej*/
				}
			}
		}
		wypisz_plansze_logiczna(plansza); /*Wypisanie planszy logicznej po zakonczeniu gry*/
		if(trudnosc!=4){ /*Wynik nie jest obliczany dla trybu wlasnej planszy*/
			printf("\nWynik koncowy: " GREEN "%d\n" RESET, plansza->wynik); /*Wyswietlenie wyniku koncowego*/
			printf("\nWpisz swoje imie:\n");
			char imie[16];
			scanf("%15s", imie); /*Pobranie imienia gracza*/
			zapisz_wynik("wyniki", imie, plansza->wynik); /*Zapisanie wyniku w pliku tekstowym*/
			posortuj_wyniki_w_pliku("wyniki"); /*Sortowanie wynikow w pliku tekstowym*/
			wypisz_plik("podium",""); /*Wypisanie logo podium*/
			wypisz_top5("wyniki"); /*Wypisanie TOP 5 wynikow*/
			printf("\nDziekuje za gre, %s. Gratuluje wyniku! :)\n", imie); /*Podziekowanie za gre :)*/
		}
		zwolnij_plansze(plansza); /*Zwolnienie pamieci zaalokowanej dla planszy*/
	} else { /*Obsluga bledu, gdy nie udalo sie zaalokowac planszy*/
		printf("Nie udalo sie utworzyc planszy.\n");
		return 5;
	}
	return 0;
}