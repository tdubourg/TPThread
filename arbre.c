
//* Put the defines here :
//#define CPP
//#define TO_CSV;

#include "arbre.h"

// cr�ation d'un arbre binaire

t_arbre *creer_arbre(t_element e, t_element* valeur, t_arbre *g, t_arbre *d) {
	t_arbre *arbre = (t_arbre*) malloc(sizeof (t_arbre));
	arbre->gauche = g;
	arbre->droit = d;
	arbre->cle = e;
	arbre->valeur = valeur;
	arbre->hauteur = 0;

	return arbre;
}

// destruction d'un noeud de l'arbre, cette fonction renvoie l'�l�ment,

t_element detruire_noeud_arbre(t_arbre *noeud) {
	//t_element el = (t_element ) malloc(sizeof(t_element));
	//*el = *(noeud->cle);
	t_element el = noeud->cle;
	//free(noeud->valeur);
	//noeud->valeur = NULL;
	free(noeud);
	return el;
}

// destruction d'un arbre binairegedit

void detruire_arbre(t_arbre *a) {
	//* Si l'arbre a des sous arbres, on détruit d'abord récursivement les sous arbres :
	if (a->gauche != NULL) {
		detruire_arbre(a->gauche);
	}

	if (a->droit != NULL) {
		detruire_arbre(a->droit);
	}

	//* Quand le ménage "plus bas" est terminé, on détruit ce qui reste, le noeud courant :
	detruire_noeud_arbre(a);
}

// insertion d'un t_element v dans un t_arbre a, cette fonction renvoie le nouvel arbre

t_arbre *inserer_arbre(t_arbre *a, t_element v, t_element* factors, unsigned v_size) {
	t_arbre* curr = a;
	t_arbre *prec = NULL;
	int left = 0;
	while (curr != NULL) {
		t_element e = curr->cle;
		if (e > v) {
			// On descend dans la branche gauche de ce noeud
			// car la valeur du noeud courant est plus grande
			// que celle à insérer
			prec = curr;
			curr = curr->gauche;
			left = 1;
		} else if (e < v) {
			// On descend dans l'arbre droit cette fois
			prec = curr;
			curr = curr->droit;
			left = 0;
		} else {//* l'élément existe déjà ds l'arbre on peut pas insérer
			return a;
		}
	}

	t_arbre *n = creer_arbre(v, factors, NULL, NULL);
	n->val_size = v_size;

	if (left) {
		prec->gauche = n;
	} else {
		prec->droit = n;
	}

	return a;
}

//suppression du minimum necessaire à supprimer_arbre

t_element supprimer_min(t_arbre *a) {
	t_arbre *c;
	t_element tmp;
	if (a->gauche == NULL) {
		tmp = (a->cle);
		c = a;
		a = a->droit;
		detruire_noeud_arbre(c);
		return tmp;
	} else {
		return supprimer_min(a->gauche);
	}
}


// recherche d'un t_element e dans le t_arbre a, cette fonction renvoie la r�f�rence du noeud dans l'arbre

t_arbre *rechercher_arbre(t_arbre *a, t_element v) {

	t_arbre* curr = a;
	while (curr != NULL) {
		t_element e = curr->cle;
#ifdef MAPTREE
		printf("curr elem key : %u", e);
#endif
		if (e > v) {
			// On descend dans la branche gauche de ce noeud
			// car la valeur du noeud courant est plus grande
			// que celle à insérer
			curr = curr->gauche;
		} else if (e < v) {
			// On descend dans l'arbre droit cette fois
			curr = curr->droit;
		} else {// ==
			return curr;
		}
	}
#ifdef MAPTREE
		printf("NOT FOUND \n");
#endif
	return curr;
}



// calculer la hauteur des noeuds du t_arbre a

int calculer_hauteur(t_arbre *a) {
	if (a->gauche == NULL && a->droit == NULL) {
		a->hauteur = 0;
	} else {
		if (a->gauche != NULL) {
			a->hauteur = 1 + calculer_hauteur(a->gauche);
		}
		if (a->droit != NULL) {
			int curr;
			curr = 1 + calculer_hauteur(a->droit); //on prend la valeur maximum de la hauteur.
			if (curr > a->hauteur) {
				a->hauteur = curr;
			}
		}
	}
	return a->hauteur;
}


// afficher l'arbre et la hauteur des noeuds

void afficher_arbre(t_arbre *a) {
	static int hauteur_courante = 0;
	hauteur_courante++;
	int i;
	for (i = 1; i <= hauteur_courante; i++) {
		printf("\t");
	}
	printf("(Noeud) Cle : %u ; hauteur : %u\n", a->cle, a->hauteur);
	if (a->gauche != NULL) {
		for (i = 1; i <= hauteur_courante; i++) {
			printf("\t");
		}
		printf("SAG : \n");
		afficher_arbre(a->gauche);
	}
	if (a->droit != NULL) {
		for (i = 1; i <= hauteur_courante; i++) {
			printf("\t");
		}
		printf("SAD : \n");
		afficher_arbre(a->droit);
	}
	hauteur_courante--;
}

t_arbre *maj_hauteur_depuis_enfants(t_arbre *x) {
	int hauteur_gauche = (x->gauche != NULL) ? x->gauche->hauteur : H_NOEUD_VIDE;
	int hauteur_droit = (x->droit != NULL) ? x->droit->hauteur : H_NOEUD_VIDE;

	if (x->gauche == NULL && x->droit == NULL) { //* Si x est une feuille : 0
		x->hauteur = 0;
	} else { //* Sinon : on calcul a partir de ce qu'il y a en dessous
		x->hauteur = 1
			+ ((hauteur_gauche > hauteur_droit) ?
			hauteur_gauche : hauteur_droit);
	}

	return x;
}

t_arbre *rotation_droite(t_arbre *y) {
	t_arbre *b = y->gauche->droit;
	t_arbre *x = y->gauche;

	x->droit = y;
	y->gauche = b;

	//* Note : les hauteurs de A, B et C ne sont pas changées durant la rotation
	//* On MàJ celles de x et y à partir de celles de A, B et C
	y = maj_hauteur_depuis_enfants(y);

	int hauteur_droit = (x->droit != NULL) ? x->droit->hauteur : H_NOEUD_VIDE;
	x->hauteur = 1
		+ ((hauteur_droit > x->hauteur) ? hauteur_droit : x->hauteur);

	return x;
}

t_arbre *rotation_gauche(t_arbre *x) {
	t_arbre *b = x->droit->gauche;
	t_arbre *y = x->droit;

	x->droit = b;
	y->gauche = x;

	//* Note : les hauteurs de A, B et C ne sont pas changées durant la rotation
	//* On MàJ celles de x et y à partir de celles de A, B et C
	x = maj_hauteur_depuis_enfants(x);

	int hauteur_droit = (y->droit != NULL) ? y->droit->hauteur : H_NOEUD_VIDE;
	y->hauteur = 1
		+ ((hauteur_droit > x->hauteur) ? hauteur_droit : x->hauteur);

	return y;
}

t_arbre *equilibrer_arbre(t_arbre *a) {
	if (a->gauche != NULL && a->gauche->gauche != NULL) { // si x,y,a,b existent
		// Possibilité d'une rotation droite, peut-on le faire ?
		int hauteur_B =
			(a->gauche->droit != NULL) ?
			a->gauche->droit->hauteur : H_NOEUD_VIDE;
		int hauteur_C = (a->droit != NULL) ? a->droit->hauteur : H_NOEUD_VIDE;
		int hauteur_A = a->gauche->gauche->hauteur;
		int max = (hauteur_B > hauteur_C) ? hauteur_B : hauteur_C; // max(h(b), h(c))
		while (hauteur_A >= max) { // si h(a) > max(h(b), h(c))
			// Lancement d'une rotation droite :
			a = rotation_droite(a);
			if (a->gauche != NULL && a->gauche->gauche != NULL) { // si x,y,a,b existent
				hauteur_B =
					(a->gauche->droit != NULL) ?
					a->gauche->droit->hauteur : H_NOEUD_VIDE;
				hauteur_C =
					(a->droit != NULL) ? a->droit->hauteur : H_NOEUD_VIDE;
				hauteur_A = a->gauche->gauche->hauteur;
				max = (hauteur_B > hauteur_C) ? hauteur_B : hauteur_C; // max(h(b), h(c))
			} else {
				break;
			}
		}
	}
	if (a->droit != NULL && a->droit->droit != NULL) { // si x,y,c existent (on peut faire sans a et b, rotation_gauche() sait faire) //* @IMPORTANT : (by td) J'ai supposé que la condition appliquable pr la rotation droite (décrite ds l'énoncé) était appliquable pr la rotation gauche
		// Possibilité d'une rotation gauche, peut-on le faire ?
		int hauteur_B =
			(a->droit->gauche != NULL) ?
			a->droit->gauche->hauteur : H_NOEUD_VIDE;
		int hauteur_A = (a->gauche != NULL) ? a->gauche->hauteur : H_NOEUD_VIDE;
		int hauteur_C = a->droit->droit->hauteur;
		int max = (hauteur_B > hauteur_A) ? hauteur_B : hauteur_A; // max(h(b), h(a))
		while (hauteur_C >= max) { // si h(c) > max(h(b), h(a))
			// Lancement d'une rotation droite :
			a = rotation_gauche(a);

			if (a->droit != NULL && a->droit->droit != NULL) {
				hauteur_B =
					(a->droit->gauche != NULL) ?
					a->droit->gauche->hauteur : H_NOEUD_VIDE;
				hauteur_A =
					(a->gauche != NULL) ? a->gauche->hauteur : H_NOEUD_VIDE;
				hauteur_C = a->droit->droit->hauteur;
				max = (hauteur_B > hauteur_A) ? hauteur_B : hauteur_A; // max(h(b), h(a))
			} else {
				break;
			}
		}
	}
	if (a->droit != NULL) {
		a->droit = equilibrer_arbre(a->droit);
		a->droit = maj_hauteur_depuis_enfants(a->droit);
	}
	if (a->gauche != NULL) {
		a->gauche = equilibrer_arbre(a->gauche);
		a->gauche = maj_hauteur_depuis_enfants(a->gauche);
	}
	a = maj_hauteur_depuis_enfants(a);
	return a;
}
