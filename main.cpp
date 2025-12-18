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
#include "FonctionsAide.h"
#include "Sauvegarde.h"
#include "MainWindow.h" 
#include <QApplication>
#include "MenuConfiguration.h"

using namespace std;
using namespace Akropolis;

void LancerPartie(Akropolis::Configuration& config) {
    Akropolis::Partie* partie= new Akropolis::Partie(1, config.getMode());
    partie->setDifficulte(config.getDifficulte());
    partie->setConfig(config);
    config.setPartie(partie);


    for (size_t i = 0; i < config.getNomsJoueurs().size(); ++i) {
        partie->ajouterJoueur(config.getNomsJoueurs()[i], i + 1);
    }

    if (config.getMode() == Akropolis::ModeJeu::SOLO) {
        partie->ajouterJoueur("Illustre Constructeur", 2, true);
    }

    partie->lancerPartie();
    AfficherResultats(*partie);
}

int LancerConsole() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    try {
        int choixMenu;
        MenuConfiguration menuConfig;
        while (true) {
            cout << "\n--- MENU PRINCIPAL ---" << endl;
            cout << "[1] Créer une nouvelle partie" << endl;
            cout << "[2] Charger une partie enregistrée" << endl;
            cout << "[3] Quitter le jeu" << endl;

            if (!readIntOrQuit(choixMenu, 1, 3, "Votre choix")) {
                choixMenu = 3;
            }
            if (choixMenu == 1) {
                Configuration config = menuConfig.demarrerConfiguration();
                LancerPartie(config);
            } else if (choixMenu == 2) {
                Sauvegarde::MenuSauvegardes();
            } else if (choixMenu == 3) {
                cout << "\n A bientot !" << endl;
                break;
            }
        }
    } catch (const PartieAnnulee& e) {
        cout << "\n\n=== FIN DE PARTIE ===" << endl;
        cout << "\nPartie quitée";
        //cout << e.getInfo() << endl;
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

int LancerQt(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

int main(int argc, char *argv[]) {
    int modeDeJeu;

    while (true) {
        cout << "Veuillez choisir le mode de jeu (Console : 1; Qt : 2) : ";
        if (cin >>modeDeJeu) {
            if (modeDeJeu == 1) {
                return LancerConsole();
            }
            else if (modeDeJeu == 2) {
                return LancerQt(argc, argv);
            }
        }
        
        cout << "Choix invalide." << endl;
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return 0;
}
