#include <stdio.h>
#include <stdlib.h>

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

/*Do zmiany!!*/
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

int main(){
	FILE *logo = fopen("logo", "r");
	int trudnosc;
    if (logo == NULL) {
		perror("Nie można otworzyć pliku");
		return 1;
    }

    char linia[256];
    while (fgets(linia, sizeof(linia), logo)) {
		printf("%s", linia);
    }
	printf("Witaj w grze Minesweeper!\n");
	printf("Wybierz poziom trudnosci:\n");
	printf("Latwy\t\t- 1\nNormalny\t- 2\nTrudny\t\t- 3\n");
	scanf("%d", &trudnosc);
	printf("Wybrany poziom trudnosci to: %d\n", trudnosc);	
	fclose(logo);
	return 0;
}
