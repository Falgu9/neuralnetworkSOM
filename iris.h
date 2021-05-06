#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define NBVECTOR 150
#define NBDONNEES 4
#define NBNEURONE 60

#define false 0 
#define true 1

typedef struct iris iris;
struct iris{
    double d[4];
    char * nom;
};


typedef struct chaine chaine;
struct chaine{
    double min;
    int index[2];
    chaine *suivant;
};


char * nomfichier = "iris.data";

/*void filescan(iris * vector);
void normalize(iris * vector,iris * normalizedvector);
void moyenne(iris * normalizedvector,double * moyennevector);
void initneurone(double * moyennetab, iris * neuronevector);
void randomize(int * randomizedindex);
double distance_euclidienne(iris * normalizedvector,iris * neuronevector,int i,int j);
void winner(iris * normalizedvector,iris * neuronevector,int * winners);

void printvector(iris * vector);
void printtab(double * tab);
void printneurone(iris * vector);
void printtab2(int * tab,int nb);*/