/**
* \file jeu.c
* \brief Implémentation des fonctions faisant évoluer les cellules.
* \author Ajavon Wesley
*/

#include "jeu.h"
#if MODETEXTE


int cyclique = 1;


int compte_voisins_vivants_cyclique (int i, int j, grille g){
	int v = 0, l=g.nbl, c = g.nbc;
	v+= est_vivante(modulo(i-1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);

	return v; 
}

int compte_voisins_vivants_non_cyclique (int i, int j, grille g){
	int v = 0, l=g.nbl, c = g.nbc;
	if(i == 0) {
		if(j == 0) {
			v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
			v+= est_vivante(modulo(i+1,l),modulo(j,c),g);
			v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);
		} else if (j == c) {
			v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
			v+= est_vivante(modulo(i+1,l),modulo(j-1,c),g);
			v+= est_vivante(modulo(i+1,l),modulo(j,c),g);
		} else {
			v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
			v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
			v+= est_vivante(modulo(i+1,l),modulo(j-1,c),g);
			v+= est_vivante(modulo(i+1,l),modulo(j,c),g);
			v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);
		}
	} else if(i == l) {
		if(j == 0) {
			v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
			v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
			v+= est_vivante(modulo(i-1,l),modulo(j+1,c),g);
		} else if(j == c) {
			v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
			v+= est_vivante(modulo(i-1,l),modulo(j-1,c),g);
			v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
		} else {
			v+= est_vivante(modulo(i-1,l),modulo(j-1,c),g);
			v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
			v+= est_vivante(modulo(i-1,l),modulo(j+1,c),g);
			v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
			v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
		}
	} else if(j == 0) {
		v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
		v+= est_vivante(modulo(i-1,l),modulo(j+1,c),g);
		v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
		v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);
		v+= est_vivante(modulo(i+1,l),modulo(j,c),g);

	} else if(j == c) {
		v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
		v+= est_vivante(modulo(i-1,l),modulo(j-1,c),g);
		v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
		v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);
		v+= est_vivante(modulo(i+1,l),modulo(j,c),g);

	}	else {
		v+= est_vivante(modulo(i-1,l),modulo(j-1,c),g);
		v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
		v+= est_vivante(modulo(i-1,l),modulo(j+1,c),g);
		v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
		v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
		v+= est_vivante(modulo(i+1,l),modulo(j-1,c),g);
		v+= est_vivante(modulo(i+1,l),modulo(j,c),g);
		v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);
	}

	return v;
}

void evolue (grille *g, grille *gc){
	copie_grille (*g,*gc); // copie temporaire de la grille
	int (*pcompte_voisins_vivants) (int,int, grille); // pointeur fonction qui change de valeur quand on appuie sur c

	if(cyclique == 0) {
		pcompte_voisins_vivants = &compte_voisins_vivants_non_cyclique;
	} else {
		pcompte_voisins_vivants = &compte_voisins_vivants_cyclique;

	}

	int i,j,l=g->nbl, c = g->nbc,v;
	for (i=0; i<l; i++)
	{
		for (j=0; j<c; ++j)
		{
			v = (*pcompte_voisins_vivants) (i, j, *gc);

			if (est_vivante(i,j,*g)) 
			{ // evolution d'une cellule vivante
				if ((v!=2 && v!= 3) || (g->cellules[i][j] == 8)){
					set_morte(i,j,*g);
				} else if((v == 2 || v == 3))
				vieillir(i,j,*g);
			}
			else if (est_nonViable(i,j,*g)){
				;
			} else
			{ // evolution d'une cellule morte
				if ( v==3 ) set_vivante(i,j,*g);
			}
		}
	}
	return;
}

#else


int compte_voisins_vivants_cyclique (int i, int j, grille g){
	int v = 0, l=g.nbl, c = g.nbc;
	v+= est_vivante(modulo(i-1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);

	return v;
}

int compte_voisins_vivants_non_cyclique (int i, int j, grille g){
	int v = 0, l=g.nbl, c = g.nbc;

	v+= (i-1 >= 0 && j-1 >= 0) ? est_vivante(i-1,j-1,g) : 0;
	v+= (i-1 >= 0) ? est_vivante(i-1,j,g) : 0;
	v+= (i-1 >= 0 && j+1 < c) ? est_vivante(i-1,j+1,g) : 0;
	v+= (j-1 >= 0) ? est_vivante(i,j-1,g) : 0;
	v+= (j+1 < c) ? est_vivante(i,j+1,g) : 0;
	v+= (i+1 < l && j-1 >= 0) ? est_vivante(i+1,j-1,g) : 0;
	v+= (i+1 < l) ? est_vivante(i+1,j,g) : 0;
	v+= (i+1 < l && j+1 < c) ? est_vivante(i+1,j+1,g) : 0;

	return v;
}

void evolue (grille *g, grille *gc, int *tempsEvolution, int (*compte_voisins_vivants_cyclique) (int, int, grille), int vieillissement){
	(*tempsEvolution)++;
	copie_grille (*g,*gc); // copie temporaire de la grille
	int i,j,l=g->nbl, c = g->nbc,v;
	for (i=0; i<l; i++)
	{
		for (j=0; j<c; ++j)
		{
			if (!est_nonViable(i, j, *g)) {
				v = compte_voisins_vivants_cyclique(i, j, *gc);
				if (est_vivante(i,j,*g))
				{
					// evolution d'une cellule vivante
					if ( v!=2 && v!= 3 ) set_morte(i,j,*g);
					// sinon, si vieillissement activé, incrémentation de son âge (vieillissement)
					else if (vieillissement) g->cellules[i][j]++;
					else g->cellules[i][j] = 1; // Réinitialisation de l'age si la cellule avait subit un vieillissement

					if (vieillissement && g->cellules[i][j] > 8) set_morte(i,j,*g);
				}
				else
				{ // evolution d'une cellule morte
					if ( v==3 ) set_vivante(i,j,*g);
				}
			}
		}
	}
	return;
}

#endif
