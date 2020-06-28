#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "netlist.h"
#include "entree_sortie.h"

int main(int argc, char** argv){

  Netlist* n = NULL;
  char s1[100];
  char s2[15] = ".balay.int";
  FILE* f;

  clock_t t_init;
  clock_t t_final;
  double tps;
  
	if ( argc != 2) {
		printf ( "Erreur format: %s <NomFichier>\n", argv[0]);
		return 1;
	}
  printf("Lecture de la Netlist en cours...\n");
  n = lecture_netlist(argv[1], n);
  strcpy(s1, argv[1]);
  strcat(s1, s2);
  printf("recherche des intersections de la Netlist en cours...\n");
  t_init = clock();
  n = intersect_balayage(n);
  t_final = clock();
  tps = ((double)(t_final - t_init))/CLOCKS_PER_SEC;
  printf("\ntemps de recherche des intersections : %lf \n\n", tps);


  f = fopen("temps_calc.txt", "a");  
  fprintf(f,"%f \n",tps);
  fclose(f);

  printf("sauvegarde des intersections de la Netlist en cours...\n");
  sauvegarde_intersection(s1, n);
  printf("sauvegarde effectuée dans le fichier '%s' avec succès.\n", s1);

  supprimer_netlist(n);
  return 0;
  
}
	
