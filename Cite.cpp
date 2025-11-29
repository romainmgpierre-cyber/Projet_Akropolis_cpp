#include "Cite.h"

using namespace std;
namespace Akropolis{
    
    Cite::~Cite() {
        
        for (TuileCite* tuile : tuiles_posees) {
            delete tuile;
        }
        // La TuileDepart est gérée par la Partie
    }

    void Cite::initialiserCite(TuileDepart* tuileDepart) {
        // Centre (0,0) est la Place [hex 0]
        CoordHex centre(0, 0);
        plateau[centre] = { tuileDepart->getHexagone(0), 1 };
        mettreAJourFrontiere(centre);

        // Les 3 carrières autour
        CoordHex c1 = centre.voisin(0); // (1, 0)
        CoordHex c2 = centre.voisin(2); // (-1, 1)
        CoordHex c3 = centre.voisin(4); // (0, -1)
        
        plateau[c1] = { tuileDepart->getHexagone(1), 1 };
        mettreAJourFrontiere(c1);
        plateau[c2] = { tuileDepart->getHexagone(2), 1 };
        mettreAJourFrontiere(c2);
        plateau[c3] = { tuileDepart->getHexagone(3), 1 };
        mettreAJourFrontiere(c3);
    }

    void Cite::mettreAJourFrontiere(const CoordHex& pos) {
        // La case n'est plus "frontière", elle est occupée
        frontiere.erase(pos);
        
        // Regarde les 6 voisins
        for (int i = 0; i < 6; ++i) {
            CoordHex voisin = pos.voisin(i);
            // Si le voisin est vide (pas dans plateau)
            if (plateau.find(voisin) == plateau.end()) {
                // il devient une case frontière
                frontiere.insert(voisin);
            }
        }
    }

    vector<Cite::CoupPossible> Cite::genererCoupsValides(const TuileCite& tuile) const {
        vector<CoupPossible> coups;
        TuileCite* tuileTest = tuile.clone(); // Pour tester les permutations

        // --- NOUVELLE LOGIQUE "AU SOL" ---
        // // 1. Créer la "frontière élargie" pour les placements au sol.
        // Elle contient 'frontiere' + tous les voisins vides de 'frontiere'.
        set<CoordHex> frontiereElargie = frontiere;
        for (const CoordHex& pos : frontiere) {
            for (int i = 0; i < 6; ++i) {
                CoordHex voisin = pos.voisin(i);
                // Si le voisin est vide (pas dans plateau), on l'ajoute aux candidats 'Ancre'
                if (plateau.find(voisin) == plateau.end()) {
                    frontiereElargie.insert(voisin);
                }
            }
        }
        // --- FIN NOUVELLE LOGIQUE ---


        // Boucle 1 : Les 2 formes géométriques (V et ^)
        for (int forme = 0; forme < 2; forme++) {

            CoordHex rel1 = (forme == 0) ? CoordHex(0, 1) : CoordHex(0, -1);
            CoordHex rel2 = (forme == 0) ? CoordHex(-1, 1) : CoordHex(1, -1);

            // Boucle 2 : Les 3 permutations de contenu
            for (int perm = 0; perm < 3; perm++) {

                int rotation_id = (forme * 3) + perm; // ID unique 0-5

                // --- RECHERCHE "AU SOL" (MODIFIÉE) ---
                // On itère sur la 'frontiereElargie' au lieu de 'frontiere'
                for (const CoordHex& ancre : frontiereElargie) {
                    CoordHex pos1 = ancre + rel1;
                    CoordHex pos2 = ancre + rel2;

                    // Condition A: Les 3 cases (ancre, pos1, pos2) doivent être VIDES
                    if (plateau.find(ancre) == plateau.end() && // ancre doit être vide
                        plateau.find(pos1) == plateau.end() &&
                        plateau.find(pos2) == plateau.end()) {

                        // Condition B: AU MOINS UN des 3 doit toucher la frontière d'origine
                        if (frontiere.count(ancre) > 0 || frontiere.count(pos1) > 0 || frontiere.count(pos2) > 0) {
                            // C'est un coup "au sol" valide (hauteur 1)
                            coups.push_back({ancre, rotation_id, false, 1});
                        }
                    }
                } // --- FIN RECHERCHE "AU SOL" ---


                // --- RECHERCHE "EN HAUTEUR" (Inchangée) ---
                // Itérer sur toutes les cases DÉJÀ OCCUPÉES
                for (const auto& paire : plateau) {
                    const CoordHex& ancre = paire.first;
                    const auto& data = paire.second;

                    CoordHex pos1 = ancre + rel1;
                    CoordHex pos2 = ancre + rel2;

                    auto it1 = plateau.find(pos1);
                    auto it2 = plateau.find(pos2);

                    // Si les 3 cases (ancre, pos1, pos2) sont TOUTES OCCUPÉES
                    if (it1 != plateau.end() && it2 != plateau.end()) {

                        unsigned int h_ancre = data.second;
                        unsigned int h1 = it1->second.second;
                        unsigned int h2 = it2->second.second;
                        unsigned int nouvelleHauteur = max({h_ancre, h1, h2}) + 1;

                        // TODO: Ajouter la validation (ex: pas de Carrière sur Carrière)
                        // if (estUnCoupValide(ancre, pos1, pos2, ...))
                        coups.push_back({ancre, rotation_id, true, nouvelleHauteur});
                    }
                } // --- FIN RECHERCHE "EN HAUTEUR" ---

                tuileTest->rotationHoraire(); // Permute les hexagones pour le test suivant
            }
        }

        delete tuileTest; // Libérer le clone

        // TODO: Dé-dupliquer les coups (plusieurs rotations peuvent donner le même coup)
        return coups;
    }

    int Cite::placerTuile(TuileCite* tuile, const CoupPossible& coup) {
        // 1. Appliquer les rotations/permutations à la *vraie* tuile
        // (coup.rotation / 3) donne la forme (0 ou 1)
        // (coup.rotation % 3) donne le nb de permutations
        for (int i = 0; i < (coup.rotation % 3); ++i) {
            tuile->rotationHoraire();
        }
        
        // Définir les 3 positions
        int forme = coup.rotation / 3;
        CoordHex rel1 = (forme == 0) ? CoordHex(0, 1) : CoordHex(0, -1);
        CoordHex rel2 = (forme == 0) ? CoordHex(-1, 1) : CoordHex(1, -1); 

        CoordHex pos0 = coup.ancre;
        CoordHex pos1 = pos0 + rel1;
        CoordHex pos2 = pos0 + rel2;
        
        // Mettre à jour la tuile et l'inventaire
        tuile->setHauteur(coup.hauteur);
        tuiles_posees.push_back(tuile); // La Cité prend possession

        // --- LOGIQUE DE GAIN DE PIERRES PAR RECOUVREMENT ---
        int pierresGagnees = 0;

        // Vérifie si la tuile est posée en hauteur (recouvrement)
        if (coup.recouvrement) {
            // On vérifie les 3 cases couvertes
            array<CoordHex, 3> casesCouvertes = {pos0, pos1, pos2};
            
            for (const auto& caseCouvee : casesCouvertes) {
                // Regarde quel hexagone était présent à la coordonnée avant le placement
                auto it = plateau.find(caseCouvee);
                if (it != plateau.end()) {
                    HexagoneConstruction* hexCouvert = it->second.first;
                    
                    // Si l'hexagone couvert était une Carrière
                    if (dynamic_cast<Carriere*>(hexCouvert)) {
                        pierresGagnees++;
                    }
                }
            }
        }
        
        // Mettre à jour le plateau (la carte)
        plateau[pos0] = { tuile->getHexagone(0), coup.hauteur };
        plateau[pos1] = { tuile->getHexagone(1), coup.hauteur };
        plateau[pos2] = { tuile->getHexagone(2), coup.hauteur };

        // 5. Mettre à jour la frontière
        mettreAJourFrontiere(pos0);
        mettreAJourFrontiere(pos1);
        mettreAJourFrontiere(pos2);

        // On retourne le nombre de pierres gagnées
        return pierresGagnees;
    }



    void Cite::afficher(ostream& f) const {
        if (plateau.empty()) {
            f << "  (Cité vide)" << endl;
            return;
        }

        
        for (const auto& paire : plateau) {
            const CoordHex& coord = paire.first; 
            const auto& data = paire.second;     

            HexagoneConstruction* hex = data.first;
            unsigned int hauteur = data.second;

            f << "  (q=" << coord.getQ() << ", r=" << coord.getR()<< ") [H:" << hauteur << "] ";

            
            if (Place* p = dynamic_cast<Place*>(hex)) {
                f << "Place " << p->getNbEtoile() << "*";
            } else if (dynamic_cast<Carriere*>(hex)) {
                f << "Carrière";
            } else if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
                
                f << "Quartier " << q->getType().getCouleur(); 
            } else {
                f << "Inconnu";
            }
            f << endl;
        }
    }
    
}