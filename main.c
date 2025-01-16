#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define BLACK "\033[30m"
#define RED	"\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define BRIGHT_BLACK "\033[90m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_RED "\033[91m"

#define BRIGHT_RED_BG "\033[101m"

#define RESET "\033[0m"

/*Symbole graficznej reprezentacji planszy*/
#define ZAKRYTE		'#'			/*Zakryte pole*/
#define PUSTE		'.'  		/*Odkryte puste pole*/
#define MINA		'X'			/*Odkryte pole z miną*/
#define FLAGA		'P'			/*Zakryte pole z flagą*/
#define LICZBA(x)	('0' + (x))	/*Odkryte pole z liczbą (1-8)*/

/*Stale potrzebne do zapisu wynikow*/
#define MAX_WYNIKOW	100	/*Maksymalna liczba wynikow*/
#define MAX_IMIE 16 /*Maksymalna liczba liter w imieniu*/

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
	bool pierwszy_ruch;
	int wiersze;	/*Liczba wierszy planszy*/
	int kolumny;	/*Liczba kolumn planszy*/
	int miny;		/*Liczba min na planszy*/
	int odkryte_pola;
	int wynik;
	Pole** pola;	/*Dwuwymiarowa tablica struktur Pole*/
} Plansza; 

typedef struct{
	char imie[16];
	int wynik;
}Wynik;


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
	plansza->pierwszy_ruch = true;
	plansza->odkryte_pola = 0;
	plansza->wynik = 0;
	
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
			continue;
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

void inicjuj_pierwszy_ruch(Plansza* plansza, int x, int y){
	rozmiesc_miny(plansza, x, y);
	oblicz_sasiednie_miny(plansza);
	plansza->pierwszy_ruch = false;
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
	
	if(plansza->pierwszy_ruch){
		inicjuj_pierwszy_ruch(plansza, x, y);
	}
	
	if (!pole->zakryte){
		printf("Pole (%d, %d) jest juz odkryte.\n", x, y);
		return;
	}
	
	if(pole->flaga){
		printf("Wybrane pole posiada flage. Czy chcesz kontynuowac? T/N\n");
		char kontynuacja;
		do {
			scanf(" %c", &kontynuacja);
			if(kontynuacja == 'N'){
				printf("Pole (%d, %d) nie zostalo odkryte.\n", x, y);
				return;
			} else if(kontynuacja == 'T'){
				pole->flaga = false;
				break;
			} else {
				printf("Nieprawidlowy wybor. Sprobuj ponownie: T/N.\n");
			}
		} while(1);
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
				if(plansza->pola[nx][ny].zakryte){
					odkryj(plansza, nx, ny);
				}
			}
		}
	}
	
	if(plansza->odkryte_pola == plansza->wiersze * plansza->kolumny - plansza->miny){
		plansza->koniec_gry = true;
		plansza->wygrana = true;
	}
}

char widoczny_symbol_pola(const Pole* pole){
	if(pole->flaga) return FLAGA;
	if(pole->zakryte) return ZAKRYTE;
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
	
	const char* kolory[] = {RESET, CYAN, GREEN, RED, BLUE, YELLOW, BRIGHT_CYAN, MAGENTA, BRIGHT_BLACK};
	
	printf("     "); /*Odstep na indeksy wierszy*/
	for(int i = 0; i < plansza->kolumny; i++){
		printf("%3d", i);
	}
	printf("\n");
	
	printf("     ");
	for(int i = 0; i < plansza -> kolumny; i++){
		printf("___");
	}
	printf("\n");
	
	for(int i = 0; i < plansza->wiersze; i++){
		printf("%3d |", i);
		for(int j = 0; j < plansza->kolumny; j++){
			Pole* pole = &plansza->pola[i][j];
			char symbol = widoczny_symbol_pola(pole);
			
			if(symbol >= '1' && symbol <= '8'){
				int liczba = symbol - '0';
				printf("%s%3c%s", kolory[liczba], symbol, RESET);
			} else if (symbol == MINA){
				printf(BRIGHT_RED_BG "%3c" RESET, symbol);
			} else if (symbol == FLAGA){
				printf(BRIGHT_RED "%3c" RESET, symbol);
			} else {
				printf("%3c", symbol);
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
	if(!plansza){
		printf("Blad. Plansza nie istnieje.\n");
		return;
	}
	
	const char* kolory[] = {RESET, CYAN, GREEN, RED, BLUE, YELLOW, BRIGHT_CYAN, MAGENTA, BRIGHT_BLACK};
	
	printf("     "); /*Odstep na indeksy wierszy*/
	for(int i = 0; i < plansza->kolumny; i++){
		printf("%3d", i);
	}
	printf("\n");
	
	printf("     ");
	for(int i = 0; i < plansza -> kolumny; i++){
		printf("___");
	}
	printf("\n");
	
	for(int i = 0; i < plansza->wiersze; i++){
		printf("%3d |", i);
		for(int j = 0; j < plansza->kolumny; j++){
			Pole* pole = &plansza->pola[i][j];
			char symbol = logiczny_symbol_pola(pole);
			
			if(symbol >= '1' && symbol <= '8'){
				int liczba = symbol - '0';
				printf("%s%3c%s", kolory[liczba], symbol, RESET);
			} else if (symbol == MINA){
				printf(BRIGHT_RED_BG "%3c" RESET, symbol);
			} else {
				printf("%3c", symbol);
			}
		}
		printf("\n");
	}
}

void odczytaj_polecenie(Plansza* plansza){
	char komenda[8];
	int x, y;
	
	printf("\nPodaj polecenie:\n");
	scanf("%s", komenda);
	
	if(strcmp(komenda, "r") == 0){
		scanf("%d %d", &x, &y);
		odkryj(plansza, x, y);
	} else if(strcmp(komenda, "f") == 0){
		scanf("%d %d", &x, &y);
		flaga(plansza, x, y);
	} else if(strcmp(komenda, "k") == 0){
		plansza->koniec_gry = true;
	} else {
		printf("Nieznane polecenie: %s\n", komenda);
	}
}

int oblicz_wynik(Plansza* plansza, int mnoznik){
	plansza->wynik=mnoznik*plansza->odkryte_pola;
	return plansza->wynik;
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
		printf("%s%s" RESET, kolor, linia); /*Wypisanie linii na ekran*/
	}
	printf("\n");
	fclose(plik); /*Zamkniecie pliku*/
}

int porownaj_wyniki(const void* a, const void* b) {
    int wynik_a, wynik_b;
	char imie_a[MAX_IMIE], imie_b[MAX_IMIE];
	
    sscanf((const char*)a, "%s %d", imie_a, &wynik_a);
    sscanf((const char*)b, "%s %d", imie_b, &wynik_b);
	
	if(wynik_a != wynik_b){
		return wynik_b - wynik_a;
	}
	
	return strcmp(imie_a, imie_b);
}

void zapisz_wynik(const char* nazwa_pliku, const char* imie, int wynik){
	FILE* plik = fopen(nazwa_pliku, "a");
	if(!plik){
		printf("Nie mozna otworzyc pliku: %s.\n", nazwa_pliku);
		return;
	}
	if(fprintf(plik, "%-15s %d\n", imie, wynik) < 0) {
		printf("Blad podczas zapisu do pliku: %s", nazwa_pliku);
	}
	fclose(plik);
}

void posortuj_wyniki_w_pliku(const char* nazwa_pliku) {
	char wyniki[MAX_WYNIKOW][MAX_IMIE + 10]; // Bufor na linie z imieniem i wynikiem
	int liczba_wynikow = 0;

	FILE* plik = fopen(nazwa_pliku, "r");
	if(!plik){
		printf("Nie można otworzyć pliku do odczytu: %s.\n", nazwa_pliku);
		return;
	}

	while(fgets(wyniki[liczba_wynikow], sizeof(wyniki[0]), plik)){
		liczba_wynikow++;
		if(liczba_wynikow >= MAX_WYNIKOW)break;
	}
	fclose(plik);

	// Sortowanie wyników
	qsort(wyniki, liczba_wynikow, sizeof(wyniki[0]), porownaj_wyniki);

	// Nadpisanie pliku posortowaną listą
	plik = fopen(nazwa_pliku, "w");
	if(!plik){
		printf("Nie można otworzyć pliku do zapisu: %s.\n", nazwa_pliku);
		return;
	}

	for(int i = 0; i < liczba_wynikow; i++){
		fprintf(plik, "%s", wyniki[i]);
	}
	fclose(plik);
}

void wypisz_top5(const char* nazwa_pliku){
	FILE* plik = fopen(nazwa_pliku, "r");
	if(!plik){
		printf("Nie można otworzyć pliku do odczytu: %s.\n", nazwa_pliku);
		return;
	}
	
	char linia[256];
	int licznik = 1;
	
	while(fgets(linia, sizeof(linia), plik) && licznik < 6){
		printf("%d. %s", licznik, linia);
		licznik++;
	}
	fclose(plik);
}

int main(){
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
		printf("Plansza zostala poprawnie utworzona!\n\n");
		while(!plansza->koniec_gry){
			printf("\nAktualny wynik:" GREEN "%d\n\n" RESET, oblicz_wynik(plansza, mnoznik));
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
		printf("\nWynik koncowy: " GREEN "%d\n" RESET, plansza->wynik);
		printf("\nWpisz swoje imie:\n");
		char imie[16];
		scanf("%15s", imie);
		zapisz_wynik("wyniki", imie, plansza->wynik);
		posortuj_wyniki_w_pliku("wyniki");
		wypisz_plik("podium","");
		wypisz_top5("wyniki");
		printf("\nDziekuje za gre, %s. Gratuluje wyniku! :)\n", imie);
		zwolnij_plansze(plansza);
	}
	
	return 0;
}