/*
 * File:   arbre.h
 * Author: troll
 *
 * Created on March 27, 2012, 12:41 AM
 */

#ifndef ARBRE_H
#define	ARBRE_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef MAP
#define MAPTREE
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef CPP
//* Note : utilisable uniquement en C++, désactivé ici
#include <chrono>
#include "iostream"
#endif

#define MAXMOT 256

#define H_NOEUD_VIDE -1
#define MAX_FACTORS 30

/*typedef struct t_element {
	unsigned elt;
} t_element;*/

typedef int t_element;

typedef struct t_noeud {
	//t_element valeur;
	t_element cle;
	t_element* valeur;
	unsigned val_size;
	int hauteur;
	struct t_noeud *gauche;
	struct t_noeud *droit;
} t_arbre;

t_arbre *inserer_arbre(t_arbre *a, t_element v, t_element* factors, unsigned v_size); // Déclaration, utile pour la suite, la définitione est plus bas.
t_arbre *equilibrer_arbre(t_arbre *a);
t_arbre *creer_arbre(t_element e, t_element* valeur, t_arbre *g, t_arbre *d);


#ifdef	__cplusplus
}
#endif

#endif	/* ARBRE_H */

