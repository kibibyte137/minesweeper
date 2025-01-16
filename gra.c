#include "gra.h"
#include "struktury.h"
#include "plansza.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*	Funkcja flaga
	Ustawia lub usuwa flage na polu planszy w zaleznosci od aktualnego stanu pola.
	Parametry:
		plansza - wskaznik na strukture Plansza, ktora zawiera informacje o calej planszy gry
		x - numer wiersza na planszy (indeksowany od 0)
		y - numer kolumny na planszy (indeksowany od 0)
	Dzialanie:
		- Jesli pole jest zakryte, funkcja ustawia flage na tym polu.
		- Jesli pole juz posiada flage, funkcja usuwa flage, przywracajac pole do stanu zakrytego.
*/

void flaga(Plansza* plansza, int x, int y){
	Pole* pole = &plansza->pola[x][y]; /*Pobranie wskaznika na konkretne pole na planszy*/
	if(pole->zakryte && !pole->flaga){ /*Sprawdzenie, czy pole jest zakryte i czy nie jest flaga*/
		pole->flaga = true; /*Ustawienie flagi na zakrytym polu*/
	} else if(pole->flaga){ /*Sprawdzenie, czy pole juz posiada flage*/
		pole->flaga = false; /*Usuniecie flagi*/
	} else {
		printf("Nie mozna ustawic flagi na polu o wspolrzednych x:%d y:%d.\n", x, y); /*Wypisanie komunikatu o bledzie*/
	}
}

/*	Funkcja inicjuj_pierwszy_ruch
	Inicjalizuje pierwszy ruch wykonany przez gracza w grze.
	Parametry:
		plansza - wskaznik na strukture Plansza, ktora zawiera informacje o calej planszy gry
		start_x - wspolrzedna x (numer wiersza) pierwszego ruchu wykonanego przez gracza (indeksowany od 0)
		start_y - wspolrzedna y (numer kolumny) pierwszego ruchu wykonanego przez gracza (indeksowany od 0)
	Dzialanie:
		Wywoluje funkcje rozmiesc_miny, oblicz_sasiednie_miny oraz oznacza, ze pierwszy ruch zostal wykonany.
*/ 

void inicjuj_pierwszy_ruch(Plansza* plansza, int start_x, int start_y){
	rozmiesc_miny(plansza, start_x, start_y); /*Rozmieszczenie min po zarejestrowaniu pierwszego ruchu z pominieciem jego wspolrzednych oraz wspolrzednych jego sasiedztwa*/
	oblicz_sasiednie_miny(plansza); /*Wywolanie funkcji oblicz_sasiednie_miny*/
	plansza->pierwszy_ruch = false; /*Oznaczenie, ze pierwszy ruch zostal wykonany*/
}

/*	Funkcja odkryj
	Odkrywa pole na planszy gry i wykonuje odpowiednie akcje w zaleznosci od stanu pola.
	Parametry:
		plansza - wskaznik na strukture Plansza, ktora zawiera informacje o calej planszy gry
		x - numer wiersza na planszy (indeksowany od 0)
		y - numer kolumny na planszy (indeksowany od 0)
	Dzialanie:
		- Sprawdza, czy gra jest zakonczona oraz czy wspolrzedne sa poprawne.
		- Inicjalizuje pierwszy ruch, jesli jeszcze go nie wykonano.
		- Sprawdza, czy pole jest juz odkryte lub oznaczone flaga, i obsluguje te przypadki.
		- Odkrywa pole i, jesli to mina, konczy gre.
		- Jezeli pole nie ma sasiednich min, odkrywa sasiednie pola rekurencyjnie.
		- Sprawdza, czy wszystkie pola niebedace minami zostaly odkryte, aby oznaczyc gre jako wygrana.
*/

void odkryj(Plansza* plansza, int x, int y){
	if(plansza->koniec_gry){ /*Sprawdzenie, czy gra zostala zakonczona*/
		printf("Gra zostala juz zakonczona.\n"); /*Wypisanie komunikatu*/
		return; /*Zakonczenie dzialania funkcji*/
	}
	
	if(x < 0 || x >= plansza->wiersze || y < 0 || y >= plansza->kolumny){ /*Sprawdzenie, czy podane wspolrzedne sa w granicach planszy*/
		printf("Wspolrzedne (%d, %d) sa poza zakresem planszy.\n", x, y); /*Wypisanie komunikatu o bledzie*/
		return; /*Zakonczenie dzialania funkcji*/
	}
	
	Pole* pole = &plansza->pola[x][y]; /*Pobranie wskaznika na wybrane pole*/
	if(plansza->pierwszy_ruch){ /*Zainicjowanie gry, jesli to pierwszy ruch gracza*/
		inicjuj_pierwszy_ruch(plansza, x, y); 
	}
	
	if (!pole->zakryte){ /*Sprawdzenie, czy pole jest odkryte*/
		printf("Pole (%d, %d) jest juz odkryte.\n", x, y); /*Wypisanie komunikatu*/
		return; /*Zakonczenie dzialania funkcji*/
	}
	
	if(pole->flaga){ /*Sprawdzenie, czy pole posiada flage*/
		printf("Wybrane pole posiada flage. Czy chcesz kontynuowac? T/N\n"); /*Wypisanie komunikatu o wyborze*/
		char kontynuacja;
		do { /*Petla, ktora nie zakonczy sie, dopoki uzytkownik nie poda poprawnej odpowiedzi*/
			scanf(" %c", &kontynuacja); /*Pobranie decyzji uzytkownika*/
			if(kontynuacja == 'N'){ /*Pozostawienie pola zakrytym, gdy gracz wybierze 'N'*/
				printf("Pole (%d, %d) nie zostalo odkryte.\n", x, y);
				return; /*Zakonczenie dzialania funkcji*/
			} else if(kontynuacja == 'T'){
				pole->flaga = false; /*Usuniecie flagi oraz wykonanie dalszych instrukcji funkcji, gdy gracz wybierze 'T'*/
				break;
			} else {
				printf("Nieprawidlowy wybor. Sprobuj ponownie: T/N.\n"); /*Wypisanie komunikatu w przypadku nieprawidlowego wyboru*/
			}
		} while(1);
	}
	
	pole->zakryte = false; /*Odkrycie pola*/
	
	if(pole->mina){ /*Sprawdzenie, czy pole zawiera mine*/
		plansza->koniec_gry = true; /*Zakonczenie gry*/
		return; /*Zakonczenie dzialania funkcji*/
	}
	
	plansza->odkryte_pola++; /*Zwiekszenie licznika odkrytych pol o 1*/
	
	if(pole->sasiednie_miny == 0){ /*Sprawdzenie, czy pole nie ma sasiednich min*/
		/*Rekurencyjne odkrycie sasiednich pol, jesli pole nie ma sasiednich min*/
		int kierunki[8][2] = { /*Tablica przesuniec dla 8 kierunkow*/
			{-1, -1},	{-1, 0},	{-1, 1},	/*Gorne sasiedzwtow*/
			{ 0, -1},				{ 0, 1},	/*Lewe i prawe sasiedztwo*/
			{ 1, -1},	{ 1, 0},	{ 1, 1}		/*Dolne sasiedztwo*/
		};
		
		for(int k = 0; k < 8; k++){
			int nx = x + kierunki[k][0]; /*Nowa wspolrzedna x*/
			int ny = y + kierunki[k][1]; /*Nowa wspolrzedna y*/
				
			if(nx >= 0 && nx < plansza->wiersze && ny >= 0 && ny < plansza->kolumny){ /*Sprawdzenie, czy nowe sasiednie znajduja sie w granicach planszy*/
				if(plansza->pola[nx][ny].zakryte){
					odkryj(plansza, nx, ny); /*Odkrycie pola, jesli bylo zakryte*/
				}
			}
		}
	}
	
	if(plansza->odkryte_pola == plansza->wiersze * plansza->kolumny - plansza->miny){ /*Sprawdzenie, czy wszystkie pola niebedace minami zostaly odkryte*/
		plansza->koniec_gry = true; /*Zakonczenie gry*/
		plansza->wygrana = true; /*Oznaczenie gry jako wygranej*/
	}
}

/*	Funkcja odczytaj_polecenie
	Przetwarza wejscie od uzytkownika i wykonuje odpowiednie akcje w grze.
	Parametry:
		plansza - wskaznik na strukture Plansza, ktora zawiera informacje o calej planszy gry
	Dzialanie:
		- Odczytuje komende uzytkownika i interpretuje ja jako jedna z mozliwych akcji:
			- "r" - odkrycie pola (wywolanie funkcji odkryj).
			- "f" - ustawienie lub usuniecie flagi (wywolanie funkcji flaga).
			- "k" - zakonczenie gry.
		- Informuje o blednym poleceniu w przypadku nieznanego wejscia.
*/

void odczytaj_polecenie(Plansza* plansza){
	char komenda[8];
	int x, y;
	
	printf("\nPodaj polecenie:\n");
	scanf("%s", komenda); /*Pobranie polecenia uzytkownika*/
	
	if(strcmp(komenda, "r") == 0){ /*Komenda "r" - odczytanie wspolrzednych i odkrycie pola*/
		scanf("%d %d", &x, &y);
		odkryj(plansza, x, y);
	} else if(strcmp(komenda, "f") == 0){ /*Komenda "f" - odczytanie wspolrzednych i ustawienie/usuniecie flagi*/
		scanf("%d %d", &x, &y);
		flaga(plansza, x, y);
	} else if(strcmp(komenda, "k") == 0){ /*Komenda "k" - zakonczenie gry*/
		plansza->koniec_gry = true;
	} else {
		printf("Nieznane polecenie: %s\nDostepne polecenia:\n", komenda); /*Wypisanie komunikatu w przypadku nieznanej komendy oraz podanie prawidlowej skladni*/
		printf("r [x] [y]\t- Odslon pole (x, y)\n");
		printf("f [x] [y]\t- Ustaw lub usun flage na polu (x, y)\n");
		printf("k\t\t- Zakoncz gre\n");
	}
}