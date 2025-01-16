#ifndef WYPISZ_H
#define WYPISZ_H

#include "struktury.h"
#include "kolory.h"
#include "makra.h"

#include <stdio.h>
#include <stdlib.h>

/*Deklaracja funkcji widoczny_symbol_pola*/
char widoczny_symbol_pola(const Pole* pole);

/*Deklaracja funkcji logiczny_symbol_pola*/
char logiczny_symbol_pola(const Pole* pole);

/*Deklaracja funkcji wypisz_plansze_widoczna*/
void wypisz_plansze_widoczna(Plansza* plansza);

/*Deklaracja funkcji wypisz_plansze_logiczna*/
void wypisz_plansze_logiczna(Plansza* plansza);

/*Deklaracja funkcji wypisz_plik*/
void wypisz_plik(const char* nazwa_pliku, const char* kolor);

/*Deklaracja funkcji wypisz_top5*/
void wypisz_top5(const char* nazwa_pliku);

#endif