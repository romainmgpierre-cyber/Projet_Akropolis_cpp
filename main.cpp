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
// #include "MainWindow.h"
//#include "HexGridWidget.h"
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "MenuConfiguration.h"
#include "FonctionsAide.h"
#include "Sauvegarde.h"

using namespace std;
using namespace Akropolis;


// Fonction de lancement d'une nouvelle partie
void LancerPartie(const Akropolis::Configuration& config) {
    // Création de la partie en utilisant les donnees de 'config'
    Akropolis::Partie partie(1, config.getMode());
    partie.setDifficulte(config.getDifficulte());

    // Boucle pour ajouter les joueurs humains
    for (size_t i = 0; i < config.getNomsJoueurs().size(); ++i) {
        partie.ajouterJoueur(config.getNomsJoueurs()[i], i + 1);
    }

    // Ajout de l'IA si mode solo
    if (config.getMode() == Akropolis::ModeJeu::SOLO) {
        partie.ajouterJoueur("Illustre Constructeur", 2, true);
    }

    // Lancement de la boucle de jeu
    partie.lancerPartie();

    // Affichage des resultats
    AfficherResultats(partie);
}


int main() {
    // Initialisation de la graine aléatoire
    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    try {
        int choixMenu;
        MenuConfiguration menuConfig;
        while (true) {
            // Affichage du menu
            cout << "\n--- MENU PRINCIPAL ---" << endl;
            cout << "[1] Créer une nouvelle partie" << endl;
            cout << "[2] Charger une partie enregistrée (pas encore implémenté)" << endl;
            cout << "[3] Quitter le jeu" << endl;

            // Cas où on quite le jeu
            if (!readIntOrQuit(choixMenu, 1, 3, "Votre choix")) {
                choixMenu = 3;
            }
            if (choixMenu == 1) {
                // configuration de la partie
                Configuration config = menuConfig.demarrerConfiguration();
                // Lancement de la partie
                LancerPartie(config);
            } else if (choixMenu == 2) {
                // on charge le menu des sauvegardes
                //Sauvegarde::MenuSauvegardes();
                //Sauvegarde::EnregistrerPartie();
            } else if (choixMenu == 3) {
                cout << "\n A bientot !" << endl;
                break;
            }

        }
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
     w.show();//     return a.exec();
}*/