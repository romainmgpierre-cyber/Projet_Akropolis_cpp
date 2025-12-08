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



    // void Cite::afficher(ostream& f) const {
    //     if (plateau.empty()) {
    //         f << "  (Cité vide)" << endl;
    //         return;
    //     }

        
    //     for (const auto& paire : plateau) {
    //         const CoordHex& coord = paire.first; 
    //         const auto& data = paire.second;     

    //         HexagoneConstruction* hex = data.first;
    //         unsigned int hauteur = data.second;

    //         f << "  (q=" << coord.getQ() << ", r=" << coord.getR()<< ") [H:" << hauteur << "] ";

            
    //         if (Place* p = dynamic_cast<Place*>(hex)) {
    //             f << "Place " << p->getNbEtoile() << "*";
    //         } else if (dynamic_cast<Carriere*>(hex)) {
    //             f << "Carrière";
    //         } else if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
                
    //             f << "Quartier " << q->getType().getCouleur(); 
    //         } else {
    //             f << "Inconnu";
    //         }
    //         f << endl;
    //     }
    // }
    void Cite::ajouterTuileIA(TuileCite* tuile) {
    // L'IA considère les tuiles comme acquises mais ne les place pas sur le plateau
        tuiles_posees.push_back(tuile);
    
    }
    


// Ajout dans Cite.cpp à la fin ou à la place de l'ancien afficher

// Helper pour générer le texte (MAR, CAR, P TEM 2*, etc.)
string Cite::getEtiquetteHexagone(const CoordHex& pos) const {
    auto it = plateau.find(pos);
    
    // CAS 1 : CASE VIDE -> Coordonnées
    if (it == plateau.end()) {
        std::ostringstream ss;
        ss << pos.getQ() << "," << pos.getR();
        return ss.str();
    }

    // CAS 2 : CASE OCCUPÉE
    HexagoneConstruction* hex = it->second.first;
    string etiquette = "";

    if (dynamic_cast<Carriere*>(hex)) {
        return "CAR";
    } 
    else if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
        // On prend les 3 premières lettres du nom en majuscule (ex: MARche -> MAR)
        string nom = q->getType().getNom();
        string shortNom = nom.substr(0, 3);
        for (auto & c: shortNom) c = toupper(c);
        return shortNom;
    } 
    else if (Place* p = dynamic_cast<Place*>(hex)) {
        // Format : P TEM 2*
        string nom = p->getType().getNom();
        string shortNom = nom.substr(0, 3);
        for (auto & c: shortNom) c = toupper(c);
        return "P " + shortNom + " " + to_string(p->getNbEtoile()) + "*";
    }

    return "?";
}

// Dans Cite.cpp

void Cite::afficherGraphique(ostream& os) const {
    if (plateau.empty()) {
        os << "(Cite vide)" << endl;
        return;
    }

    // 1. DÉTERMINER LES BORNES
    int min_col = 1000, max_col = -1000;
    int min_lig = 1000, max_lig = -1000;

    // On regarde toutes les tuiles posées
    std::set<CoordHex> zonesAInclure;
    for(auto const& pair : plateau) zonesAInclure.insert(pair.first);
    for(auto const& c : frontiere) zonesAInclure.insert(c);

    for (const auto& pos : zonesAInclure) {
        CoordGrille cg = axialToOffset(pos.getQ(), pos.getR());
        if (cg.col < min_col) min_col = cg.col;
        if (cg.col > max_col) max_col = cg.col;
        if (cg.lig < min_lig) min_lig = cg.lig;
        if (cg.lig > max_lig) max_lig = cg.lig;
    }

    // --- MODIFICATION ICI : MARGE PLUS GRANDE ---
    // Mettez 3, 4 ou 5 selon la taille que vous voulez voir autour
    int marge = 1; 

    min_col -= marge; 
    max_col += marge;
    min_lig -= marge; 
    max_lig += marge;

    // 2. CONFIGURATION GÉOMÉTRIQUE
    int step_x = LARGEUR_TOIT + HAUTEUR_PENTE;
    int step_y = 2 * HAUTEUR_PENTE;
    int odd_offset = HAUTEUR_PENTE;

    int nb_cols_grille = max_col - min_col + 1;
    int nb_ligs_grille = max_lig - min_lig + 1;

    int width_px = (nb_cols_grille + 1) * step_x + LARGEUR_TOIT + 2;
    int height_px = (nb_ligs_grille + 1) * step_y + HAUTEUR_PENTE + 2;

    vector<string> buffer(height_px, string(width_px, ' '));

    // 3. DESSIN
    for (int lig = min_lig; lig <= max_lig; ++lig) {
        for (int col = min_col; col <= max_col; ++col) {
            
            CoordHex posReelle = offsetToAxial(col, lig);
        
            string label = getEtiquetteHexagone(posReelle);

            int draw_col = col - min_col;
            int draw_lig = lig - min_lig;

            int start_x = draw_col * step_x + 1;
            int start_y = draw_lig * step_y;
            if (col % 2 != 0) start_y += odd_offset;

            // TOIT
            for (int i = 0; i < LARGEUR_TOIT; ++i) 
                buffer[start_y][start_x + HAUTEUR_PENTE + i] = '_';

            // PENTES
            for (int k = 0; k < HAUTEUR_PENTE; ++k) {
                int y_top = start_y + 1 + k;
                buffer[y_top][start_x + HAUTEUR_PENTE - 1 - k] = '/';
                buffer[y_top][start_x + HAUTEUR_PENTE + LARGEUR_TOIT + k] = '\\';

                int y_bot = start_y + HAUTEUR_PENTE + 1 + k;
                buffer[y_bot][start_x + k] = '\\';
                buffer[y_bot][start_x + (2 * HAUTEUR_PENTE + LARGEUR_TOIT) - 1 - k] = '/';

                if (k == HAUTEUR_PENTE - 1) { 
                    for (int i = 0; i < LARGEUR_TOIT; ++i) 
                        buffer[y_bot][start_x + HAUTEUR_PENTE + i] = '_';
                }
            }

            // TEXTE
            int center_y = start_y + HAUTEUR_PENTE;
            int center_x = start_x + HAUTEUR_PENTE + (LARGEUR_TOIT / 2);
            int txt_start = center_x - (label.length() / 2);

            if (txt_start >= 0 && txt_start + label.length() < (size_t)width_px) {
                for (size_t i = 0; i < label.length(); ++i) {
                    buffer[center_y][txt_start + i] = label[i];
                }
            }
        }
    }

    // 4. AFFICHAGE
    os << "\n";
    for (const auto& line : buffer) {
        size_t end = line.find_last_not_of(" ");
        if (end != string::npos) {
            os << line.substr(0, end + 1) << "\n";
        }
    }
}

}