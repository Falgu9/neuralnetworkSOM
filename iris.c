#include "iris.h"

void filescan(iris * vector){
    FILE* fichier = fopen(nomfichier, "r");
    char chaine[1000];
    int i = 0;
    while (fgets(chaine, 1000, fichier) != NULL){
        //printf("%s", chaine);
        vector[i].nom = malloc(sizeof(char) * (50));
        sscanf(chaine, "%lf,%lf,%lf,%lf,%s", &vector[i].d[0], &vector[i].d[1], &vector[i].d[2],&vector[i].d[3],vector[i].nom);
        i++;
    }
    fclose(fichier);
}

void normalize(iris * vector,iris * normalizedvector){
    double somme = 0;
    for(int i = 0; i < NBVECTOR; i++){
        //printf("%d\n",i);
        for(int j = 0;j < NBDONNEES; j++){
            somme += pow(vector[i].d[j], 2);
        }
        somme = sqrt(somme);
        for(int j = 0;j < NBDONNEES; j++){
            normalizedvector[i].d[j] = vector[i].d[j] / somme;
            normalizedvector[i].nom = vector[i].nom;
        }
        somme = 0;
    }
}

void moyenne(iris * normalizedvector,double * moyennetab){
    for(int i = 0; i < NBDONNEES; i++){
        for(int j = 0;j < NBVECTOR; j++){
            moyennetab[i] += normalizedvector[j].d[i];
        }
        moyennetab[i] = moyennetab[i]/NBVECTOR;
    }
}

void initneurone(double * moyennetab, iris ** neuronevector,int li,int co){

    for(int i = 0;i <li; i++){
        for(int j = 0; j < co;j++){
            for(int d = 0;d < NBDONNEES;d++){
                double r = (-0.05) + (0.025 - -0.05) * ((double)rand() / (double)RAND_MAX);
                //printf("%f\n",r);
                neuronevector[i][j].d[d] = moyennetab[d] - r;
            }
        }
    }
}

int * randomize(){
    int * randomizedindex = malloc(sizeof(int*) * NBVECTOR);
    int buffer_swap;
    for(int i = 0; i < NBVECTOR; i++){
        randomizedindex[i] = i;
    }
    for(int i = 0; i < NBVECTOR; i++){
        int r = (0) + (NBVECTOR - 0) * ((double)rand() / (double)RAND_MAX);
        buffer_swap =  randomizedindex[i];
        randomizedindex[i] = randomizedindex[r];
        randomizedindex[r] = buffer_swap;
    }
    return randomizedindex;
}

double distance_euclidienne(iris * normalizedvector,iris ** neuronevector,int i,int j,int k){
    return sqrt(pow(neuronevector[j][k].d[0] - normalizedvector[i].d[0],2) + pow(neuronevector[j][k].d[1] - normalizedvector[i].d[1],2) + pow(neuronevector[j][k].d[2] - normalizedvector[i].d[2],2) + pow(neuronevector[j][k].d[3] - normalizedvector[i].d[3],2));
}

void affinage(int *index,int index2,iris * normalizedvector,iris ** neuronevector,int it,int nbit,double * coeff,double coeffbase,int palier,int li, int co){
    int i,j;
    i = index[0] - palier;
    j = index[1] - palier;
    *coeff = coeffbase  * (1.0 - ((double)it/(double)nbit));
    //printf("coeff = %lf \n",*coeff);
    //printf("index-2 = %d,%d, index = %d,%d index+2 = %d,%d\n",index[0]-palier,index[1]-palier,index[0],index[1],index[0]+palier,index[1]+palier);
    for(;i <= index[0]+palier; i++){
        //printf("i = %d\n",i);
        for(; j <= index[1]+palier;j++){
            if(i >= 0 && i < li && j >= 0 && j < co){
                //printf("%d,%d\n",i,j);
                for(int d = 0; d < 4; d++){
                    neuronevector[i][j].d[d] = neuronevector[i][j].d[d] + *coeff * (normalizedvector[index2].d[d] - neuronevector[i][j].d[d]);
                }
            }
        }
        j = index[1] - palier;
    }
}

void winner(iris * normalizedvector,iris ** neuronevector,int nbit,double * coeff,int palier,int li,int co){
    double min = 100.0,d = 0;
    int *index,*randomizedindex;
    double coeffbase = *coeff;
    index = malloc(sizeof(int*) * 2);
    printf("%lf\n",coeffbase);
    for(int it = 0; it < nbit; it++){
        randomizedindex = randomize();
        for(int i = 0;i < NBVECTOR; i++){
            for(int j = 0;j < li;j++){ 
                for(int k = 0;k <co ;k++){
                    d = distance_euclidienne(normalizedvector,neuronevector,randomizedindex[i],j,k);
                    //printf("%lf\n",d);
                    if(d < min){
                        min = d;
                        index[0] = j;
                        index[1] = k;
                    }
                }
            }
            affinage(index,randomizedindex[i],normalizedvector,neuronevector,it,nbit,coeff,coeffbase,palier,li,co);
            min = 100;
        }
    }
    int ** winners,** dejavu,*indexN;
    min = 100.0;
    indexN = malloc(sizeof(int*) * 2);
    winners = malloc(sizeof(int*) * li);
    dejavu  = malloc(sizeof(int*) * li);
    for(int i = 0;i < li;i++){
        winners[i] = malloc(sizeof(int) *co);
        dejavu[i] = malloc(sizeof(int) *co);
    }
    randomizedindex = randomize();
    for(int i = 0; i < NBVECTOR;i++){
        for(int j = 0;j < li;j++){
            for(int k = 0;k<co;k++){
                d = distance_euclidienne(normalizedvector,neuronevector,randomizedindex[i],j,k);
                //printf("%lf\n",d);
                if(d < min){
                    min = d;
                    indexN[0] = j;
                    indexN[1] = k;
                }
            }
        }
        if(dejavu[indexN[0]][indexN[1]] == 0){
            if(strcmp(normalizedvector[randomizedindex[i]].nom, "Iris-setosa") == 0){
                winners[indexN[0]][indexN[1]] = 1;
            }
            if(strcmp(normalizedvector[randomizedindex[i]].nom,"Iris-versicolor") == 0){
                winners[indexN[0]][indexN[1]] = 2;
            }
            if(strcmp(normalizedvector[randomizedindex[i]].nom, "Iris-virginica") == 0){
                winners[indexN[0]][indexN[1]] = 3;
            }
            dejavu[indexN[0]][indexN[1]] = 1;
        }
        min = 100;
    }
    for(int i = 0;i < li;i++){
        for(int j = 0;j < co;j++){
            if(winners[i][j] == 1){
                printf("\e[31m%d ",winners[i][j]);
            }
            else if(winners[i][j] == 2){
                printf("\e[32m%d ",winners[i][j]);
            }
            else if(winners[i][j] == 3){
                printf("\e[34m%d ",winners[i][j]);
            }
            else{
                printf("\e[37m%d ",winners[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}


void printvector(iris * vector){
    for(int i = 0;i < NBVECTOR; i++){
        printf("%f,%f,%f,%f,[%s]\n",vector[i].d[0], vector[i].d[1], vector[i].d[2],vector[i].d[3],vector[i].nom);
    }
    printf("\n");
}
void printtab(double * tab){
    for(int i = 0;i < NBDONNEES; i++){
        printf("%f\n",tab[i]);
    }
    printf("\n");
}
void printtab2(int * tab){
    for(int i = 0;i < NBVECTOR; i++){
        printf("%d\n",tab[i]);
    }
    printf("\n");
}
void printneurone(iris ** vector,int li,int co){
    for(int i = 0;i < li; i++){
        for(int j = 0;j < co;j++){
            printf("%f,%f,%f,%f\n",vector[i][j].d[0], vector[i][j].d[1], vector[i][j].d[2],vector[i][j].d[3]);
        }
    }
    printf("\n");
}

// formule d'apprentissage W'(i,j) = W(i,j)+alpha(X(k)-W(i,j))

int main(){
    int li = 6,co = 10;
    double moyennevector[NBDONNEES];
    iris vector[NBVECTOR], normalizedvector[NBVECTOR],**neuronevector;

    neuronevector = malloc(sizeof(iris*) * li);
    for(int i = 0;i <= li;i++){
        neuronevector[i] = malloc(sizeof(iris) *co);
    }

    srand(time(NULL));
    // On ajoute le contenu du fichier iris.data dans l'objet vector
    filescan(vector);
    //printvector(vector);

    normalize(vector,normalizedvector);
    //printf("normalisation neurone :\n");printvector(normalizedvector);

    moyenne(normalizedvector,moyennevector);
    //printf("moyenne neurone :\n");printtab(moyennevector);

    initneurone(moyennevector,neuronevector,li,co);
    //printf("initialisation neurone :\n");printneurone(neuronevector,li,co);
    //int * randomizedindex = randomize();
    //printtab2(randomizedindex);
    double coeff = 0.9;
    winner(normalizedvector,neuronevector,500,&coeff,3,li,co);
    coeff = 0.09;
    winner(normalizedvector,neuronevector,1500,&coeff,1,li,co);

    return 0;
}
