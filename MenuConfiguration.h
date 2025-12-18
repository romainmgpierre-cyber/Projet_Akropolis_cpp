#ifndef MENU_CONFIG
#define MENU_CONFIG
#include <string>
#include <vector>
#include "GameExcep_Enums.h"
#include "Partie_Variante.h"


namespace Akropolis {
    class Partie;
    class Configuration {
    private:
        ModeJeu mode;
        NiveauDifficulte difficulte = NiveauDifficulte::FACILE; // On le met a facile par défaut
        int nbJoueurHumain = 0;
        std::vector<std::string> nomsJoueurs;

        Partie* partieCourante = nullptr;
    public:
        ModeJeu getMode() const {return mode;}

        //geters
        NiveauDifficulte getDifficulte()const{return difficulte;}
        int getNbjoueurHumain()const {return nbJoueurHumain;}
        std::vector<std::string> getNomsJoueurs() const {return nomsJoueurs;}
        Partie* getPartieCourante()const{return partieCourante;}

        // Constructeur
        Configuration( ModeJeu m = ModeJeu::SOLO, Akropolis::Partie* p = nullptr)
            : mode(m), partieCourante(p) {}

        //setters
        void setDifficulte(NiveauDifficulte d) { difficulte = d; }
        void setNbJoueurHumain(int nb) { nbJoueurHumain = nb; }
        void setNomsJoueurs(const std::vector<std::string>& noms) { nomsJoueurs = noms; }

        int setPartie(Akropolis::Partie* p) {
            partieCourante = p;
            return 0;
        }
    };

    class MenuConfiguration {
    public:
        
        Configuration demarrerConfiguration();

    private:
        // Fonctions d'aide pour chaque mode
        void configurerModeJeu(Configuration& config);
        void configurerSolo(Configuration& config);
        void configurerMulti(Configuration& config);
    };

}
#endif