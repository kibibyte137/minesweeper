#ifndef PLANSZA_H
#define PLANSZA_H

#include "struktury.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*Deklaracja funkcji alokuj_plansze*/
Plansza* alokuj_plansze(int wiersze, int kolumny);

/*Deklaracja funkcji inicjuj_plansze*/
void inicjuj_plansze(Plansza* plansza, int liczba_min);

/*Deklaracja funkcji zwolnij_plansze*/
void zwolnij_plansze(Plansza* plansza);

/*Deklaracja funkcji rozmiesc_miny*/
void rozmiesc_miny(Plansza* plansza, int start_x, int start_y);

/*Deklaracja funkcji oblicz_sasiednie_miny*/
void oblicz_sasiednie_miny(Plansza* plansza);

#endif