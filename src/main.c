/**
*
* \file main.c
* \brief Fichier main.
* \author Franke
*
*/
#include "io.h"


#if MODETEXTE


int main (int argc, char ** argv) {
	
	if (argc != 2 )
	{
		printf("usage : main <fichier grille>");
		return 1;
	}

	signal(SIGINT, handle_sigint);

	grille g, gc;
	init_grille_from_file(argv[1],&g);
	alloue_grille (g.nbl, g.nbc, &gc);
	affiche_grille(g);
	
	debut_jeu(&g, &gc);

	libere_grille(&g);
	libere_grille(&gc);
	return 0;
}

#else

cairo_surface_t *cs;


int main (int argc, char ** argv) {
	int initialisationErreur = 0;

	if (argc != 2 ) {
		printf("usage : main <fichier grille>\n");
		return 1;
	}

	grille g, gc;

	char fichierGrille[100] = "";
	strcat(fichierGrille, argv[1]);
	initialisationErreur = init_grille_from_file(fichierGrille,&g);
	if (initialisationErreur) {
		printf("Erreur : Le fichier \"%s\" est introuvable\n", fichierGrille);
		printf("usage : main <fichier grille>\n");
		return 1;
	}

	alloue_grille (g.nbl, g.nbc, &gc);

		/*
		=====================
		=== CAIRO VERSION ===
		=====================
		*/

		cs = cairo_create_x11_surface0(SIZEX, SIZEY);	
		
		debut_jeu(&g, &gc);
		cairo_close_x11_surface(cs); // destroy cairo surface

	libere_grille(&g);
	libere_grille(&gc);

	return 0;
}

#endif
