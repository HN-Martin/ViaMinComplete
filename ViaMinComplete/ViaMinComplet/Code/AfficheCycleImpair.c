#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netlist.h"
#include "graphe.h"
#include "entree_sortie.h"

int main(int argc, char** argv){

  Graphe* g = lecture_graphe(argv[1],argv[2]);
  int* S = ajout_vias_cycle_impair(g);
  int i = 0;
  for (i=0; i < g->nbS; i++){
    printf("%d ", S[i]);
  }
  printf("\n");

  return 0;
}
