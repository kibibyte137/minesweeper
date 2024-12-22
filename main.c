#include <stdio.h>
#include <stdlib.h>

#define POLE '.'
#define ZAKRYTE '#'
#define MINA 'X'
#define FLAGA 'P'

typedef struct{
	int x;
	int y;
} Punkt;

typedef struct{
	int wiersze;
	int kolumny;
	int bomby;
	char** plansza_logiczna;
	char** plansza_widoczna; 
} Plansza; 

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