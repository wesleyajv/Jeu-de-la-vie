/**
*
* \file io.c
* \brief Implémentation de l'input et de l'output des grilles.
* \author Ajavon Wesley
*/

#include "io.h"

#if MODETEXTE

int firstCall = 1;
int compteur = 0;
int vieillissement = 0;


void affiche_trait (int c){

	int i;
	for (i=0; i<c; ++i) printf ("|---");
	printf("|\n");
	return;
}

void affiche_ligne (int c, int* ligne){
	int i;
	for (i=0; i<c; ++i) 
		if (ligne[i] == 0 ) printf ("|   ");
		else if(ligne[i] == -1){
			printf("| X ");
		}
		else printf ("| %d ",vieillissement == 1 ? ligne[i]: 0);
	printf("|\n");
	return;
}

void affiche_grille (grille g){
	if(firstCall) {
		printf("Temps d'évolution: %i\n", compteur++);
		firstCall = 0;
	}

	int i, l=g.nbl, c=g.nbc;
	printf("\n");
	affiche_trait(c);
	for (i=0; i<l; ++i) {
		affiche_ligne(c, g.cellules[i]);
		affiche_trait(c);
	}	
	printf("\n"); 
	return;
}

void efface_grille (grille g){
	printf("\n\033[%dA",g.nbl*2 + 5);
}

void handle_sigint(int sig)
{
    printf("Caught signal %d\n", sig);
    exit(0);
}

void debut_jeu(grille *g, grille *gc){

	char c = getchar();
	while (c != 'q') // touche 'q' pour quitter
	{ 

		switch (c) {
			case '\n' : 
			{ // touche "entree" pour évoluer
				printf("Temps d'évolution: %i\n", compteur++);
				evolue(g,gc);
				efface_grille(*g);
				affiche_grille(*g);
				break;
			}
			case 'n' : {
				int valide = 0;

				char * filename = NULL;
				filename = malloc(sizeof(char) * 20);


				printf("Introduisez le nom du fichier que vous voulez charger:\n");
				while ((c = getchar()) != EOF && c != '\n');
				while(valide == 0) {
					if(fgets(filename, sizeof(char) * 20, stdin) != NULL) {
						int nCaractere = strlen(filename);
						if(nCaractere > 19){
							printf("La chaine de caractères est trop longue !\n");
							return;
						} else {
							valide = 1;
							}
					} else {
						printf("Could not read the input.");
						return;
					}
				}
				init_grille_from_file(filename, g);
				alloue_grille (g->nbl, g->nbc, gc);
				compteur = 0;
				firstCall = 1;
				affiche_grille(*g);
				while ((c = getchar()) != EOF && c != '\n');
				debut_jeu(g, gc);
				return;
			}
			case 'c' : {
				if(cyclique == 0) {
					printf("Calcul du voisinage: cyclique\n");
					cyclique = 1;
	
				} else {
					printf("Calcul du voisinage: non-cyclique\n");
					cyclique = 0;

				}

				printf("Temps d'évolution: %i\n", --compteur); // Décrémente dans le cas où on appuie 'v' lors de l'affichage du premier tableau
				efface_grille(*g);
				affiche_grille(*g);
				compteur++;
				while ((c = getchar()) != EOF && c != '\n');
				break;
			}
			case 'v' : {
				if(vieillissement == 0) {
					printf("Affichage de l'âge des cellules: OUI\n");
					vieillissement = 1;
				} else {
					printf("Affichaghe de l'âge des cellules: NON\n");
					vieillissement = 0;
			}

				printf("Temps d'évolution: %i\n", --compteur);
				efface_grille(*g);
				affiche_grille(*g);
				compteur++;
				while ((c = getchar()) != EOF && c != '\n');
				break;
			}
			case 'o' : {
				int tempsOscillation = grilleOscillante(g);
				if(!tempsOscillation) {
					printf("Oscillation : Grille non oscillante\n");
				} else {
					printf("Temps d'oscillation : %d pas/période\n", tempsOscillation);
				}
				while ((c = getchar()) != EOF && c != '\n');
				break;
			}
			default : 
			{ // touche non traitée
				printf("\n\033[1A");
				break;
			}
		}

		c = getchar(); 
	}
	return;	
}

#else
/*
=====================
=== CAIRO VERSION ===
=====================
*/

#define MARGE_GAUCHE_GRILLE 25
#define MARGE_DROITE_GRILLE 25
#define MARGE_HAUTE_GRILLE 60
#define LARGEUR_GRILLE 450
#define HAUTEUR_GRILLE 400
#define CAIRO_LINE_WIDTH 2

extern cairo_surface_t *cs;
XClassHint *classHint;

cairo_surface_t *cairo_create_x11_surface0(int x, int y) {
    Display *dpy;
    Drawable win;
    int screen;
    Window rootwin;
    cairo_surface_t *cs;

    if ((dpy = XOpenDisplay(NULL)) == NULL) {
		fprintf(stderr, "ERROR: Could not open display\n");
        exit(1);
	}
    screen = DefaultScreen(dpy);
    rootwin = RootWindow(dpy, screen);

    unsigned long background = 0x00871E42;
    win=XCreateSimpleWindow(dpy, rootwin, 1, 1, x, y, 0, background, background);

    XSelectInput(dpy, win, ExposureMask|ButtonPressMask|KeyPressMask);
    XMapWindow(dpy, win);


    XStoreName(dpy, win, "Jeu de la vie");

    classHint = XAllocClassHint();
    if (classHint) {
	classHint->res_name = "Jeu de la vie";
	classHint->res_class = "Jeu de la vie";
	XSetClassHint(dpy, win, classHint);
	XFree(classHint);
    }

    Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False); 
    XSetWMProtocols(dpy, win, &wm_delete_window, 1);

    cs = cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, screen), x, y);
    cairo_xlib_surface_set_size(cs, x, y);

    return cs;
}

void cairo_close_x11_surface() {
   Display *dpy = cairo_xlib_surface_get_display(cs);
   cairo_surface_destroy(cs);
   XCloseDisplay(dpy);
}

void affiche_ligne (int c, int* ligne, int vieillissement, int hauteur, float tailleLigneGrille){
	int i;
	char ageBuffer[255];
	float tailleColonneGrille = (float) LARGEUR_GRILLE / (float) c;

	cairo_t *cr, *crcells;
	cr = cairo_create(cs);
	crcells = cairo_create(cs);
	
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_set_source_rgb(crcells, 0.0, 0.0, 0.0);
	cairo_set_line_width(cr, CAIRO_LINE_WIDTH);
	
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille));


	for (i=0; i<c; ++i) {

		if (ligne[i] == 0) {
			cairo_line_to(cr, MARGE_GAUCHE_GRILLE + i * tailleColonneGrille, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + tailleLigneGrille);
			cairo_move_to(cr, MARGE_GAUCHE_GRILLE + ((i+1) * tailleColonneGrille), MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille));
			cairo_line_to(cr, MARGE_GAUCHE_GRILLE + ((i+1) * tailleColonneGrille), MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + tailleLigneGrille);
		} else if (ligne[i] == -1) {
			cairo_line_to(cr, MARGE_GAUCHE_GRILLE + i * tailleColonneGrille, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + tailleLigneGrille);
			cairo_line_to(cr, MARGE_GAUCHE_GRILLE + ((i+1) * tailleColonneGrille), MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + 1);
			cairo_line_to(cr, MARGE_GAUCHE_GRILLE + ((i+1) * tailleColonneGrille), MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + tailleLigneGrille);
			cairo_line_to(cr, MARGE_GAUCHE_GRILLE + i * tailleColonneGrille, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + tailleLigneGrille);
			cairo_move_to(cr, MARGE_GAUCHE_GRILLE + ((i) * tailleColonneGrille), MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille));
			cairo_line_to(cr, MARGE_GAUCHE_GRILLE + ((i+1) * tailleColonneGrille), MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + tailleLigneGrille);
			cairo_line_to(cr, MARGE_GAUCHE_GRILLE + ((i+1) * tailleColonneGrille), MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille));
		} else {
			cairo_line_to(cr, MARGE_GAUCHE_GRILLE + i * tailleColonneGrille, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + tailleLigneGrille);
			cairo_move_to(cr, MARGE_GAUCHE_GRILLE + ((i-1) * tailleColonneGrille) + 2, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + 2);
			cairo_rectangle(crcells, MARGE_GAUCHE_GRILLE + i * tailleColonneGrille + 3, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille + 3),	tailleColonneGrille - 6, tailleLigneGrille - 6);
			cairo_fill(crcells);
			
			if (vieillissement) {
				sprintf(ageBuffer, "%d", ligne[i]);
				cairo_move_to(cr, MARGE_GAUCHE_GRILLE + ((i) * tailleColonneGrille) + tailleColonneGrille/2 - 5,
				MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille) + (tailleLigneGrille*2/3) + 2);
				cairo_set_source_rgb (cr, 0.75, 0.75, 0.75);
				cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
				cairo_set_font_size(cr, 25);
				cairo_show_text(cr, ageBuffer);

				cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
			}

			cairo_move_to(cr, MARGE_GAUCHE_GRILLE + ((i+1) * tailleColonneGrille), MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille));
		}
	}

	cairo_fill(crcells);
	cairo_stroke(cr);
	cairo_destroy(cr);
	cairo_destroy(crcells);
		
	return;
}

void affiche_trait (int c, int hauteur, float tailleLigneGrille){
	cairo_t *cr;
	cr = cairo_create(cs);
	
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_set_line_width(cr, CAIRO_LINE_WIDTH);
	
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille));

	int i;
	for (i = 0; i < c; ++i) {
	    cairo_line_to(cr, MARGE_GAUCHE_GRILLE + LARGEUR_GRILLE, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille));
	    cairo_move_to(cr, MARGE_GAUCHE_GRILLE, MARGE_HAUTE_GRILLE + (hauteur * tailleLigneGrille));
	}
	cairo_stroke(cr);
	cairo_destroy(cr);
}

void affiche_grille (grille g, int tempsEvolution, int comptageCyclique, int vieillissement, int tempsOscillation){

	int i, l=g.nbl, c=g.nbc;
	char strTemps[255], strComptageCyclique[255], strVieillissement[255], strOscillation[255];
	sprintf(strTemps, "- Temps d'évolution : %d", tempsEvolution);
	sprintf(strComptageCyclique, comptageCyclique ? "- Comptage : Cyclique" : "- Comptage : Non-cyclique");
	sprintf(strVieillissement, vieillissement ? "- Vieillissement : Active" : "- Vieillissement : Desactive");

	if (tempsOscillation == -1) {
	    sprintf(strOscillation, "- Oscillation : Non testee");
	} else if (tempsOscillation == 0) {
	    sprintf(strOscillation, "- Oscillation : Non oscillant");
	} else {
	    sprintf(strOscillation, "- Oscillation : %d pas/période", tempsOscillation);
	}

	cairo_t *cr;
	cr = cairo_create(cs);

	cairo_set_source_rgb(cr, 0.1, 0.0, 0.0);

	cairo_select_font_face(cr, "Arial",
		CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_BOLD);

	cairo_set_font_size(cr, 26);
	cairo_move_to(cr, 175, 40);
	cairo_show_text(cr, "Jeu de la vie");

	cairo_set_font_size(cr, 22);
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE, 485);

	cairo_show_text(cr, "Commandes :");

	cairo_move_to(cr, MARGE_GAUCHE_GRILLE, 770);
	cairo_show_text(cr, "Informations :");

	cairo_set_source_rgb(cr, 0.18, 0.0, 0.0);

	cairo_set_font_size(cr, 18);
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 10, 515);
	cairo_show_text(cr, "- Entrée / clic gauche : Fait évoluer la grille");
	cairo_set_font_size(cr, 18);
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 10, 550);
	cairo_show_text(cr, "- n : Charger une nouvelle grille");
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 10, 585);
	cairo_show_text(cr, "- c : Passer en mode cyclique/non-cyclique");
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 10, 620);
	cairo_show_text(cr, "- v : Activer/desactiver le vieillissement");
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 10, 655);
	cairo_show_text(cr, "- d : Ouvrir la doc (doxygen & firefox requis)");
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 10, 690);
	cairo_show_text(cr, "- q / clic droit : Quitter le programme");
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 10, 725);
	cairo_show_text(cr, "- o : Tester si la grille est oscillante");
	
	cairo_set_font_size(cr, 15);

	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 10, 800);
	cairo_show_text(cr, strTemps);
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 10, 835);
	cairo_show_text(cr, strComptageCyclique);  
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 250, 800);
	cairo_show_text(cr, strVieillissement);  
	cairo_move_to(cr, MARGE_GAUCHE_GRILLE + 250, 835);
	cairo_show_text(cr, strOscillation);  

	cairo_destroy(cr);

	int hauteur = 0;
	float tailleLigne = (float) HAUTEUR_GRILLE / (float) g.nbl;
	affiche_trait(c, hauteur, tailleLigne);
	for (i=0; i<l; ++i) {
	    affiche_ligne(c, g.cellules[i], vieillissement, hauteur, tailleLigne);
	    affiche_trait(c, ++hauteur, tailleLigne);
	}
	return;
}

void drawTextInput(char *input, char *erreur) {
	cairo_t *cr;
	cr = cairo_create(cs);

	char inputLabel[255];
	if (strcmp(erreur, "") == 0) {
		sprintf(inputLabel, "Veuillez indiquer le numero de la grille a charger :");
	} else {
		sprintf(inputLabel, "%s :", erreur);
	}
	

	cairo_text_extents_t extentsInput;
	cairo_text_extents_t extentsInputLabel;

	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, 0, 855, SIZEX, SIZEY-855);
	cairo_fill(cr);

	cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);

	cairo_select_font_face(cr, "Arial",
	    CAIRO_FONT_SLANT_NORMAL,
	    CAIRO_FONT_WEIGHT_NORMAL);

	cairo_set_font_size(cr, 18);

	cairo_move_to(cr, 15, 880);
	cairo_show_text(cr, inputLabel);

	cairo_text_extents (cr, inputLabel, &extentsInputLabel);

	cairo_move_to(cr, 23 + extentsInputLabel.width + 2, 880);
	cairo_show_text(cr, input);

	cairo_text_extents (cr, input, &extentsInput);

	int inputOffset = 23 + extentsInputLabel.width + 5 + extentsInput.width;
	cairo_move_to(cr, inputOffset, 860);
	cairo_set_line_width(cr, 1);
	cairo_line_to(cr, inputOffset, 885);
	cairo_stroke(cr);


	cairo_destroy(cr);  
}

void efface_grille () {
	cairo_t *cr;
	cr = cairo_create(cs);
	cairo_set_source_rgb (cr, 0.529, 0.118, 0.259);
	cairo_paint(cr);
	cairo_destroy(cr);
}

void debut_jeu(grille *g, grille *gc) {
	int tempsEvolution = 1;
	int vieillissement = 0;
	int refreshGrille = 0;
	int endGame = 0;
	int tempsOscillation = -1; // -1 par défaut => oscillation non testée

	int comptageCyclique = 1;
	int (*compte_voisins_vivants) (int, int, grille) = compte_voisins_vivants_cyclique;

	XEvent e;
	
	KeySym key;
	char inputBuffer[255];

	while(1) {
		XNextEvent(cairo_xlib_surface_get_display(cs), &e);
		
		if (e.type==Expose && e.xexpose.count<1) {
		    affiche_grille(*g, tempsEvolution, comptageCyclique, vieillissement, tempsOscillation);
		} else if (e.type == KeyPress) { // Touche pressée
			if (e.xkey.keycode == 36 || e.xkey.keycode == 104) { // Touche entrée (ou entrée numpad)
			    evolue(g,gc,&tempsEvolution,compte_voisins_vivants,vieillissement);
			    refreshGrille = 1;
			} else if (e.xkey.keycode == 57) { // Touche n
			    tempsOscillation = -1;

			    drawTextInput("", "");
			    int exitGridChange = 0;
			    int erreurInitialisation = 0;
			    FILE * testFile = NULL;

				do {
				    char numeroGrille[10] = "";
				    char fichierGrille[100] = "grilles/grille";
					
					while (1) {
					    XNextEvent(cairo_xlib_surface_get_display(cs), &e);
					    if (e.type == KeyPress && XLookupString(&e.xkey,inputBuffer,255,&key,0)==1) {
						if (e.xkey.keycode == 36 || e.xkey.keycode == 104) {
						    break;
						    } else if (e.xkey.keycode == 22) { // Effacer
							    numeroGrille[strlen(numeroGrille)-1] = '\0';
							} else if (e.xkey.keycode == 9) { // Escape
							    exitGridChange = 1;
							    break;
							} else {
							    strcat(numeroGrille, inputBuffer);
							}
							drawTextInput(numeroGrille, "");
						}
					}

					if (!exitGridChange) {
					    strcat(fichierGrille, numeroGrille);
					    strcat(fichierGrille, ".txt");
					    testFile = fopen(fichierGrille, "r");
					    if (testFile != NULL) {
						libere_grille(g);
						libere_grille(gc);
						erreurInitialisation = init_grille_from_file(fichierGrille, g);
						if (erreurInitialisation) {
						    printf("Erreur : Le fichier grille \"%s\" est introuvable\n", fichierGrille);
						    drawTextInput("", "grille introuvable");
						    }
						    fclose(testFile);
						    testFile = NULL;
						} else {
						    erreurInitialisation = 1;
						    drawTextInput("", "fichier introuvable");
						}
					}
				} while (erreurInitialisation && !exitGridChange);

				if (!exitGridChange) {
				    tempsEvolution = 1; // Réinitialisation du temps
				    alloue_grille (g->nbl, g->nbc, gc);
				}
				refreshGrille = 1;

			} else if (e.xkey.keycode == 54) { // Touche c
				// voisinnage cyclique / non-cyclique
				if (comptageCyclique) { // On repasse à un comptage non-cyclique
				    comptageCyclique = 0;
				    compte_voisins_vivants = &(compte_voisins_vivants_non_cyclique);
				} else { // On repasse à un comptage cyclique
				    comptageCyclique = 1;
				    compte_voisins_vivants = &(compte_voisins_vivants_cyclique);
				}
				refreshGrille = 1;
			} else if (e.xkey.keycode == 55) { // Touche v
			    vieillissement = !vieillissement;
			    refreshGrille = 1;
			} else if (e.xkey.keycode == 40) {
			    if(system("doxygen && firefox ./doc/html") == 0) {
				drawTextInput("","Couldn't load documentation.");
			    }
			} else if (e.xkey.keycode == 32) { // Touche o (oscillation)
			    tempsOscillation = grilleOscillante(g, compte_voisins_vivants, vieillissement);
			    refreshGrille = 1;
			} else if (e.xkey.keycode == 38) { // Touche q
			    endGame = 1;
			}
		} else if (e.type == ButtonPress) {
		    if (e.xbutton.button == 1) { // Clic gauche (fait évoluer le jeu)
		        evolue(g,gc,&tempsEvolution,compte_voisins_vivants,vieillissement);
		        refreshGrille = 1;
		    } else if (e.xbutton.button == 3) { // Clic droit (quitte le jeu)
		        endGame = 1;
		    }
		} else if (e.type == ClientMessage) {
		    // Le seul msg qu'on peut recevoir est celui de fermeture : test non nécessaire
		    endGame = 1;
		}

		if (endGame) {
		    printf("=== Fin du programme. A bientot ! ===\n");
		    return;
		}

		if (refreshGrille) {
		    efface_grille();
		    affiche_grille(*g, tempsEvolution, comptageCyclique, vieillissement, tempsOscillation);
		    refreshGrille = 0;
		}
	}
    return;
}

#endif


