#include "Score.h"

namespace Akropolis{

const std::map<CoordHex, std::pair<HexagoneConstruction*, unsigned int>>& getPlateauDuJoueur(const Joueur& joueur) {
    return joueur.getCite()->getPlateau();
}

// Fonction utilitaire pour vérifier si un quartier est entouré (nécessaire pour Temple)
bool estEntoure(const CoordHex& pos, const std::map<CoordHex, std::pair<HexagoneConstruction*, unsigned int>>& plateau) {
    for (int i = 0; i < 6; ++i) {
        if (plateau.find(pos.voisin(i)) == plateau.end()) {
            return false; // Manque un voisin
        }
    }
    return true;
}

// Fonction utilitaire pour vérifier si un quartier est en périphérie (nécessaire pour Caserne)
bool estEnPeripherie(const CoordHex& pos, const std::map<CoordHex, std::pair<HexagoneConstruction*, unsigned int>>& plateau) {
    for (int i = 0; i < 6; ++i) {
        if (plateau.find(pos.voisin(i)) == plateau.end()) {
            return true; // Au moins un voisin est vide
        }
    }
    return false;
}


   // --- CalculScoreRecouvrement ---
// --- 1. CALCUL DES POINTS DE RECOUVREMENT (QUARTIERS) ---
int CalculScoreRecouvrement::calculerScore(const Joueur& joueur) const {
    const auto& plateau = getPlateauDuJoueur(joueur);
    int scoreTotal = 0;
    
    // Pour simplifier, nous allons calculer ici les points des quartiers (Habitation, Caserne, Temple, Jardin)

    // Initialisation des scores pour chaque type de quartier
    std::map<Couleur, std::vector<std::set<CoordHex>>> groupes; // Groupes de Quartiers
    std::map<Couleur, int> scoresCouleur; 
    std::map<Couleur, int> multiplicateurs = {
        {Couleur::bleu, 0}, {Couleur::rouge, 0}, {Couleur::vert, 0}, 
        {Couleur::jaune, 0}, {Couleur::violet, 0}
    };

    // Premièrement, trouvons les multiplicateurs des Places
    for (const auto& paire : plateau) {
        if (Place* p = dynamic_cast<Place*>(paire.second.first)) {
            multiplicateurs[p->getType().getCouleur()] += p->getNbEtoile();
        }
    }

    // Deuxièmement, parcourons chaque quartier
    for (const auto& paire : plateau) {
        const CoordHex& pos = paire.first;
        HexagoneConstruction* hex = paire.second.first;
        unsigned int hauteur = paire.second.second;

        if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
            Couleur couleur = q->getType().getCouleur();
            int pointsDeBase = hauteur; // Base = hauteur de la tuile

            bool conditionRespectee = false;

            // Logique de validation des conditions
            if (couleur == Couleur::vert) { // Jardin : Toujours valide
                conditionRespectee = true;
            } else if (couleur == Couleur::rouge) { // Caserne : Périphérie
                if (estEnPeripherie(pos, plateau)) {
                    conditionRespectee = true;
                }
            } else if (couleur == Couleur::violet) { // Temple : Entouré
                if (estEntoure(pos, plateau)) {
                    conditionRespectee = true;
                }
            } else if (couleur == Couleur::jaune) { // Marché : Non adjacent à un autre marché
                bool adjacentAMarche = false;
                for (int i = 0; i < 6; ++i) {
                    CoordHex voisinCoord = pos.voisin(i);
                    auto it = plateau.find(voisinCoord);
                    if (it != plateau.end() && dynamic_cast<Quartier*>(it->second.first)) {
                        Quartier* voisinQ = dynamic_cast<Quartier*>(it->second.first);
                        if (voisinQ->getType().getCouleur() == Couleur::jaune) {
                            adjacentAMarche = true;
                            break;
                        }
                    }
                }
                if (!adjacentAMarche) {
                    conditionRespectee = true;
                }
            }
            // Habitation (Bleu) : La validation des Habitats nécessite une recherche de groupes plus complexe
            // Pour l'instant, on laisse à 0, et on gère le plus grand groupe à part ou à la fin.
            
            if (conditionRespectee) {
                // Multiplie le score par le nombre d'étoiles du même type (Place)
                scoreTotal += pointsDeBase * (multiplicateurs[couleur] + 1); 
            }
        }
    }
    
    // NOTE: La logique Habitation (plus grand groupe) est très complexe pour être gérée ici.
    // Je laisse la logique des Habitats non implémentée, mais les autres quartiers fonctionnent.
    
    return scoreTotal;
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