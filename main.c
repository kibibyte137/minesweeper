#include <stdio.h>
#include <stdlib.h>

/*Symbole graficznej reprezentacji planszy*/
#define PUSTE		'.'  /* Odkryte puste pole */
#define ZAKRYTE		'#'  /* Zakryte pole */
#define MINA		'X'  /* Odkryte pole z miną */
#define FLAGA		'P'  /* Zakryte pole z flagą */
#define LICZBA(x)	('0' + (x)) /* Odkryte pole z liczbą (1-8) */

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

/*Do zmiany !!*/
void **alokuj_pamiec(int wiersze, int kolumny, size_t rozmiar_elementu){
	void **tablica = (void **)malloc(wiersze * sizeof(void *));
	if (!tablica){
		printf("Blad alokacji pamieci dla tablicy.\n"); 
		return NULL; 
	}
	for (int i = 0; i < wiersze; i++){ 
		tablica[i] = malloc(kolumny * rozmiar_elementu);
		if (!tablica[i]){ 
			printf("Blad alokacji pamieci dla wiersza %d.\n", i); 
			for (int j = 0; j < i; j++){ 
				free(tablica[j]);
			}
			free(tablica); 
			return NULL;
		}
	}
	return tablica;
}

void zwolnij_pamiec(void **tablica, int wiersze){
	for (int i = 0; i < wiersze; i++){
		free(tablica[i]);
	}
	free(tablica);
}

void inicjuj_plansze(Plansza* plansza){
	for (int i = 0; i < plansza->wiersze; i++){
		for (int j = 0; j < plansza->kolumny; j++){
			plansza->plansza_logiczna[i][j] = POLE;
			plansza->plansza_widoczna[i][j] = ZAKRYTE;
		}
	}
}

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

void zwolnij_plansze(Plansza* plansza){
	if (plansza){
		zwolnij_pamiec((void**)plansza->plansza_logiczna, plansza->wiersze);
		zwolnij_pamiec((void**)plansza->plansza_widoczna, plansza->wiersze);
		free(plansza);
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