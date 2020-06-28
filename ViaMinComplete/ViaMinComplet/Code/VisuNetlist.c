#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "SVGwriter.h"
#include "netlist.h"
#include "entree_sortie.h"


int main(int argc, char** argv){

  srand(time(NULL));	

  if ( argc != 2) {
    printf ( "Erreur format: %s <NomFichier>\n", argv[0]);
    return 1;
  }
  int i = 0;
  int j = 0;
  Netlist* n = NULL;
  char s[100] = "svg_de_";
  strcat(s, argv[1]);
  SVGwriter* svg = (SVGwriter*)malloc(sizeof(SVGwriter)*1);  // IL FAUT ALLOUER NOUS MEME LE SVGwriter
  Cell_segment* pt_c = NULL; 
  Point* pMax = NULL;



  printf("Lecture de la Netlist en cours...\n");
  n = lecture_netlist(argv[1], n);
  printf("Modélisation graphique de la Netlist en cours...\n");
  
  pMax = recherche_point_max(n);
  SVGinit(svg, s, (int)(pMax->x)+10, (int)(pMax->y)+10);

  printf("%d %d \n", (int)(pMax->x), (int)(pMax->y));
  for (i=0; i< n->NbRes; i++){
  //pour chaque reseau, une couleur de points et de lignes
    SVGlineRandColor(svg);
    SVGpointColor(svg, svg->lineColor);

    //on dessine chaque point de ce reseau
    for(j=0; j<n->T_Res[i]->NbPt; j++){
      SVGpoint(svg, n->T_Res[i]->T_Pt[j]->x, n->T_Res[i]->T_Pt[j]->y);
    }

    // on trace chaque segment de ce reseau
    for(j=0; j<n->T_Res[i]->NbSeg; j++){
      SVGline(svg, n->T_Res[i]->T_Pt[n->T_Res[i]->T_Seg[j]->p1]->x, n->T_Res[i]->T_Pt[n->T_Res[i]->T_Seg[j]->p1]->y, n->T_Res[i]->T_Pt[n->T_Res[i]->T_Seg[j]->p2]->x, n->T_Res[i]->T_Pt[n->T_Res[i]->T_Seg[j]->p2]->y);
    }
  }
  
  SVGfinalize(svg);
  supprimer_netlist(n);
  supprimer_point(pMax);
  strcat(s,".html");
  printf("Modélisation de la Netlist terminée avec succès.\n");
  printf("Visualisation disponible dans le fichier '%s'.\n", s);

  return 0;
  
}
