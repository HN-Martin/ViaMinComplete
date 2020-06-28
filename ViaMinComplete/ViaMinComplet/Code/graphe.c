#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "netlist.h"
#include "graphe.h"
#include "entree_sortie.h"

Graphe* creer_graphe(int nbS){
  Graphe* nv_graphe = (Graphe*)malloc(sizeof(Graphe)*1);
  nv_graphe->nbS = nbS;
  nv_graphe->nbA = 0;
  nv_graphe->tabS = (Sommet**)malloc(sizeof(Sommet*)*nbS);

  return nv_graphe;
}

Sommet* creer_sommet(int numS, int SouP, Segment* seg, Point* pt){
  Sommet* s = (Sommet*)malloc(sizeof(Sommet)*1);
  s->numS = numS;
  s->nbA = 0;
  s->LA = NULL;
  s->SouP = SouP;
  s->seg = seg;
  s->pt = pt;

  return s;
}

Arete* creer_arete(int numA, int u, int v){
  Arete* a = (Arete*)malloc(sizeof(Arete)*1);
  a->numA = numA;
  a->u = u;
  a->v = v;

  return a;
}

void ajouter_arete(Sommet* s1, Sommet* s2, Graphe* g){

  ElementListeA* ElemListeA_1 = (ElementListeA*)malloc(sizeof(ElementListeA)*1);
  ElemListeA_1->a = creer_arete(g->nbA, s1->numS, s2->numS);
  ElemListeA_1->suiv = s1->LA;
  s1->LA = ElemListeA_1;
  s1->nbA++;

  ElementListeA* ElemListeA_2 = (ElementListeA*)malloc(sizeof(ElementListeA)*1);
  ElemListeA_2->a = creer_arete(g->nbA, s2->numS, s1->numS);
  ElemListeA_2->suiv = s2->LA;
  s2->LA = ElemListeA_2;
  s2->nbA++;

  g->nbA = g->nbA+1;

  return;
}



Graphe* lecture_graphe(char* nom_fichier_netlist, char* nom_fichier_intersec){
  
  Netlist* N = lecture_netlist(nom_fichier_netlist, N);

  Segment** tab_seg = tab_segment(N);
  int i = 0;
  int j = 0;
  int k = 0;
  int k1 = 0;

  int trouve = 0;
  char buffer[100];
  int nbSommets = 0;

  int NumResA;
  int p1_A;   
  int p2_A;   
  int NumResB;
  int p1_B;   
  int p2_B;

  Sommet* sA;
  Sommet* sB;
 
  for (i=0; i < N->NbRes; i++){
    nbSommets += N->T_Res[i]->NbPt + N->T_Res[i]->NbSeg;
  }

  Graphe* g = creer_graphe(nbSommets);


  for (i=0; i < N->NbRes; i++){
    for (j=0; j < N->T_Res[i]->NbPt ; j++){
      // on cree un sommet par Point
      g->tabS[k+k1] = creer_sommet(k+k1, 0, NULL, N->T_Res[i]->T_Pt[j] );

      k1++;
    }
    for (j=0; j < N->T_Res[i]->NbSeg ; j++){  
      // on cree un sommet par Segment
      g->tabS[k+k1] = creer_sommet(k+k1, 1, N->T_Res[i]->T_Seg[j], NULL );

      // on cree l'arete entre le Segment et p1
      ajouter_arete(g->tabS[k+k1],g->tabS[k+ (N->T_Res[i]->T_Seg[j]->p1) ],g); 
      // on cree l'arete entre le Segment et p2   
      ajouter_arete(g->tabS[k+k1],g->tabS[k+ (N->T_Res[i]->T_Seg[j]->p2) ],g); 
      g->nbA+=2;
      k1++;
    }
    
    k+= N->T_Res[i]->NbPt + N->T_Res[i]->NbSeg;
    k1 = 0;
  }


  // ouverture du fichier en mode lecture
  FILE* f = fopen(nom_fichier_intersec,"r");
  
  //test si l'ouverture s'est bien passee
  if (f == NULL){
    fprintf(stderr,"Erreur à l'ouverture du fichier '%s'\n", nom_fichier_intersec);
    return NULL;
  }
  
  while (fgets(buffer, 100, f) != NULL){

    sscanf(buffer, "%d %d %d %d %d %d", &NumResA, &p1_A, &p2_A, &NumResB, &p1_B, &p2_B);

    trouve = 0;
    for (i=0; i < nbSommets; i++){
      if (g->tabS[i]->SouP == 1 && g->tabS[i]->seg->NumRes == NumResA && g->tabS[i]->seg->p1 == p1_A && g->tabS[i]->seg->p2 == p2_A){
        sA = g->tabS[i];
        trouve++;
      }

      if (g->tabS[i]->SouP == 1 && g->tabS[i]->seg->NumRes == NumResB && g->tabS[i]->seg->p1 == p1_B && g->tabS[i]->seg->p2 == p2_B){
        sB = g->tabS[i];
        trouve++;
      }

      if (trouve == 2){
        break;
      }
    }

    ajouter_arete(sA,sB,g);  
    g->nbA++;
  }

  fclose(f);
  return g;
}


void affiche_graphe(Graphe* g){
  printf("\nnombre de sommets : %d        nombre d'aretes : %d\n\n", g->nbS, g->nbA);
  int i = 0;
  for (i=0; i< g->nbS; i++){
    if (g->tabS[i]->SouP == 0){
      printf("Point : numS %d  nbA %d    Res= %d  x= %lf y= %lf \n",g->tabS[i]->numS, g->tabS[i]->nbA, g->tabS[i]->pt->num_res, g->tabS[i]->pt->x, g->tabS[i]->pt->y);
    }
    else{
       printf("Segment : numS %d  nbA %d    Res= %d  p1= %d p2= %d  \n",g->tabS[i]->numS, g->tabS[i]->nbA, g->tabS[i]->seg->NumRes, g->tabS[i]->seg->p1, g->tabS[i]->seg->p2);  
    }
  }

}   

void affiche_graphe_avec_aretes(Graphe* g){

  ElementListeA* pt_c = NULL;

  printf("\nnombre de sommets : %d        nombre d'aretes : %d\n\n", g->nbS, g->nbA);
  int i = 0;
  for (i=0; i< g->nbS; i++){
    if (g->tabS[i]->SouP == 0){
      printf("Point : numS %d  nbA %d    Res= %d  x= %lf y= %lf \n",g->tabS[i]->numS, g->tabS[i]->nbA, g->tabS[i]->pt->num_res, g->tabS[i]->pt->x, g->tabS[i]->pt->y);
    }
    else{
       printf("Segment : numS %d  nbA %d    Res= %d  p1= %d p2= %d  \n",g->tabS[i]->numS, g->tabS[i]->nbA, g->tabS[i]->seg->NumRes, g->tabS[i]->seg->p1, g->tabS[i]->seg->p2);  
    }
    pt_c = g->tabS[i]->LA;
    while(pt_c != NULL){
      printf("  Arete : numA %d  numSommet_s1 %d   numSommet_s2 %d\n", pt_c->a->numA, pt_c->a->u, pt_c->a->v);
      pt_c = pt_c->suiv;
    }
  }
}  



// EXERCICE 6 


int* remplir_tableau_S(Graphe* g){
  int* S = (int*)malloc(sizeof(int)*g->nbS);
  int i = 0;

  ElementListeA* pt_c = NULL;
  Sommet* s1 = NULL;
  Sommet* s2 = NULL;

  int segH_trouve = 0;
  int segV_trouve = 0;

  for (i=0; i< g->nbS; i++){
    if (g->tabS[i]->SouP == 1){   // si on a un sommet-seg 
      if (g->tabS[i]->seg->HouV == 0){   // si c'est un segment horizontal on le met sur la face 1
        S[i] = 1;
      }
      else{                 // sinon, s'il est vertical, on le met sur la face 2
        S[i] = 2;
      }
    }

    else{     // si on a un sommet-pt
      segH_trouve = 0;
      segV_trouve = 0;
      pt_c = g->tabS[i]->LA;    

      while (pt_c != NULL){
	s1 = g->tabS[pt_c->a->u];
	s2 = g->tabS[pt_c->a->v];
        // un sommet-point est forcement relie uniquement a des sommets-segments donc s2 est un sommet-seg
        if (s2->seg->HouV == 0){
          segH_trouve = 1;
        }
        if (s2->seg->HouV == 1){
          segV_trouve = 1;
        }
        
        pt_c = pt_c->suiv;
      }
      
      // si ce sommet-pt est relie a au moins : 1 seg V et 1 seg H alors, on met un via
      if (segH_trouve == 1 && segV_trouve == 1){
        S[i] = 0;
      }
      else{
        S[i] = 42; // un nombre positif quelconque...
      }

    }
  }

  return S;
}



// EXERCICE 7 

ListeSommet* detecte_cycle_impair(Graphe* g, int* S, int* tabMarquage){

  int i = 0;
  int trouve = 0;
  ListeSommet* res = NULL;
  ListeSommet* pt_p = NULL;  // pointeur prec
  ListeSommet* pt_c = NULL;  // pointeur courant
  ListeSommet* pt_n = NULL;  // pointeur next
  
  // on parcourt les sommets (qui ne sont pas encore explorés
  for (i = 0; i < g->nbS; i++){  
    if (tabMarquage[i] == -1){  // on choisit le 1er sommet inexploré que l'on trouve
      // on va l'explorer recursivement 
      trouve = detecte_cycle_impair_rec(g, S, tabMarquage, i, -1, &res);

      // si on detecte un cycle impair, on s'arrete et on le retourne.
      if (trouve != 0){

        pt_c = res->suiv;
        while(pt_c->numS != trouve){
          pt_p = pt_c;
          pt_c = pt_c->suiv;
        }
        pt_p->suiv = NULL;
        
        while(pt_c != NULL){
          pt_n = pt_c->suiv;
          free(pt_c);
          pt_c = pt_n;
        }


        return res;
      }

    }
  }


  return res;
}


// retourne 0 si pas de cycle impair
// retourne le numS si on trouve un cycle impair, 
int detecte_cycle_impair_rec(Graphe* g, int* S, int* tabMarquage, int depart, int pere, ListeSommet** res ){

  ListeSommet* nv_sommet = NULL;
  Sommet* s = g->tabS[depart];
  ElementListeA* pt_c = s->LA;
  int v = 0;
  int resultat = 0;

  // Depart (pere == -1) : on met le premier sommet sur la face 1 
  if (pere == -1){
    *res = creer_ListeSommet(depart);
    tabMarquage[depart] = 1;
  }

  // si on tombe sur un Sommet via, on s'arrete 
  else if (tabMarquage[depart] == 0){
    return 0;
  }

  // si on tombe sur un sommet inexploré (-1) on le met sur la face opposée au sommet precedent
  else if (tabMarquage[depart] == -1){ 
    if (tabMarquage[pere] == 1){
      tabMarquage[depart] = 2;
    }
    else if (tabMarquage[pere] == 2){
      tabMarquage[depart] = 1;
    }
    nv_sommet = creer_ListeSommet(depart);
    nv_sommet->suiv = *res;
    *res = nv_sommet;
  }

  // si on a 1->1 ou 2->2 on a trouve un cycle impair
  // on renvoie le numero du sommet 
  else if  (tabMarquage[pere] == tabMarquage[depart]){
    nv_sommet = creer_ListeSommet(depart);
    nv_sommet->suiv = *res;
    *res = nv_sommet;
    //Afficher_ListeSommet(*res);
    return depart;
  }
  else{ // si on est sur un sommet ni a -1, ni a 0 (donc soit 1, soit 2) et qui n'est pas un cycle impair 
    return 0;  // on ne continue pas, il n'y a pas de cycle impair
  }

  
  
  while (pt_c != NULL){
    v = pt_c->a->v;
    resultat = detecte_cycle_impair_rec(g, S, tabMarquage, v, depart, res);
    
    if (resultat != 0){
      return resultat;
    }
    pt_c = pt_c->suiv;
  }
  
  // si on a explore toutes les aretes voisines sans trouver de cycle, on peut supprimer ce sommet de la liste chainee
  *res = (*res)->suiv;
  free(nv_sommet);
  return 0;
}

ListeSommet* creer_ListeSommet(int numSommet){
  ListeSommet* LS = (ListeSommet*)malloc(sizeof(ListeSommet)*1);
  LS->numS = numSommet;
  LS->suiv = NULL;

  return LS;
}

void Afficher_ListeSommet(ListeSommet* LS){
  while( LS != NULL){
    printf("%d ", LS->numS);
    LS = LS->suiv;
  }

  printf("\n");
  return;
}

int* ajout_vias_cycle_impair(Graphe* g){
  int* S = (int*)malloc(sizeof(int)*g->nbS);
  int* tabMarquage = (int*)malloc(sizeof(int)*g->nbS);

  int i = 0;
  int fin = 0;
  ListeSommet* cycle = NULL;
  ListeSommet* pt_c = NULL;
  ListeSommet* pt_n = NULL;

  for (i=0; i< g->nbS; i++){
    S[i] = -1;
  }
  
  while (fin ==0){
    // on initialise le tableau de marquage 
    for (i = 0; i < g->nbS; i++){
      if (S[i] == 0){
        tabMarquage[i] = 0;
      }
      else{
        tabMarquage[i] = -1;
      }
    }

    // on regarde s'il reste des cycles impairs
    cycle = detecte_cycle_impair(g, S, tabMarquage);

    if (cycle != NULL){
      // si on trouve un cycle, on va mettre un via sur le premier Sommet du cycle qui sera un point !
      pt_c = cycle; 
      while (pt_c != NULL){
        if (g->tabS[pt_c->numS]->SouP == 0){
          S[pt_c->numS] = 0;
          break;
        }
        pt_c = pt_c->suiv;
      }
    }
    else{
      fin = 1;
    }
   
    // on libere la liste chainee du cycle
    pt_c = cycle; 
    while (pt_c != NULL){
      pt_n = pt_c->suiv;
      free(pt_c);
      pt_c = pt_n;
    }
    
  }
  
  // correspond a la fonction bicolore...
  // le tableau de marquage utilisé dans la detection se remplit comme demandé dans la fonction bicolore de l'enonce
  // en prenant le derdier tableau de marquage qui ne trouve pas de cycle, on obtient la fonction bicolore.

  // Les sommets a 1 seront lies a des sommets a 2 et vice versa
  // les sommets a 0 restent a 0
  // il ne reste plus de sommet a -1
  for (i=0; i< g->nbS; i++){
    S[i] = tabMarquage[i];
  }

  free(tabMarquage);
  return S;
}

