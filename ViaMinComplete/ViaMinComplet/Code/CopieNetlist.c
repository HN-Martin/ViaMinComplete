#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netlist.h"
#include "entree_sortie.h"

int main(int argc, char** argv){

  Netlist* n = NULL;
  char s[100] = "copie_de_";
  
  if ( argc != 2) {
    printf ( "Erreur format: %s <NomFichier>\n", argv[0]);
    return 1;
  }
  printf("Lecture de la Netlist en cours...\n");
  n = lecture_netlist(argv[1], n);

  strcat(s, argv[1]);
  printf("Copie de la Netlist en cours...\n");
  ecriture_netlist(s, n);
  printf("Copie effectuée dans le fichier '%s' avec succès.\n", s);

  supprimer_netlist(n);
  return 0;
  
}
	
