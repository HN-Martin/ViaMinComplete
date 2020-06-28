#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netlist.h"
#include "entree_sortie.h"

int main(int argc, char** argv){

  Netlist* n = NULL;
  char s1[100];
  char s2[15] = ".balayAVL.int";
  
  if ( argc != 2) {
    printf ( "Erreur format: %s <NomFichier>\n", argv[0]);
    return 1;
  }
  printf("Lecture de la Netlist en cours...\n");
  n = lecture_netlist(argv[1], n);
  strcpy(s1, argv[1]);
  strcat(s1, s2);
  printf("recherche des intersections (avec AVL) de la Netlist en cours...\n");
  n = intersect_balayage_AVL(n);
  printf("sauvegarde des intersections de la Netlist en cours...\n");
  sauvegarde_intersection(s1, n);
  printf("sauvegarde effectuée dans le fichier '%s' avec succès.\n", s1);

  supprimer_netlist(n);
  return 0;
  
}
	
