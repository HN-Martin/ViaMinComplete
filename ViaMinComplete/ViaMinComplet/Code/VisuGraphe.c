#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "SVGwriter.h"
#include "netlist.h"
#include "graphe.h"
#include "entree_sortie.h"


int main(int argc, char** argv){

  srand(time(NULL));	

  if ( argc != 3) {
    printf ( "Erreur format: %s <NomFichierNetlist> <NomFichierIntersections>\n", argv[0]);
    return 1;
  }
  int i = 0;
  int j = 0;
  Netlist* n = NULL;
  Graphe* g = NULL;
  char s[120] = "svg_graphe_de_";
  strcat(s, argv[1]);

  SVGwriter* svg = (SVGwriter*)malloc(sizeof(SVGwriter)*1);  // IL FAUT ALLOUER NOUS MEME LE SVGwriter

  ElementListeA* pt_c = NULL;

  double x = 0;
  double y = 0; 

  double xmax = 0;
  double ymax = 0; 

  Sommet* s1 = NULL;
  double x1 = 0;
  double y1 = 0;

  Sommet* s2 = NULL;
  double x2 = 0;
  double y2 = 0;

  Point* pMax = NULL;

  printf("Lecture du graphe en cours...\n");
  g = lecture_graphe(argv[1],argv[2]);
  n = lecture_netlist(argv[1], n);

  printf("Modélisation graphique du graphe en cours...\n");

  // on recherche la taille max pour initialiser SVG a la bonne taille
  for (i=0; i< g->nbS; i++){
    if (g->tabS[i]->SouP == 0){
      x = g->tabS[i]->pt->x;
      y = g->tabS[i]->pt->y;
    }
    else {
      x = (n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p1]->x + n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p2]->x ) / 2.0;
      y = (n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p1]->y + n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p2]->y ) / 2.0;
    }

    if (x > xmax){
      xmax = x;
    }

    if (y > ymax){
      ymax = y;
    }
  }

  SVGinit(svg, s, xmax, ymax);

  for (i=0; i< g->nbS; i++){

    if (g->tabS[i]->SouP == 0){  // si c'est un sommet point

      SVGpointColor(svg,"#000000");
      SVGlineColor(svg,"#777777");

      // on va dessiner les aretes adjacentes a ce sommet en parcourant la liste d'adjacence LA
      pt_c = g->tabS[i]->LA;
      while (pt_c != NULL){ 
        // on recupere les sommets des aretes adjacentes
	s1 = g->tabS[pt_c->a->u];
	s2 = g->tabS[pt_c->a->v];

        // pour le sommet s1 de l'arete : 	
        // on recalcule les coordonnees des sommets des aretes selon si ce sont des sommets-pt ou des sommets-seg
        if (s1->SouP == 0){
          // pour les sommets-points, on prend les coordonnees x,y du point
	  x1 = s1->pt->x;
          y1 = s1->pt->y;
	}
	else {
          // pour les sommets-segments, on va retrouver les coordonnees du milieu du segment a l'aide de la netlist
          x1 = (n->T_Res[s1->seg->NumRes]->T_Pt[s1->seg->p1]->x + n->T_Res[s1->seg->NumRes]->T_Pt[s1->seg->p2]->x ) / 2.0;
          y1 = (n->T_Res[s1->seg->NumRes]->T_Pt[s1->seg->p1]->y + n->T_Res[s1->seg->NumRes]->T_Pt[s1->seg->p2]->y ) / 2.0;
        }

        // pareil pour le sommet s2 de l'arete
        if (s2->SouP == 0){
	  x2 = s2->pt->x;
          y2 = s2->pt->y;
	}
	else {
          x2 = (n->T_Res[s2->seg->NumRes]->T_Pt[s2->seg->p1]->x + n->T_Res[s2->seg->NumRes]->T_Pt[s2->seg->p2]->x ) / 2.0;
          y2 = (n->T_Res[s2->seg->NumRes]->T_Pt[s2->seg->p1]->y + n->T_Res[s2->seg->NumRes]->T_Pt[s2->seg->p2]->y ) / 2.0;
        }
 
        // on dessine un segment SVG pour cette arete
	SVGline(svg, x1, y1, x2, y2);     
        pt_c = pt_c->suiv;
      }

      // on oublie pas de dessiner le point ( a la fin, pour qu'il apparaisse au dessus des aretes)
      // on va dessiner le sommet correspondant au point
      x = g->tabS[i]->pt->x;
      y = g->tabS[i]->pt->y;
      SVGpoint(svg, x, y); 
    }
    else {    // si c'est un segment

      SVGpointColor(svg,"#770000");
      SVGlineColor(svg,"#FF0000");


      // on va dessiner les aretes adjacentes a ce sommet en parcourant la liste d'adjacence LA
      pt_c = g->tabS[i]->LA;
      while (pt_c != NULL){ 
        // on recupere les sommets des aretes adjacentes
	s1 = g->tabS[pt_c->a->u];
	s2 = g->tabS[pt_c->a->v];
     
        // si c'est une arete de conflit (entre 2 segments qui se croisent) on dessine l'arete 
        // les aretes de continuite ont deja toutes ete dessines a partir des sommets-pts 
        if (s1->SouP == 1 && s2->SouP == 1){
          x1 = (n->T_Res[s1->seg->NumRes]->T_Pt[s1->seg->p1]->x + n->T_Res[s1->seg->NumRes]->T_Pt[s1->seg->p2]->x ) / 2.0;
          y1 = (n->T_Res[s1->seg->NumRes]->T_Pt[s1->seg->p1]->y + n->T_Res[s1->seg->NumRes]->T_Pt[s1->seg->p2]->y ) / 2.0;
          x2 = (n->T_Res[s2->seg->NumRes]->T_Pt[s2->seg->p1]->x + n->T_Res[s2->seg->NumRes]->T_Pt[s2->seg->p2]->x ) / 2.0;
          y2 = (n->T_Res[s2->seg->NumRes]->T_Pt[s2->seg->p1]->y + n->T_Res[s2->seg->NumRes]->T_Pt[s2->seg->p2]->y ) / 2.0;
	  SVGline(svg, x1, y1, x2, y2);     
        }

        pt_c = pt_c->suiv;
      }

      // on oublie pas de dessiner le segment ( a la fin, pour qu'il apparaisse au dessus des aretes)
      // on va dessiner le sommet correspondant au segment
      x = (n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p1]->x + n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p2]->x ) / 2.0;
      y = (n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p1]->y + n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p2]->y ) / 2.0;
      SVGpoint(svg, x, y);  

    }  
  }

  SVGfinalize(svg);
  supprimer_netlist(n);

  strcat(s,".html");
  printf("Modélisation du graphe terminée avec succès.\n");
  printf("Visualisation disponible dans le fichier '%s'.\n", s);

  return 0;
  
}
