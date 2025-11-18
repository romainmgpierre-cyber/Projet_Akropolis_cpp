#include "Cite.h"
#include "TuileCite_TuileDep.h"
#include "GameExcep_Enums.h"
#include "CoordHex.h"
#include "HexCons_Carr_Quart_Place.h"
#include "Joueur.h"
#include "Partie_Variante.h"
#include "Pioche_ChoixTuile.h"
#include "Score.h"
#include "Type.h"

using namespace std;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    cout << "Bienvenue dans AKROPOLIS - Mode console" << endl; 
    cout << "=======================================" << endl;
    cout << "========================================" << endl;
    cout << "=== RAPPEL DES RÈGLES AKROPOLIS ===" << endl;
    cout << "========================================" << endl;
    cout << "But du jeu : Avoir le plus de points de victoire en fin de partie." << endl;
    cout << "Fin de partie : Épuisement des tuiles cité." << endl;

    cout << "\n--- TYPES DE QUARTIERS (Points de Victoire) ---" << endl;
    cout << "- Habitations (Bleu) : Seul le plus grand groupe adjacent rapporte des points." << endl;
    cout << "- Marchés (Jaune) : Ne doivent pas être adjacents à un autre marché." << endl;
    cout << "- Casernes (Rouge) : Doivent se trouver en périphérie de la cité." << endl;
    cout << "- Temples (Violet) : Doivent être complètement entourés." << endl;
    cout << "- Jardins (Vert) : Pas de contrainte de placement." << endl;

    cout << "\n--- PLACEMENT ET RESSOURCES ---" << endl;
    cout << "- Les Places (Multiplicateurs) : Augmentent le score des quartiers de même type (Cumulables)." << endl;
    cout << "- Les Carrières (Gris) : Ne rapportent pas de points, mais donnent des Pierres lorsqu'elles sont recouvertes." << endl;
    cout << "- Les Tuiles Cité peuvent être posées au sol ou superposées (gain de pierres)." << endl;

cout << "========================================\n\n" << endl;

    int modeChoisi = 0;
    do {
        cout << "Choisissez votre mode de jeu (1: Solo / 2: Multijoueur): ";
        if (!(cin >> modeChoisi) || (modeChoisi != 1 && modeChoisi != 2)) {
            cout << "Entrée invalide. Veuillez entrer 1 ou 2 svp." << endl;
            clearInputBuffer();
            modeChoisi = 0; // 
        }
    } while (modeChoisi == 0);
    clearInputBuffer();
    
    ModeJeu mode;
    if (modeChoisi == 1) {
        mode = ModeJeu::SOLO;
    } else {
        mode = ModeJeu::MULTIJOUEUR;
    }
    Partie partie(1, mode);

    int nbJoueursHumains;
    if (mode == ModeJeu::SOLO) {
        nbJoueursHumains = 1;
    } else {
        nbJoueursHumains = 0;
        do {
            cout << "Nombre de joueurs humains (2 à 4): ";
            if (!(cin >> nbJoueursHumains) || (nbJoueursHumains < 2 || nbJoueursHumains > 4)) {
                cout << " Entrée invalide. Veuillez entrer un nombre entre 2 et 4." << endl;
                clearInputBuffer();
                nbJoueursHumains = 0;
            }
        } while (nbJoueursHumains == 0);
        clearInputBuffer(); 
    }

    for (int i = 0; i < nbJoueursHumains; ++i) {
        string nom;
        cout << "Nom du Joueur " << i + 1 << " : ";
        getline(cin, nom); 
        if (nom.empty()) {
            nom = "Joueur" + to_string(i + 1);
        }
        
        try {
            partie.ajouterJoueur(nom);
        } catch (const GameException& e) {
            cerr << "Erreur lors de l'ajout des joueurs : " << e.getInfo() << endl;
            return 1;
        }
    }

    if (mode == ModeJeu::SOLO) {
        partie.ajouterJoueur("Illustre Constructeur");  
        int difficulteChoisie = 0;
        do {
            cout << "Niveau de difficulté de l'Illustre Constructeur (1: Facile / 2: Moyen / 3: Difficile): ";
            if (!(cin >> difficulteChoisie) || (difficulteChoisie < 1 || difficulteChoisie > 3)) {
                cout << "❌ Entrée invalide. Veuillez entrer un nombre entre 1 et 3." << endl;
                clearInputBuffer();
                difficulteChoisie = 0;
            }
        } while (difficulteChoisie == 0)
        clearInputBuffer();

        NiveauDifficulte diff;
        if (difficulteChoisie == 1) {
            diff = NiveauDifficulte::FACILE;
        } else if (difficulteChoisie == 2) {
            diff = NiveauDifficulte::MOYEN;
        } else {
            diff = NiveauDifficulte::DIFFICILE;
        }
        partie.setDifficulte(diff);
    }

    cout << "\n\n Paramétrage terminé. Lancement de la partie..." << endl;
    
    try {
        partie.lancerPartie(); 
    } catch (const GameException& e) {
        cerr << "\n💥 Erreur fatale pendant le jeu : " << e.getInfo() << endl;
        return 1;
    }

    ;
    return 0;
}

