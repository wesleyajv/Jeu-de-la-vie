/**
* \file grille.c
* \brief Fonctions d'allocations mémoire et d'ouverture de fichier.
* \author Ajavon Wesley
*/


#include "jeu.h"




int init_grille_from_file (char * filename, grille* g){
	int result = 0;
	FILE * pfile = NULL;
	pfile = fopen(filename, "r");
	if(pfile == NULL) return 1;

	int i,j,n,l,c,vivantes=0,nonViable=0,m,x,y;
	
	if(fscanf(pfile, "%d", & l) == 0) {
		printf("Couldn't read file input.");
		exit(-1);
	}
	if(fscanf(pfile, "%d", & c) == 0) {
		printf("Couldn't read file input.");
		exit(-1);
	}
	
	alloue_grille(l,c,g);
	
	if(fscanf(pfile, "%d", & vivantes) == 0) {
		printf("Couldn't read file input.");
		exit(-1);
	}
	for (n=0; n< vivantes; ++n){
		if(fscanf(pfile, "%d", & i) == 0) {
			printf("Couldn't read file input.");
			exit(-1);
		}
		if(fscanf(pfile, "%d", & j) == 0) {
			printf("Couldn't read file input.");
			exit(-1);
		}
		set_vivante(i,j,*g);
	}
	
	if(fscanf(pfile, "%d", & nonViable) == 0) {
		printf("Couldn't read file input.");
		exit(-1);
	}
	for (m=0; m< nonViable; ++m){
			if(fscanf(pfile, "%d", & x) == 0) {
				printf("Couldn't read file input.");
				exit(-1);
			}
			if(fscanf(pfile, "%d", & y) == 0) {
				printf("Couldn't read file input.");
				exit(-1);
			}
			set_nonViable(x,y,*g);
		}


	fclose (pfile);
	return result;
}


void copie_grille (grille gs, grille gd){
	int i, j;
	for (i=0; i<gs.nbl; ++i) for (j=0; j<gs.nbc; ++j) gd.cellules[i][j] = gs.cellules[i][j];
	return;	
}

void alloue_grille (int l, int c, grille* g) {
	if((g->cellules = (int **)malloc(l * sizeof(int *))) == NULL) {
		printf("No memory allocated");
		exit(-1);
	}

	for(int i = 0; i < l; i++) {
		if((g->cellules[i] = (int *)malloc(c * sizeof(int))) == NULL) {
			printf("No memory allocated");
			exit(-1);
		}
	}

	g->nbl = l;
	g->nbc = c;

	for(int i = 0; i < l; i++) {
		for(int j = 0; j < c; j++) {
			set_morte(i, j, *g);
		}
	}

}

void libere_grille (grille* g) {
	for(int i = 0; i < g->nbl; i++) {
		free(g->cellules[i]);
		g->cellules[i] = NULL;
	}

	free(g->cellules);
	g->cellules = NULL;

}

int grillesEgales(grille *g1, grille *g2) {
	int i, j;
	for (i = 0; i < g1->nbl; i++) {
		for (j = 0; j < g1->nbc; j++) {
			if (g1->cellules[i][j] != g2->cellules[i][j]) {
				return 0;
			}
		}
	}
	return 1;
}

int grilleVide(grille *g) {
	int i, j;
	for (i = 0; i < g->nbl; i++) {
		for (j = 0; j < g->nbc; j++) {
			if (g->cellules[i][j] > 0) {
				return 0;
			}
		}
	}
	return 1;
}



#if MODETEXTE

int grilleOscillante(grille *g) {
	int tempsEvolutionOscillation = 0;
	grille copieInitiale, copieIteree, tmp;
	alloue_grille (g->nbl, g->nbc, &copieInitiale);
	copie_grille(*g, copieInitiale);

	alloue_grille (g->nbl, g->nbc, &copieIteree);
	copie_grille(*g, copieIteree);

	alloue_grille (g->nbl, g->nbc, &tmp);
	copie_grille(*g, tmp);

	int maxInterval = 1000; // On suppose qu'au-delà de 1000 évolutions, une grille ne peut pas être oscillante

	int maxDelais = 100; // On suppose qu'au-delà de 100 évolutions, il ne peut plus y avoir de comportement oscillatoire
	int i = 0;
	 do {
		while (tempsEvolutionOscillation < maxInterval) {
			evolue(&copieIteree,&tmp); // Met à jour tempsEvolutionOscillation
			++tempsEvolutionOscillation;
			if (grilleVide(&copieInitiale)) {
				return 0;
			}
			if (grillesEgales(&copieInitiale, &copieIteree)) {
				return tempsEvolutionOscillation;
			}
		}

		evolue(&copieInitiale,&tmp);
		++tempsEvolutionOscillation;
		copie_grille(copieInitiale, copieIteree);
		tempsEvolutionOscillation = 0; // Réinitialisation du temps d'une oscillation
		i++;
	} while (i < maxDelais);


	

	return 0;
}
#else


int grilleOscillante(grille *g, int (*compte_voisins_vivants) (int, int, grille), int vieillissement) {
	int tempsEvolutionOscillation = 0;
	grille copieInitiale, copieIteree, tmp;
	alloue_grille (g->nbl, g->nbc, &copieInitiale);
	copie_grille(*g, copieInitiale);

	alloue_grille (g->nbl, g->nbc, &copieIteree);
	copie_grille(*g, copieIteree);

	alloue_grille (g->nbl, g->nbc, &tmp);
	copie_grille(*g, tmp);

	int maxInterval = 1000; // On suppose qu'au-delà de 1000 évolutions, une grille ne peut pas être oscillante

	int maxDelais = 100; // On suppose qu'au-delà de 100 évolutions, il ne peut plus y avoir de comportement oscillatoire
	int i = 0;
	 do {
		while (tempsEvolutionOscillation < maxInterval) {
			evolue(&copieIteree,&tmp,&tempsEvolutionOscillation,compte_voisins_vivants,vieillissement); // Met à jour tempsEvolutionOscillation
			if (grilleVide(&copieInitiale)) {
				return 0;
			}
			if (grillesEgales(&copieInitiale, &copieIteree)) {
				return tempsEvolutionOscillation;
			}
		}

		evolue(&copieInitiale,&tmp,&tempsEvolutionOscillation,compte_voisins_vivants,vieillissement);
		copie_grille(copieInitiale, copieIteree);
		tempsEvolutionOscillation = 0; // Réinitialisation du temps d'une oscillation
		i++;
	} while (i < maxDelais);


	

	return 0;
}


#endif


