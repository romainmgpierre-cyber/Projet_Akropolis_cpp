#include "MenuConfiguration.h"
#include "Cite.h"
#include <sstream>
#include "FonctionsAide.h"
using namespace Akropolis;

Configuration MenuConfiguration::demarrerConfiguration() {
    // Choix du Mode de Jeu
    int modeChoisi = 0;
    if (!readIntOrQuit(modeChoisi, 1, 2, "\nChoisissez votre mode de jeu (1: Solo / 2: Multijoueur)")) {
        // En cas d'annulation q, on lance l'exception pour arreter le flux
        throw PartieAnnulee("Configuration annulee");
    }

    ModeJeu mode = (modeChoisi == 1) ? ModeJeu::SOLO : ModeJeu::MULTIJOUEUR;
    Configuration config(mode);

    // Configuration specifique au mode
    if (mode == ModeJeu::SOLO) {
        configurerSolo(config);
    } else {
        configurerMulti(config);
    }

    return config;
}

void MenuConfiguration::configurerSolo(Configuration& config) {

    // Saisie du nom du joueur humain
    string nomJoueur;
    if (readStringOrQuit(nomJoueur, "Entrez votre nom")) {
        // Si l'utilisateur a entré quelque chose, sinon le nom par défaut sera utilisé
        if (nomJoueur.empty()) nomJoueur = "Joueur";

        // On prépare le vecteur des noms de joueurs humains
        vector<string> noms = { nomJoueur };
        config.setNomsJoueurs(noms);
        config.setNbJoueurHumain(1); // Un seul joueur humain
    }

    // Choix de la difficulté de l'IA
    int difficulteChoisie = 0;
    cout << "\n--- Niveaux de difficulte ---" << endl;
    cout << "[1] Hippodamos (Facile) : Quartiers comptent niveau 1." << endl;
    cout << "[2] Metagenes (Moyen) : Quartiers niveau 1 + Bonus Carrieres." << endl;
    cout << "[3] Callicrates (Difficile): Quartiers comptent niveau 2." << endl;

    if (!readIntOrQuit(difficulteChoisie, 1, 3, "Choisissez le niveau de l'IA")) {
        // Annulation par l'utilisateur: l'exception sera propagée.
        throw PartieAnnulee("Configuration annulee par le joueur.");
    }

    NiveauDifficulte diff;
    if (difficulteChoisie == 1) diff = NiveauDifficulte::FACILE;
    else if (difficulteChoisie == 2) diff = NiveauDifficulte::MOYEN;
    else diff = NiveauDifficulte::DIFFICILE;

    config.setDifficulte(diff);
}


void MenuConfiguration::configurerMulti(Configuration& config) {

    // Saisie du nombre de joueurs
    int nbJoueursHumains = 0;
    if (!readIntOrQuit(nbJoueursHumains, 2, 4, "Nombre de joueurs (2 a 4)")) {
        // Annulation par l'utilisateur: l'exception sera propagée.
        throw PartieAnnulee("Configuration annulee par le joueur.");
    }
    config.setNbJoueurHumain(nbJoueursHumains);

    //  Saisie des noms de chaque joueur
    vector<string> noms;
    for (int i = 0; i < nbJoueursHumains; ++i) {
        string nom;
        string prompt = "Nom du Joueur " + to_string(i + 1);

        if (!readStringOrQuit(nom, prompt)) {
            // Annulation pendant la saisie d'un nom
            throw PartieAnnulee("Configuration annulee par le joueur.");
        }

        if (nom.empty()) nom = "Joueur " + to_string(i + 1);

        noms.push_back(nom);
    }

    config.setNomsJoueurs(noms);

    // La difficulté n'est pas réglée en mode multijoueur car pas d'IA
}






