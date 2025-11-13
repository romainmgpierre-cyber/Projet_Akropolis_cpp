#ifndef SCORE_H
#define SCORE_H
#include <vector>
#include "Joueur.h"
using namespace std;

namespace Akropolis {
           // --- Tableau des scores ---
    class CalculScoreBase {
    public:
        virtual ~CalculScoreBase() = default;
        virtual int calculerScore(const Joueur& joueur) const = 0;
    };

    class CalculScoreRecouvrement : public virtual CalculScoreBase {
    public:
        int calculerScore(const Joueur& joueur) const ;
    };

    class CalculScorePlaces : public virtual CalculScoreBase {
    public:
        int calculerScore(const Joueur& joueur) const ;
    };

    class CalculScoreMultiplicateurs : public virtual CalculScoreBase {
    public:
        int calculerScore(const Joueur& joueur) const;
    };

    class TableauScore :
        public CalculScoreRecouvrement,
        public CalculScorePlaces,
        public CalculScoreMultiplicateurs
    {
    private:
        Joueur* joueur;
        vector<pair<Joueur*, int>> scores;

    public:
        void ajouterJoueur(Joueur* j);
        void calculerScores();
        void afficherScores(ostream& f = cout) const;
        int calculerScore(const Joueur& joueur) const{
        return CalculScoreRecouvrement::calculerScore(joueur)
             + CalculScorePlaces::calculerScore(joueur)
             + CalculScoreMultiplicateurs::calculerScore(joueur);
    }
    };
}

#endif