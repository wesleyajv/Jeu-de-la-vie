/**
 * \file grille.h
 * \brief Header file du fichier grille.c
 * \author Ajavon Wesley
*/
#ifndef __GRILLE_H
#define __GRILLE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


extern int vieillissement;



/**
* \struct grille
* \brief structure grille : nombre de lignes, nombre de colonnes, tableau de tableau de cellules
* \typedef struct grille
*/
typedef struct {
	int nbl; //!< Nombre de lignes
	int nbc; //!< Nombre de colonnes
	int** cellules; //!< tableau à deux dimensions
} grille;
 
/**
* \brief alloue une grille de taille l*c, et initialise toutes les cellules à mortes
* \param [in] l Nombre de lignes
* \param [in] c Nombre de colonnes
* \param [in] g Pointeur vers une structure grille
* \relates grille
*/
void alloue_grille (int l, int c, grille* g);

/**
* \brief libère une grille
* \param [in] g Pointeur vers une structure grille
* \relates grille
*/
void libere_grille (grille* g);


/**
* \brief rend vivante la cellule (i,j) de la grille g
* \param [in] i Index de la ligne
* \param [in] j Index de la colonne
* \param [in] g Structure grille
* \relates grille
*/
static inline void set_vivante(int i, int j, grille g){g.cellules[i][j] = 1;}

/**
* \brief rend morte la cellule (i,j) de la grille g
* \param [in] i Index de la ligne
* \param [in] j Index de la colonne
* \param [in] g Structure grille
* \relates grille
*/
static inline void set_morte(int i, int j, grille g){g.cellules[i][j] = 0;}

/**
* \brief teste si la cellule (i,j) de la grille g est vivante
* \param [in] i Index de la ligne
* \param [in] j Index de la colonne
* \param [in] g Structure grille
* \relates grille
* \return 1 si la cellule est vivante, 0 dans le cas contraire.
*/
static inline int est_vivante(int i, int j, grille g){return g.cellules[i][j] > 0;}

/**
* \brief recopie gs dans gd (sans allocation)
* \param [in] gs Grille source
* \param [out] gd Grille destination
* \relates grille
*/
void copie_grille (grille gs, grille gd);

/**
* \brief rend non viable la cellule (i,j) de la grille g 
* \param [in] x Index de la ligne
* \param [in] y Index de la colonne
* \param [in] g Structure grille
* \relates grille
*/
static inline void set_nonViable(int x, int y, grille g) {g.cellules[x][y] = -1;}

/**
* \brief teste si la cellule (i,j) de la grille g est non viable
* \param [in] i Index de la ligne
* \param [in] j Index de la colonne
* \param [in] g Structure grille
* \relates grille
* \return 1 si la cellule est non viable, 0 dans le cas contraire.
*/
static inline int est_nonViable(int i, int j, grille g) {return g.cellules[i][j] == -1;}


/**
 * Vérifie si les grilles sont égales
 * @param[in]  g1  Première grille à comparer
 * @param[in]  g2  Seconde grille
 * @return     int 1 si égales, 0 sinon
 */
int grillesEgales(grille *g1, grille *g2);

/**
 * Vérifie si une grille est vide (<=> aucune cellule vivante)
 * @param[in]  g   Grille à tester
 * @return     int 1 si vide, 0 sinon
 */
int grilleVide(grille *g);


/**
 * Vérifie si les grilles sont égales
 * @param[in]  g                      Grille à tester
 * @param[in]  compte_voisins_vivants Pointeur sur une fonction de comptage des voisins vivants d'une cellule
 * @param[in]  vieillissement         Indique si le vieillissement est activé (1) ou non (0)
 * @return int                        Temps d'une période (en pas de temps), sinon 0 (si la grille n'est pas oscillante)
 */

#if MODETEXTE

/**
 * \brief fait vieillir la cellule d'1 an
 * \param [in] i Index de la ligne
 * \param [in] j Index de la colonne
 * \param [in] g Structure grille
 * \relates grille
 */
static inline void vieillir(int i, int j, grille g) {if(vieillissement == 1) g.cellules[i][j] += 1; else g.cellules[i][j] = 1;}


/**
 * Vérifie si les grilles sont égales
 * @param[in]  g                      Grille à tester
 * @return int                        Temps d'une période (en pas de temps), sinon 0 (si la grille n'est pas oscillante)
 */
int grilleOscillante(grille *g);
#else

int grilleOscillante(grille *g, int (*compte_voisins_vivants) (int, int, grille), int vieillissement);


#endif

/**
* \brief alloue et initalise la grille g à partir d'un fichier
* \param [in] filename Nom du fichier que l'on veut ouvrir
* \param [in] g Pointeur vers une structure grille
*/
int init_grille_from_file (char * filename, grille* g);
#endif
