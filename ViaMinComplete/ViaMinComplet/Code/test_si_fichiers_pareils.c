#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
  char buffer1[200];
  char buffer2[200];
  
  int erreur = 0;
  int num_ligne = 0;
  if ( argc != 3) {
    printf ( "Erreur format: %s <NomFichier1> <NomFichier2>\n", argv[0]);
    return 1;
  }
  printf("comparaison des fichiers en cours...\n");

  // ouverture des fichiers en mode lecture
  FILE* f1 = fopen(argv[1],"r");
  FILE* f2 = fopen(argv[2],"r");

  //test si les ouvertures se sont bien passees
  if (f1 == NULL || f2 == NULL){
    fprintf(stderr,"Erreur à l'ouverture d'un des fichiers '%s' et '%s'\n", argv[1], argv[2]);
    return -1;
  }
  
  while (fgets(buffer1, 200, f1) != NULL){
    fgets(buffer2, 200, f2);
    num_ligne++;
    if (strcmp(buffer1,buffer2) != 0){
      printf("Il y a une différence dans les 2 fichiers au niveau de la ligne %d :\n",num_ligne);
      printf("%s\n",buffer1);
      printf("%s\n",buffer2);
      fclose(f1);
      fclose(f2);
      return 0;
    }
  }

  printf("Les 2 fichiers sont bien identiques.\n");

  fclose(f1);
  fclose(f2);
  return 0;
}


