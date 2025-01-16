#ifndef WYNIKI_H
#define WYNIKI_H

#include "kolory.h"
#include "makra.h"
#include "struktury.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Deklaracja funkcji oblicz_wynik*/
int oblicz_wynik(Plansza* plansza, int mnoznik);

/*Deklaracja funkcji porownaj_wyniki*/
int porownaj_wyniki(const void* a, const void* b);

/*Deklaracja funkcji zapisz_wynik*/
void zapisz_wynik(const char* nazwa_pliku, const char* imie, int wynik);

/*Deklaracja funkcji posortuj_wyniki_w_pliku*/
void posortuj_wyniki_w_pliku(const char* nazwa_pliku);

#endif