/**
 * \file jeu.h
 * \brief Header file du fichier jeu.c
 * \author Ajavon Wesley
 */

#ifndef __JEU_H
#define __JEU_H

#include "grille.h"

#if MODETEXTE

/**
 * \brief sert de déclencheur pour le pointeur de fonction
 */
extern int cyclique;


/**
* \brief modulo modifié pour traiter correctement les bords i=0 et j=0
* dans le calcul des voisins avec bords cycliques
*
* \param [in] i Entier dont on veut le modulo
* \param [in] m Diviseur
* \return Le modulo m de la somme i + m.
*/
static inline int modulo(int i, int m) {return (i+m)%m;}

/**
*\brief compte le nombre de voisins vivants de la cellule (i,j)
* les bords sont cycliques.
*
* \param [in] i Index de la ligne
* \param [in] j Index de la colonne
* \param [in] g Structure de grille
* \relates grille
* \return Un entier correspondant aux nombres de cellules voisines vivantes.
*/
int compte_voisins_vivants_cyclique (int i, int j, grille g);

/**
* \brief compte le nombre de voisins vivants de la cellule (i,j)
* les bords sont non-cycliques.
* \param [in] i Index de la ligne
* \param [in] j Index de la colonne
* \param [in] g Structure de grille
* \relates grille
* \return Un entier correspondant aux nombres de cellules voisines vivantes
*/
int compte_voisins_vivants_non_cyclique (int i, int j, grille g);

/**
* \brief fait évoluer la grille g d'un pas de temps
* \param [in] g Pointeur vers une grille.
* \param [in] gc Pointeur de la copie de g.
* \relates grille
*/
void evolue (grille *g, grille *gc);

#else

/**
 * Modulo modifié pour traiter correctement le calcul des voisins avec bords cycliques
 * @param[in]  i Première opérande
 * @param[in]  m Seconde opérande
 * @return       Valeur du modulo modifié
 */
static inline int modulo(int i, int m) {return (i+m)%m;}

/**
 * Compte le nombre de voisins vivants de la cellule (i,j). Les bords sont cycliques.
 * @param[in]  i Ligne de la cellule
 * @param[in]  j Colonne de la cellule
 * @param[in]  g Grille dans laquelle se trouve cette cellule et ses voisins à compter
 * @return       Nombre de voisins vivants
 */
int compte_voisins_vivants_cyclique (int i, int j, grille g);

/**
 * Compte le nombre de voisins vivants de la cellule (i,j). Les bords sont non cycliques.
 * @param[in]  i Ligne de la cellule
 * @param[in]  j Colonne de la cellule
 * @param[in]  g Grille dans laquelle se trouve cette cellule et ses voisins à compter
 * @return       Nombre de voisins vivants
 */
int compte_voisins_vivants_non_cyclique (int i, int j, grille g);

/**
 * Fait évoluer la grille g d'un pas de temps
 * @param[in]  g                      Pointeur sur la grille à faire évoluer
 * @param[out] gc                     Pointeur sur la grille évoluée
 * @param[in]  tempsEvolution         Entier représentant le temps (pas) d'évolution de la grille en cours
 * @param[in]  compte_voisins_vivants Pointeur sur une fonction de comptage des voisins vivants d'une cellule
 * @param[in]  vieillissement         Indique si le vieillissement est activé (1) ou non (0)
 */
void evolue (grille *g, grille *gc, int *tempsEvolution, int (*compte_voisins_vivants) (int, int, grille), int vieillissement);

#endif

#endif
