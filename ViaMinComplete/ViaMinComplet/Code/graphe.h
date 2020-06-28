#ifndef _GRAPHE_H_
#define _GRAPHE_H_


typedef struct arete{
  int numA;
  int u;      // num du sommet de depart de l'arete
  int v;      // num du sommet d'arrivee de l'arete
  //double poids;    // non utilise
} Arete;

typedef struct elementListeA{
  Arete* a;
  struct elementListeA* suiv;
} ElementListeA;


typedef struct sommet{
  int numS;
  int nbA;    // nombre d'aretes adjacentes
  ElementListeA* LA;
  int SouP;   // vaut 0 si sommet = un point, 1 si sommet = un segment
  Segment* seg;
  Point* pt;
} Sommet;

typedef struct graphe{
  int nbS;
  int nbA;    // nombre d'aretes total dans le graphe
  Sommet** tabS;
} Graphe;

typedef struct listeSommet{
  int numS;
  struct listeSommet* suiv;
} ListeSommet;


Graphe* creer_graphe(int nbS);
Sommet* creer_sommet(int numS, int SouP, Segment* seg, Point* pt);
Arete* creer_arete(int numA, int u, int v);
void ajouter_arete(Sommet* s1, Sommet* s2, Graphe* g);

Graphe* lecture_graphe(char* nom_fichier_netlist, char* nom_fichier_intersec);
void affiche_graphe(Graphe* g);
void affiche_graphe_avec_aretes(Graphe* g);

int* remplir_tableau_S(Graphe* g);

ListeSommet* detecte_cycle_impair(Graphe* g, int* S, int* tabMarquage);
int detecte_cycle_impair_rec(Graphe* g, int* S, int* tabMarquage, int depart, int pere, ListeSommet** res );
ListeSommet* creer_ListeSommet(int numSommet);
void Afficher_ListeSommet(ListeSommet* LS);

int* ajout_vias_cycle_impair(Graphe* g);
#endif
