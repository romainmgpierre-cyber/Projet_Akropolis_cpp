#include "Score.h"
#include <set>
#include <queue>
#include <algorithm>


namespace Akropolis{

    // Helpers (déjà présents ou à rajouter)
    const std::map<CoordHex, std::pair<HexagoneConstruction*, unsigned int>>& getPlateauDuJoueur(const Joueur& joueur) {
        return joueur.getCite()->getPlateau();
    }

    bool estVarianteActive(const vector<Variante>& variantes, const string& nomCible) {
        for (const auto& v : variantes) {
            if (v.getNom() == nomCible && v.estActive()) return true;
        }
        return false;
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

        // 1. Compter les Etoiles (Places) - Inchangé
        for (const auto& paire : plateau) {
            if (Place* p = dynamic_cast<Place*>(paire.second.first)) {
                details[p->getType().getCouleur()].nbEtoiles += p->getNbEtoile();
            }
        }

        // --- DÉTECTION DES VARIANTES ACTIVES ---
        // (Les noms doivent correspondre exactement à ceux que tu donneras dans Partie)
        bool vHabitation = estVarianteActive(variantesActives, "Architectes");
        bool vMarche     = estVarianteActive(variantesActives, "Commercants");
        bool vCaserne    = estVarianteActive(variantesActives, "Sentinelles");
        bool vTemple     = estVarianteActive(variantesActives, "Pretres");
        bool vJardin     = estVarianteActive(variantesActives, "Jardiniers");

        // 2. Calculer la valeur des quartiers valides
        for (const auto& paire : plateau) {
            const CoordHex& pos = paire.first;
            unsigned int hauteur = paire.second.second;
            HexagoneConstruction* hex = paire.second.first;

            if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
                Couleur coul = q->getType().getCouleur();
                
                bool valide = false;
                int multiplicateur = 0; // Par défaut x0

                if (coul == Couleur::jaune) { // MARCHE
                    bool adjacentMarche = false;
                    bool adjacentPlaceMarche = false; 

                    for(int i=0; i<6; ++i) {
                        auto it = plateau.find(pos.voisin(i));
                        if(it != plateau.end()){
                            // Condition de base : adjacent à un autre quartier marché
                            if(Quartier* qV = dynamic_cast<Quartier*>(it->second.first))
                                if(qV->getType().getCouleur() == Couleur::jaune) adjacentMarche = true;
                            
                            // VARIANTE COMMERCANTS : Adjacent à une Place Marché
                            if (vMarche) {
                                if(Place* pV = dynamic_cast<Place*>(it->second.first))
                                    if(pV->getType().getCouleur() == Couleur::jaune) adjacentPlaceMarche = true;
                            }
                        }
                    }
                    if(!adjacentMarche) {
                        valide = true;
                        if (vMarche && adjacentPlaceMarche) multiplicateur = 2;
                    }

                } else if (coul == Couleur::rouge) { // CASERNE
                    if (estEnPeripherie(pos, plateau)) {
                        valide = true;
                        
                        // VARIANTE SENTINELLES : 3 ou 4 espaces vides adjacents
                        if (vCaserne) {
                            int nbVides = 0;
                            for(int i=0; i<6; ++i) {
                                if (plateau.find(pos.voisin(i)) == plateau.end()) nbVides++;
                            }
                            if (nbVides == 3 || nbVides == 4) multiplicateur = 2;
                        }
                    }

                } else if (coul == Couleur::violet) { // TEMPLE
                    if (estEntoure(pos, plateau)) {
                        valide = true;
                        
                        // VARIANTE PRETRES : Niveau supérieur (hauteur >= 2)
                        if (vTemple && hauteur >= 2) multiplicateur = 2;
                    }

                } else if (coul == Couleur::vert) { // JARDIN
                    valide = true; // Toujours valide de base
                    
                    // VARIANTE JARDINIERS : Adjacent à un Lac
                    // Un Lac est une case vide entourée de 6 hexagones
                    if (vJardin) {
                        bool adjacentLac = false;
                        for(int i=0; i<6; ++i) {
                            CoordHex voisin = pos.voisin(i);
                            // Si le voisin est vide...
                            if (plateau.find(voisin) == plateau.end()) {
                                // ...et qu'il est entouré (fonction existante estEntoure)
                                if (estEntoure(voisin, plateau)) {
                                    adjacentLac = true;
                                    break;
                                }
                            }
                        }
                        if (adjacentLac) multiplicateur = 2;
                    }
                }
                
                // Calcul final pour ce quartier
                if (valide && coul != Couleur::bleu) {
                    details[coul].valeurTuiles += (hauteur * multiplicateur);
                }
            }
        }

        // 3. Cas spécial Bleu (Habitation)
        int scoreBleu = calculerScoreBleu(plateau);
        
        // VARIANTE ARCHITECTES : Si le score du groupe >= 10, on double
        if (vHabitation && scoreBleu >= 10) {
            scoreBleu *= 2;
        }
        
        details[Couleur::bleu].valeurTuiles = scoreBleu;

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

    // Score pour l'IA
int TableauScore::calculerScoreIA(const Joueur& joueurIA, NiveauDifficulte diff) const {
    const auto& tuiles = joueurIA.getCite()->getTuiles();
    

    
    map<Couleur, int> nbEtoiles;
    map<Couleur, int> nbQuartiers;
    
    // Valeurs par défaut de la tuile de départ :
    nbEtoiles[Couleur::bleu] = 1; 
    int nbCarrieres = 3;         

   
    for (const auto* tuile : tuiles) {
        for (size_t i = 0; i < 3; ++i) {
            HexagoneConstruction* hex = tuile->getHexagone(i);

            if (Place* p = dynamic_cast<Place*>(hex)) {
                nbEtoiles[p->getType().getCouleur()] += p->getNbEtoile();
            } 
            else if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
                int hauteurVirtuelle = 1;
                // Règle Difficile (Callicratès) : compte niveau 2
                if (diff == NiveauDifficulte::DIFFICILE) {
                    hauteurVirtuelle = 2;
                }
                nbQuartiers[q->getType().getCouleur()] += hauteurVirtuelle;
            }
            else if (dynamic_cast<Carriere*>(hex)) {
                nbCarrieres++;
            }
        }
    }

    int scoreTotal = 0;

    for (Couleur c : {Couleur::bleu, Couleur::jaune, Couleur::rouge, Couleur::violet, Couleur::vert}) {
        scoreTotal += (nbQuartiers[c] * nbEtoiles[c]);
    }

    // Règle Moyen (Métagénès) : +2 points par carrière
    if (diff == NiveauDifficulte::MOYEN) {
        scoreTotal += (nbCarrieres * 2);
    }

    return scoreTotal;
}
    void AfficherResultats(Partie& partie) {
     //Fin de partie et Résultats
        cout << "\n\n*****************************************" << endl;
        cout << "* RÉSULTATS FINAUX                      *" << endl;
        cout << "*****************************************" << endl;

        TableauScore calculateurFinal;
        calculateurFinal.setVariantesActives(partie.getVariantesActives());
        const vector<Joueur*>& tousLesJoueurs = partie.getJoueurs();

        if (partie.getMode() == ModeJeu::SOLO) {
            // RÉSULTATS SOLO
            Joueur* humain = nullptr;
            Joueur* ia = nullptr;

            // Identification des rôles
            for (auto* j : tousLesJoueurs) {
                if (j->isIA()) ia = j;
                else humain = j;
            }

            if (humain && ia) {
                // Score Humain (Calcul standard)
                cout << "\n--- VOTRE CITE (" << humain->getNom() << ") ---" << endl;
                calculateurFinal.afficherDetailsScore(*humain, cout);
                int scoreHumain = calculateurFinal.calculerScore(*humain);

                // Score IA (Calcul spécifique selon difficulté)
                cout << "\n--- ILLUSTRE ARCHITECTE (" << ia->getNom() << ") ---" << endl;
                // On n'affiche pas les détails complexes pour l'IA, juste le total
                int scoreIA = calculateurFinal.calculerScoreIA(*ia, partie.getDifficulte());

                cout << "Difficulte : ";
                switch(partie.getDifficulte()) {
                case NiveauDifficulte::FACILE: cout << "Facile (Hippodamos)"; break;
                case NiveauDifficulte::MOYEN: cout << "Moyen (Metagenes)"; break;
                case NiveauDifficulte::DIFFICILE: cout << "Difficile (Callicrates)"; break;
                }
                cout << endl;
                cout << "SCORE FINAL IA : " << scoreIA << " points." << endl;

                // Comparaison et Verdict
                cout << "\n*****************************************" << endl;
                cout << "              VERDICT                    " << endl;
                cout << "*****************************************" << endl;
                cout << humain->getNom() << " : " << scoreHumain << " pts | " << humain->getNbPierres() << " pierres" << endl;
                cout << ia->getNom() << " : " << scoreIA << " pts | " << ia->getNbPierres() << " pierres" << endl;
                cout << "-----------------------------------------" << endl;

                if (scoreHumain > scoreIA) {
                    cout << "VICTOIRE ! Vous avez vaincu l'Illustre Architecte !" << endl;
                }
                else if (scoreIA > scoreHumain) {
                    cout << "DEFAITE. L'Illustre Architecte remporte la victoire." << endl;
                }
                else {
                    // EGALITÉ : Départage aux pierres
                    cout << "EGALITE aux points ! On compare les pierres..." << endl;
                    if (humain->getNbPierres() > ia->getNbPierres()) {
                        cout << "Vous avez plus de pierres. VICTOIRE !" << endl;
                    } else {
                        // "En cas d'égalité, c'est le joueur ayant le plus de Pierres qui l'emporte."

                        if (ia->getNbPierres() > humain->getNbPierres())
                            cout << "L'IA a plus de pierres. DEFAITE." << endl;
                        else
                            cout << "EGALITE PARFAITE (Points et Pierres) ! Quel match !" << endl;
                    }
                }
            }
        }
        else {
            //RÉSULTATS MULTIJOUEUR (Classique)
            // On calcule et stocke les scores pour le tri
            vector<pair<Joueur*, int>> scoresFinaux;

            for(const auto& joueur : tousLesJoueurs) {
                calculateurFinal.afficherDetailsScore(*joueur, cout);
                scoresFinaux.push_back({joueur, calculateurFinal.calculerScore(*joueur)});
                cout << endl;
            }

            // Tri décroissant pour trouver le vainqueur
            sort(scoresFinaux.begin(), scoresFinaux.end(),
                 [](const pair<Joueur*, int>& a, const pair<Joueur*, int>& b) {
                     if (a.second != b.second) {
                         return a.second > b.second; // Plus de points d'abord
                     }
                     return a.first->getNbPierres() > b.first->getNbPierres(); // Pierres si égalité
                 });

            cout << "*****************************************" << endl;
            cout << "VAINQUEUR : " << scoresFinaux[0].first->getNom()
                 << " avec " << scoresFinaux[0].second << " points !" << endl;
            cout << "*****************************************" << endl;
        }

        cout << "\nMerci d'avoir joue a Akropolis !" << endl;
    }
}
