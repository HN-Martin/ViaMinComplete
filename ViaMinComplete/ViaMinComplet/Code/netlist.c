#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "netlist.h"
#include "entree_sortie.h"



//=================================================================
//    FONCTIONS POUR CREER LES DIFFERENTES STRUCTURES
//=================================================================

Segment* creer_segment(int NumRes, int p1, int p2, int HouV){
  Segment* pt_segment = (Segment*)malloc(sizeof(Segment)*1);
  pt_segment->NumRes = NumRes;
  pt_segment->p1 = p1;
  pt_segment->p2 = p2;
  pt_segment->HouV = HouV;
  pt_segment->Lintersec = NULL;
  
  return pt_segment;
}

Segment* copier_segment(Segment* seg){
  Segment* pt_segment = creer_segment(seg->NumRes, seg->p1, seg->p2, seg->HouV);

  return pt_segment;
}

Cell_segment* creer_cell_segment(Segment* seg){
  Cell_segment* pt_cell_segment = (Cell_segment*)malloc(sizeof(Cell_segment)*1);
  pt_cell_segment->seg = seg;
  pt_cell_segment->suiv = NULL;
  
  return pt_cell_segment;
}

Point* creer_point(double x, double y, int num_res){
  Point* pt_point = (Point*)malloc(sizeof(Point)*1);
  pt_point->x = x;
  pt_point->y = y;
  pt_point->num_res = num_res;
  pt_point->Lincid = NULL;
  
  return pt_point;
}

Reseau* creer_reseau(int NumRes, int NbPt, int NbSeg){
  Reseau* pt_reseau = (Reseau*)malloc(sizeof(Reseau)*1);
  pt_reseau->NumRes = NumRes;
  pt_reseau->NbPt = NbPt;
  pt_reseau->T_Pt = (Point**)malloc(sizeof(Point*)*NbPt);
  pt_reseau->NbSeg = NbSeg;
  pt_reseau->T_Seg = (Segment**)malloc(sizeof(Segment*)*NbSeg);

  return pt_reseau;
}

Netlist* creer_netlist(int NbRes){
  Netlist* pt_netlist = (Netlist*)malloc(sizeof(Netlist)*1);
  pt_netlist->NbRes = NbRes;
  pt_netlist->T_Res= (Reseau**)malloc(sizeof(Reseau*)*NbRes);
  pt_netlist->NbSeg = 0;

  return pt_netlist;
}

Extremite* creer_extremite(double x, int VouGouD, Segment* Seg, int NumPt){
  Extremite* pt_extremite = (Extremite*)malloc(sizeof(Extremite)*1);
  pt_extremite->x = x;
  pt_extremite->VouGouD = VouGouD;
  pt_extremite->PtrSeg = Seg;
  pt_extremite->NumPt = NumPt;

  return pt_extremite;
}


Cell_segment_y* creer_cell_segment_y(Segment* seg, double y){
  Cell_segment_y* pt_cell_segment_y = (Cell_segment_y*)malloc(sizeof(Cell_segment_y)*1);
  pt_cell_segment_y->y = y;
  pt_cell_segment_y->seg = seg;
  pt_cell_segment_y->suiv = NULL;
  
  return pt_cell_segment_y;
}


//=================================================================
//  FONCTIONS POUR DETRUIRE/SUPPRIMER LES DIFFERENTES STRUCTURES
//=================================================================


//_________________________________________________________________
// supprime 1 segment et son champ Lintersec
void supprimer_segment(Segment* seg){
  supprimer_uniquement_cell_segment(seg->Lintersec);
  free(seg);
}



//______________________________________________________________________________________
// supprime 1 Cellule de 'liste chainee de segments' parmi une liste
Cell_segment* supprimer_cell_segment(Cell_segment* liste, Cell_segment* cell_a_suppr){
  Cell_segment* pt_c = NULL;  // pointeur courant
  Cell_segment* pt_p = NULL;  // pointeur precedent
  pt_c = liste;
  
  // si la liste est vide
  if (liste == NULL){
    printf("la liste de segments est vide. rien à supprimer ici.\n");
    return liste;
  }

  // si on supprime en tete 
  if ((pt_c->seg->p1 == cell_a_suppr->seg->p1) && (pt_c->seg->p2 == cell_a_suppr->seg->p2) && (pt_c->seg->NumRes == cell_a_suppr->seg->NumRes)){
    liste = pt_c->suiv;
    free(pt_c);
    return liste;
  }

  pt_p = pt_c;
  pt_c = pt_c->suiv;

  // cas general
  while (pt_c != NULL){
    if ((pt_c->seg->p1 == cell_a_suppr->seg->p1) && (pt_c->seg->p2 == cell_a_suppr->seg->p2) && (pt_c->seg->NumRes == cell_a_suppr->seg->NumRes)){
      pt_p->suiv = pt_c->suiv;
      free(pt_c);
      return liste;
    }
    pt_p = pt_c;
    pt_c = pt_c->suiv;
  }
  
  // si le segment n'est pas dans la liste
  printf("Le segment à supprimer n'a pas été trouvé.\n");
  return liste;
}



//______________________________________________________
// supprime une liste chainee complete de Cell_segment AVEC les segments contenus dedans

void supprimer_liste_cell_segment(Cell_segment* liste){
  Cell_segment* pt_c = NULL;  // pointeur courant
  Cell_segment* pt_n = NULL;  // pointeur suivant
  pt_c = liste;

  while (pt_c != NULL){
    pt_n = pt_c->suiv;  // on sauvegarde l'acces au suiv pour ne pas le perdre

    free(pt_c->seg);    // on libere le segment courant
    free(pt_c);
    
    pt_c = pt_n;        // on passe au Cell_segment suivant
  }
  return;
}




//______________________________________________________
// supprime une liste chainee complete de Cell_segment SANS supprimer les segments dedans

void supprimer_uniquement_cell_segment(Cell_segment* liste){
  Cell_segment* pt_c = NULL;  // pointeur courant
  Cell_segment* pt_n = NULL;  // pointeur suivant
  pt_c = liste;

  while (pt_c != NULL){
    pt_n = pt_c->suiv;  // on sauvegarde l'acces au suiv pour ne pas le perdre
    free(pt_c);		// on libere ce Cell_segment
    pt_c = pt_n;        // on passe au Cell_segment suivant
  }
  return;
}


//______________________________________________________
// supprime un point
void supprimer_point(Point* p){
  supprimer_uniquement_cell_segment(p->Lincid);
  free(p);
  return;
}



//______________________________________________________
// supprime un reseau
void supprimer_reseau(Reseau* r){
  int i = 0;
  
  // pour chaque point du tableau de points du reseau
  for(i=0; i< r->NbPt; i++){
    
    // on verifie si le point n'a pas deja ete supprime
    if (r->T_Pt[i] != NULL){
      supprimer_point(r->T_Pt[i]);  // on le supprime
    }
  }
  for(i=0; i< r->NbSeg; i++){
    
    // on verifie si le segment n'a pas deja ete supprime
    if (r->T_Seg[i] != NULL){
      supprimer_segment(r->T_Seg[i]);  // on le supprime
    }
  }
  //on supprime les tableaux de pointeurs du reseau
  free(r->T_Pt);
  free(r->T_Seg);

  //on supprime le reseau
  free(r);
  return;
}

//________________________________________________________________
// supprime une netlist (meme principe que pour supprimer un reseau)
void supprimer_netlist(Netlist* n){
  int i = 0;
  
  // pour chaque reseau du tableau de reseau de la netlist
  for(i=0; i< n->NbRes; i++){
    
    // on verifie si le reseau n'a pas deja ete supprime
    if (n->T_Res[i] != NULL){
      supprimer_reseau(n->T_Res[i]);  // on le supprime
    }
  }

  //on supprime la netlist
  free(n->T_Res);
  free(n);
  return;
}


//=================================================================
//  FONCTIONS POUR AJOUTER DES STRUCTURES A D'AUTRES STRUCTURES
//=================================================================

//________________________________________________________________
Reseau* ajouter_point(Reseau* r, Point* p, int index){

  // tests si les parametres d'entree sont incoherents
  if (r == NULL){
    fprintf(stderr,"Le réseau n'existe pas.\n");
    return NULL;
  }
  if (p == NULL){
    fprintf(stderr,"Le point à ajouter n'existe pas.\n");
    return NULL;
  }
  if (index >= r->NbPt){
    fprintf(stderr,"Ce réseau ne peut contenir que %d points\n", r->NbPt);
    return NULL;
  }
  
  // on ajoute le point dans la case [index] du tableau de points du reseau
  r->T_Pt[index] = p;
  return r;
}


//________________________________________________________________
Netlist* ajouter_reseau(Netlist* n, Reseau* r, int index){

  // tests si les parametres d'entree sont incoherents
  if (n == NULL){
    fprintf(stderr,"La netlist n'existe pas.\n");
    return NULL;
  }
  if (r == NULL){
    fprintf(stderr,"Le réseau à ajouter n'existe pas.\n");
    return NULL;
  }
  if (index >= n->NbRes){
    fprintf(stderr,"Cette netlist ne peut contenir que %d réseaux\n", n->NbRes);
    return NULL;
  }
  
  // on ajoute le reseau dans la case [index] du tableau de reseaux de la netlist
  n->T_Res[index] = r;
  return n;
}


//________________________________________________________________
Cell_segment* ajouter_segment(Cell_segment* liste_seg, Segment* seg){

  // tests si les parametres d'entree sont incoherents
  if (seg == NULL){
    fprintf(stderr,"Le segment à ajouter n'existe pas.\n");
    return NULL;
  }
  
  // cas d'une liste_seg vide : seg devient seul element de la liste chainee a retourner
  if (liste_seg == NULL){
    Cell_segment* new_cell = creer_cell_segment(seg);
    return new_cell;
  }
  
  // cas general : ajout du nouveau segment en tete de la liste chainee liste_seg existante
  Cell_segment* new_cell = creer_cell_segment(seg);
  new_cell->suiv = liste_seg;
  return new_cell;
}







//=================================================================
//  FONCTIONS POUR LIRE/ECRIRE DES NETLIST DANS DES FICHIERS
//=================================================================



//________________________________________________________________
// lecture_netlist permet de lire une netlist a partir d'un fichier 
// et de creer la struct Netlist correspondante
Netlist* lecture_netlist(char* nom_fichier, Netlist* n){
  
  int i = 0;
  int j = 0;
  int k = 0;
  
  int NbRes = 0;
  //Netlist* n = NULL;
  
  int NumRes = 0;
  int NbPt = 0;
  int NbSeg = 0;
  Reseau* r = NULL;
  
  int index = 0;
  int x = 0;
  int y = 0;
  Point* p = NULL;
  
  int p1 = 0;
  int p2 = 0;
  int HouV = 0;
  Segment* seg = NULL;

  
  // ouverture du fichier en mode lecture
  FILE* f = fopen(nom_fichier,"r");
  
  //test si l'ouverture s'est bien passee
  if (f == NULL){
    fprintf(stderr,"Erreur à l'ouverture du fichier '%s'\n", nom_fichier);
    return NULL;
  }
  
  // on recupere les parametres pour creer la netlist
  NbRes = GetEntier(f);
  n = creer_netlist(NbRes);
  for(i=0; i< NbRes; i++){
  
    // pour chaque reseau, on recupere les parametres pour creer le reseau
    NumRes = GetEntier(f);
    NbPt = GetEntier(f);
    NbSeg = GetEntier(f);
    r = creer_reseau(NumRes, NbPt, NbSeg);
    
    for(j=0; j< NbPt; j++){
    
      // pour chaque point, on recupere les parametres pour creer le point
      index = GetEntier(f);
      x = GetEntier(f);
      y = GetEntier(f);
      p = creer_point(x, y, NumRes);
      
      // on ajoute ce point au tableau de points du reseau
      r = ajouter_point(r, p, index);
    }
    
    for(k=0; k< NbSeg; k++){
    
      // pour chaque segment on recupere les parametres pour creer le segment 
      p1 = GetEntier(f);
      p2 = GetEntier(f);

      // on verifie si le segment est vertical ou horizontal
      if (r->T_Pt[p1]->x == r->T_Pt[p2]->x){
        seg = creer_segment(NumRes, p1, p2,1);  // meme x donc meme abscisse donc segment vertical donc 1
      }else{
        seg = creer_segment(NumRes, p1, p2,0);   // meme y donc meme ordonnée donc segment horizontal donc 0
      }

      //on ajoute le segment aux listes des segments incidents des deux structures points p1 et p2
      r->T_Pt[p1]->Lincid = ajouter_segment(r->T_Pt[p1]->Lincid, seg);
      r->T_Pt[p2]->Lincid = ajouter_segment(r->T_Pt[p2]->Lincid, seg);
      
      // on ajoute le segment au tableau de segments du reseau
      r->T_Seg[k] = seg;
    }
    
    // on ajoute le reseau complet a la netlist avant de passer au reseau suivant
    n = ajouter_reseau(n, r, NumRes);

    // on additionne le nombre de segments dans chaque reseau 
    //pour obtenir le nombre total de segments contenus dans la netlist
    n->NbSeg += NbSeg;   
  }
  fclose(f);
  return n;
}




//________________________________________________________________
// ecriture_netlist permet de copier une structure Netlist dans un nouveau fichier 
void ecriture_netlist(char* nom_fichier, Netlist* n){

  int i = 0;
  int j = 0;
  
  // ouverture du fichier en mode ecriture
  FILE* f = fopen(nom_fichier,"w");
  
  //test si l'ouverture s'est bien passee
  if (f == NULL){
    fprintf(stderr,"Erreur à l'ouverture du fichier '%s'\n", nom_fichier);
    return;
  }
  
  // on ecrit le nb de reseaux sur la 1ere ligne
  fprintf(f,"%d\n", n->NbRes); 
  
  for (i=0; i< n->NbRes; i++){
    
    // on ecrit le num de reseau, le nb de points et le nb de segments
    fprintf(f,"%d %d %d\n", n->T_Res[i]->NumRes, n->T_Res[i]->NbPt, n->T_Res[i]->NbSeg);
    
    for (j=0; j< n->T_Res[i]->NbPt; j++){
    
      // on ecrit le num du point, son x et son y
      // x et y sont des types 'double' mais on n'a pas besoin des virgules donc %.0f
      fprintf(f,"  %d %.0f %.0f\n", j, n->T_Res[i]->T_Pt[j]->x, n->T_Res[i]->T_Pt[j]->y);  
    }
    for (j=0; j< n->T_Res[i]->NbSeg; j++){

      // on ecrit le p1 et p2 pour chaque segment
      fprintf(f,"  %d %d\n", n->T_Res[i]->T_Seg[j]->p1, n->T_Res[i]->T_Seg[j]->p2);
    } 
  }
  fclose(f);
  return;
}


//________________________________________________________________
// sauvegarde_intersection permet de sauvegarder les intersections de segments 
// d'une Netlist dans un fichier "***.ini"
void sauvegarde_intersection(char* nom_fichier, Netlist* n){

  int i = 0;
  int j = 0;
  Cell_segment* pt_c = NULL;

  // ouverture du fichier en mode ecriture
  FILE* f = fopen(nom_fichier,"w");
  
  //test si l'ouverture s'est bien passee
  if (f == NULL){
    fprintf(stderr,"Erreur à l'ouverture du fichier '%s'\n", nom_fichier);
    return;
  }
  
  for (i=0; i< n->NbRes; i++){
    for (j=0; j< n->T_Res[i]->NbSeg; j++){
      pt_c = n->T_Res[i]->T_Seg[j]->Lintersec;
      while (pt_c != NULL){
        if (pt_c->seg->NumRes > i){
          fprintf(f,"%d %d %d %d %d %d\n", i, n->T_Res[i]->T_Seg[j]->p1, n->T_Res[i]->T_Seg[j]->p2,pt_c->seg->NumRes, pt_c->seg->p1, pt_c->seg->p2 );
        }
        pt_c = pt_c->suiv; 
      }
    }
  }
  fclose(f);
  return;
}








//=================================================================
//                   FONCTIONS AUXILIAIRES
//=================================================================


// permet de retourner un point fictif avec la plus grande abscisse et ordonnée de la netlist 
// pour optimiser la taille de la fenetre de visualisation SVG 

Point* recherche_point_max(Netlist* n){
  int i = 0;
  int j = 0;
  
  double maxX = 0;
  double maxY = 0;
  
  for (i=0; i< n->NbRes; i++){
    for (j=0; j< n->T_Res[i]->NbPt; j++){
      if (n->T_Res[i]->T_Pt[j]->x > maxX){
        maxX = n->T_Res[i]->T_Pt[j]->x;
      }
      if (n->T_Res[i]->T_Pt[j]->y > maxY){
        maxY = n->T_Res[i]->T_Pt[j]->y;
      }
    }
  }
  
  Point* pMax = creer_point(maxX, maxY, 0);
  return pMax;
}

// Calcul du minimum entre a et b
double min(double a, double b){
  if (a<=b)
    return a;

  else
    return b;
}

// Calcul du maximun entre a et b
double max(double a, double b){
  if (a>=b)
    return a;

  else
    return b;
}



//=================================================================
//              FONCTIONS DE CALCUL D'INTERSECTIONS
//=================================================================



int intersection (Netlist *N, Segment *s1, Segment *s2){

  // si ce sont tous les 2 des segments verticaux ou horizontaux : pas d'intersection
  if (s1->HouV == s2->HouV){
    return 0;
  }

  Segment* sh = NULL;
  Segment* sv = NULL;
  
  // on determine lequel est vertical, lequel est horizontal
  if (s1->HouV == 0){ 
    sh = s1;
    sv = s2;
  }
  else {
    sh = s2;
    sv = s1;
  }

  // on recupere les p1 et p2 du segment horizontal
  Point* ph1 = N->T_Res[sh->NumRes]->T_Pt[sh->p1];
  Point* ph2 = N->T_Res[sh->NumRes]->T_Pt[sh->p2];

  // on recupere les p1 et p2 du segment vertical
  Point* pv1 = N->T_Res[sv->NumRes]->T_Pt[sv->p1];
  Point* pv2 = N->T_Res[sv->NumRes]->T_Pt[sv->p2];


  /* si le segment vertival est :
     - trop a gauche du segment horizontal 
     - trop a droite du segment horizontal 
        --> pas d'intersection
  */

  if ( (pv1->x < min(ph1->x,ph2->x)) || (pv1->x > max(ph1->x,ph2->x)) ){
    return 0;
  }


  /* si le segment horizontal est :
     - trop en dessous du segment vertical 
     - trop au dessus du segment vertical 
        --> pas d'intersection
  */

  if ( (ph1->y < min(pv1->y,pv2->y)) || (ph1->y > max(pv1->y,pv2->y)) ){
    return 0;
  }

  // sinon, c'est que les segments se croisent. 
  
  return 1;
}

void remplir_Lintersec (Netlist* N, Segment* s1, Segment* s2){
  //si s1 et s2 se croisent, on va remplir leur champs 'Lintersec'
  if (intersection(N,s1,s2) == 1){
    s1->Lintersec = ajouter_segment(s1->Lintersec,s2);
    s2->Lintersec = ajouter_segment(s2->Lintersec,s1);
  }
  return;
}


int nb_segment(Netlist* N){
  return N->NbSeg;
}


Segment** tab_segment(Netlist* N){

  int i = 0;
  int j = 0;
  int k = 0;  

  // on cree un tableau de la taille du nombre de segment dans la netlist
  int NbSeg = nb_segment(N);
  Segment** tab = (Segment**)malloc(sizeof(Segment*) *NbSeg);

  // on le remplit avec les pointeur sur segment dans chacun des tableaux de segments dans chaque reseau
  for (i = 0; i < N->NbRes; i++){
    for (j = 0; j < N->T_Res[i]-> NbSeg; j++){
      tab[k] = N->T_Res[i]->T_Seg[j];
      k++;
    }
  }
  return tab;
}


Netlist* intersect_naif(Netlist* N){
  int NbSeg = nb_segment(N);
  Segment** tab = tab_segment(N);
  int i = 0;
  int j = 0;
  if (NbSeg < 2)    // s'il y a moins de 2 segments dans toute la netlist.. pas d'intersections possible
    return N;

  for (i = 0; i < NbSeg; i++){
    for (j = i+1; j < NbSeg; j++){
      remplir_Lintersec(N, tab[i], tab[j]);
        
    }
  }
  free(tab);
  return N;
}






//=================================================================
//              FONCTIONS POUR LE BALAYAGE
//=================================================================


int nb_extremite(Netlist* N){
  int nb = 0;
  int i = 0;
  int j = 0;

  for (i = 0; i < N->NbRes; i++){
    for (j = 0; j < N->T_Res[i]-> NbSeg; j++){
      if (N->T_Res[i]->T_Seg[j]->HouV == 0) {
        nb++;
      }
      nb++;
    }
  }
  return nb;
}


Extremite** creer_echeancier(Netlist* N, int taille){

  int i = 0;
  int j = 0;
  int k = 0;
  Extremite** tab = (Extremite**)malloc(sizeof(Extremite*) * taille);
  Point* p1 = NULL;
  Point* p2 = NULL;

  for (i = 0; i < N->NbRes; i++){
    for (j = 0; j < N->T_Res[i]-> NbSeg; j++){
      // si c'est un segment horizontal, on va creer 2 extremites (Gauche et Droite)
      if (N->T_Res[i]->T_Seg[j]->HouV == 0) {
        // on verifie lequel de p1 et p2 se trouve a gauche et a droite
        
        p1 = N->T_Res[i]->T_Pt[ N->T_Res[i]->T_Seg[j]->p1 ];
        p2 = N->T_Res[i]->T_Pt[ N->T_Res[i]->T_Seg[j]->p2 ];

	if (p1->x < p2->x) {
          tab[k] = creer_extremite(p1->x,1, N->T_Res[i]->T_Seg[j], N->T_Res[i]->T_Seg[j]->p1 );
	  tab[k+1] = creer_extremite(p2->x,2, N->T_Res[i]->T_Seg[j], N->T_Res[i]->T_Seg[j]->p2 );
        }
        else {
          tab[k] = creer_extremite(p1->x,2, N->T_Res[i]->T_Seg[j], N->T_Res[i]->T_Seg[j]->p1 );
	  tab[k+1] = creer_extremite(p2->x,1, N->T_Res[i]->T_Seg[j], N->T_Res[i]->T_Seg[j]->p2 );
        }
        k+=2;
      }
      // si c'est un segment vertical, on va creer une seule extremite (Segment Vertical)
      else {
	p1 = N->T_Res[i]->T_Pt[ N->T_Res[i]->T_Seg[j]->p1 ];
        tab[k] = creer_extremite(p1->x ,0 , N->T_Res[i]->T_Seg[j], -1 );
        k++;
      }
    }
  }
  return tab;
}

void supprimer_echeancier(Extremite** tab, int taille){
  int i = 0;
  for(i = 0; i<taille; i++){
    free(tab[i]);
  }
  free(tab);
  return;
}


// retourne -1 si e1 < e2  
// retourne  1 si e1 > e2
// retourne  0 si e1 = e2 (si on compare 2 fois la meme extremite...)
int comparaison_extremite(Extremite* e1, Extremite* e2){
  // si l'abscisse de e1 < abscisse de e2
  if (e1->x < e2->x){
    return -1;
  }

  // si l'abscisse de e1 > abscisse de e2
  if (e1->x > e2->x){
    return 1;
  }

  // sinon, les 2 extremites ont la meme abscisse

  //On regarde alors si ce sont des extremites Vertical, Gauche ou Droite
  //D'apres l'enonce, Gauche (1) < Vertical (0) < Droite(2)
  if (e1->VouGouD == 0){
    if (e2->VouGouD == 1)
      return 1;
    if (e2->VouGouD == 2)
      return -1;
    if (e2->VouGouD == 0)
      return 0;   
  }
  
  if (e1->VouGouD == 1){
    if (e2->VouGouD == 1)
      return 0;
    else
      return -1;
  }

  if (e1->VouGouD == 2){
    if (e2->VouGouD == 2)
      return 0;
    else
      return 1;
  }

  //on n'atteint jamais cette partie du code normalement...
  return 3;
}


Extremite** tri_echeancier(Extremite** tab, int taille){

  tab =  tri_echeancier_rec(tab, 0, taille-1);
  return tab;
}


Extremite** tri_echeancier_rec(Extremite** tab, int deb, int fin){

  int i;

  if (fin - deb == 1){
    if (comparaison_extremite(tab[deb], tab[fin]) == 1){
      Extremite* tmp = tab[fin];
      tab[fin] = tab[deb];
      tab[deb] = tmp;
    }
    return tab;
  }

  if (deb < fin){
    i = partition(tab, deb, fin);
    tri_echeancier_rec(tab, deb, i-1);
    tri_echeancier_rec(tab, i+1, fin);
  }
  return tab;
}


int partition (Extremite** tab, int deb, int fin){

  int i;
  int j;
  Extremite* pivot;
  Extremite* tmp;

  pivot = tab[deb];
  i = deb + 1;
  j = fin;
  
  while (i<j){
    while (i<j && comparaison_extremite(tab[i], pivot) < 1){    // tant que tab[i] <= pivot
      i++;
    }
    while (i<j && comparaison_extremite(tab[j], pivot) > -1){    // tant que tab[j] >= pivot
      j--;
    }
    if (i<j){
      tmp = tab[i];
      tab[i] = tab[j];
      tab[j] = tmp;
      i++;
      j--;
    }
    // pour traiter les cas avec des extremites egales (meme x et meme VouGouD)
    if (i == j){
      if (comparaison_extremite(tab[i], pivot) == 1){
        j--;
      }
    }
  }

  tab[deb] = tab[j];
  tab[j] = pivot;
  return j;
}


Cell_segment_y* Inserer(Cell_segment_y* listeT, Segment* seg, Netlist* N){
  
  // tests si les parametres d'entree sont incoherents
  if (seg == NULL){
    fprintf(stderr,"Le segment à ajouter n'existe pas.\n");
    return NULL;
  }

  double y = N->T_Res[seg->NumRes]->T_Pt[seg->p1]->y;

  // cas d'une listeT vide : seg devient seul element de la liste chainee a retourner
  if (listeT == NULL){
    Cell_segment_y* new_cell = creer_cell_segment_y(seg, y);
    return new_cell;
  }
  
  // cas general : ajout du nouveau segment a sa place dans la liste chainee listeT existante
  // on ajoute les segments en les triant par y croissants
  

  // si seg est le plus petit segment il est placé en tete de liste
  Cell_segment_y* pt_c = listeT;
  Cell_segment_y* pt_p = NULL;

  if (y < pt_c->y ){
    Cell_segment_y* new_cell = creer_cell_segment_y(seg,y);
    new_cell->suiv = listeT;
    return new_cell;
  }

  pt_p = pt_c;
  pt_c = pt_c->suiv;

  // sinon, on le place a sa place dans la liste
  while (pt_c != NULL){

    if (y < pt_c->y){
      Cell_segment_y* new_cell = creer_cell_segment_y(seg,y);
      new_cell->suiv = pt_c;
      pt_p->suiv = new_cell;
      return listeT;
    }
    pt_p = pt_c;
    pt_c = pt_c->suiv;
  }

  // sinon, si seg est plus haut que tous les segments de la liste,
  // on le place tout a la fin de la liste  
  Cell_segment_y* new_cell = creer_cell_segment_y(seg,y);
  pt_p->suiv = new_cell;

  return listeT;
}


Cell_segment_y* Supprimer(Cell_segment_y* listeT, Segment* seg){

  Cell_segment_y* pt_c = NULL;  // pointeur courant
  Cell_segment_y* pt_p = NULL;  // pointeur precedent
  pt_c = listeT;
  
  // si la liste est vide
  if (listeT == NULL){
    printf("la liste de segments est vide. rien à supprimer ici.\n");
    return listeT;
  }

  // si on supprime en tete 
  if ((pt_c->seg->p1 == seg->p1) && (pt_c->seg->p2 == seg->p2) && (pt_c->seg->NumRes == seg->NumRes)){
    listeT = pt_c->suiv;
    free(pt_c);
    return listeT;
  }

  pt_p = pt_c;
  pt_c = pt_c->suiv;

  // cas general
  while (pt_c != NULL){
  if ((pt_c->seg->p1 == seg->p1) && (pt_c->seg->p2 == seg->p2) && (pt_c->seg->NumRes == seg->NumRes)){
      pt_p->suiv = pt_c->suiv;
      free(pt_c);
      return listeT;
    }
    pt_p = pt_c;
    pt_c = pt_c->suiv;
  }
  
  // si le segment n'est pas dans la liste
  printf("Le segment à supprimer n'a pas été trouvé.\n");
  return listeT;
}



Segment* Prem_segment_apres(Cell_segment_y* listeT, double y){

  Cell_segment_y* pt_c = NULL;  // pointeur courant
  pt_c = listeT;

  // on parcourt la liste chainee de segments horizontaux
  while (pt_c != NULL){

    // des qu'on trouve un segment plus haut que y, on le retourne
    if (pt_c->y >= y){
      return pt_c->seg;
    }
    pt_c = pt_c->suiv;
  }

  // si on en trouve pas, c'est que tous les segments horizontaux etaient en dessous de ce y 
  // donc on retourne NULL
  return NULL;
}


Segment* AuDessus(Cell_segment_y* listeT, Segment* seg){
 
  Cell_segment_y* pt_c = NULL;  // pointeur courant
  pt_c = listeT;

  // on parcourt la liste chainee de segments horizontaux
  while (pt_c != NULL){

    // des qu'on trouve le segment seg dans la liste 
    if ((pt_c->seg->p1 == seg->p1) && (pt_c->seg->p2 == seg->p2) && (pt_c->seg->NumRes == seg->NumRes)){

      // on va aller chercher le segment juste après (s'il existe) et on le retourne
      pt_c = pt_c->suiv;

      if (pt_c == NULL){ // si le suivant est NULL
        return NULL;
      }
      else{              // si le suivant existe
        return pt_c->seg;
      }
    }
    pt_c = pt_c->suiv;
  }
  
  // ...cas etrange si on ne trouve pas seg dans la listeT... 
  return NULL;
}


Netlist* intersect_balayage(Netlist* N){

  int i = 0;
  double y1 = 0;
  double y2 = 0;
  double ybas = 0;
  double yhaut = 0;
  Segment* h = NULL;

  int taille = nb_extremite(N);

  Extremite** echeancier = creer_echeancier(N, taille);
  echeancier = tri_echeancier(echeancier, taille);
  /*
  // affiche toutes les extremites avec leur abscisses et leur VouGouD pour voir si le tableau est bien trie
  for (i = 0; i< taille; i++){
    printf(" %f  %d\n", echeancier[i]->x, echeancier[i]->VouGouD);
  }
  */

  Cell_segment_y* listeT = NULL;

  for (i=0; i< taille; i++){
    switch(echeancier[i]->VouGouD){

      case 1:   // si extremite gauche
        listeT = Inserer(listeT, echeancier[i]->PtrSeg, N);
        break;

      case 2:   // si extremite droite 
        listeT = Supprimer(listeT, echeancier[i]->PtrSeg);
        break;

      case 0:   // si extremite verticale
        // on recupere les ybas et yhaut de ce segment vertical
        y1 = N->T_Res[echeancier[i]->PtrSeg->NumRes]->T_Pt[echeancier[i]->PtrSeg->p1]->y;
        y2 = N->T_Res[echeancier[i]->PtrSeg->NumRes]->T_Pt[echeancier[i]->PtrSeg->p2]->y;
        
        ybas = min(y1, y2);
        yhaut = max(y1, y2);

        // on cherche dans la liste des segments horizontaux, le premier seg >= ybas
        h = Prem_segment_apres(listeT, ybas);

        // on parcourt les segments horizontaux qui intersecte ce segment vertical 
        // en completant les Lintersec des ces segments et du segment vertical
        while ( h != NULL  && N->T_Res[h->NumRes]->T_Pt[h->p1]->y <= yhaut){
          if (echeancier[i]->PtrSeg->NumRes != h->NumRes){
             echeancier[i]->PtrSeg->Lintersec = ajouter_segment(echeancier[i]->PtrSeg->Lintersec,h);
             h->Lintersec = ajouter_segment(h->Lintersec, echeancier[i]->PtrSeg);
          }
          h = AuDessus(listeT, h);
        }
        break;
     
    }
  }
  supprimer_echeancier(echeancier, taille);
  return N;
}


//=================================================================
//              FONCTIONS POUR LES AVL 
//=================================================================


Noeud_AVL* creer_noeud_AVL(double y, Segment* seg){
  Noeud_AVL* nv_noeud = (Noeud_AVL*)malloc(sizeof(Noeud_AVL));
  nv_noeud->y = y;
  nv_noeud->hauteur = 1;
  nv_noeud->seg = seg;
  nv_noeud->pere = NULL;
  nv_noeud->fg = NULL;
  nv_noeud->fd = NULL;
    
  return nv_noeud;
}


int hauteur(Noeud_AVL* noeud){
  if(noeud == NULL) 
    return 0;
  return noeud->hauteur;
}

void maj_hauteur(Noeud_AVL* noeud){
  if(noeud == NULL) 
    return;
  if (hauteur(noeud->fg) >= hauteur(noeud->fd)){ 
    noeud->hauteur = 1 + hauteur(noeud->fg);
  }
  else{
    noeud->hauteur = 1 + hauteur(noeud->fd);
  }
  return;
}


void rotation_droite(Noeud_AVL** abr, Noeud_AVL* pere ){

    Noeud_AVL* racine = *abr;
    Noeud_AVL* fg = (*abr)->fg;
    Noeud_AVL* fd = (*abr)->fd;

    racine->fg = fg->fd;
    if (fg->fd != NULL)
      fg->fd->pere = racine;

    fg->fd = racine;
    racine->pere = fg;

    *abr = fg;
    fg->pere = pere;

    maj_hauteur((*abr)->fd);
    maj_hauteur(*abr);
}

void rotation_gauche(Noeud_AVL** abr, Noeud_AVL* pere ){

    Noeud_AVL* racine = *abr;
    Noeud_AVL* fg = (*abr)->fg;
    Noeud_AVL* fd = (*abr)->fd;

    racine->fd = fd->fg;
    if (fd->fg != NULL)
      fd->fg->pere = racine;

    fd->fg = racine;
    racine->pere = fd;

    *abr = fd;
    fd->pere = pere;
    
    maj_hauteur((*abr)->fg);
    maj_hauteur(*abr);
}

Noeud_AVL* Inserer_AVL(Noeud_AVL* abr, Noeud_AVL* pere, double y , Segment* seg){

  if (abr == NULL) {
    Noeud_AVL* nv_noeud = creer_noeud_AVL(y, seg);  
    nv_noeud->pere = pere;
    return nv_noeud;
  }
  else {
    
    if (abr->y < y ) {
      abr->fd = Inserer_AVL(abr->fd, abr, y, seg); 
      maj_hauteur(abr);
    }
    else if (abr->y > y ) {
      abr->fg = Inserer_AVL(abr->fg, abr, y, seg);
      maj_hauteur(abr);
    } 
    else {
    // si on a le meme y, on compare avec les valeur des champs p1 des segments
      if (abr->seg->p1 < seg->p1){
        abr->fd = Inserer_AVL(abr->fd, abr, y, seg); 
      }
      else if (abr->seg->p1 > seg->p1){
        abr->fg = Inserer_AVL(abr->fg, abr, y, seg);
      }
      else{

      // si on a le meme p1, on compare avec les valeur des champs p2 des segments
        if (abr->seg->p2 < seg->p2){
          abr->fd = Inserer_AVL(abr->fd, abr, y, seg); 
        }
        else if (abr->seg->p2 > seg->p2){
          abr->fg = Inserer_AVL(abr->fg, abr, y, seg);
        }
        else{
        // si on a le meme p2, on compare avec les valeur des champs NumRes des segments
          if (abr->seg->NumRes < seg->NumRes){
            abr->fd = Inserer_AVL(abr->fd, abr, y, seg); 
          }
          else if (abr->seg->NumRes > seg->NumRes){
            abr->fg = Inserer_AVL(abr->fg, abr, y, seg);
          }
          else{
            // c'est qu'on essaye d'inserer un segment qui est deja dans l'arbre... ne rien faire
          }
        }
      }
      maj_hauteur(abr);
      
    }
    

    int hauteurfg = hauteur(abr->fg);
    int hauteurfd = hauteur(abr->fd);

    // on fait les rotations necessaires pour re-equilibrer l'AVL 
    // apres avoir fait notre insertion

    if( hauteurfg - hauteurfd == 2 ) {
      if( hauteur(abr->fg->fg) <  hauteur(abr->fg->fd) )
        rotation_gauche(&(abr->fg), abr); 
      rotation_droite(&abr, pere);
    }

    else if( hauteurfd - hauteurfg == 2 ) {
      if( hauteur( abr->fd->fg) >  hauteur( abr->fd->fd) )
        rotation_droite(&(abr->fd), abr);
      rotation_gauche(&abr, pere);
    }
  } 

  return abr;
}
 

Noeud_AVL* Supprimer_AVL(Noeud_AVL* abr, Noeud_AVL* pere, double y, Segment* seg){

  Noeud_AVL* tmp = NULL;

  // si l'arbre est vide, il n'y a rien a supprimer
  if (abr == NULL) {
    //printf("\n Suppression IMPOSSIBLE\n");
    return NULL;
  }
  
  if (abr->y < y){
    abr->fd = Supprimer_AVL(abr->fd, abr, y, seg);
  }
  else if (abr->y > y ){
    abr->fg = Supprimer_AVL(abr->fg, abr, y, seg);
  }
  else {   //  le cas : y == abr->y
    // si c'est un segment avec le meme y mais different de celui que l'on veut supprimer
    if (abr->seg->p1 != seg->p1 || abr->seg->p2 != seg->p2  || abr->seg->NumRes != seg->NumRes){
      if (abr->seg->p1 > seg->p1){
        abr->fg = Supprimer_AVL(abr->fg, abr, y, seg);
      }
      else if (abr->seg->p1 < seg->p1){
        abr->fd = Supprimer_AVL(abr->fd, abr, y, seg);
      }
      else{
        if (abr->seg->p2 > seg->p2){
          abr->fg = Supprimer_AVL(abr->fg, abr, y, seg);
        }
        else if (abr->seg->p2 < seg->p2){
          abr->fd = Supprimer_AVL(abr->fd, abr, y, seg);
        }
	else{
          if (abr->seg->NumRes > seg->NumRes){
            abr->fg = Supprimer_AVL(abr->fg, abr, y, seg);
          }
          else if (abr->seg->NumRes < seg->NumRes){
            abr->fd = Supprimer_AVL(abr->fd, abr, y, seg);
          }
          else{
            //c'est le bon segment donc dans le else en dessous
          }
        }
      }
    }

    //  si c'est le bon segment a supprimer 
    else {  
      // cas particulier pour si hauteur(abr->fg)= hauteur(abr->fd)= 0   on peut supprimer directement
      if (abr->fg == NULL && abr->fd == NULL){  
        free(abr);
        return NULL;  
      }
      // si l'AVL est plus grand a gauche, on va remplacer le noeud a supprimer par le max du sous arbre gauche 
      else if ( hauteur(abr->fg) >= hauteur(abr->fd) ){ 
        tmp = abr;
        abr = supprimer_max(abr->fg, abr);
       // printf("max =   res= %d  p1= %d   p2= %d   y= %7.0f   \n", abr->seg->NumRes, abr->seg->p1, abr->seg->p2, abr->y);
        //si le max du fg est directement le fg de abr on va le prendre directement et le mettre a la place de abr
        if (tmp->fg == abr){
          abr->fd = tmp->fd;
          if (tmp->fd != NULL)
            tmp->fd->pere = abr;

          abr->pere = tmp->pere;
          free(tmp);
        }
        else{

          // on le met a la place du noeud tmp que l'on veut supprimer
          abr->fd = tmp->fd;
          if (tmp->fd != NULL)
            tmp->fd->pere = abr;

          abr->fg = tmp->fg;
          tmp->fg->pere = abr;

          abr->pere = tmp->pere;
          free(tmp);
        }      
      }
      // sinon on va remplacer par le min du sous arbre droit
      else {
        tmp = abr;
        abr = supprimer_min(abr->fd, abr);
        //printf("min =   res= %d  p1= %d   p2= %d   y= %7.0f   \n", abr->seg->NumRes, abr->seg->p1, abr->seg->p2, abr->y);
       //si le min du fd est directement le fd de abr on va le prendre directement et le mettre a la place de abr
        if (tmp->fd == abr){
          abr->fg = tmp->fg;
          if (tmp->fg != NULL)
            tmp->fg->pere = abr;

          abr->pere = tmp->pere;
          free(tmp);
        }
        else{
          // on le met a la place du noeud tmp que l'on veut supprimer
          abr->fd = tmp->fd;
          tmp->fd->pere = abr;

          abr->fg = tmp->fg;
          if (tmp->fg != NULL)
            tmp->fg->pere = abr;

          abr->pere = tmp->pere;
          free(tmp);
        }  
      }    
    }
  } 
  maj_hauteur(abr);

  int hauteurfg = hauteur(abr->fg);
  int hauteurfd = hauteur(abr->fd);

  // on fait les rotations necessaires pour re-equilibrer l'AVL 
  // apres avoir fait notre suppression

  if( hauteurfg - hauteurfd == 2 ) {
    if( hauteur(abr->fg->fg) <  hauteur(abr->fg->fd) )
      rotation_gauche(&(abr->fg), abr); 
    rotation_droite(&abr, abr->pere);
  }

  else if( hauteurfd - hauteurfg == 2 ) {
    if( hauteur( abr->fd->fg) >  hauteur( abr->fd->fd) )
      rotation_droite(&(abr->fd), abr);
    rotation_gauche(&abr, abr->pere);
  }
  return abr;
}


Noeud_AVL* supprimer_max(Noeud_AVL* abr, Noeud_AVL* racine){
  
  Noeud_AVL* max = NULL;

  // si abr est NULL, il n'y a rien ici, on return NULL
  if (abr == NULL){
    return NULL;
  }

  // si abr n'a pas de fils droit, c'est qu'il est deja le max de l'arbre
  if (abr->fd == NULL){
    
    max = abr;
    if (racine->fg != max){
      double a = 0;
      double b = 0;
      //if (max->fg != NULL) 
        //a= max->fg->y;

      if (max->pere->fd != NULL)
        b = max->pere->fd->y;
      max->pere->fd = max->fg;
         
      //printf("in !    racine = %7.0f    max = %7.0f  pere = %7.0f  fd_pere = %7.0f   fg_max = %7.0f \n", racine->y, max->y, max->pere->y, a, b );
      if (max->fg != NULL)
        max->fg->pere = max->pere;
    }

    return max;
  }
  else {
    // tant que le fils droit de abr possede un fils droit, 
    // c'est qu'il existe un noeud plus grand que abr dans le sous arbre droit de abr 
    max = supprimer_max(abr->fd, racine);
    maj_hauteur(abr);

    int hauteurfg = hauteur(abr->fg);
    int hauteurfd = hauteur(abr->fd);

    // on fait les rotations necessaires pour re-equilibrer l'AVL 
    // apres avoir retire notre element max

    if( hauteurfg - hauteurfd == 2 ) {
      if( hauteur(abr->fg->fg) <  hauteur(abr->fg->fd) )
        rotation_gauche(&(abr->fg), abr); 
      rotation_droite(&abr, abr->pere);
    }

    else if( hauteurfd - hauteurfg == 2 ) {
      if( hauteur( abr->fd->fg) >  hauteur( abr->fd->fd) )
        rotation_droite(&(abr->fd), abr);
      rotation_gauche(&abr, abr->pere);
    }
  }
  return max;
}


Noeud_AVL* supprimer_min(Noeud_AVL* abr, Noeud_AVL* racine){
  
  Noeud_AVL* min = NULL;

  // si abr est NULL, il n'y a rien ici, on return NULL
  if (abr == NULL){
    return NULL;
  }

  // si abr n'a pas de fils gauche, c'est qu'il est deja le min de l'arbre
  if (abr->fg == NULL){

    min = abr;
    if (racine->fd != min){
      min->pere->fg = min->fd;
      if (min->fd != NULL)
        min->fd->pere = min->pere;
    }

    return min;
  }
  else {
    // tant que le fils gauche de abr possede un fils gauche, 
    // c'est qu'il existe un noeud plus petit que abr->fg dans le sous arbre gauche de abr 
    min = supprimer_min(abr->fg, racine);
    maj_hauteur(abr);

    int hauteurfg = hauteur(abr->fg);
    int hauteurfd = hauteur(abr->fd);

    // on fait les rotations necessaires pour re-equilibrer l'AVL 
    // apres avoir retire notre element min

    if( hauteurfg - hauteurfd == 2 ) {
      if( hauteur(abr->fg->fg) <  hauteur(abr->fg->fd) )
        rotation_gauche(&(abr->fg), abr); 
      rotation_droite(&abr, abr->pere);
    }

    else if( hauteurfd - hauteurfg == 2 ) {
      if( hauteur( abr->fd->fg) >  hauteur( abr->fd->fd) )
        rotation_droite(&(abr->fd), abr);
      rotation_gauche(&abr, abr->pere);
    }
  }

  return min;
}




Noeud_AVL* Prem_segment_apres_AVL(Noeud_AVL* abr, double y){

  Noeud_AVL* resultat = NULL;
  Noeud_AVL* pt_c = abr;
  
  if (abr == NULL){
    return NULL;
  }  

  // on cherche un noeud plus grand ou egal a y
  while (pt_c != NULL){

    // si on est dans un noeud plus grand ou egal a y
    //   - on garde ce noeud 
    //   - on regarde dans le fils gauche s'il y en a pas un plus petit qui soit quand meme >= y
    if (pt_c->y >= y){
      resultat = pt_c;
      pt_c = pt_c->fg;
      //printf("go a gauche\n");
    }

    // si on est dans un noeud plus petit que y 
    //   - on cherche dans le fil droit un noeud plus grand
    else{
      pt_c = pt_c->fd;
      //printf("go a droite\n");
    }
  }
  return resultat;
}

  
Noeud_AVL* AuDessus_AVL(Noeud_AVL* abr, Noeud_AVL* elem){

  Noeud_AVL* resultat = NULL;
  Noeud_AVL* pt_c = abr; 

  if (abr == NULL){
    return NULL;
  }
  
  // si elem a un fils droit, l'element AuDessus sera le min du sous arbre droit
  if (elem->fd != NULL){
    resultat = elem->fd;
    while (resultat->fg != NULL){ 
      resultat = resultat->fg;
    }
    return resultat;
  }
  // si elem n'a pas de fd, l'element AuDessus sera le premier pere plus grand que lui.
  else {
    pt_c = elem;
    resultat = pt_c->pere;
    while (resultat != NULL){
      // si elem se trouve dans un sous arbre gauche d'un noeud pere, ce pere est AuDessus de elem.
      if (resultat->fg == pt_c){
        return resultat;
      }
      // si elem se trouve dans un sous arbre droit, alors ce pere est plus petit que elem 
      // on va voir le noeud grand-pere... etc
      pt_c = resultat;
      resultat = resultat->pere;
    }
  }
  // on atteint ce point en sortant de la 2eme boucle while... donc resultat == NULL 
  return resultat;
}
 


Netlist* intersect_balayage_AVL(Netlist* N){

  int i = 0;
  double y = 0;
  double y1 = 0;
  double y2 = 0;
  double ybas = 0;
  double yhaut = 0;
  Noeud_AVL* h = NULL;

  int taille = nb_extremite(N);

  Extremite** echeancier = creer_echeancier(N, taille);
  echeancier = tri_echeancier(echeancier, taille);
  /*
  // affiche toutes les extremites avec leur reseau, leur abscisses et leur VouGouD pour voir si le tableau est bien trie
  for (i = 0; i< taille; i++){
    printf("Res= %4d   p1= %2d    x= %7.0f      VouGouD= %d \n", echeancier[i]->PtrSeg->NumRes, echeancier[i]->PtrSeg->p1, echeancier[i]->x, echeancier[i]->VouGouD) ;
  } 
  */ 

  Noeud_AVL* abrT = NULL;
  int NBNOEUDS = 0;
  for (i=0; i< taille; i++){
    switch(echeancier[i]->VouGouD){

      case 1:   // si extremite gauche
        NBNOEUDS++;
	printf("%d 1    %d\n", i, NBNOEUDS);
        y = N->T_Res[echeancier[i]->PtrSeg->NumRes]->T_Pt[echeancier[i]->PtrSeg->p1]->y;
        abrT = Inserer_AVL(abrT, NULL, y, echeancier[i]->PtrSeg);
        //afficher_AVL(abrT, 0, 0);
        break;

      case 2:   // si extremite droite 
        NBNOEUDS--;
	printf("%d 2     %d\n",i, NBNOEUDS);
        //afficher_AVL(abrT, 0, 0);
        y = N->T_Res[echeancier[i]->PtrSeg->NumRes]->T_Pt[echeancier[i]->PtrSeg->p1]->y;
        //printf("Res= %4d   p1= %2d   p2= %2d  x= %7.0f  y= %7.0f   VouGouD= %d \n", echeancier[i]->PtrSeg->NumRes, echeancier[i]->PtrSeg->p1, echeancier[i]->PtrSeg->p2, echeancier[i]->x, y, echeancier[i]->VouGouD) ;   
        abrT = Supprimer_AVL(abrT, NULL, y, echeancier[i]->PtrSeg);
        //afficher_AVL(abrT, 0, 0);
        break;
    
      case 0:   // si extremite verticale 
	printf("%d 0    %d\n",i, NBNOEUDS);
        // on recupere les ybas et yhaut de ce segment vertical
        y1 = N->T_Res[echeancier[i]->PtrSeg->NumRes]->T_Pt[echeancier[i]->PtrSeg->p1]->y;
        y2 = N->T_Res[echeancier[i]->PtrSeg->NumRes]->T_Pt[echeancier[i]->PtrSeg->p2]->y;

        ybas = min(y1, y2);
        yhaut = max(y1, y2);

        // on cherche dans la liste des segments horizontaux, le premier seg >= ybas
        h = Prem_segment_apres_AVL(abrT, ybas);

        // on parcourt les segments horizontaux qui intersecte ce segment vertical 
        // en completant les Lintersec des ces segments et du segment vertical
        while ( h != NULL  && h->y <= yhaut){
          if (echeancier[i]->PtrSeg->NumRes != h->seg->NumRes){
             echeancier[i]->PtrSeg->Lintersec = ajouter_segment(echeancier[i]->PtrSeg->Lintersec,h->seg);
             h->seg->Lintersec = ajouter_segment(h->seg->Lintersec, echeancier[i]->PtrSeg);
          }
          h = AuDessus_AVL(abrT, h);

          //if (h != NULL)
            //printf("Res= %4d   p1= %2d    x= %7.0f   y = %7.0f   VouGouD= %d \n", echeancier[i]->PtrSeg->NumRes, echeancier[i]->PtrSeg->p1, echeancier[i]->x, h->y, echeancier[i]->VouGouD) ;
        }
        break; 
     
    }
  }
  supprimer_echeancier(echeancier, taille);
  return N;
}


void afficher_AVL(Noeud_AVL* abr, int a, int GouD){
  if (abr != NULL){
    int i = 0;
    for (i = 0; i<a;i++)
      printf("  ");
    printf("%d y=%.0f  h=%d  res=%d  p1=%d   p2=%d   %d\n",GouD,abr->y, abr->hauteur, abr->seg->NumRes, abr->seg->p1, abr->seg->p2,a);
    afficher_AVL(abr->fg,a+1,1);
    afficher_AVL(abr->fd,a+1,2);
  }
}

int verif_AVL(Noeud_AVL* abr,double ymin, int p1min, double ymax, int p1max){

  if (abr == NULL) 
    return 0;
  
  int erreur = 0;
  int erreur1 = 0;
  int erreur2 = 0;
  int erreur3 = 0;

  if (abr->y > ymax)
    erreur = 1;

  if (abr->y < ymin)
    erreur = 2;

  if (abr->y == ymax && abr->seg->p1 > p1max)
    erreur = 3;

  if (abr->y == ymin && abr->seg->p1 < p1min)
    erreur = 4;


  if (erreur > 0){
      printf("Erreur AVL : %d // %.0f     %d      %.0f     %.0d\n ", erreur, ymin, p1min, ymax, p1max);
      afficher_AVL(abr,0,0);
  }

  erreur1 = verif_AVL(abr->fg,ymin,p1min, abr->y,abr->seg->p1);
  erreur2 = verif_AVL(abr->fd,abr->y,abr->seg->p1,ymax,p1max);
  erreur = max(erreur1, erreur2);
  erreur3 = max(erreur3, erreur);
  return erreur3;
}


int verif_hauteur_AVL(Noeud_AVL* abr){
  if (abr == NULL) 
    return 0;

  int erreur = 0;
  int erreur1 = 0;
  int erreur2 = 0;
  int erreur3 = 0;

  if (hauteur(abr->fg)- hauteur(abr->fd) >=2 || hauteur(abr->fg)- hauteur(abr->fd) <= -2){
    printf("\nErreur hauteur: y= %f  h= %d    hg = %d     hd = %d\n", abr->y, abr->hauteur, hauteur(abr->fg),hauteur(abr->fd));
    afficher_AVL(abr,0,0);
    erreur = 1;
  }

  erreur1 = verif_hauteur_AVL(abr->fg);
  erreur2 = verif_hauteur_AVL(abr->fd);
  erreur3 = max(erreur1, erreur2);
  erreur3 = max(erreur3, erreur);
  return erreur3;
}


