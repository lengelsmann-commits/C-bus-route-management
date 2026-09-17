#ifndef FONCTIONS
#define FONCTIONS

#include "types.h"

extern long long compteur_ops;

/**
 * entrée nomFichier: const char*, trajets: Trajet[], maxTrajets: int
 * pré-cond: nomFichier est un chemin valide vers un fichier CSV
 * sortie: int
 * post-cond: retourne le nombre de trajets chargés ou -1 en cas d'erreur
 */
int chargerTrajets_boyat(const char* nomFichier, Trajet trajets[], int maxTrajets);

/**
 * entrée trajets: Trajet[], nbTrajets: int
 * pré-cond: nbTrajets >= 0
 * sortie: Ø
 * post-cond: affiche dans la console le récapitulatif de tous les trajets
 */
void afficherTousLesTrajets_boyat(Trajet trajets[], int nbTrajets);

/**
 * entrée trajets: Trajet[], nbTrajets: int, numBus: int
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: affiche les détails et les passagers du bus correspondant au numéro numBus
 */
void afficherTrajetParBus_boyat(Trajet trajets[], int nbTrajets, int numBus);

/**
 * entrée d1: Date, d2: Date
 * pré-cond: Ø
 * sortie: int
 * post-cond: retourne une valeur <0 si d1 < d2, 0 si égales, >0 si d1 > d2
 */
int comparerDates_boyat_engelsmann(Date d1, Date d2);

/**
 * entrée t1: Trajet, t2: Trajet
 * pré-cond: Ø
 * sortie: int
 * post-cond: retourne le résultat de comparaison alphabétique (ville) puis chronologique
 */
int comparerTrajet_boyat_engelsmann(Trajet t1, Trajet t2);

/**
 * entrée trajets: Trajet[], nbTrajets: int
 * pré-cond: nbTrajets > 0
 * sortie: Ø
 * post-cond: trie le tableau de trajets par ville de départ et date (tri par insertion)
 */
void trierTrajets_boyat_engelsmann(Trajet trajets[], int nbTrajets);

/**
 * entrée horaire: int
 * pré-cond: format HHMM
 * sortie: Ø
 * post-cond: affiche l'horaire au format HH:MM
 */
void afficherHoraire_boyat_engelsmann(int horaire);

/**
 * entrée date: Date
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: affiche la date au format JJ/MM/AAAA
 */
void afficherDate_boyat_engelmann(Date date);

/**
 * entrée chaine: char[]
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: remplace le premier caractère '\n' trouvé par un terminateur '\0'
 */
void retirerRetourChariot_cours(char chaine[]);

/**
 * entrée t: Trajet[], nbTrajets: int, numBus: int, client: Client
 * pré-cond: numBus existe dans le tableau
 * sortie: Ø
 * post-cond: ajoute un nouveau client au bus spécifié si la capacité (55) le permet
 */
void ajouterClient_engelsmann(Trajet t[], int nbTrajets, int numBus, Client client);

/**
 * entrée t: Trajet[], nbTrajets: int, numBus: int, client_id: int
 * pré-cond: client_id est un identifiant valide
 * sortie: Ø
 * post-cond: supprime le client spécifié et décale les éléments suivants pour combler le vide
 */
void retirerClient_engelmann(Trajet t[], int nbTrajets, int numBus, int client_id);

/**
 * entrée t: Trajet[], nbTrajets: int, numBus: int, client_id: int, nom: char[]
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: modifie le nom du client spécifié dans le bus choisi
 */
void ajouteNomClient_boyat_engelsmann(Trajet t[], int nbTrajets, int numBus, int client_id, char nom[]);

/**
 * entrée t: Trajet[], nbTrajets: int, numBus: int, client_id: int, prix: double
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: modifie le prix du billet du client spécifié dans le bus choisi
 */
void ajoutePrixClient_boyat_engelsmann(Trajet t[], int nbTrajets, int numBus, int client_id, double prix);

/**
 * entrée nomFichier: char[], trajets: Trajet[], nbTrajets: int
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: enregistre l'intégralité des données dans le fichier spécifié au format CSV
 */
void sauvegarderTrajets_boyat_engelsmann(char nomFichier[], Trajet trajets[], int nbTrajets);

/**
 * entrée t: Trajet
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: affiche les informations détaillées d'un seul trajet (en-tête et passagers)
 */
void afficherTrajet_boyat_engelsmann(Trajet t);

/**
 * entrée t: Trajet[], nbTrajets: int, villeDepart: char[], depart: Date
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: affiche les trajets partant d'une ville à une date donnée dont l'arrivée est le lendemain
 */
void filtrerTrajetsVilleDateLendemain_engelsmann(Trajet t[], int nbTrajets, char villeDepart[], Date depart);

/**
 * entrée t: Trajet[], nbTrajets: int, villeDepart: char[], villeArrivee: char[], dateDepart: Date
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: affiche les trajets correspondants aux critères de lieu et de date
 */
void filtrerTrajetsDepartArriveeDate_boyat(Trajet t[], int nbTrajets, char villeDepart[], char villeArrivee[], Date dateDepart);

/**
 * entrée t: Trajet
 * pré-cond: horaires de départ et d'arrivée présents
 * sortie: double
 * post-cond: calcule le CA net (somme billets - frais horaires - forfait fixe)
 */
double calculerChiffreAffaires_boyat_engelsmann(Trajet t);

/**
 * entrée trajets: Trajet[], nbTrajets: int
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: trie les trajets par chiffre d'affaires décroissant
 */
void trierTrajetsCA_boyat_engelsmann(Trajet trajets[], int nbTrajets);

/**
 * entrée clients: ClientDeplace[], i: int, j: int
 * pré-cond: i et j sont des indices valides
 * sortie: Ø
 * post-cond: échange les positions de deux éléments dans le tableau temporaire d'optimisation
 */
void echangerClientDeplace_boyat_engelsmann(ClientDeplace clients[], int i, int j);

/**
 * entrée c1: Client, c2: Client
 * pré-cond: Ø
 * sortie: int
 * post-cond: retourne une valeur pour trier les clients par prix de billet décroissant
 */
int comparerClientsPrixDesc_boyat_engelsmann(Client c1, Client c2);

/**
 * entrée clients: ClientDeplace[], bas: int, haut: int
 * pré-cond: Ø
 * sortie: int
 * post-cond: fonction pivot pour l'algorithme QuickSort appliqué aux clients
 */
int partitionnerClients_boyat_engelsmann(ClientDeplace clients[], int bas, int haut);

/**
 * entrée clients: ClientDeplace[], bas: int, haut: int
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: trie récursivement les clients par prix décroissant
 */
void quickSortClients_boyat_engelsmann(ClientDeplace clients[], int bas, int haut);

/**
 * entrée clients: ClientDeplace[], n: int
 * pré-cond: n > 0
 * sortie: Ø
 * post-cond: lance le tri rapide sur le tableau de clients à déplacer
 */
void trierClientsParPrix_boyat_engelsmann(ClientDeplace clients[], int n);

/**
 * entrée t1: Trajet, t2: Trajet
 * pré-cond: Ø
 * sortie: int
 * post-cond: retourne 1 si les deux trajets ont mêmes villes et date, sinon 0
 */
int sontTrajetsIdentiques_boyat_engelsmann(Trajet t1, Trajet t2);

/**
 * entrée t1: Trajet, t2: Trajet
 * pré-cond: Ø
 * sortie: int
 * post-cond: retourne la différence absolue en minutes entre les heures de départ
 */
int differenceTemps_boyat_engelsmann(Trajet t1, Trajet t2);

/**
 * entrée t: Trajet[], nbTrajets: int
 * pré-cond: Ø
 * sortie: Ø
 * post-cond: supprime les bus déficitaires, tente de réassigner les passagers et affiche le bilan financier
 */
void optimiserReseau_boyat_engelsmann(Trajet t[], int nbTrajets);

#endif // FONCTIONS