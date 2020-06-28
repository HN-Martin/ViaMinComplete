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
  char s[120] = "svg_via_de_";
  strcat(s, argv[1]);

  clock_t t_init;
  clock_t t_final;
  double tps;

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

  printf("Lecture du graphe en cours...\n");
  g = lecture_graphe(argv[1],argv[2]);
  n = lecture_netlist(argv[1], n);
  printf("nombre de sommets : %d\n", g->nbS);
  printf("Répartition des segments sur les 2 faces et création des vias en cours...\n");

  t_init = clock();
  int * S = remplir_tableau_S(g);
  t_final = clock();
  tps = ((double)(t_final - t_init))/CLOCKS_PER_SEC;
  printf("temps de creations des vias : %lf \n", tps);

  int nbVias = 0;
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
      if (S[i] == 0){
        SVGpointColor(svg,"#000000");
        nbVias++;

        // on va dessiner le sommet correspondant au point
        x = g->tabS[i]->pt->x;
        y = g->tabS[i]->pt->y;
        SVGpoint(svg, x, y); 
      }
    }
    else {    // si c'est un segment
 
      // on le dessine en Rouge si face 1, en Blue si face 2 
      if (S[i] == 1){        
        SVGlineColor(svg,"#FF0000");
      }
      else {
        SVGlineColor(svg,"#0000FF");
      }

      // on recupere les coordonnees x,y des points p1/p2 de ce sommet-segment
      x1 = n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p1]->x;
      y1 = n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p1]->y;

      x2 = n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p2]->x;
      y2 = n->T_Res[g->tabS[i]->seg->NumRes]->T_Pt[g->tabS[i]->seg->p2]->y;

      SVGline(svg, x1, y1, x2, y2);     
    }
  }
  
  SVGfinalize(svg);
  supprimer_netlist(n);
  free(S);
  
  strcat(s,".html");
  printf("Modélisation du graphe terminée avec succès.\n");
  printf("   >>> Il y a %d vias sur ce graphe <<<\n", nbVias);
  printf("Visualisation disponible dans le fichier '%s'.\n", s);

  return 0;
  
}
