#include "Score.h"

namespace Akropolis{
    
    // --- CalculScoreRecouvrement ---
int CalculScoreRecouvrement::calculerScore(const Joueur& joueur) const {
    // TODO : Calculer les points liés aux tuiles recouvertes pour ce joueur
    return 0;
}

// --- CalculScorePlaces ---
int CalculScorePlaces::calculerScore(const Joueur& joueur) const {
    // TODO : Calculer les points des places selon les règles du jeu
    return 0;
}

// --- CalculScoreMultiplicateurs ---
int CalculScoreMultiplicateurs::calculerScore(const Joueur& joueur) const {
    // TODO : Calculer les multiplicateurs selon les quartiers ou autres critères
    return 0;
}

// --- TableauScore ---
void TableauScore::ajouterJoueur(Joueur* j) {
    scores.push_back({j, 0});
}

void TableauScore::calculerScores() {
    // On parcourt tous les joueurs et on calcule le score total
    for(auto& pair : scores) {
        Joueur* j = pair.first;

        int scoreRecouvrement    = CalculScoreRecouvrement::calculerScore(*j);
        int scorePlaces          = CalculScorePlaces::calculerScore(*j);
        int scoreMultiplicateur  = CalculScoreMultiplicateurs::calculerScore(*j);

        pair.second = scoreRecouvrement + scorePlaces + scoreMultiplicateur;
    }
}

void TableauScore::afficherScores(ostream& f) const {
    f << "=== Scores ===\n";
    for(const auto& pair : scores) {
        // TODO : remplacer getNom() par la méthode réelle pour récupérer le nom du joueur
        f << "Joueur " /* << pair.first->getNom() */ << ": " << pair.second << " pts\n";
    }
}

}