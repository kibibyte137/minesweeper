#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*Symbole graficznej reprezentacji planszy*/
#define ZAKRYTE		'#'			/*Zakryte pole*/
#define PUSTE		'.'  		/*Odkryte puste pole*/
#define MINA		'X'			/*Odkryte pole z miną*/
#define FLAGA		'P'			/*Zakryte pole z flagą*/
#define LICZBA(x)	('0' + (x))	/*Odkryte pole z liczbą (1-8)*/

typedef struct{	/*Struktura dla wspolrzednych*/
	int x;		/*Numer wiersza*/
	int y;		/*Numer kolumny*/
} Punkt;

typedef struct{				/*Struktura dla pola*/
	bool zakryte;			/*true, jesli pole jest zakryte, false w przeciwnym wypadku*/
	bool mina;				/*true, jesli pole zawiera mine, false w przeciwnym wypadku*/
	bool flaga;				/*true, jesli pole posiada flage, false w przeciwnym wypadku*/
	int sasiednie_miny;		/*Liczba sasiadujacych min*/
	Punkt wspolrzedne;		/*Współrzedne pola (wiersz, kolumna)*/
} Pole;

typedef struct{		/*Struktura dla planszy*/
	bool koniec_gry;
	bool wygrana;
	int wiersze;	/*Liczba wierszy planszy*/
	int kolumny;	/*Liczba kolumn planszy*/
	int miny;		/*Liczba min na planszy*/
	int odkryte_pola;
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
		Ustawia wszystkie pola jako zakryte, bez min, bez flag, z zerowa liczba sasiadujacych min.
		Ustawia liczbe min w strukturze Plansza.
*/

void inicjuj_plansze(Plansza* plansza, int liczba_min){
	if(!plansza){ /*Sprawdzenie, czy wskaznik plansza nie jest NULL*/
		printf("Nie mozna zainicjalizowac planszy: wskaznik NULL.\n");
		return;
	}
	
	plansza->miny = liczba_min; /*Ustawienie liczby min na planszy*/
	plansza->koniec_gry = false;
	plansza->wygrana = false;
	plansza->odkryte_pola = 0;
	
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

void odkryj(Plansza* plansza, int x, int y){
	if(plansza->koniec_gry){
		printf("Gra zostala juz zakonczona.\n");
		return;
	}
	
	if(x < 0 || x >= plansza->wiersze || y < 0 || y >= plansza->kolumny){
		printf("Wspolrzedne (%d, %d) sa poza zakresem planszy.\n", x, y);
		return;
	}
	
	Pole* pole = &plansza->pola[x][y];
	
	if (!pole->zakryte){
		printf("Pole (%d, %d) jest juz odkryte.\n", x, y);
		return;
	}
	
	if(pole->flaga){
		printf("Wybrane pole posiada flage. Czy chcesz kontynuowac? T/N\n");
		char kontynuacja;
		scanf("%c", &kontynuacja);
		
		if(kontynuacja == 'N'){
			printf("Pole (%d, %d) nie zostalo odkryte.\n", x, y);
			return;
		} else if(kontynuacja != 'T'){
			printf("Nieprawidlowy wybor. Pole (%d, %d) nie zostalo odkryte.\n", x, y);
			return;
		}
	}
	
	pole->zakryte = false;
	
	if(pole->mina){
		plansza->koniec_gry = true;
		return;
	}
	
	plansza->odkryte_pola++;
	
	if(pole->sasiednie_miny == 0){
		int kierunki[8][2] = { /*Tablica przesuniec dla 8 kierunkow*/
			{-1, -1},	{-1, 0},	{-1, 1},	/*Gorne sasiedzwtow*/
			{ 0, -1},				{ 0, 1},	/*Lewe i prawe sasiedztwo*/
			{ 1, -1},	{ 1, 0},	{ 1, 1}		/*Dolne sasiedztwo*/
		};
		
		for(int k = 0; k < 8; k++){
			int nx = x + kierunki[k][0]; /*Nowa wspolrzedna x*/
			int ny = y + kierunki[k][1]; /*Nowa wspolrzedna y*/
				
			if(nx >= 0 && nx < plansza->wiersze && ny >= 0 && ny < plansza->kolumny){
				odkryj(plansza, nx, ny);
			}
		}
	}
	
	if(plansza->odkryte_pola == plansza->wiersze * plansza->kolumny - plansza->miny){
		plansza->koniec_gry = true;
	}
}

char widoczny_symbol_pola(const Pole* pole){
	if(pole->zakryte) return ZAKRYTE;
	if(pole->flaga) return FLAGA;
	if(pole->mina) return MINA;
	if(pole->sasiednie_miny > 0 && !pole->mina) return LICZBA(pole->sasiednie_miny);
	return PUSTE;
}

char logiczny_symbol_pola(const Pole* pole){
	if(pole->mina) return MINA;
	if(pole->sasiednie_miny > 0 && !pole->mina) return LICZBA(pole->sasiednie_miny);
	return PUSTE;
}

/*	Funkcja wypisz_plansze_widoczna
	Wypisuje widoczny stan planszy gry.
	Parametry:
		plansza - wskaznik na strukture Plansza, ktora zawiera informacje o calej planszy gry
	Dzialanie:
		Wypisuje stan planszy gry w postaci symboli, ktore reprezentuja rozne stany pol.
*/
void wypisz_plansze_widoczna(Plansza* plansza){
	if(!plansza){
		printf("Blad. Plansza nie istnieje.\n");
		return;
	}
	for(int i = 0; i < plansza->wiersze; i++){
		for(int j = 0; j < plansza->kolumny; j++){
			Pole* pole = &plansza->pola[i][j];
			printf("%c ", widoczny_symbol_pola(pole));
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
	if(!plansza){
		printf("Blad. Plansza nie istnieje.\n");
		return;
	}
	for(int i = 0; i < plansza->wiersze; i++){
		for(int j = 0; j < plansza->kolumny; j++){
			Pole* pole = &plansza->pola[i][j];
			printf("%c ", logiczny_symbol_pola(pole));
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

int main(){
	bool koniec_gry = false;
	
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
		rozmiesc_miny(plansza);
		oblicz_sasiednie_miny(plansza);
		printf("Plansza zostala poprawnie utworzona!\n");
		wypisz_plansze_logiczna(plansza);
		wypisz_plansze_widoczna(plansza);
		zwolnij_plansze(plansza);
	}
	
	return 0;
}