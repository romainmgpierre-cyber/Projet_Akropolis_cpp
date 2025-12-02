#ifndef SCORE_H
#define SCORE_H
#include <vector>
#include <map>
#include <iostream>
#include "Joueur.h"
#include "GameExcep_Enums.h" // Assurez-vous d'avoir accès à Couleur et toString

using namespace std;

namespace Akropolis {

    // Structure pour stocker le détail du score d'une couleur
    struct ScoreDetail {
        int nbEtoiles;      // Nombre d'étoiles 
        int valeurTuiles;   // Somme des hauteurs des tuiles valides
        
        int totalOfficiel() const { return (nbEtoiles + 1) * valeurTuiles; } // +1 car les étoiles s'ajoutent au multiplicateur de base 1 ? 
        // NOTE : Dans les règles officielles Akropolis, c'est Score = ValeurTuiles * NbEtoiles.
    };

    // --- Tableau des scores ---
    class CalculScoreBase {
    public:
        virtual ~CalculScoreBase() = default;
        virtual int calculerScore(const Joueur& joueur) const = 0;
    };

    // On garde ces classes pour la compatibilité, mais la logique principale sera dans TableauScore
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

    public:
        void ajouterJoueur(Joueur* j);
        void calculerScores(); // Met à jour le vecteur scores
        void afficherScores(ostream& f = cout) const; // Affiche juste le résumé
        
        // Nouvelle fonction pour afficher le détail demandé
        void afficherDetailsScore(const Joueur& joueur, ostream& f = cout) const;

        // Fonction helper pour calculer les détails
        map<Couleur, ScoreDetail> calculerDetails(const Joueur& joueur) const;

        int calculerScore(const Joueur& joueur) const override {
            // Recalcul complet via la nouvelle méthode
            map<Couleur, ScoreDetail> details = calculerDetails(joueur);
            int total = 0;
            for (auto const& entry : details) {
                total += entry.second.totalOfficiel();
            }
            return total;
        }
    };
}

#endif