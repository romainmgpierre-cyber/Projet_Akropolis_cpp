#ifndef MENU_CONFIG
#define MENU_CONFIG
#include <string>
#include <vector>
#include "GameExcep_Enums.h"


namespace Akropolis {
    class Configuration {
    private:
        ModeJeu mode;
        NiveauDifficulte difficulte = NiveauDifficulte::FACILE; // On le met a facile par défaut
        int nbJoueurHumain = 0;
        std::vector<std::string> nomsJoueurs;
    public:
        ModeJeu getMode() const {return mode;}

        
        NiveauDifficulte getDifficulte()const{return difficulte;}
        int getNbjoueurHumain()const {return nbJoueurHumain;}
        std::vector<std::string> getNomsJoueurs() const {return nomsJoueurs;}
        Configuration(ModeJeu m = ModeJeu::SOLO) : mode(m) {}
        
        void setDifficulte(NiveauDifficulte d) { difficulte = d; }
        void setNbJoueurHumain(int nb) { nbJoueurHumain = nb; }
        void setNomsJoueurs(const std::vector<std::string>& noms) { nomsJoueurs = noms; }
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