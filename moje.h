#ifndef DETAILED_CUBE_H
#define DETAILED_CUBE_H

#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <iostream>
#include <list>
#include <fstream>
using namespace std;

class player{
public:
float angle;
float level;
float speed;
int lives;
player();
};

class opponent{
public:
int type;
float angle;
float level;
float speed;
opponent();
opponent(int typ);
opponent(int typ, float angl, float lev, float spee);
void checker();
};

bool przedzial(float co,float mniejsza, float wieksza);
void jedynkuj(int tab[10][100]);
void leworuch(int tab[10][100], int w);
void praworuch(int tab[10][100], int w);
void wys(int tab[10][100]);
bool tlumacz(int tab[10][100],float kat,float lev);
bool tlumaczdol(int tab[10][100],float kat,float lev);

#endif
