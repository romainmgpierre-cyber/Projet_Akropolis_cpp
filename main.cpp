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
    

    ;
    return 0;
}

