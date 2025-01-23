#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "tryby.h"

/*Funkcja main programu*/
int main(int argc, char *argv[]){
	int opt; /*Zmienna przechowujaca opcje getopt*/
	int f_flag = 0; /*Flaga oznaczajaca wlaczenie trybu wczytywania z pliku*/
	int t_flag = 0; /*Flaga oznaczajaca wlaczenie trybu testowania generacji plansz*/
	char *plik = NULL; /*Zmienna przechowujaca nazwe pliku podana przez uzytkownika*/
	
	/*Parsowanie argumentow*/
	while((opt = getopt(argc, argv, "f:ht")) != -1){
		switch (opt){
			case 'f':
				f_flag = 1; /*Ustawienie flagi dla trybu wczytywania z pliku*/
				plik = optarg; /*Pobranie nazwy pliku podanej po -f*/
				break;
			case 't':
				t_flag = 1; /*Ustawienie flagi dla trybu testowania generacji plansz*/
				break;
			case 'h':
				pomoc(); /*Wywolanie funkcji wyswietlajacej podreczna pomoc*/
				return 0; /*Zakonczenie programu po wyswietleniu pomocy */
			default:
				printf("Nieprawidlowa opcja.\n"); /*Blad dla nieznanych opcji*/
				pomoc(); /*Wyswietlenie pomocy*/
				return 1; /*Zakonczenie programu z bledem*/
		}
	}

	if(f_flag){
		/*Tryb wczytywania z pliku*/
		if(plik){
			gra_z_pliku(plik); /*Symulacja gry na podstawie pliku*/
		} else {
			fprintf(stderr, "Nie podano nazwy pliku dla opcji -f.\n"); /*Obsluga bledu braku podania nazwy pliku*/
			return 2; /*Zakonczenie programu z bledem*/
		}
	} else if(t_flag){	
		printf("\n");
		test(1);
		printf("\n");
		test(2);
		printf("\n");
		test(3);
		printf("\n");
	} else if(optind == 1){
		/*Brak argumentow - klasyczna gra*/
		klasyczna_gra(); /*Rozpoczecie klasycznej gry*/
	}

	return 0; /*Zakonczenie programu sukcesem*/
}