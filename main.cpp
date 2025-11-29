/*#include "Cite.h"
#include "TuileCite_TuileDep.h"
#include "GameExcep_Enums.h"
#include "CoordHex.h"
#include "HexCons_Carr_Quart_Place.h"
#include "Joueur.h"
#include "Partie_Variante.h" 
#include "Pioche_ChoixTuile.h" 
#include "Score.h"
#include "Type.h"
#include <iostream>
#include <string>
#include <limits> 
#include <sstream>      // Pour la conversion string -> int
#include <algorithm>    // Pour std::transform et std::tolower
#include "MainWindow.h"
#include <QApplication>

using namespace std;
using namespace Akropolis;

// --- DÉCLARATION DES FONCTIONS D'AIDE ---

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Fonction utilitaire : vérifie si l'entrée est 'q', 'quitter', 'stop', sinon la retourne
bool checkQuit(const string& input) {
    string lower_input = input;
    transform(lower_input.begin(), lower_input.end(), lower_input.begin(), ::tolower);
    if (lower_input == "q" || lower_input == "quitter" || lower_input == "stop") {
        throw PartieAnnulee("Partie annulée par le joueur.");
    }
    return true;
}

// Fonction pour lire un ENTIER avec vérification de sortie et de validité
bool readIntOrQuit(int& value, int min, int max, const string& prompt) {
    while (true) {
        cout << prompt << " ('q' pour quitter) : ";
        string input;
        
        if (!getline(cin, input) || input.empty()) {
            throw PartieAnnulee("Lecture interrompue.");
        }
        
        checkQuit(input); // Vérifie si l'utilisateur veut quitter

        stringstream ss(input);
        if ((ss >> value) && (value >= min) && (value <= max) && ss.eof()) {
            return true;
        }

        cout << "❌ Entrée invalide. Veuillez entrer un nombre entre " << min << " et " << max << " svp." << endl;
        // Laisser le buffer propre pour la prochaine itération
    }
}

// Fonction pour lire une CHAÎNE avec vérification de sortie
bool readStringOrQuit(string& value, const string& prompt) {
    cout << prompt << " ('q' pour quitter) : ";
    if (!getline(cin, value) || value.empty()) {
        throw PartieAnnulee("Lecture interrompue.");
    }
    return checkQuit(value);
}


// --- FONCTION PRINCIPALE ---

int main() {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    // 1. Encapsuler toute la logique dans un try-catch
    try {
        cout << "Bienvenue dans AKROPOLIS - Mode console" << endl; 
        cout << "=======================================" << endl;
        // ... (RAPPEL DES RÈGLES) ...
        cout << "========================================\n\n" << endl;
        cout << "Vous pouvez taper 'q' à n'importe quelle invite pour annuler.\n" << endl;


        int modeChoisi = 0;
        // Utilisation de la fonction d'aide pour la saisie
        readIntOrQuit(modeChoisi, 1, 2, "Choisissez votre mode de jeu (1: Solo / 2: Multijoueur)");
        
        ModeJeu mode;
        if (modeChoisi == 1) {
            mode = ModeJeu::SOLO;
        } else {
            mode = ModeJeu::MULTIJOUEUR;
        }
        Partie partie(1, mode);

        // 2. Gestion du nombre de joueurs (Multijoueur)
        int nbJoueursHumains;
        if (mode == ModeJeu::SOLO) {
            nbJoueursHumains = 1;
        } else {
            // Utilisation de la fonction d'aide
            readIntOrQuit(nbJoueursHumains, 2, 4, "Nombre de joueurs humains (2 à 4)");
        }

        // 3. Saisie des noms
        for (int i = 0; i < nbJoueursHumains; ++i) {
            string nom;
            // Utilisation de la fonction d'aide pour la saisie de chaîne
            if (!readStringOrQuit(nom, "Nom du Joueur " + to_string(i + 1))) {
                // Ce bloc est techniquement inutile car checkQuit() lance déjà l'exception, mais on le laisse.
            }
            
            if (nom.empty()) {
                nom = "Joueur" + to_string(i + 1);
            }
            
            partie.ajouterJoueur(nom);
        }

        // 4. Gestion de la difficulté (Solo)
        if (mode == ModeJeu::SOLO) {
            partie.ajouterJoueur("Illustre Constructeur");  
            int difficulteChoisie = 0;
            
            // Utilisation de la fonction d'aide
            readIntOrQuit(difficulteChoisie, 1, 3, "Niveau de difficulté de l'Illustre Constructeur (1: Facile / 2: Moyen / 3: Difficile)");
            
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
        
        // La boucle de jeu doit aussi lancer l'exception PartieAnnulee si le joueur quitte.
        partie.lancerPartie(); 

    } catch (const PartieAnnulee& e) {
        // 5. Bloc de capture pour la sortie demandée par le joueur
        cout << "\n\n=== FIN DE PARTIE PRÉMATURÉE ===\n" << endl;
        cout << e.getInfo() << endl;
        return 0; 
    } catch (const GameException& e) {
        // Bloc de capture pour les erreurs logiques du jeu
        cerr << "\n ERREUR FATALE DU JEU: " << e.getInfo() << endl;
        return 1;
    } catch (const exception& e) {
        // Bloc de capture pour les erreurs système
        cerr << "\n ERREUR SYSTÈME: " << e.what() << endl;
        return 1;
    }

    return 0;
}
*/

#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
