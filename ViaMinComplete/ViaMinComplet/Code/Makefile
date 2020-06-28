all : CopieNetlist VisuNetlist Sauv_Intersec_Naif Sauv_Intersec_Balayage Sauv_Intersec_AVL AfficheGraphe VisuGraphe VisuViaTableauS AfficheCycleImpair VisuViaCycleImpair

VisuViaCycleImpair : VisuViaCycleImpair.o graphe.o netlist.o entree_sortie.o SVGwriter.o
	gcc -o VisuViaCycleImpair VisuViaCycleImpair.o graphe.o netlist.o entree_sortie.o SVGwriter.o

AfficheCycleImpair : AfficheCycleImpair.o graphe.o netlist.o entree_sortie.o
	gcc -o AfficheCycleImpair AfficheCycleImpair.o graphe.o netlist.o entree_sortie.o

VisuViaTableauS : VisuViaTableauS.o graphe.o netlist.o entree_sortie.o SVGwriter.o
	gcc -o VisuViaTableauS VisuViaTableauS.o graphe.o netlist.o entree_sortie.o SVGwriter.o

VisuGraphe : VisuGraphe.o graphe.o netlist.o entree_sortie.o SVGwriter.o
	gcc -o VisuGraphe VisuGraphe.o graphe.o netlist.o entree_sortie.o SVGwriter.o

AfficheGraphe : AfficheGraphe.o graphe.o netlist.o entree_sortie.o
	gcc -o AfficheGraphe AfficheGraphe.o graphe.o netlist.o entree_sortie.o

Sauv_Intersec_AVL : Sauv_Intersec_AVL.o netlist.o entree_sortie.o
	gcc -o Sauv_Intersec_AVL Sauv_Intersec_AVL.o netlist.o entree_sortie.o

Sauv_Intersec_Balayage : Sauv_Intersec_Balayage.o netlist.o entree_sortie.o
	gcc -o Sauv_Intersec_Balayage Sauv_Intersec_Balayage.o netlist.o entree_sortie.o

Sauv_Intersec_Naif : Sauv_Intersec_Naif.o netlist.o entree_sortie.o
	gcc -o Sauv_Intersec_Naif Sauv_Intersec_Naif.o netlist.o entree_sortie.o

VisuNetlist : VisuNetlist.o netlist.o entree_sortie.o SVGwriter.o
	gcc -o VisuNetlist VisuNetlist.o netlist.o entree_sortie.o SVGwriter.o

CopieNetlist : CopieNetlist.o netlist.o entree_sortie.o
	gcc -o CopieNetlist CopieNetlist.o netlist.o entree_sortie.o

VisuViaCycleImpair.o : VisuViaCycleImpair.c graphe.h netlist.h entree_sortie.h SVGwriter.h
	gcc -c -o VisuViaCycleImpair.o VisuViaCycleImpair.c

AfficheCycleImpair.o : AfficheCycleImpair.c graphe.h netlist.h entree_sortie.h
	gcc -c -o AfficheCycleImpair.o AfficheCycleImpair.c 

VisuViaTableauS.o : VisuViaTableauS.c graphe.h netlist.h entree_sortie.h SVGwriter.h
	gcc -c -o VisuViaTableauS.o VisuViaTableauS.c

VisuGraphe.o : VisuGraphe.c graphe.h netlist.h entree_sortie.h SVGwriter.h
	gcc -c -o VisuGraphe.o VisuGraphe.c

AfficheGraphe.o : AfficheGraphe.c graphe.h netlist.h entree_sortie.h
	gcc -c -o AfficheGraphe.o AfficheGraphe.c 

graphe.o : graphe.c netlist.h entree_sortie.h
	gcc -c -o graphe.o graphe.c

Sauv_Intersec_AVL.o : Sauv_Intersec_AVL.c netlist.h entree_sortie.h
	gcc -c -o Sauv_Intersec_AVL.o Sauv_Intersec_AVL.c

Sauv_Intersec_Balayage.o : Sauv_Intersec_Balayage.c netlist.h entree_sortie.h
	gcc -c -o Sauv_Intersec_Balayage.o Sauv_Intersec_Balayage.c

Sauv_Intersec_Naif.o : Sauv_Intersec_Naif.c netlist.h entree_sortie.h
	gcc -c -o Sauv_Intersec_Naif.o Sauv_Intersec_Naif.c

VisuNetlist.o : VisuNetlist.c netlist.h entree_sortie.h SVGwriter.h
	gcc -c -o VisuNetlist.o VisuNetlist.c

CopieNetlist.o : CopieNetlist.c netlist.h entree_sortie.h
	gcc -c -o CopieNetlist.o CopieNetlist.c

SVGwriter.o : SVGwriter.c
	gcc -c -o SVGwriter.o SVGwriter.c

netlist.o : netlist.c entree_sortie.h
	gcc -c -o netlist.o netlist.c

entree_sortie.o : entree_sortie.c
	gcc -c -o entree_sortie.o entree_sortie.c

clean : 
	rm -f *.o CopieNetlist VisuNetlist Sauv_Intersec_Naif Sauv_Intersec_Balayage Sauv_Intersec_AVL AfficheGraphe VisuGraphe VisuViaTableauS AfficheCycleImpair VisuViaCycleImpair
 
