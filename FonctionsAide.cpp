#include "Cite.h"
#include <sstream>

// DÉCLARATION DES FONCTIONS D'AIDE
using namespace Akropolis;
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Fonction utilitaire : vérifie si l'entrée est 'q', 'quitter', 'stop'
bool checkQuit(const string& input) {
    string lower_input = input;
    transform(lower_input.begin(), lower_input.end(), lower_input.begin(), ::tolower);
    if (lower_input == "q" || lower_input == "quitter" || lower_input == "stop") {
        throw PartieAnnulee("Partie annulee par le joueur.");
    }
    return true;
}

// Fonction pour lire un ENTIER avec vérification
bool readIntOrQuit(int& value, int min, int max, const string& prompt) {
    while (true) {
        cout << prompt << " ('q' pour quitter) : ";
        string input;

        if (!getline(cin, input) || input.empty()) {
            continue; // Ignore les lignes vides accidentelles
        }

        checkQuit(input);

        stringstream ss(input);
        if ((ss >> value) && (value >= min) && (value <= max) && ss.eof()) {
            return true;
        }

        cout << "Entree invalide. Veuillez entrer un nombre entre " << min << " et " << max << endl;
    }
}

// Fonction pour lire une CHAÎNE avec vérification
bool readStringOrQuit(string& value, const string& prompt) {
    cout << prompt << " ('q' pour quitter) : ";
    if (!getline(cin, value)) {
        throw PartieAnnulee("Lecture interrompue.");
    }
    return checkQuit(value);
}