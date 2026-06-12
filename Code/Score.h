#ifndef SCORE_H
#define SCORE_H
#include <vector>
#include <map>
#include <iostream>
#include "Joueur.h"
#include "GameExcep_Enums.h"
#include "Partie_Variante.h"


using namespace std;

namespace Akropolis {

    // pour stocker le détail du score d'une couleur
    struct ScoreDetail {
        int nbEtoiles;      
        int valeurTuiles;   
        
        int totalOfficiel() const { return (nbEtoiles) * valeurTuiles; } 
        
    };

    // tableau des scores
    class CalculScoreBase {
    public:
        virtual ~CalculScoreBase() = default;
        virtual int calculerScore(const Joueur& joueur) const = 0;
    };

    // la logique principale sera dans TableauScore
    class CalculScoreRecouvrement : public virtual CalculScoreBase {
    public:
        int calculerScore(const Joueur& joueur) const override;
    };

    class CalculScorePlaces : public virtual CalculScoreBase {
    public:
        int calculerScore(const Joueur& joueur) const override;
    };

    class CalculScoreMultiplicateurs : public virtual CalculScoreBase {
    public:
        int calculerScore(const Joueur& joueur) const override;
    };

    class TableauScore :
        public CalculScoreRecouvrement,
        public CalculScorePlaces,
        public CalculScoreMultiplicateurs
    {
    private:
        Joueur* joueur;
        vector<pair<Joueur*, int>> scores;
        vector<Variante> variantesActives;

    public:
        void setVariantesActives(const vector<Variante>& vars) {
            variantesActives = vars;
        }
        void ajouterJoueur(Joueur* j);
        void calculerScores(); 
        void afficherScores(ostream& f = cout) const; 
        
        
        void afficherDetailsScore(const Joueur& joueur, ostream& f = cout) const;

       
        map<Couleur, ScoreDetail> calculerDetails(const Joueur& joueur) const;

        int calculerScore(const Joueur& joueur) const override {
           
            map<Couleur, ScoreDetail> details = calculerDetails(joueur);
            int total = 0;
            for (auto const& entry : details) {
                total += entry.second.totalOfficiel();
            }
            return total;
        }
        int calculerScoreIA(const Joueur& joueurIA, NiveauDifficulte diff) const; //Score pour l'IA
    };
    
    void AfficherResultats(Partie& partie);
}

#endif