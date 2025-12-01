#include "Score.h"
#include <set>
#include <queue>
#include <algorithm>

namespace Akropolis{

    // Helpers (déjà présents ou à rajouter)
    const std::map<CoordHex, std::pair<HexagoneConstruction*, unsigned int>>& getPlateauDuJoueur(const Joueur& joueur) {
        return joueur.getCite()->getPlateau();
    }

    // --- Algorithme pour trouver le score des Bleus (Plus grand groupe) ---
    int calculerScoreBleu(const std::map<CoordHex, std::pair<HexagoneConstruction*, unsigned int>>& plateau) {
        std::set<CoordHex> visites;
        int maxScoreGroupe = 0;

        for (const auto& paire : plateau) {
            CoordHex startNode = paire.first;
            
            // Si c'est un quartier bleu et pas encore visité
            if (Quartier* q = dynamic_cast<Quartier*>(paire.second.first)) {
                if (q->getType().getCouleur() == Couleur::bleu && visites.find(startNode) == visites.end()) {
                    
                    // Parcours en largeur (BFS) pour trouver la taille du groupe
                    int currentGroupScore = 0;
                    std::queue<CoordHex> file;
                    file.push(startNode);
                    visites.insert(startNode);

                    while(!file.empty()) {
                        CoordHex current = file.front();
                        file.pop();

                        // On ajoute la hauteur de la tuile au score du groupe
                        // (Dans Akropolis, un quartier niveau 2 vaut 2 points, niveau 3 vaut 3 points, etc.)
                        unsigned int hauteur = plateau.at(current).second;
                        currentGroupScore += hauteur;

                        // Vérifier les voisins
                        for(int i=0; i<6; ++i) {
                            CoordHex voisin = current.voisin(i);
                            auto it = plateau.find(voisin);
                            
                            // Si voisin existe, est bleu, et pas visité
                            if (it != plateau.end() && visites.find(voisin) == visites.end()) {
                                if (Quartier* qVoisin = dynamic_cast<Quartier*>(it->second.first)) {
                                    if (qVoisin->getType().getCouleur() == Couleur::bleu) {
                                        visites.insert(voisin);
                                        file.push(voisin);
                                    }
                                }
                            }
                        }
                    }
                    // On garde seulement le score du plus grand groupe
                    if (currentGroupScore > maxScoreGroupe) {
                        maxScoreGroupe = currentGroupScore;
                    }
                }
            }
        }
        return maxScoreGroupe;
    }

    // --- Helpers de validation existants ---
    bool estEntoure(const CoordHex& pos, const std::map<CoordHex, std::pair<HexagoneConstruction*, unsigned int>>& plateau) {
        for (int i = 0; i < 6; ++i) {
            if (plateau.find(pos.voisin(i)) == plateau.end()) return false;
        }
        return true;
    }

    bool estEnPeripherie(const CoordHex& pos, const std::map<CoordHex, std::pair<HexagoneConstruction*, unsigned int>>& plateau) {
        for (int i = 0; i < 6; ++i) {
            if (plateau.find(pos.voisin(i)) == plateau.end()) return true;
        }
        return false;
    }


    // --- CŒUR DU CALCUL ---
    map<Couleur, ScoreDetail> TableauScore::calculerDetails(const Joueur& joueur) const {
        map<Couleur, ScoreDetail> details;
        
        // Initialiser les couleurs à 0
        for (Couleur c : {Couleur::bleu, Couleur::jaune, Couleur::rouge, Couleur::violet, Couleur::vert}) {
            details[c] = {0, 0};
        }

        const auto& plateau = getPlateauDuJoueur(joueur);

        // 1. Compter les Etoiles (Places)
        for (const auto& paire : plateau) {
            if (Place* p = dynamic_cast<Place*>(paire.second.first)) {
                details[p->getType().getCouleur()].nbEtoiles += p->getNbEtoile();
            }
        }

        // 2. Calculer la valeur des quartiers valides (Sauf Bleu qui est spécial)
        for (const auto& paire : plateau) {
            const CoordHex& pos = paire.first;
            unsigned int hauteur = paire.second.second;

            if (Quartier* q = dynamic_cast<Quartier*>(paire.second.first)) {
                Couleur coul = q->getType().getCouleur();
                
                bool valide = false;

                if (coul == Couleur::jaune) { // Marché
                    bool adjacentMarché = false;
                    for(int i=0; i<6; ++i) {
                        auto it = plateau.find(pos.voisin(i));
                        if(it != plateau.end()){
                            if(Quartier* qV = dynamic_cast<Quartier*>(it->second.first))
                                if(qV->getType().getCouleur() == Couleur::jaune) adjacentMarché = true;
                        }
                    }
                    if(!adjacentMarché) valide = true;

                } else if (coul == Couleur::rouge) { // Caserne
                    if (estEnPeripherie(pos, plateau)) valide = true;

                } else if (coul == Couleur::violet) { // Temple
                    if (estEntoure(pos, plateau)) valide = true;

                } else if (coul == Couleur::vert) { // Jardin
                    valide = true; // Toujours valide
                }
                
                // Note : Bleu est traité à part car c'est un groupe entier, pas tuile par tuile.

                if (valide && coul != Couleur::bleu) {
                    details[coul].valeurTuiles += hauteur;
                }
            }
        }

        // 3. Cas spécial Bleu (Habitation)
        details[Couleur::bleu].valeurTuiles = calculerScoreBleu(plateau);

        return details;
    }

    // --- AFFICHAGE DÉTAILLÉ ---
    void TableauScore::afficherDetailsScore(const Joueur& joueur, ostream& f) const {
        map<Couleur, ScoreDetail> details = calculerDetails(joueur);
        int scoreTotal = 0;

        f << "Détails du score pour " << joueur.getNom() << " :" << endl;
        f << "------------------------------------------------" << endl;

        // Ordre d'affichage
        vector<Couleur> ordre = {Couleur::bleu, Couleur::jaune, Couleur::rouge, Couleur::violet, Couleur::vert};

        for (Couleur c : ordre) {
            ScoreDetail d = details[c];
            int sousTotal = d.totalOfficiel();
            scoreTotal += sousTotal;

            string nomCouleur = toString(c); // Utilise votre fonction existante
            // Mise en forme : Habitation (Bleu) = 2 étoiles * 5 tuiles = 10
            f << " - " << nomCouleur << " :\t" 
              << d.nbEtoiles << " etoile(s) x " 
              << d.valeurTuiles << " valeur tuiles = " 
              << sousTotal << " pts" << endl;
        }
        
        // Bonus Pierre : 1 point par pierre restante
        int pointsPierres = joueur.getNbPierres();
        scoreTotal += pointsPierres;
        f << " - Pierres :\t" << pointsPierres << " (1 pt par pierre)" << endl; 

        f << "------------------------------------------------" << endl;
        f << " SCORE TOTAL : " << scoreTotal << " POINTS" << endl;
        f << "================================================" << endl;
    }

    // --- Implémentations héritées (pour compatibilité) ---
    int CalculScoreRecouvrement::calculerScore(const Joueur& joueur) const { return 0; }
    int CalculScorePlaces::calculerScore(const Joueur& joueur) const { return 0; }
    int CalculScoreMultiplicateurs::calculerScore(const Joueur& joueur) const { return 0; }

    void TableauScore::ajouterJoueur(Joueur* j) {
        scores.push_back({j, 0});
    }

    void TableauScore::calculerScores() {
        for(auto& pair : scores) {
            pair.second = calculerScore(*pair.first);
        }
    }

    void TableauScore::afficherScores(ostream& f) const {
        f << "\n=== TABLEAU DES SCORES FINAL ===" << endl;
        // On trie pour avoir le gagnant en premier ?
        // (Copie locale pour trier)
        auto scoresTries = scores; 
        sort(scoresTries.begin(), scoresTries.end(), [](const pair<Joueur*, int>& a, const pair<Joueur*, int>& b) {
            return a.second > b.second; // Décroissant
        });

        for(const auto& pair : scoresTries) {
            afficherDetailsScore(*pair.first, f);
            f << endl;
        }
        
        f << "🏆 LE GAGNANT EST : " << scoresTries[0].first->getNom() << " ! 🏆" << endl;
    }

}