#ifndef GRA_H
#define GRA_H

#include "struktury.h"
#include "plansza.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*Deklaracja funkcji flaga*/
void flaga(Plansza* plansza, int x, int y);

/*Deklaracja funkcji inicjuj_pierwszy_ruch*/
void inicjuj_pierwszy_ruch(Plansza* plansza, int start_x, int start_y);

/*Deklaracja funkcji odkryj*/
void odkryj(Plansza* plansza, int x, int y);

/*Deklaracja funkcji odczytaj_polecenie*/
void odczytaj_polecenie(Plansza* plansza);

#endif