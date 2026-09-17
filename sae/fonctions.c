#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonctions.h"

int chargerTrajets_boyat(const char* nomFichier, Trajet trajets[], int maxTrajets) {
    FILE* file = fopen(nomFichier, "r");
    if(file == NULL)
    {
        perror("Erreur ouverture fichier");
        return -1;
    }

    int nbTrajets = 0;

    while(nbTrajets < maxTrajets && !feof(file)) {
        Trajet* t = &trajets[nbTrajets];
        t->nbClients = 0;
        t->horairesDepartPresents = 1;
        t->horairesArriveePresents = 1;

        if(fscanf(file, "%d,", &t->numBus) != 1)
            break;

        if (fscanf(file, "%49[^,],", t->villeDepart) != 1)
            break;

        if (fscanf(file, "%49[^,],", t->villeArrivee) != 1)
            break;

        if (fscanf(file, "%d/%d/%d,", &t->dateDepart.jour,&t->dateDepart.mois, &t->dateDepart.annee) != 3)
            break;

        if (fscanf(file, "%d,", &t->horaireDepart) != 1) {
            t->horairesDepartPresents = 0;
            t->horaireDepart = 0;
            fscanf(file, ","); // Consommer la virgule vide
        }

        if (fscanf(file, "%d,", &t->horaireArrivee) != 1) {
            t->horairesArriveePresents = 0;
            t->horaireArrivee = 0;
            fscanf(file, ",");
        }

        while(t->nbClients < 55) {
            Client* c = &t->clients[t->nbClients]; // pointeur du client actuellement selectionner
            c->nomPresent = 0;
            c->prixPresent = 0;
            strcpy(c->nom, "");
            c->prixBillet = 0.0;

            int peek = fgetc(file);
            if(peek == '\n' || peek == EOF) {
                break;  // Fin de ligne, plus de clients
            }
            ungetc(peek, file);

            if(fscanf(file, "%d:", &c->id) != 1)
                break;

            peek = fgetc(file);
            if(peek == ':') {
                // Nom vide, on passe au prix
                c->nomPresent = 0;
            }
            else {
                ungetc(peek, file);
                if(fscanf(file, "%49[^:]", c->nom) == 1 && strlen(c->nom) > 0) {
                    c->nomPresent = 1;
                }
                // Consommer le ':' après le nom
                fgetc(file);
            }

            peek = fgetc(file);
            if(peek == ',' || peek == '\n' || peek == EOF) {
                // Prix vide
                c->prixPresent = 0;
                c->prixBillet = 0.0;
                if(peek == '\n' || peek == EOF) {
                    t->nbClients++;
                    break;
                }
                // Si c'était ',', on continue avec le prochain client
            } else {
                ungetc(peek, file);
                if(fscanf(file, "%lf", &c->prixBillet) == 1) {
                    c->prixPresent = 1;
                }

                peek = fgetc(file);
                if(peek == '\n' || peek == EOF) {
                    t->nbClients++;
                    break;
                }
                // Si ce n'est pas ',', problème
                if(peek != ',') {
                    ungetc(peek, file);
                }
            }

            t->nbClients++;
        }

        nbTrajets++;
    }

    fclose(file);
    return nbTrajets;
}

void afficherHoraire_boyat_engelsmann(int horaire) {
    int heures = horaire / 100;
    int minutes = horaire % 100;
    printf("%02d:%02d", heures, minutes);
}

void afficherDate_boyat_engelmann(Date date) {
    printf("%02d/%02d/%d", date.jour, date.mois, date.annee);
}

void afficherTousLesTrajets_boyat(Trajet trajets[], int nbTrajets) {
    printf("\n========== TOUS LES TRAJETS PREVUS ==========\n\n");

    for(int i = 0; i < nbTrajets; i++) {
        printf("Bus numero %d: %s -> %s\n", trajets[i].numBus, trajets[i].villeDepart, trajets[i].villeArrivee);

        printf("  Date de depart: ");
        afficherDate_boyat_engelmann(trajets[i].dateDepart);
        printf("\n");

        if(trajets[i].horairesDepartPresents && trajets[i].horairesArriveePresents) {
            printf("  Depart: ");
            afficherHoraire_boyat_engelsmann(trajets[i].horaireDepart);
            printf(" - Arrivee: ");
            afficherHoraire_boyat_engelsmann(trajets[i].horaireArrivee);
            printf("\n");
        }
        else {
            printf("  Horaires: Non renseignes\n");
        }

        printf("  Clients: %d/55\n", trajets[i].nbClients);
        if(trajets[i].horairesDepartPresents == 0 || trajets[i].horairesArriveePresents == 0) {
            printf("  (Horaires non renseignes, chiffre d'affaires non calculable)\n");
        }
        else {
            printf("  CA: %.2lf euros\n", calculerChiffreAffaires_boyat_engelsmann(trajets[i]));
        }
        printf("\n");
    }
}

void afficherTrajetParBus_boyat(Trajet trajets[], int nbTrajets, int numBus) {
    printf("\n========== RECHERCHE TRAJET BUS NUMERO %d ==========\n\n", numBus);

    int trouve = 0;
    for (int i = 0; i < nbTrajets; i++) {
        if (trajets[i].numBus == numBus) {
            trouve = 1;

            printf("Informations du trajet:\n");
            printf("  Depart: %s\n", trajets[i].villeDepart);
            printf("  Arrivee: %s\n", trajets[i].villeArrivee);
            printf("  Date: ");
            afficherDate_boyat_engelmann(trajets[i].dateDepart);
            printf("\n");

            if (trajets[i].horairesDepartPresents && trajets[i].horairesArriveePresents) {
                printf("  Horaire depart: ");
                afficherHoraire_boyat_engelsmann(trajets[i].horaireDepart);
                printf("\n");
                printf("  Horaire arrivee: ");
                afficherHoraire_boyat_engelsmann(trajets[i].horaireArrivee);
                printf("\n");
            } else {
                printf("  Horaires: Non renseignes\n");
            }

            printf("\nListe des passagers (%d):\n", trajets[i].nbClients);
            printf("  ID   | Nom                  | Prix\n");
            printf("  -----|----------------------|----------\n");

            for (int j = 0; j < trajets[i].nbClients; j++) {
                Client* p = &trajets[i].clients[j];
                printf("  %-4d | ", p->id);

                if (p->nomPresent) {
                    printf("%-20s | ", p->nom);
                } else {
                    printf("%-20s | ", "Non renseigne");
                }

                if (p->prixPresent) {
                    printf("%.2f EUR\n", p->prixBillet);
                } else {
                    printf("Non renseigne\n");
                }
            }
            printf("\n");
        }
    }

    if (!trouve) {
        printf("Aucun trajet trouve pour le bus numero %d\n\n", numBus);
    }
}

int comparerDates_boyat_engelsmann(Date d1, Date d2) {
    if (d1.annee != d2.annee) return d1.annee - d2.annee;
    if (d1.mois != d2.mois)  return d1.mois - d2.mois;
    return d1.jour - d2.jour;
}

int comparerTrajet_boyat_engelsmann(Trajet t1, Trajet t2) {
    int cmpVille = strcmp(t1.villeDepart, t2.villeDepart);
    if (cmpVille != 0) return cmpVille;

    return comparerDates_boyat_engelsmann(t1.dateDepart, t2.dateDepart);
}

void trierTrajets_boyat_engelsmann(Trajet trajets[], int nbTrajets) {
    for (int i = 1; i < nbTrajets; i++) {
        Trajet cle = trajets[i]; // élément a inserer
        int j = i - 1;

        // Tant qu'on doit décaler vers la droite
        while (j >= 0 && comparerTrajet_boyat_engelsmann(trajets[j], cle) > 0) {
            trajets[j + 1] = trajets[j];
            j--;
        }

        // On place l'élément à la bonne position
        trajets[j + 1] = cle;
    }
}

void retirerRetourChariot_cours(char chaine[]) {
    for(int i = 0; chaine[i] != '\0'; i++) {
        if(chaine[i] == '\n') {
            chaine[i] = '\0';
        }
	}
}

void ajouterClient_engelsmann(Trajet t[], int nbTrajets, int numBus, Client client) {
    printf("\n========== AJOUTER UN CLIENT POUR LE BUS NUMERO %d ==========\n\n", numBus);

    int trouve = 0;
    for (int i = 0; i < nbTrajets; i++) {
        if (t[i].numBus == numBus) {
            trouve = 1;

            if (t[i].nbClients < 55) {
				t[i].clients[t[i].nbClients].id = t[i].nbClients + 1;
                strcpy(t[i].clients[t[i].nbClients].nom, client.nom);
				t[i].clients[t[i].nbClients].prixBillet = client.prixBillet;
				t[i].clients[t[i].nbClients].nomPresent = client.nomPresent;
                t[i].clients[t[i].nbClients].prixPresent = client.prixPresent;

				t[i].nbClients++;

                printf("Client ajoute avec succes au bus numero %d.\n\n", numBus);
            } else {
				printf("Le bus numero %d est complet. Impossible d'ajouter un client.\n\n", numBus);
            }
        }
    }

    if (!trouve) {
        printf("Aucun trajet trouve pour le bus numero %d\n\n", numBus);
    }
}

void retirerClient_engelmann(Trajet t[], int nbTrajets, int numBus, int client_id) {
    printf("\n========== SUPPRIMER UN CLIENT POUR LE BUS NUMERO %d ==========\n\n", numBus);

    int trouve = 0;
    for (int i = 0; i < nbTrajets; i++) {
        if (t[i].numBus == numBus) {
            trouve = 1;

            if(t[i].nbClients == 0) {
                printf("Aucun client a supprimer dans le bus numero %d.\n\n", numBus);
                return;
			}

            if (client_id > t[i].nbClients) {
				printf("Client avec l'id %d non trouve dans le bus numero %d.\n\n", client_id, numBus);
                return;
            }
            
            for (int j = client_id - 1; j < t[i].nbClients - 1; j++) {
				t[i].clients[j] = t[i].clients[j + 1];
				t[i].clients[j].id--;
            }
			t[i].nbClients--;
			printf("Client avec l'id %d supprime avec succes du bus numero %d.\n\n", client_id, numBus);
        }
    }

    if (!trouve) {
        printf("Aucun trajet trouve pour le bus numero %d\n\n", numBus);
    }
}

void ajouteNomClient_boyat_engelsmann(Trajet t[], int nbTrajets, int numBus, int client_id, char nom[]) {
    printf("\n========== AJOUTER/MODIFIER UN LE NOM DU CLIENT NUMERO %d POUR LE BUS NUMERO %d ==========\n\n", client_id, numBus);

    int trouve = 0;
    for (int i = 0; i < nbTrajets; i++) {
        if (t[i].numBus == numBus) {
        
            for (int j = 0; j < 55; j++){
                if (t[i].clients[j].id == client_id) {
                    trouve = 1;

                    strcpy(t[i].clients[j].nom, nom);
					printf("Le nom du client numero %d pour le bus numero %d a ete modifie avec succes.\n\n", client_id, numBus);
                }
            }   
            
        }
    }

    if (!trouve) {
        printf("Aucun client ou trajet trouve pour le client numero %d et le bus numero %d\n\n", client_id, numBus);
    }
}

void ajoutePrixClient_boyat_engelsmann(Trajet t[], int nbTrajets, int numBus, int client_id, double prix){
    printf("\n========== AJOUTER/MODIFIER UN LE PRIX DU CLIENT NUMERO %d POUR LE BUS NUMERO %d ==========\n\n", client_id, numBus);

    int trouve = 0;
    for (int i = 0; i < nbTrajets; i++) {
        if (t[i].numBus == numBus) {
        
            for (int j = 0; j < 55; j++){
                if (t[i].clients[j].id == client_id) {
                    trouve = 1;

                    t[i].clients[j].prixBillet = prix ;
					printf("Le prix du client numero %d pour le bus numero %d a ete modifie avec succes.\n\n", client_id, numBus);
                }
            }   
        }
    }

    if (!trouve) {
        printf("Aucun client ou trajet trouve pour le client numero %d et le bus numero %d\n\n", client_id, numBus);
    }
}

void sauvegarderTrajets_boyat_engelsmann(char nomFichier[], Trajet trajets[], int nbTrajets){
    FILE* file = fopen(nomFichier, "w");
    if (file == NULL)
    {
        perror("Erreur ouverture fichier");
        return -1;
    }

    for (int i = 0; i < nbTrajets; i++) {
		fprintf(file, "%d,", trajets[i].numBus);
        fprintf(file, "%s,", trajets[i].villeDepart);
        fprintf(file, "%s,", trajets[i].villeArrivee);
        fprintf(file, "%02d/%02d/%d,", trajets[i].dateDepart.jour, trajets[i].dateDepart.mois, trajets[i].dateDepart.annee);
        
        if(trajets[i].horairesDepartPresents) {
            fprintf(file, "%d,", trajets[i].horaireDepart);
        } else {
            fprintf(file, ",");
		}

        if(trajets[i].horairesArriveePresents) {
            fprintf(file, "%d,", trajets[i].horaireArrivee);
        }
        else {
            fprintf(file, ",");
        }

        for (int j = 0; j < trajets[i].nbClients; j++) {
			fprintf(file, "%d:", trajets[i].clients[j].id);
            if (trajets[i].clients[j].nomPresent) {
                fprintf(file, "%s:", trajets[i].clients[j].nom);
            } else {
                fprintf(file, ":");
            }
            if (trajets[i].clients[j].prixPresent) {
                fprintf(file, "%.2lf", trajets[i].clients[j].prixBillet);
            }
            if (j < trajets[i].nbClients - 1) {
                fprintf(file, ",");
			}
            else {
				fprintf(file, "\n");
            }
        }
    }

	printf("Trajets sauvegardes avec succes dans le fichier %s.\n\n", nomFichier);
	fclose(file);
}

void afficherTrajet_boyat_engelsmann(Trajet t) {
    printf("\nInformations du trajet:\n");
	printf("  Bus numero: %d\n", t.numBus);
    printf("  Depart: %s\n", t.villeDepart);
    printf("  Arrivee: %s\n", t.villeArrivee);
    printf("  Date: ");
    afficherDate_boyat_engelmann(t.dateDepart);
    printf("\n");

    if (t.horairesDepartPresents && t.horairesArriveePresents) {
        printf("  Horaire depart: ");
        afficherHoraire_boyat_engelsmann(t.horaireDepart);
        printf("\n");
        printf("  Horaire arrivee: ");
        afficherHoraire_boyat_engelsmann(t.horaireArrivee);
        printf("\n");
    }
    else {
        printf("  Horaires: Non renseignes\n");
    }

    printf("\nListe des passagers (%d):\n", t.nbClients);
    printf("  ID   | Nom                  | Prix\n");
    printf("  -----|----------------------|----------\n");

    for (int j = 0; j < t.nbClients; j++) {
        Client* p = &t.clients[j];
        printf("  %-4d | ", p->id);

        if (p->nomPresent) {
            printf("%-20s | ", p->nom);
        }
        else {
            printf("%-20s | ", "Non renseigne");
        }

        if (p->prixPresent) {
            printf("%.2f EUR\n", p->prixBillet);
        }
        else {
            printf("Non renseigne\n");
        }
    }
    printf("\n");
}

void filtrerTrajetsVilleDateLendemain_engelsmann(Trajet t[], int nbTrajets, char villeDepart[], Date depart) {
    int trouve = 0;

    for (int i = 0; i < nbTrajets; i++) {
        if (strcmp(t[i].villeDepart, villeDepart) == 0 && comparerDates_boyat_engelsmann(t[i].dateDepart, depart) == 0) {

            if (t[i].horairesDepartPresents == 0 || t[i].horairesArriveePresents == 0)
                continue; // On ne peut pas determiner si le trajet arrive le lendemain sans horaires

            if (t[i].horaireDepart / 100 > t[i].horaireArrivee / 100)  //trajet de moins de 24 heures 
            {
                trouve = 1;
                afficherTrajet_boyat_engelsmann(t[i]);
            }
            else if (t[i].horaireDepart / 100 == t[i].horaireArrivee / 100 && t[i].horaireDepart % 100 >= t[i].horaireArrivee % 100) // trajet de min 1 minutes et max 24
            {
                trouve = 1;
                afficherTrajet_boyat_engelsmann(t[i]);
            }
        }
    }

    if (!trouve) {
        printf("\nAucun trajet trouve pour la ville de depart %s a la date donnee qui arrive le lendemain\n\n", villeDepart);
    }
}

void filtrerTrajetsDepartArriveeDate_boyat(Trajet t[], int nbTrajets, char villeDepart[], char villeArrivee[], Date dateDepart) {
    int trouve = 0;

    for (int i = 0; i < nbTrajets; i++) {
        if (strcmp(t[i].villeDepart, villeDepart) == 0 &&
            strcmp(t[i].villeArrivee, villeArrivee) == 0 &&
            comparerDates_boyat_engelsmann(t[i].dateDepart, dateDepart) == 0) {
			
            trouve = 1;
			afficherTrajet_boyat_engelsmann(t[i]);
        }
    }

    if (!trouve) {
        printf("\nAucun trajet trouve pour la ville de depart %s, la ville d'arrivee %s a la date donnee\n\n", villeDepart, villeArrivee);
    }
}

double calculerChiffreAffaires_boyat_engelsmann(Trajet t) {
	double ca = -100;

    for(int i = 0; i < t.nbClients; i++) {
        if(t.clients[i].prixPresent) {
            ca += t.clients[i].prixBillet;
        }
	}

	int nbHeureT = t.horaireArrivee / 100 - t.horaireDepart / 100;

    if(nbHeureT < 0)
		nbHeureT += 24;

    if(t.horaireArrivee % 100 >= t.horaireDepart % 100)
		nbHeureT++;

    ca -= nbHeureT * 50;

	return ca;
}

void trierTrajetsCA_boyat_engelsmann(Trajet trajets[], int nbTrajets) {
    for (int i = 1; i < nbTrajets; i++) {
        Trajet cle = trajets[i]; // élément a inserer
        int j = i - 1;

        // Tant qu'on doit décaler vers la droite
        while (j >= 0 && calculerChiffreAffaires_boyat_engelsmann(trajets[j]) < calculerChiffreAffaires_boyat_engelsmann(cle)) {
            trajets[j + 1] = trajets[j];
            j--;
        }

        // On place l'élément à la bonne position
        trajets[j + 1] = cle;
    }
}

void echangerClientDeplace_boyat_engelsmann(ClientDeplace clients[], int i, int j) {
    ClientDeplace temp = clients[i];
    clients[i] = clients[j];
    clients[j] = temp;
}

int comparerClientsPrixDesc_boyat_engelsmann(Client c1, Client c2) {
    if (!c1.prixPresent && !c2.prixPresent) return 0;
    if (!c1.prixPresent) return 1;  // c1 sans prix va à la fin
    if (!c2.prixPresent) return -1; // c2 sans prix va à la fin
    if (c1.prixBillet > c2.prixBillet) return -1;
    if (c1.prixBillet < c2.prixBillet) return 1;
    return 0;
}

int partitionnerClients_boyat_engelsmann(ClientDeplace clients[], int bas, int haut) {
    Client pivot = clients[haut].client;
    int i = bas - 1;

    for (int j = bas; j < haut; j++) {
        if (comparerClientsPrixDesc_boyat_engelsmann(clients[j].client, pivot) < 0) {
            i++;
            echangerClientDeplace_boyat_engelsmann(clients, i, j);
        }
    }
    echangerClientDeplace_boyat_engelsmann(clients, i + 1, haut);
    return i + 1;
}

void quickSortClients_boyat_engelsmann(ClientDeplace clients[], int bas, int haut) {
    if (bas < haut) {
        int pi = partitionnerClients_boyat_engelsmann(clients, bas, haut);
        quickSortClients_boyat_engelsmann(clients, bas, pi - 1);
        quickSortClients_boyat_engelsmann(clients, pi + 1, haut);
    }
}

void trierClientsParPrix_boyat_engelsmann(ClientDeplace clients[], int n) {
    if (n > 1) {
        quickSortClients_boyat_engelsmann(clients, 0, n - 1);
    }
}

int sontTrajetsIdentiques_boyat_engelsmann(Trajet t1, Trajet t2) {
    return (strcmp(t1.villeDepart, t2.villeDepart) == 0 &&
        strcmp(t1.villeArrivee, t2.villeArrivee) == 0 &&
        comparerDates_boyat_engelsmann(t1.dateDepart, t2.dateDepart) == 0);
}

int differenceTemps_boyat_engelsmann(Trajet t1, Trajet t2) {
    if (!t1.horairesDepartPresents || !t2.horairesDepartPresents) {
        return 24 * 60; 
    }

    int h1 = t1.horaireDepart / 100;
    int m1 = t1.horaireDepart % 100;
    int h2 = t2.horaireDepart / 100;
    int m2 = t2.horaireDepart % 100;

    int temps1 = h1 * 60 + m1;
    int temps2 = h2 * 60 + m2;

    int diff = temps1 - temps2;
    if (diff < 0) diff = -diff; // Valeur absolue

    return diff;
}

void optimiserReseau_boyat_engelsmann(Trajet t[], int nbTrajets) {
    printf("\n========== OPTIMISATION DU RESEAU ==========\n\n");

    double caInitial = 0;
    int totalInitial = nbTrajets;
    for (int i = 0; i < totalInitial; i++) {
        caInitial += calculerChiffreAffaires_boyat_engelsmann(t[i]);
    }

    int reassignedCount = 0;
    int refundedCount = 0;
    int busSupprimes = 0;
    int ecritureIdx = 0; // Index pour reconstruire le tableau en place

    // On parcourt tous les trajets
    for (int i = 0; i < totalInitial; i++) {
        double ca = calculerChiffreAffaires_boyat_engelsmann(t[i]);

        // SI LE BUS EST DEFICITAIRE
        if (ca < 0) {
            busSupprimes++;

            // On trie les clients du bus actuel par prix décroissant localement
            // On utilise un petit tableau temporaire sécurisé pour la pile (55 clients max)
            ClientDeplace clientsLocaux[55];
            for (int k = 0; k < t[i].nbClients; k++) {
                clientsLocaux[k].client = t[i].clients[k];
                clientsLocaux[k].originalTrajetIndex = i;
                clientsLocaux[k].originalBusNum = t[i].numBus;
            }
            trierClientsParPrix_boyat_engelsmann(clientsLocaux, t[i].nbClients);

            // Tentative de réassignation pour chaque client du bus supprimé
            for (int k = 0; k < t[i].nbClients; k++) {
                int bestTrajetIdx = -1;
                int minTimeDiff = 1441; // Plus de 24h en minutes

                // Chercher un bus de remplacement PARMI LES BUS NON DÉFICITAIRES déjà validés ou à venir
                for (int j = 0; j < totalInitial; j++) {
                    // On ne réassigne pas vers un bus qui va être supprimé (CA < 0)
                    if (calculerChiffreAffaires_boyat_engelsmann(t[j]) < 0) continue;
                    if (t[j].nbClients >= 55) continue;

                    if (sontTrajetsIdentiques_boyat_engelsmann(t[i], t[j])) {
                        int diff = differenceTemps_boyat_engelsmann(t[i], t[j]);
                        if (diff < minTimeDiff) {
                            minTimeDiff = diff;
                            bestTrajetIdx = j;
                        }
                    }
                }

                if (bestTrajetIdx != -1) {
                    // Ajout au bus de destination
                    int idxC = t[bestTrajetIdx].nbClients;
                    t[bestTrajetIdx].clients[idxC] = clientsLocaux[k].client;
                    t[bestTrajetIdx].clients[idxC].id = idxC + 1;
                    t[bestTrajetIdx].nbClients++;
                    reassignedCount++;
                }
                else {
                    refundedCount++;
                }
            }
            // Le bus n'est pas copié à ecritureIdx, il est donc "supprimé"
        }
        else {
            // SI LE BUS EST RENTABLE, on le garde
            if (ecritureIdx != i) {
                t[ecritureIdx] = t[i]; // On décale le trajet vers la gauche pour combler les vides
            }
            ecritureIdx++;
        }
    }

    nbTrajets = ecritureIdx; // Mise à jour du nombre de trajets pour l'appelant

    double caFinal = 0;
    for (int i = 0; i < nbTrajets; i++) {
        caFinal += calculerChiffreAffaires_boyat_engelsmann(t[i]);
    }

    printf("Operation terminee.\n");
    printf("Passagers reassignes : %d\n", reassignedCount);
    printf("Passagers rembourses : %d\n", refundedCount);
    printf("Nombre de bus supprimes : %d\n", busSupprimes);
    printf("Gain total genere : %.2lf euros\n\n", caFinal - caInitial);
}