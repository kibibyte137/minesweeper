#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*Symbole graficznej reprezentacji planszy*/
#define PUSTE		'.'  		/*Odkryte puste pole*/
#define ZAKRYTE		'#'			/*Zakryte pole*/
#define MINA		'X'			/*Odkryte pole z miną*/
#define FLAGA		'P'			/*Zakryte pole z flagą*/
#define LICZBA(x)	('0' + (x))	/*Odkryte pole z liczbą (1-8)*/

/*Warunki sprawdzania stanu pola*/
#define CZY_PUSTE(pole)		((pole).sasiednie_miny == 0 && (pole).mina == 0)
#define CZY_ZAKRYTE(pole)	((pole).stan == ZAKRYTE)
#define CZY_FLAGA(pole)		((pole).stan == FLAGA)
#define CZY_MINA(pole)		((pole).mina == 1)
#define CZY_LICZBA(pole)	((pole).sasiednie_miny > 0 && (pole).mina == 0)
#define CZY_ODKRYTE(pole)	((pole).stan != ZAKRYTE && (pole).stan != FLAGA)

/*Symbol pola na podstawie jego stanu*/
#define SYMBOL_POLA(pole) \
	(CZY_ZAKRYTE(pole) ? ZAKRYTE : \
	CZY_FLAGA(pole) ? FLAGA : \
	CZY_MINA(pole) ? MINA : \
	CZY_LICZBA(pole) ? LICZBA((pole).sasiednie_miny) : PUSTE)

typedef struct{	/*Struktura dla wspolrzednych*/
	int x;		/*Numer wiersza*/
	int y;		/*Numer kolumny*/
} Punkt;

typedef struct{				/*Struktura dla pola*/
	int mina;				/*1, jesli pole zawiera mine, 0 w przeciwnym wypadku*/
	int sasiednie_miny;		/*Liczba sasiadujacych min*/
	char stan;				/*Stan pola: ZAKRYTE, ODKRYTE, FLAGA*/
	Punkt wspolrzedne;		/*Współrzedne pola (wiersz, kolumna)*/
} Pole;

typedef struct{		/*Struktura dla planszy*/
	int wiersze;	/*Liczba wierszy planszy*/
	int kolumny;	/*Liczba kolumn planszy*/
	int miny;		/*Liczba min na planszy*/
	Pole** pola;	/*Dwuwymiarowa tablica struktur Pole*/
} Plansza; 

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
		Ustawia wszystkie pola jako zakryte, bez min, z zerowa liczba sasiadujacych min.
		Ustawia liczbe min w strukturze Plansza.
*/

void inicjuj_plansze(Plansza* plansza, int liczba_min){
	if(!plansza){ /*Sprawdzenie, czy wskaznik plansza nie jest NULL*/
		printf("Nie mozna zainicjalizowac planszy: wskaznik NULL.\n");
		return;
	}
	
	plansza->miny = liczba_min; /*Ustawienie liczby min na planszy*/
	
	/*Inicjalizacja kazdego pola planszy*/
	for(int i = 0; i < plansza->wiersze; i++){
		for(int j = 0; j < plansza->kolumny; j++){
			plansza->pola[i][j] = (Pole){
				.mina = 0,				/*Pole poczatkowo nie zawiera miny*/
				.sasiednie_miny = 0,	/*Brak sasiadujacych min*/
				.stan = ZAKRYTE,		/*Pole jest zakryte*/
				.wspolrzedne = {i, j}	/*Wspolrzedne pola*/
			};
		}
	}
	
	/*Funkcja rozmiesc_miny (logika rozmieszczania min planowana jest jego osobna funkcja)*/
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
	Parametry:
		plansza - wskaznik do struktury Plansza
	Dzialanie:
		Ustawia 'plansza->miny' min w losowych miejscach na planszy.
*/

void rozmiesc_miny(Plansza* plansza){
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
		
		Pole* pole = &plansza->pola[losowy_wiersz][losowa_kolumna];
		
		if(CZY_ZAKRYTE(*pole) && !pole->mina){ /*Mine umiescic mozna tylko na polu zakrytym, ktore nie ma jeszcze miny*/
			pole->mina = 1; /*Ustawienie miny*/
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
	if(CZY_ZAKRYTE(*pole)){ /*Sprawdzenie, czy pole jest zakryte*/
		pole->stan = FLAGA; /*Ustawienie flagi na zakrytym polu*/
	} else if(CZY_FLAGA(*pole)){ /*Sprawdzenie, czy pole juz posiada flage*/
		pole->stan = ZAKRYTE; /*Usuniecie flagi, ustawienie stanu pola na ZAKRYTE*/
	} else {
		printf("Nie mozna ustawic flagi na polu o wspolrzednych x:%d y:%d.", x, y); /*Wypisanie komunikatu o bledzie*/
	}
}

/*	Funkcja wypisz_plik
	Wczytuje i wypisuje zawartosc pliku.
	Parametry:
		nazwa_pliku - wskaznik do ciagu znakow zawierajacy nazwe pliku
	Dzialanie:
		Wypisuje tekst znajdujacy sie w pliku o podanej nazwie linia po linii.
*/

void wypisz_plik(const char* nazwa_pliku){
	FILE* plik = fopen(nazwa_pliku, "r"); /*Otworzenie pliku w trybie odczytu*/
	if(!plik){ /*Obsluga bledu, jesli plik nie istnieje - wypisanie komunikatu i przerwanie funkcji*/
		printf("Nie mozna otworzyc pliku: %s\n", nazwa_pliku);
		return;
	}
	char linia[256]; /*Bufor na linie tekstu (maksymalnie 255 znakow)*/
	while(fgets(linia, sizeof(linia), plik)){ /*Czytanie linii, dopoki sa w pliku*/
		printf("%s", linia); /*Wypisanie linii na ekran*/
	}
	fclose(plik); /*Zamkniecie pliku*/
}

/*
void wypisz_plansze_widoczna(Plansza* plansza){
	for (int i = 0; i < plansza->wiersze; i++){
		for (int j = 0; j < plansza->kolumny; j++){
			printf("%c ", plansza->plansza_widoczna[i][j]);
		}
		printf("\n");
	}
}


void wypisz_plansze_logiczna(Plansza* plansza){
	for (int i = 0; i < plansza->wiersze; i++){
		for (int j = 0; j < plansza->kolumny; j++){
			printf("%c ", plansza->plansza_logiczna[i][j]);
		}
		printf("\n");
	}
}
*/

int main(){
	Plansza* plansza = NULL;
	int trudnosc, mnoznik, wiersze, kolumny, miny;
	
	wypisz_plik("logo");
	printf("Witaj w grze Minesweeper!\n");
	printf("Wybierz poziom trudnosci:\n");
	printf("Latwy\t\t- 1\nNormalny\t- 2\nTrudny\t\t- 3\nWlasna plansza\t- 4\n");
	
	scanf("%d", &trudnosc);
	
	switch(trudnosc){
		case 1:
			printf("Wybrany poziom trudnosci to: Latwy.\n");
			mnoznik = 1;
			plansza = alokuj_plansze(9, 9);
			if(plansza != NULL){
				inicjuj_plansze(plansza, 10);
				rozmiesc_miny(plansza);
				oblicz_sasiednie_miny(plansza);
			}
			break;
		case 2:
			printf("Wybrany poziom trudnosci to: Normalny.\n");
			mnoznik = 2;
			plansza = alokuj_plansze(16, 16);
			if(plansza != NULL){
				inicjuj_plansze(plansza, 40);
				rozmiesc_miny(plansza);
				oblicz_sasiednie_miny(plansza);
			}
			break;
		case 3:
			printf("Wybrany poziom trudnosci to: Trudny.\n");
			mnoznik = 3;
			plansza = alokuj_plansze(16, 30);
			if(plansza != NULL){
				inicjuj_plansze(plansza, 99);
				rozmiesc_miny(plansza);
				oblicz_sasiednie_miny(plansza);
			}
			break;
		case 4:
			int wiersze, kolumny, miny;
			printf("Wybrany poziom trudnosci to: Wlasna plansza.\n");
			printf("Podaj liczbe wierszy planszy:\n");
			scanf("%d", &wiersze);
			printf("Podaj liczbe kolumn planszy:\n");
			scanf("%d", &kolumny);
			printf("Podaj liczbe min na planszy:\n");
			scanf("%d", &miny);
			printf("Tworze plansze %d x %d o liczbie min: %d.\n", wiersze, kolumny, miny);
			plansza = alokuj_plansze(wiersze, kolumny);
			if(plansza != NULL){
				inicjuj_plansze(plansza, miny);
				rozmiesc_miny(plansza);
				oblicz_sasiednie_miny(plansza);
			}
			break;
		default:
			printf("Nieprawidlowy wybor poziomu trudnosci.\n");
			return 1;
	}
	if(plansza != NULL){
		printf("Plansza zostala poprawnie utworzona!\n");
		zwolnij_plansze(plansza);
	}
	return 0;
}
