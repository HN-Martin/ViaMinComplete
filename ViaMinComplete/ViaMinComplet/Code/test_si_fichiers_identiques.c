#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
  char buffer1[200];
  char buffer2[200];
  
  int ligne_trouvee = 0;
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

  fclose(f2);

  while (fgets(buffer1, 200, f1) != NULL){
    f2 = fopen(argv[2],"r");
    num_ligne++;
    while (fgets(buffer2, 200, f2)){

      if (strcmp(buffer1,buffer2) == 0){
        ligne_trouvee++;
        
      }
    }
    if (ligne_trouvee != 1){
      if (ligne_trouvee == 0){
        printf("La ligne n°%d du fichier 1 n'a pas été trouvée dans le fichier 2.\n",num_ligne);
      }
      if (ligne_trouvee > 1){
        printf("La ligne n°%d du fichier 1 a été trouvée %d fois dans le fichier 2.\n",num_ligne, ligne_trouvee);
      }
      printf("%s\n",buffer1);

      fclose(f1);
      fclose(f2);
      return 0;
    }
    ligne_trouvee = 0;
    fclose(f2);
  }

  printf("Les 2 fichiers sont bien identiques.\n");

  fclose(f1);

  return 0;
}


