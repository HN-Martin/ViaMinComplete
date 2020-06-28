#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netlist.h"
#include "graphe.h"
#include "entree_sortie.h"

int main(int argc, char** argv){

  Graphe* g = lecture_graphe(argv[1],argv[2]);
  affiche_graphe_avec_aretes(g);
  return 0;
}
