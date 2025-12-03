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
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Akropolis;

// DÉCLARATION DES FONCTIONS D'AIDE

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

        cout << "Entree invalide. Veuillez entrer un nombre entre " << min << " et " << max << " svp." << endl;
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


// FONCTION PRINCIPALE

int main() {
    // Initialisation de la graine aléatoire
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    try {
        cout << "=======================================" << endl;
        cout << "   BIENVENUE DANS AKROPOLIS (CONSOLE)  " << endl;
        cout << "=======================================" << endl;
        cout << "Vous pouvez taper 'q' a tout moment pour quitter.\n" << endl;

        // 1. Choix du Mode de Jeu
        int modeChoisi = 0;
        readIntOrQuit(modeChoisi, 1, 2, "Choisissez votre mode de jeu (1: Solo / 2: Multijoueur)");

        ModeJeu mode = (modeChoisi == 1) ? ModeJeu::SOLO : ModeJeu::MULTIJOUEUR;

        // Création de la partie
        Partie partie(1, mode);


        // 4. Gestion de la difficulté (Solo)
        if (mode == ModeJeu::SOLO) {
            // MODE SOLO
            int i = 1;
            string nomJoueur;
            if (readStringOrQuit(nomJoueur, "Entrez votre nom")) {
                if (nomJoueur.empty()) nomJoueur = "Architecte";

                partie.ajouterJoueur(nomJoueur, 1);
            }



            // On ajoute l'IA (qui sera le joueur 2)
            partie.ajouterJoueur("Illustre Constructeur", 2, true);

            int difficulteChoisie = 0;
            cout << "\n--- Niveaux de difficulte ---" << endl;
            cout << "1. Hippodamos (Facile) : Quartiers comptent niveau 1." << endl;
            cout << "2. Metagenes  (Moyen)  : Quartiers niveau 1 + Bonus Carrieres." << endl;
            cout << "3. Callicrates(Difficile): Quartiers comptent niveau 2." << endl;

            readIntOrQuit(difficulteChoisie, 1, 3, "Choisissez le niveau de l'IA");

            NiveauDifficulte diff;
            if (difficulteChoisie == 1) diff = NiveauDifficulte::FACILE;
            else if (difficulteChoisie == 2) diff = NiveauDifficulte::MOYEN;
            else diff = NiveauDifficulte::DIFFICILE;

            partie.setDifficulte(diff);

        }
        else {
            // --- MODE MULTIJOUEUR ---
            int nbJoueursHumains;
            readIntOrQuit(nbJoueursHumains, 2, 4, "Nombre de joueurs (2 a 4)");

            for (int i = 0; i < nbJoueursHumains; ++i) {
                string nom;
                readStringOrQuit(nom, "Nom du Joueur " + to_string(i + 1));
                if (nom.empty()) nom = "Joueur" + to_string(i + 1);
                partie.ajouterJoueur(nom, i); // isIA = false par défaut
            }
        }

        cout << "\n\n Parametrage termine. Lancement de la partie..." << endl;

        //Lancement de la boucle de jeu
        partie.lancerPartie();

        //Fin de partie et Résultats
        cout << "\n\n*****************************************" << endl;
        cout << "* RÉSULTATS FINAUX                      *" << endl;
        cout << "*****************************************" << endl;

        TableauScore calculateurFinal;
        const vector<Joueur*>& tousLesJoueurs = partie.getJoueurs();

        if (mode == ModeJeu::SOLO) {
            // RÉSULTATS SOLO
            Joueur* humain = nullptr;
            Joueur* ia = nullptr;

            // Identification des rôles
            for (auto* j : tousLesJoueurs) {
                if (j->isIA()) ia = j;
                else humain = j;
            }

            if (humain && ia) {
                // Score Humain (Calcul standard)
                cout << "\n--- VOTRE CITE (" << humain->getNom() << ") ---" << endl;
                calculateurFinal.afficherDetailsScore(*humain, cout);
                int scoreHumain = calculateurFinal.calculerScore(*humain);

                // Score IA (Calcul spécifique selon difficulté)
                cout << "\n--- ILLUSTRE ARCHITECTE (" << ia->getNom() << ") ---" << endl;
                // On n'affiche pas les détails complexes pour l'IA, juste le total
                int scoreIA = calculateurFinal.calculerScoreIA(*ia, partie.getDifficulte());

                cout << "Difficulte : ";
                switch(partie.getDifficulte()) {
                case NiveauDifficulte::FACILE: cout << "Facile (Hippodamos)"; break;
                case NiveauDifficulte::MOYEN: cout << "Moyen (Metagenes)"; break;
                case NiveauDifficulte::DIFFICILE: cout << "Difficile (Callicrates)"; break;
                }
                cout << endl;
                cout << "SCORE FINAL IA : " << scoreIA << " points." << endl;

                // Comparaison et Verdict
                cout << "\n*****************************************" << endl;
                cout << "              VERDICT                    " << endl;
                cout << "*****************************************" << endl;
                cout << humain->getNom() << " : " << scoreHumain << " pts | " << humain->getNbPierres() << " pierres" << endl;
                cout << ia->getNom() << " : " << scoreIA << " pts | " << ia->getNbPierres() << " pierres" << endl;
                cout << "-----------------------------------------" << endl;

                if (scoreHumain > scoreIA) {
                    cout << "VICTOIRE ! Vous avez vaincu l'Illustre Architecte !" << endl;
                }
                else if (scoreIA > scoreHumain) {
                    cout << "DEFAITE. L'Illustre Architecte remporte la victoire." << endl;
                }
                else {
                    // EGALITÉ : Départage aux pierres
                    cout << "EGALITE aux points ! On compare les pierres..." << endl;
                    if (humain->getNbPierres() > ia->getNbPierres()) {
                        cout << "Vous avez plus de pierres. VICTOIRE !" << endl;
                    } else {
                        // "En cas d'égalité, c'est le joueur ayant le plus de Pierres qui l'emporte."

                        if (ia->getNbPierres() > humain->getNbPierres())
                            cout << "L'IA a plus de pierres. DEFAITE." << endl;
                        else
                            cout << "EGALITE PARFAITE (Points et Pierres) ! Quel match !" << endl;
                    }
                }
            }
        }
        else {
            //RÉSULTATS MULTIJOUEUR (Classique)
            // On calcule et stocke les scores pour le tri
            vector<pair<Joueur*, int>> scoresFinaux;

            for(const auto& joueur : tousLesJoueurs) {
                calculateurFinal.afficherDetailsScore(*joueur, cout);
                scoresFinaux.push_back({joueur, calculateurFinal.calculerScore(*joueur)});
                cout << endl;
            }

            // Tri décroissant pour trouver le vainqueur
            sort(scoresFinaux.begin(), scoresFinaux.end(),
                 [](const pair<Joueur*, int>& a, const pair<Joueur*, int>& b) {
                     if (a.second != b.second) {
                         return a.second > b.second; // Plus de points d'abord
                     }
                     return a.first->getNbPierres() > b.first->getNbPierres(); // Pierres si égalité
                 });

            cout << "*****************************************" << endl;
            cout << "VAINQUEUR : " << scoresFinaux[0].first->getNom()
                 << " avec " << scoresFinaux[0].second << " points !" << endl;
            cout << "*****************************************" << endl;
        }

        cout << "\nMerci d'avoir joue a Akropolis !" << endl;

    } catch (const PartieAnnulee& e) {
        cout << "\n\n=== FIN DE PARTIE ===" << endl;
        cout << e.getInfo() << endl;
        return 0;
    } catch (const GameException& e) {
        cerr << "\n ERREUR FATALE DU JEU: " << e.getInfo() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "\n ERREUR SYSTEME: " << e.what() << endl;
        return 1;
    }

    return 0;
}

/*
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
*/
