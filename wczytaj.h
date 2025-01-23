#ifndef WCZYTAJ_H
#define WCZYTAJ_H

#include "plansza.h"
#include "struktury.h"
#include "wypisz.h"
#include "gra.h"
#include "wyniki.h"
#include "kolory.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*Deklaracja funkcji wczytaj_plansze*/
Plansza* wczytaj_plansze(const char* nazwa_pliku, Ruchy* ruchy);

/*Deklaracja funkcji dodaj_ruch*/
void dodaj_ruch(Ruchy* ruchy, int x, int y);

/*Deklaracja funkcji inicjuj_ruchy*/
void inicjuj_ruchy(Plansza* plansza, Ruchy* ruchy);

#endif