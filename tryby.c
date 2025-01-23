#include "tryby.h"
#include "plansza.h"
#include "gra.h"
#include "wypisz.h"
#include "wyniki.h"
#include "wczytaj.h"

#include "struktury.h"
#include "kolory.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*Tryb klasycznej gry*/
void klasyczna_gra(){
	bool koniec_gry = false; /*Oznaczenie, ze gra nie skonczyla sie*/
	
	Plansza* plansza = NULL;
	int trudnosc, mnoznik, wiersze, kolumny, miny;
	
	wypisz_plik("logo", ""); /*Wypisanie logo gry*/
	
	printf("Witaj w grze Minesweeper!\n"); /*Ekran wyboru trudnosci*/
	printf("Wybierz poziom trudnosci:\n");
	printf(GREEN "Latwy\t\t- 1\n" YELLOW "Normalny\t- 2\n" RED "Trudny\t\t- 3\n" CYAN "Wlasna plansza\t- 4\n" RESET);
	
	if(scanf("%d", &trudnosc) != 1){ /*Obsluga bledu wczytywania trudnosci*/
		printf("Blad. Nie udalo sie odczytac poziomu trudnosci.\n");
		return;
	}
	
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
			if(scanf("%d", &wiersze) != 1 || wiersze <= 0){ /*Obsluga bledu niepoprawnej liczby wierszy*/
				printf("Niepoprawna liczba wierszy. Wprowadz wartosc wieksza od 0.\n");
				return;
			}
			
			printf("Podaj liczbe kolumn planszy:\n");
			if(scanf("%d", &kolumny) != 1 || kolumny <= 0){ /*Obsluga bledu niepoprawnej liczby kolumn*/
				printf("Niepoprawna liczba kolumn. Wprowadz wartosc wieksza od 0.\n");
				return;
			}
			
			printf("Podaj liczbe min na planszy:\n");
			if(scanf("%d", &miny) != 1 || miny <= 0 || miny >= (wiersze * kolumny)){ /*Obsluga bledu niepoprawnej liczby min na planszy*/
				printf("Niepoprawna liczba min. Wprowadz wartosc wieksza od 0 i mniejsza niz liczba pol.\n");
				return;
			}
			
			printf("Tworze plansze %d x %d o liczbie min: %d.\n", wiersze, kolumny, miny); /*Komunikat potwierdzajacy tworzenie planszy o podanych parametrach*/
			break;
		default:
			printf("Nieprawidlowy wybor poziomu trudnosci. Wybierz 1, 2, 3 lub 4.\n"); /*Obsluga bledu wyboru nieprawidlowego poziomu trudnosci*/
			return;
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
					oblicz_wynik(plansza, mnoznik);
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
			if(scanf("%15s", imie) != 1){ /*Obsluga bledu wczytywania imienia*/
				printf("Blad. Nie udalo sie wczytac imienia.\n");
			} else {
				zapisz_wynik("wyniki", imie, plansza->wynik); /*Zapisanie wyniku w pliku tekstowym*/
				posortuj_wyniki_w_pliku("wyniki"); /*Sortowanie wynikow w pliku tekstowym*/
				wypisz_plik("podium",""); /*Wypisanie logo podium*/
				wypisz_top5("wyniki"); /*Wypisanie TOP 5 wynikow*/
				printf("\nDziekuje za gre, %s. Gratuluje wyniku! :)\n", imie); /*Podziekowanie za gre :)*/
			}
		}
		zwolnij_plansze(plansza); /*Zwolnienie pamieci zaalokowanej dla planszy*/
	} else { /*Obsluga bledu, gdy nie udalo sie zaalokowac planszy*/
		printf("Nie udalo sie utworzyc planszy.\n");
		return;
	}
}

/*Tryb wczytywania planszy z pliku*/
void gra_z_pliku(const char *nazwa_pliku){	
	Ruchy ruchy; /*Inicjalizacja struktury Ruchy*/
	ruchy.liczba_ruchow = 0; /*Inicjalizacja liczby ruchow*/
	ruchy.poprawne = 0; /*Inicjalizacja liczby poprawnych ruchow*/
	ruchy.ruchy = NULL; /*Inicjalizacja tablicy ruchow*/

    Plansza* plansza = wczytaj_plansze(nazwa_pliku, &ruchy); /*Wczytanie planszy z pliku*/
    if(!plansza){ /*Obsluga bledu wczytywania planszy*/
        printf("Nie udalo sie wczytac planszy. Upewnij sie ze plik istnieje i ma poprawny format.\n");
        return;
    }
	
	printf("Wyglad planszy:\n");
	wypisz_plansze_logiczna(plansza); /*Wypisanie planszy logicznej*/
	printf("\nRuchy:\n");
	inicjuj_ruchy(plansza, &ruchy); /*Symulacja przebiegu gry*/
	printf("Liczba poprawnych ruchow: " CYAN "%d\n" RESET, ruchy.poprawne); /*Wyswietlenie liczby poprawnych ruchow*/
	zwolnij_plansze(plansza); /*Zwolnienie pamieci planszy*/
	if(ruchy.ruchy != NULL){ /*Sprawdzenie czy tablica ruchow istnieje*/
		for (int i = 0; i < ruchy.liczba_ruchow; i++){
			free(ruchy.ruchy[i]); /*Zwolnienie pamieci dla kazdego ruchu*/
		}
		free(ruchy.ruchy); /*Zwolnienie tablicy ruchow*/
		ruchy.ruchy = NULL; /*Ustawienie wskaznika na NULL*/
	}
}

/*Podreczna pomoc*/
void pomoc(){
	wypisz_plik("pomoc", "");
	printf("\nZasady gry:\n");
	printf("Gra „Minesweeper” polega na odkrywaniu poszczegolnych pol planszy w taki sposob,\n");
	printf("by nie natrafic na mine. Na kazdym z odkrytych pol napisana jest liczba min,\n");
	printf("ktore bezposrednio sie z nim stykaja (od 0 do 8). Zadaniem gracza jest wydedukowanie\n");
	printf("polozenia min na tej podstawie. Gracz ma mozliwosc umieszczenia flagi na wybranym polu,\n");
	printf("dzieki czemu jest ono zabezpieczone przed przypadkowym odkryciem.\n\n");
	printf("Gra konczy sie " GREEN "wygrana" RESET ", kiedy odsloniete zostana wszystkie pola nieposiadajace min.\n");
	printf("Gra konczy sie " RED "przegrana" RESET ", kiedy zostanie odsloniete pole z mina.\n");
	printf("\nDostepne opcje uruchomienia:\n");
	printf(YELLOW "./minesweeper\t\t\t- Tryb klasycznej gry\n");
	printf("./minesweeper -f [nazwa_pliku]\t- Tryb wczytania planszy z pliku o podanej nazwie\n");
	printf("./minesweeper -t\t\t- Tryb testowania generacji plansz\n");
	printf("./minesweeper -h\t\t- Wyswietlenie podrecznej pomocy\n" RESET);
	printf("\nDostepne polecenia (w trybie klasycznej gry):\n");
	printf(CYAN "r [x] [y]\t- Odslon pole (x, y)\n");
	printf("f [x] [y]\t- Ustaw lub usun flage na polu (x, y)\n");
	printf("k\t\t- Zakoncz gre\n" RESET);
}

/*Test generacji planszy*/
void test(int trudnosc){
	int wiersze, kolumny, miny;
	switch(trudnosc){
		case 1: /*Latwy poziom trudnosci*/
			wiersze = 9;
			kolumny = 9;
			miny = 10;
			break;
		case 2: /*Normalny poziom trudnosci*/
			wiersze = 16;
			kolumny = 16;
			miny = 40;
			break;
		case 3: /*Trudny poziom trudnosci*/
			wiersze = 16;
			kolumny = 30;
			miny = 99;
			break;
		default:
			wiersze = 0;
			kolumny = 0;
			miny = 0;
			break;
	}
	Plansza* plansza = alokuj_plansze(wiersze, kolumny); /*Alokacja pamieci dla planszy*/
	if(plansza != NULL){
		inicjuj_plansze(plansza, miny); /*Inicjalizacja planszy*/
		rozmiesc_miny(plansza, 0, 0); /*Rozmieszczenie min, ustawienie punktu (0, 0) jako poczatkowego ruchu*/
		oblicz_sasiednie_miny(plansza); /*Obliczenie sasiednich min*/
		wypisz_plansze_logiczna(plansza); /*Wypisanie planszy logicznej*/
		zwolnij_plansze(plansza); /*Zwolnienie pamieci planszy*/
	} else {
		printf("Blad alokacji pamieci dla planszy."); /*Obsluga bledu alokacji pamieci dla planszy*/
		return;
	}
}
	