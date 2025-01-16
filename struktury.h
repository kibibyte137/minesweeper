#ifndef STRUKTURY_H
#define STRUKTURY_H

#include <stdbool.h>

/*Struktura dla wspolrzednych*/
typedef struct{	
	int x;		/*Numer wiersza*/
	int y;		/*Numer kolumny*/
} Punkt;

/*Struktura dla pola*/
typedef struct{				
	bool zakryte;		/*true, jesli pole jest zakryte, false w przeciwnym wypadku*/
	bool mina;			/*true, jesli pole zawiera mine, false w przeciwnym wypadku*/
	bool flaga;			/*true, jesli pole posiada flage, false w przeciwnym wypadku*/
	int sasiednie_miny;	/*Liczba sasiadujacych min*/
	Punkt wspolrzedne;	/*Współrzedne pola (wiersz, kolumna)*/
} Pole;

/*Struktura dla planszy*/
typedef struct{		
	bool koniec_gry;	/*true, jesli gra zostala zakonczona, false w przeciwnym wypadku*/
	bool wygrana;		/*true, jesli gra zakonczyla sie wygrana, false w przeciwnym wypadku*/
	bool pierwszy_ruch; /*true, jesli biezacy ruch jest pierwszym ruchem gry, false w przeciwnym wypadku*/
	int wiersze;		/*Liczba wierszy planszy*/
	int kolumny;		/*Liczba kolumn planszy*/
	int miny;			/*Liczba min na planszy*/
	int odkryte_pola;	/*Liczba odkrytych pol*/
	int wynik;			/*Wynik gracza*/
	Pole** pola;		/*Dwuwymiarowa tablica struktur Pole*/
} Plansza; 

#endif