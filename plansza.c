#include "plansza.h"
#include "struktury.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*	Funkcja alokuj_plansze
	Alokuje pamiec dla struktury Plansza oraz dwuwymiarowej tabilcy pol.
	Parametry:
		wiersze - liczba wierszy planszy
		kolumny - liczba kolumn planszy
	Zwraca:
		Wskaznik do zaalokowanej struktury Plansza lub NULL w przypadku bledu.
*/

Plansza* alokuj_plansze(int wiersze, int kolumny){
	Plansza* plansza = malloc(sizeof(Plansza)); /*Alokacja pamieci dla struktury Plansza*/
	if(!plansza){ /*Obsluga bledu alokacji - wypisanie komunikatu oraz zwrocenie NULL*/
		printf("Blad alokacji pamieci dla struktury Plansza.\n");
		return NULL;
	}
	
	/*Ustawienie podstawowych parametrow planszy*/
	plansza->wiersze = wiersze;	/*Ustawienie liczby wierszy planszy*/
	plansza->kolumny = kolumny;	/*Ustawienie liczby kolumn planszy*/
	plansza->miny = 0;			/*Liczba min zostanie ustawiona w inicjalizacji*/
	
	plansza->pola = malloc(wiersze * sizeof(Pole*)); /*Alokacja pamieci dla dwuwymiarowej tablicy wskaznikow do pol*/
	if(!plansza->pola){ /*Obsluga bledu alokacji - wypisanie komunikatu, zwolnienie wczesniej zaalokowanej pamieci oraz zwrocenie NULL*/
		printf("Blad alokacji pamieci dla pol planszy.\n");
		free(plansza);
		return NULL;
	}
	
	for(int i = 0; i < wiersze; i++){ /*Alokacja pamieci dla kazdego wiersza pol*/
		plansza->pola[i] = malloc(kolumny * sizeof(Pole));
		if(!plansza->pola[i]){ /*Obsluga bledu alokacji - wypisanie komunikatu, zwolnienie wczesniej zaalokowanej pamieci oraz zwrocenie NULL*/
			printf("Blad alokacji pamieci dla wiersza %d.\n", i);
			for(int j = 0; j < i; j++){
				free(plansza->pola[j]);
			}
			free(plansza->pola);
			free(plansza);
			return NULL;
		}
	}
	return plansza;
}


/*	Funkcja inicjuj_plansze
	Inicjalizuje wartosci pol planszy oraz ustawia liczbe min.
	Parametry:
		plansza - wskaznik do struktury Plansza z zaalokowana pamiecia
		liczba_min - liczba min do ustawienia na planszy
	Dzialanie:
		Ustawia wszystkie pola jako zakryte, bez min, bez flag, z zerowa liczba sasiadujacych min.
		Ustawia liczbe min w strukturze Plansza.
*/

void inicjuj_plansze(Plansza* plansza, int liczba_min){
	if(!plansza){ /*Sprawdzenie, czy wskaznik plansza nie jest NULL*/
		printf("Nie mozna zainicjalizowac planszy: wskaznik NULL.\n");
		return;
	}
	
	/*Inicjalizacja poszczegolnych elementow planszy*/
	plansza->miny = liczba_min; 	/*Ustawienie liczby min na planszy*/
	plansza->koniec_gry = false;	/*Gra trwa*/
	plansza->wygrana = false;		/*Gra nie jest wygrana*/
	plansza->pierwszy_ruch = true;	/*Oznaczenie pierwszego ruchu w grze*/
	plansza->odkryte_pola = 0;		/*Poczatkowo wszystkie pola sa zakryte*/
	plansza->wynik = 0;				/*Ustawienie poczatkowego wyniku na 0*/
	
	/*Inicjalizacja kazdego pola planszy*/
	for(int i = 0; i < plansza->wiersze; i++){
		for(int j = 0; j < plansza->kolumny; j++){
			plansza->pola[i][j] = (Pole){
				.zakryte = true,			/*Pole poczatkowo jest zakryte*/
				.mina = false,				/*Pole poczatkowo nie zawiera miny*/
				.flaga = false,				/*Pole poczatkowo nie posiada flagi*/
				.sasiednie_miny = 0,		/*Brak sasiadujacych min*/
				.wspolrzedne = {i, j}		/*Wspolrzedne pola*/
			};
		}
	}
}


/*	Funkcja zwolnij_plansze
	Zwalnia pamiec zaalokowana dla planszy i jej pol.
	Parametry:
		plansza - wskaznik do struktury Plansza
	Dzialanie:
		Zwalnia pamiec dla wszystkich wierszy pol, samej tablicy wskaznikow oraz struktury Plansza.
*/

void zwolnij_plansze(Plansza* plansza){
	if(!plansza) return; /*Przerwij, jesli pamiec dla planszy nie zostala zaalokowana*/
	for(int i = 0; i < plansza->wiersze; i++){ /*Zwolnienie pamieci dla kazdego wiersza*/
		free(plansza->pola[i]);
	}
	free(plansza->pola); /*Zwolnienie pamieci dla tablicy wskaznikow*/
	free(plansza); /*Zwolnienie pamieci dla struktury Plansza*/
}


/*	Funkcja rozmiesc_miny
	Rozmieszcza miny na planszy w losowych miejscach, z zachowaniem zasad:
	- Min nie mozna umiescic na polach odkrytych.
	- Liczba rozmieszczonych min odpowiada zadanej liczbie min w planszy.
	- Miny nie zostana umieszone na pierwszym polu odkrytym przez gracza, ani w jego sasiedztwie.
	Parametry:
		plansza - wskaznik do struktury Plansza
		start_x - wspolrzedna x (numer wiersza) pierwszego ruchu wykonanego przez gracza (indeksowana od 0)
		start_y - wspolrzedna y (numer kolumny) pierwszego ruchu wykonanego przez gracza (indeksowana od 0)
	Dzialanie:
		Ustawia 'plansza->miny' min w losowych miejscach na planszy.
*/

void rozmiesc_miny(Plansza* plansza, int start_x, int start_y){
	if(!plansza){ /*Sprawdzenie, czy wskaznik plansza nie jest NULL*/
		printf("Nie mozna rozmiescic min: wskaznik NULL\n");
		return;
	}
	
	srand(time(NULL)); /*Inicjalizacja generatora liczb losowych*/
	
	int miny_do_umieszczenia = plansza->miny; /*Ustawienie liczby min do umieszczenia*/
	int wiersze = plansza->wiersze; /*Ustawienie liczby wierszy planszy*/
	int kolumny = plansza->kolumny; /*Ustawienie liczby kolumn planszy*/
	
	while(miny_do_umieszczenia > 0){
		int losowy_wiersz = rand() % wiersze; /*Losowy wiersz*/
		int losowa_kolumna = rand() % kolumny; /*Losowa kolumna*/
		
		if ((losowy_wiersz >= start_x - 1 && losowy_wiersz <= start_x + 1) && (losowa_kolumna >= start_y - 1 && losowa_kolumna <= start_y + 1)) {
			continue; /*Rozmieszczenie min z pominieciem pierwszego pola odkrytego przez gracza oraz jego sasiedztwa*/
		}
		
		Pole* pole = &plansza->pola[losowy_wiersz][losowa_kolumna];
		
		if(pole->zakryte && !pole->mina){ /*Mine umiescic mozna tylko na polu zakrytym, ktore nie ma jeszcze miny*/
			pole->mina = true; /*Ustawienie miny*/
			miny_do_umieszczenia--; /*Zmniejszenie liczby min do umieszczenia o 1*/
		}
	}
}

/*	Funkcja oblicz_sasiednie_miny
	Oblicza liczbe min sasiadujacych z kazdym polem na planszy i aktualizuje wartosc 'sasiednie_miny' dla kazdego pola.
	Parametry:
		plansza - wskaznik do struktury Plansza zawierajacej pola do analizy
	Dzialanie:
		1. Iteruje po kazdym polu planszy.
		2. Resetuje wartosc 'sasiednie_miny' dla biezacego pola do 0.
		3. Sprawdza wszystkie 8 sasiednich pol wzgledem biezacego pola.
		4. Dla kazdego sasiedniego pola:
			- Sprawdza, czy pole znajduje sie w granicach planszy.
			- Jesli pole zawiera mine, zwieksza wartosc 'sasiednie_miny' biezacego pola o 1.
	Uwagi:
		- Funkcja zaklada, ze plansza zostala wczesniej zainicjalizowana i zawiera informacje o minach.
		- Resetowanie 'sasiednie_miny' przed obliczeniem jest konieczne, by uniknac kumulacji wczesniejszych wartosci.
*/

void oblicz_sasiednie_miny(Plansza* plansza){
	int kierunki[8][2] = { /*Tablica przesuniec dla 8 kierunkow*/
		{-1, -1},	{-1, 0},	{-1, 1},	/*Gorne sasiedzwtow*/
		{ 0, -1},				{ 0, 1},	/*Lewe i prawe sasiedztwo*/
		{ 1, -1},	{ 1, 0},	{ 1, 1}		/*Dolne sasiedztwo*/
	};
	
	/*Iteracja po kazdym polu planszy*/
	for(int x = 0; x < plansza->wiersze; x++){
		for(int y = 0; y < plansza->kolumny; y++){
			Pole* pole = &plansza->pola[x][y];
			pole->sasiednie_miny = 0; /*Reset liczby sasiednich min*/
			
			/*Sprawdzenie wszystkich sasiednich pol*/
			for(int k = 0; k < 8; k++){
				int nx = x + kierunki[k][0]; /*Nowa wspolrzedna x*/
				int ny = y + kierunki[k][1]; /*Nowa wspolrzedna y*/
				
				if(nx >= 0 && nx < plansza->wiersze && ny >= 0 && ny < plansza->kolumny){ /*Sprawdzenie, czy sasiednie pole znajduje sie w granicach planszy*/
					if(plansza->pola[nx][ny].mina){ /*Sprawdzenie, czy sasiednie pole zawiera mine*/
						pole->sasiednie_miny++; /*Jesli sasiednie pole zawiera mine, nastepuje zwiekszenie liczby sasiednich min biezacego pola*/
					}
				}
			}
		}
	}
}