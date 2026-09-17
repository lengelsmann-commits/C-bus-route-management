#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"

#define fileName "trajets_bus.csv"

int main() {
    Trajet t[MAX_TRAJETS];
    int nbTrajets;

    printf("Chargement des trajets depuis %s...\n", fileName);

    nbTrajets = chargerTrajets_boyat(fileName, t, MAX_TRAJETS);

    if(nbTrajets < 0) {
        printf("Erreur lors du chargement du fichier\n");
        return EXIT_FAILURE;
    }

    printf("Succes! %d trajet(s) charge(s).\n", nbTrajets);

    int choix;
    int continuer = 1;

    while(continuer) {
        printf("\n========================================\n");
        printf("    HORIZONS BUS - Gestion des trajets\n");
        printf("========================================\n");
        printf("1. Afficher tous les trajets\n");
        printf("2. Rechercher un trajet par numero de bus\n");
        printf("3. Afficher les trajets tries par ville de depart et date de depart\n");
		printf("4. Ajouter/supprimer un client a un trajet\n");
        printf("5. Modification/ajout du nom du client/prix billet par numero de bus et id client\n");
		printf("6. Sauvegarder les trajets dans un nouveau fichier\n");
        printf("7. Un filtre sur les trajets partant d'une ville, partant a une date, arrivant le lendemain\n");
        printf("8. Un filtre sur les trajets combinant ville de depart, ville d'arrivee et date de depart\n");
        printf("9. Trajet trier par chiffre d'affaire\n");
        printf("10. Optimiser le reseau (supprimer bus deficitaires)\n");
        printf("11. Quitter\n");
        printf("========================================\n");
        printf("Votre choix: ");

        if (scanf("%d", &choix) != 1) {
			while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter boucle infinie)
        }

        switch(choix) {
        case 1:
            afficherTousLesTrajets_boyat(t, nbTrajets);
            break;
        case 2: {
            int numBus;
            printf("\nEntrez le numero du bus: ");
            if (scanf("%d", &numBus) != 1) {
				printf("\nEntree invalide. Veuillez entrer un numero valide.\n");
                while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                break;
            }
            afficherTrajetParBus_boyat(t, nbTrajets, numBus);
            break;
        }
        case 3:{
            trierTrajets_boyat_engelsmann(t, nbTrajets);
            afficherTousLesTrajets_boyat(t, nbTrajets);
            break;
        }
        case 4: {
			int ajouterSupprimer;
			printf("\nVoulez-vous ajouter (1) ou supprimer (0) un client? ");
            if (scanf("%d", &ajouterSupprimer) != 1) {
                printf("\nEntree invalide. Veuillez entrer un numero valide.\n");
                while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                break;
            }

            int numBus;
            printf("\nEntrez le numero du bus: ");
            if (scanf("%d", &numBus) != 1) {
                printf("\nEntree invalide. Veuillez entrer un numero valide.\n");
                while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                break;
            }

            if (ajouterSupprimer) {
                Client client;

                printf("\nEntrez le prix du billet: ");
                if (scanf("%lf", &client.prixBillet) != 1) {
                    printf("\nEntree invalide. Veuillez entrer un prix valide.\n");
                    while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                    break;
                }
                client.prixPresent = 1;

				getchar(); // vider le buffer avant fgets (pour eviter le skip du fgets sur nom)
				printf("\nEntrez le nom du client: ");
				fgets(client.nom, sizeof(client.nom), stdin);
				retirerRetourChariot_cours(client.nom);
				client.nomPresent = 1;

				ajouterClient_engelsmann(t, nbTrajets, numBus, client);
            }
            else {
                int client_id;

                printf("\nEntrez l'id du client: ");
                if (scanf("%d", &client_id) != 1) {
                    printf("\nEntree invalide. Veuillez entrer un id valide.\n");
                    while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                    break;
                }

				retirerClient_engelmann(t, nbTrajets, numBus, client_id);
            }

            break;
        }
        case 5: {
            int modifNomBillet;
            printf("\nVoulez-vous modifier le nom (1) ou modifier le prix du bille (0) du client? ");
            if (scanf("%d", &modifNomBillet) != 1) {
                printf("\nEntree invalide. Veuillez entrer un numero valide.\n");
                while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                break;
            }

            int numBus;
            printf("\nEntrez le numero du bus: ");
            if (scanf("%d", &numBus) != 1) {
                printf("\nEntree invalide. Veuillez entrer un numero valide.\n");
                while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                break;
            }

            int idClient;
            printf("\nEntrez l'id du client: ");
            if (scanf("%d", &idClient) != 1) {
                printf("\nEntree invalide. Veuillez entrer un numero valide.\n");
                while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                break;
            }

            if (modifNomBillet) {
				char nomClient[30];
				printf("\nEntrez le nouveau nom du client: ");
				getchar(); // vider le buffer avant fgets (pour eviter le skip du fgets sur nom)
				fgets(nomClient, sizeof(nomClient), stdin);
                retirerRetourChariot_cours(nomClient);

				ajouteNomClient_boyat_engelsmann(t, nbTrajets, numBus, idClient, nomClient);
            }
            else {
				double prixBillet;
                printf("\nEntrez le nouveau prix du billet: ");
                if (scanf("%lf", &prixBillet) != 1) {
                    printf("\nEntree invalide. Veuillez entrer un prix valide.\n");
                    while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                    break;
				}

				ajoutePrixClient_boyat_engelsmann(t, nbTrajets, numBus, idClient, prixBillet);
            }

            break;
        }
        case 6: {
			char nomFichierSauvegarde[50];
			printf("\nEntrez le nom du fichier de sauvegarde: ");
			getchar(); // vider le buffer avant fgets (pour eviter le skip du fgets sur nom)
			fgets(nomFichierSauvegarde, sizeof(nomFichierSauvegarde), stdin);
			retirerRetourChariot_cours(nomFichierSauvegarde);

			sauvegarderTrajets_boyat_engelsmann(nomFichierSauvegarde, t, nbTrajets);
            break;
        }
        case 7: {
            char nomVilleDepart[20];
            Date dateDepart;

            printf("\nEntrez le nom de la ville de depart: ");
			getchar(); // vider le buffer avant fgets (pour eviter le skip du fgets sur nom)
			fgets(nomVilleDepart, sizeof(nomVilleDepart), stdin);

            retirerRetourChariot_cours(nomVilleDepart);

            printf("\nEntrez la date de depart (jj mm aaaa): ");
            if (scanf("%d %d %d", &dateDepart.jour, &dateDepart.mois, &dateDepart.annee) != 3) {
                printf("\nEntree invalide. Veuillez entrer une date valide.\n");
                while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                break;
            }

			filtrerTrajetsVilleDateLendemain_engelsmann(t, nbTrajets, nomVilleDepart, dateDepart);
            break;
        }
        case 8: {
			char nomVilleDepart[20];
			char nomVilleArrivee[20];
			Date dateDepart;

			printf("\nEntrez le nom de la ville de depart: ");
			getchar(); // vider le buffer avant fgets (pour eviter le skip du fgets sur nom)
			fgets(nomVilleDepart, sizeof(nomVilleDepart), stdin);

			printf("\nEntrez le nom de la ville d'arrivee: ");
			fgets(nomVilleArrivee, sizeof(nomVilleArrivee), stdin);

			retirerRetourChariot_cours(nomVilleDepart);
			retirerRetourChariot_cours(nomVilleArrivee);

			printf("\nEntrez la date de depart (jj mm aaaa): ");
            if (scanf("%d %d %d", &dateDepart.jour, &dateDepart.mois, &dateDepart.annee) != 3) {
                printf("\nEntree invalide. Veuillez entrer une date valide.\n");
                while (getchar() != '\n'); // vider buffer entrer scanf (pour eviter le skip du scanf sur choix)
                break;
			}

            filtrerTrajetsDepartArriveeDate_boyat(t, nbTrajets, nomVilleDepart, nomVilleArrivee, dateDepart);

            break;
        }
        case 9: {
            trierTrajetsCA_boyat_engelsmann(t, nbTrajets);
            afficherTousLesTrajets_boyat(t, nbTrajets);
            break;
        }
        case 10: {
            optimiserReseau_boyat_engelsmann(t, nbTrajets);
            break;
        }
        case 11:
            continuer = 0;
            printf("\nAu revoir!\n");
            break;
        default:
            printf("\nChoix invalide. Veuillez choisir entre 1 et 11.\n");
        }
    }

    return 0;
}
