#ifndef TYPES
#define TYPES

#define MAX_TRAJETS 5000

typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct {
    int id;
    char nom[30];
    double prixBillet;

    // boolean
    int nomPresent; // 1 si nom présent, 0 sinon
    int prixPresent; // 1 si prix présent, 0 sinon
} Client;

typedef struct {
    Client client;
    int originalBusNum;
    int originalTrajetIndex;
} ClientDeplace;

typedef struct {
    int numBus;
    char villeDepart[20];
    char villeArrivee[20];
    Date dateDepart;
    int horaireDepart;
    int horaireArrivee;
    Client clients[55];
    int nbClients;

    // boolean
    int horairesDepartPresents; // 1 si horaires présents, 0 sinon
	int horairesArriveePresents; // 1 si horaires présents, 0 sinon
} Trajet;

#endif; // TYPES