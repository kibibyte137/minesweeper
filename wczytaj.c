#include "plansza.h"
#include "struktury.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* Funkcja wczytaj_plansze
   Wczytuje dane planszy z pliku w podanym formacie.
   Parametry:
       nazwa_pliku - nazwa pliku zawierającego dane planszy
   Zwraca:
       Wskaźnik do wypełnionej struktury Plansza lub NULL w przypadku błędu.
*/
Plansza* wczytaj_plansze(const char* nazwa_pliku){
    FILE* plik = fopen(nazwa_pliku, "r");
    if (!plik) {
        printf("Nie można otworzyć pliku: %s\n", nazwa_pliku);
        return NULL;
    }

    int wiersze, kolumny, liczba_min;
    if (fscanf(plik, "%d %d %d\n", &wiersze, &kolumny, &liczba_min) != 3) {
        printf("Błąd odczytu rozmiarów planszy i liczby min.\n");
        fclose(plik);
        return NULL;
    }

    Plansza* plansza = alokuj_plansze(wiersze, kolumny);
    if (!plansza) {
        fclose(plik);
        return NULL;
    }

    inicjuj_plansze(plansza, liczba_min);

	/* Wczytywanie planszy */
	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < kolumny; j++) {
			char pole;
			if (fscanf(plik, " %c", &pole) != 1 || (pole != '0' && pole != '1')) {
				printf("Błąd odczytu danych planszy w wierszu %d.\n", i);
				printf("Oczekiwano '0' lub '1', odczytano: '%c'\n", pole);
				zwolnij_plansze(plansza);
				fclose(plik);
				return NULL;
			}
			plansza->pola[i][j].mina = (pole == '1');
		}
	}

	// Pomijanie białych znaków po wczytaniu planszy
	while (isspace(fgetc(plik)));
	ungetc(' ', plik);  // Cofamy wskaźnik o jeden znak

	/* Odczytanie ruchów */
	Punkt* ruchy = malloc(sizeof(Punkt) * (wiersze * kolumny)); // Alokacja maksymalnej możliwej liczby ruchów
	if (!ruchy) {
		printf("Błąd alokacji pamięci dla ruchów.\n");
		zwolnij_plansze(plansza);
		fclose(plik);
		return NULL;
	}

	int liczba_ruchow = 0;
	char typ_ruchu;
	while (fscanf(plik, " %c %d %d", &typ_ruchu, &ruchy[liczba_ruchow].x, &ruchy[liczba_ruchow].y) == 3) {
		if (typ_ruchu == 'r') {
			liczba_ruchow++;
		} else {
			printf("Nieznany typ ruchu: %c\n", typ_ruchu);
		}
	}


    printf("Wczytano planszę: %dx%d, liczba min: %d\n", wiersze, kolumny, liczba_min);
    printf("Wczytano ruchy (%d):\n", liczba_ruchow);
    for (int i = 0; i < liczba_ruchow; i++) {
        printf("Ruch %d: (%d, %d)\n", i + 1, ruchy[i].x, ruchy[i].y);
    }

    free(ruchy); // Zwalniamy pamięć na ruchy, jeśli ruchy nie są zapisywane globalnie
    fclose(plik);
    return plansza;
}