/**
* \file io.h
* \brief Header file du fichier io.h
* \author Ajavon Wesley
*
*/

#ifndef __IO_H
#define __IO_H

#include <string.h>
#include <signal.h>
#include "jeu.h"



#if MODETEXTE


/**
* \brief affichage d'un trait horizontal
* \param [in] c Entier servant de limite dans la boucle for.
*/
void affiche_trait (int c);

/**
* \brief affichage d'une ligne de la grille
* \param [in] c Entier servant de limite dans la boucle for.
* \param [in] ligne Pointeur vers int (tableau d'entiers).
*/
void affiche_ligne (int c, int* ligne);

/**
* \brief affichage d'une grille
* \param [in] g Structure grille.
*/
void affiche_grille (grille g);

/**
* \brief effacement d'une grille
* \param [in] g Structure grille.
*/
void efface_grille (grille g);


/**
 * \brief gère le signal CTRL+C
 * \param[in] sig Valeur du signal en int.
 */
void handle_sigint(int sig);

#else

    #define SIZEX 500
    #define SIZEY 1000

    /** 
     * Création d'une surface Xlib Cairo
     * @param[in] x Largeur de la surface (en pixels)
     * @param[in] y Hauteur de la surface (en pixels)
     */
    cairo_surface_t *cairo_create_x11_surface0(int x, int y);

    /**
     * Destruction d'une surface Xlib Cairo et fermeture de la connexion à X
     */
    void cairo_close_x11_surface();

    /**
     * Affichage d'un trait horizontal (en mode GUI Cairo)
     * @param[in] c           Nombre de cellules du trait
     * @param[in] hauteur     Numéro de la hauteur du trait à tracer (en partant du haut de la grille)
     * @param[in] tailleLigne Hauteur de la ligne (en pixels)
     */
    void affiche_trait (int c, int hauteur, float tailleLigne);

    /**
     * Affichage d'une ligne de la grille (en mode GUI Cairo)
     * @param[in] c              Nombre de cellules de la ligne
     * @param[in] ligne          Tableau indiquant pour chaque cellule si elle est vivante ou morte
     * @param[in] vieillissement Indique si le vieillissement est activé (1) ou désactivé (0)
     * @param[in] hauteur        Numéro de la hauteur du trait à tracer (en partant du haut de la grille)
     * @param[in] tailleLigne    Hauteur de la ligne (en pixels)
     */
    void affiche_ligne (int c, int* ligne, int vieillissement, int hauteur, float tailleLigne);

    /**
     * Effacement de la grille sur l'interface graphique (en mode GUI Cairo)
     */
    void efface_grille ();

    /**
     * Affiche une entrée de texte au bas de l'écran
     * @param[in] input  Entrée à afficher (saisie au clavier)
     * @param[in] erreur Erreur à afficher entre parenthèses si nécessaire
     */
    void drawTextInput(char *input, char *erreur);




/**
 * Affichage d'une grille
 * @param[in] g                  Grille à afficher
 * @param[in] tempsEvolution     Entier représentant le temps (pas) d'évolution de la grille en cours
 * @param[in] comptageCyclique   Indique si le comptage est en mode cyclique (1) ou non-cyclique (0)
 * @param[in] vieillissement     Indique si le vieillissement est activé (1) ou désactivé (0)
 * @param[in] tempsOscillation   Indique le temps d'oscillation d'une grille (ou -1 si non testé ou 0 si non oscillante)
 */
void affiche_grille (grille g, int tempsEvolution, int comptageCyclique, int vieillissement, int tempsOscillation);

#endif

/**
* \brief debute le jeu
* \param [in] g Pointeur vers une grille.
* \param [in] gc Pointeur vers une copie de g.
*/
void debut_jeu(grille *g, grille *gc);


#endif
