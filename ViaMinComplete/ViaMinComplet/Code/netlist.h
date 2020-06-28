#ifndef _NETLIST_H_
#define _NETLIST_H_

struct segment;

typedef struct cell_segment{
  struct segment* seg;
  struct cell_segment* suiv;
} Cell_segment;

typedef struct segment{
  int NumRes;
  int p1, p2;
  int HouV;     // 0 = horizontal    1 = vertical
  struct cell_segment* Lintersec;
} Segment;

typedef struct point{
  double x,y;
  int num_res;
  Cell_segment* Lincid;
} Point;

typedef struct reseau{
  int NumRes;
  int NbPt;
  Point** T_Pt;
  int NbSeg;		// le nombre total de segments dans ce reseau   		 (ajout non présent dans l'énoncé)
  Segment** T_Seg;  	// un tableau de pointeurs sur tous les segments du reseau  	 (ajout non présent dans l'énoncé)
} Reseau;

typedef struct netlist{
  int NbRes;
  Reseau** T_Res;
  int NbSeg;   		// le nombre total de segments dans une NetList			  (ajout non présent dans l'énoncé)
} Netlist;

typedef struct extremite{
  double x;
  int VouGouD;  // V=0  G=1  D=2
  Segment* PtrSeg;
  int NumPt;        
} Extremite;

typedef struct cell_segment_y{
  double y;
  struct segment* seg;
  struct cell_segment_y* suiv;
} Cell_segment_y;

typedef struct noeud_AVL {
    double y;
    int hauteur;
    Segment* seg;
    struct noeud_AVL * pere;
    struct noeud_AVL * fg;
    struct noeud_AVL * fd;
} Noeud_AVL;




//=================================================================
//    FONCTIONS POUR CREER LES DIFFERENTES STRUCTURES
//=================================================================

Segment* creer_segment(int NumRes, int p1, int p2, int HouV);
Segment* copier_segment(Segment* seg);
Cell_segment* creer_cell_segment(Segment* seg);
Point* creer_point(double x, double y, int num_res);
Reseau* creer_reseau(int NumRes, int NbPt, int NbSeg);
Netlist* creer_netlist(int NbRes);
Extremite* creer_extremite(double x, int VouGouD, Segment* Seg, int NumPt);
Cell_segment_y* creer_cell_segment_y(Segment* seg, double y);

//=================================================================
//  FONCTIONS POUR DETRUIRE/SUPPRIMER LES DIFFERENTES STRUCTURES
//=================================================================

void supprimer_segment(Segment* seg);
Cell_segment* supprimer_cell_segment(Cell_segment* liste, Cell_segment* cell_a_suppr);
void supprimer_liste_cell_segment(Cell_segment* liste);
void supprimer_uniquement_cell_segment(Cell_segment* liste);
void supprimer_point(Point* p);
void supprimer_reseau(Reseau* r);
void supprimer_netlist(Netlist* n);



//=================================================================
//  FONCTIONS POUR AJOUTER DES STRUCTURES A D'AUTRES STRUCTURES
//=================================================================

Reseau* ajouter_point(Reseau* r, Point* p, int index);
Netlist* ajouter_reseau(Netlist* n, Reseau* r, int index);
Cell_segment* ajouter_segment(Cell_segment* liste_seg, Segment* seg);



//=================================================================
//  FONCTIONS POUR LIRE/ECRIRE DES NETLIST DANS DES FICHIERS
//=================================================================

Netlist* lecture_netlist(char* nom_fichier, Netlist* n);
void ecriture_netlist(char* nom_fichier, Netlist* n);
void sauvegarde_intersection(char* nom_fichier, Netlist* n);


//=================================================================
//                   FONCTIONS AUXILIAIRES
//=================================================================

Point* recherche_point_max(Netlist* n);
double min(double a, double b);
double max(double a, double b);

//=================================================================
//              FONCTIONS DE CALCUL D'INTERSECTIONS
//=================================================================


int intersection (Netlist *N, Segment *s1, Segment *s2);
void remplir_Lintersec (Netlist* N, Segment* s1, Segment* s2);
int nb_segment(Netlist* N);
Segment** tab_segment(Netlist* N);

Netlist* intersect_naif(Netlist* N);

//=================================================================
//              FONCTIONS POUR LE BALAYAGE
//=================================================================

int nb_extremite(Netlist* N);
Extremite** creer_echeancier(Netlist* N, int taille);
void supprimer_echeancier(Extremite** tab, int taille);

int comparaison_extremite(Extremite* e1, Extremite* e2);
Extremite** tri_echeancier(Extremite** tab, int taille);
Extremite** tri_echeancier_rec(Extremite** tab, int debut, int fin);
int partition (Extremite** tab, int deb, int fin);

Cell_segment_y* Inserer(Cell_segment_y* listeT, Segment* seg, Netlist* N);
Cell_segment_y* Supprimer(Cell_segment_y* listeT, Segment* seg);
Segment* Prem_segment_apres(Cell_segment_y* listeT, double y);
Segment* AuDessus(Cell_segment_y* listeT, Segment* seg);

Netlist* intersect_balayage(Netlist* N);

//=================================================================
//              FONCTIONS POUR LES AVL 
//=================================================================


Noeud_AVL* creer_noeud_AVL(double y, Segment* seg);
int hauteur(Noeud_AVL* noeud);
void maj_hauteur(Noeud_AVL* noeud);
void rotation_droite(Noeud_AVL** abr, Noeud_AVL* pere);
void rotation_gauche(Noeud_AVL** abr, Noeud_AVL* pere);

Noeud_AVL* Inserer_AVL(Noeud_AVL* abr, Noeud_AVL* pere, double y , Segment* seg);

Noeud_AVL* Supprimer_AVL(Noeud_AVL* abr, Noeud_AVL* pere, double y, Segment* seg);
Noeud_AVL* supprimer_max(Noeud_AVL* abr, Noeud_AVL* racine);
Noeud_AVL* supprimer_min(Noeud_AVL* abr, Noeud_AVL* racine);

Noeud_AVL* Prem_segment_apres_AVL(Noeud_AVL* abr, double y);
Noeud_AVL* AuDessus_AVL(Noeud_AVL* abr, Noeud_AVL* elem);

Netlist* intersect_balayage_AVL(Netlist* N);
void afficher_AVL(Noeud_AVL* abr, int a,int GouD);
int verif_AVL(Noeud_AVL* abr,double ymin, int p1min, double ymax, int p1max);
int verif_hauteur_AVL(Noeud_AVL* abr);









#endif
