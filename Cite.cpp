#include "Cite.h"
#include "HexCons_Carr_Quart_Place.h" 
#include <iostream>
#include <algorithm>
#include <set>
#include <sstream>

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
    set<tuple<int, int, int, int>> coupsUniques; // q, r, rotation, hauteur
    TuileCite* tuileTest = tuile.clone();

    // IMPORTANT : Ces positions doivent correspondre à celles dans HexGridWidget
    // HexGridWidget utilise : (0,0), (-1,0), (0,-1)
    // Donc les positions relatives sont : rel1 = (-1, 0), rel2 = (0, -1)

    // Boucle sur les 2 formes géométriques
    for (int forme = 0; forme < 2; forme++) {
        // Forme 0 : Configuration de base (L vers bas-gauche)
        // Forme 1 : Configuration miroir (L vers haut-droite)
        CoordHex rel1, rel2;

        if (forme == 0) {
            rel1 = CoordHex(-1, 0);   // Hex 1 : gauche
            rel2 = CoordHex(0, -1);   // Hex 2 : bas
        } else {
            rel1 = CoordHex(1, 0);    // Hex 1 : droite
            rel2 = CoordHex(0, 1);    // Hex 2 : haut
        }

        // Boucle sur les 3 permutations de contenu
        for (int perm = 0; perm < 3; perm++) {
            int rotation_id = (forme * 3) + perm;

            // PLACEMENT AU SOL (Hauteur 1)

            // On teste TOUTES les cases de la frontière comme ancre potentielle
            for (const CoordHex& ancre : frontiere) {
                CoordHex pos0 = ancre;
                CoordHex pos1 = ancre - rel1;
                CoordHex pos2 = ancre - rel2;
                std::array<CoordHex, 3> candidats = {pos0, pos1, pos2};
                for(const CoordHex& ancrePotentielle : candidats) {
                    CoordHex pos1 = ancrePotentielle + rel1;
                    CoordHex pos2 = ancrePotentielle + rel2;

                    // Vérification unicité avant calculs lourds
                    auto signature = make_tuple(ancrePotentielle.getQ(), ancrePotentielle.getR(), rotation_id, 1);
                    if (coupsUniques.count(signature)) continue;

                    // RÈGLE 1 : Les 3 cases doivent être VIDES
                    if (plateau.count(ancrePotentielle) || plateau.count(pos1) || plateau.count(pos2)) continue;

                    // RÈGLE 2 : Adjacence (au moins un hexagone touche la cité)
                    bool toucheCite = false;
                    std::array<CoordHex, 3> positionsTuile = {ancrePotentielle, pos1, pos2};
                    for (const CoordHex& pos : positionsTuile) {
                        for (int dir = 0; dir < 6; dir++) {
                            if (plateau.count(pos.voisin(dir))) {
                                toucheCite = true;
                                break;
                            }
                        }
                        if (toucheCite) break;
                    }

                    if (toucheCite) {
                        coups.push_back({ancrePotentielle, rotation_id, false, 1});
                        coupsUniques.insert(signature);
                    }
                }
            }

            
            
            
            // placement en hauteur
            for (const auto& paire : plateau) {
                const CoordHex& ancre = paire.first;
                CoordHex pos1 = ancre + rel1;
                CoordHex pos2 = ancre + rel2;

                auto it1 = plateau.find(pos1);
                auto it2 = plateau.find(pos2);

                //Les 3 cases doivent être occupés
                if (it1 == plateau.end() || it2 == plateau.end()) {
                    continue; // Pas de soutien complet
                }

                // Récupération des hexagones du support
                HexagoneConstruction* support0 = paire.second.first;
                HexagoneConstruction* support1 = it1->second.first;
                HexagoneConstruction* support2 = it2->second.first;

                // Récupération des hexagones de la nouvelle tuile
                HexagoneConstruction* nouveau0 = tuileTest->getHexagone(0);
                HexagoneConstruction* nouveau1 = tuileTest->getHexagone(1);
                HexagoneConstruction* nouveau2 = tuileTest->getHexagone(2);

                
                // Une Carrière ne peut pas recouvrir une Carrière
                bool violationCarriere = false;

                if (dynamic_cast<Carriere*>(nouveau0) && dynamic_cast<Carriere*>(support0))
                    violationCarriere = true;
                if (dynamic_cast<Carriere*>(nouveau1) && dynamic_cast<Carriere*>(support1))
                    violationCarriere = true;
                if (dynamic_cast<Carriere*>(nouveau2) && dynamic_cast<Carriere*>(support2))
                    violationCarriere = true;

                
                // La tuile posée doit être "à cheval" sur au moins deux tuiles différentes (ou hexagones de départ)
                std::set<const void*> ids_support_uniques;
                std::array<HexagoneConstruction*, 3> supports_hex = {support0, support1, support2};

                for (HexagoneConstruction* support_hex : supports_hex) {
                    bool trouve_tuile_cite = false;
                    for (TuileCite* tuile : tuiles_posees) {
                        for (int i = 0; i < 3; ++i) {
                            if (tuile->getHexagone(i) == support_hex) {
                                ids_support_uniques.insert(tuile); // Utilise TuileCite* comme ID
                                trouve_tuile_cite = true;
                                break;
                            }
                        }
                        if (trouve_tuile_cite) break;
                    }

                    if (!trouve_tuile_cite) {
                        // Hexagone de la TuileDepart. On utilise le pointeur de l'HexagoneConstruction* comme ID unique.
                        ids_support_uniques.insert(support_hex);
                    }
                }
                bool violationSoutien = (ids_support_uniques.size() < 2);

                
                if (!violationCarriere && !violationSoutien) {
                    unsigned int h0 = paire.second.second;
                    unsigned int h1 = it1->second.second;
                    unsigned int h2 = it2->second.second;
                    unsigned int nouvelleHauteur = max({h0, h1, h2}) + 1;

                    coups.push_back({ancre, rotation_id, true, nouvelleHauteur});
                }
            }

            tuileTest->rotationHoraire(); // Permutation pour le test suivant
        }
    }
    delete tuileTest;
    return coups;
}

std::string Cite::validerPlacement(const TuileCite& tuile, const CoordHex& ancre, int rotation_id) const {
    // Déterminer les positions relatives selon la rotation
    int forme = rotation_id / 3;
    CoordHex rel1, rel2;

    if (forme == 0) {
        rel1 = CoordHex(-1, 0);
        rel2 = CoordHex(0, -1);
    } else {
        rel1 = CoordHex(1, 0);
        rel2 = CoordHex(0, 1);
    }

    CoordHex pos1 = ancre + rel1;
    CoordHex pos2 = ancre + rel2;

    // Vérifier si c'est un placement au sol ou en hauteur
    bool ancreOccupee = (plateau.find(ancre) != plateau.end());
    bool pos1Occupee = (plateau.find(pos1) != plateau.end());
    bool pos2Occupee = (plateau.find(pos2) != plateau.end());

    int nbCasesOccupees = (ancreOccupee ? 1 : 0) + (pos1Occupee ? 1 : 0) + (pos2Occupee ? 1 : 0);

    if (nbCasesOccupees == 0) {
        // placement au sol

        // Vérifier adjacence à la cité
        bool toucheCite = false;
        std::array<CoordHex, 3> positions = {ancre, pos1, pos2};

        for (const CoordHex& pos : positions) {
            for (int dir = 0; dir < 6; dir++) {
                if (plateau.find(pos.voisin(dir)) != plateau.end()) {
                    toucheCite = true;
                    break;
                }
            }
            if (toucheCite) break;
        }

        if (!toucheCite) {
            return "PLACEMENT AU SOL INVALIDE : La tuile doit être adjacente à la cité existante.";
        }

        return "Placement au sol valide";

    } else if (nbCasesOccupees == 3) {
        // placement en hauteur

        // Vérifier règle de la carrière
        TuileCite* tuileTest = tuile.clone();
        for (int i = 0; i < (rotation_id % 3); i++) {
            tuileTest->rotationHoraire();
        }

        auto paire0 = plateau.find(ancre);
        auto paire1 = plateau.find(pos1);
        auto paire2 = plateau.find(pos2);

        HexagoneConstruction* support0 = paire0->second.first;
        HexagoneConstruction* support1 = paire1->second.first;
        HexagoneConstruction* support2 = paire2->second.first;

        HexagoneConstruction* nouveau0 = tuileTest->getHexagone(0);
        HexagoneConstruction* nouveau1 = tuileTest->getHexagone(1);
        HexagoneConstruction* nouveau2 = tuileTest->getHexagone(2);

        // Interdiction Carrière sur Carrière
        if (dynamic_cast<Carriere*>(nouveau0) && dynamic_cast<Carriere*>(support0)) {
            delete tuileTest;
            return "❌ PLACEMENT EN HAUTEUR INVALIDE : Carrière sur Carrière interdit (position 0).";
        }
        if (dynamic_cast<Carriere*>(nouveau1) && dynamic_cast<Carriere*>(support1)) {
            delete tuileTest;
            return "❌ PLACEMENT EN HAUTEUR INVALIDE : Carrière sur Carrière interdit (position 1).";
        }
        if (dynamic_cast<Carriere*>(nouveau2) && dynamic_cast<Carriere*>(support2)) {
            delete tuileTest;
            return "❌ PLACEMENT EN HAUTEUR INVALIDE : Carrière sur Carrière interdit (position 2).";
        }

        
        // La tuile posée doit être "à cheval" sur au moins deux tuiles différentes (ou hexagones de départ)
        std::set<const void*> ids_support_uniques;
        std::array<HexagoneConstruction*, 3> supports_hex = {support0, support1, support2};

        for (HexagoneConstruction* support_hex : supports_hex) {
            bool trouve_tuile_cite = false;
            for (TuileCite* tuile : tuiles_posees) {
                for (int i = 0; i < 3; ++i) {
                    if (tuile->getHexagone(i) == support_hex) {
                        ids_support_uniques.insert(tuile);
                        trouve_tuile_cite = true;
                        break;
                    }
                }
                if (trouve_tuile_cite) break;
            }

            if (!trouve_tuile_cite) {
                // Hexagone de la TuileDepart. On utilise le pointeur de l'HexagoneConstruction* comme ID unique.
                ids_support_uniques.insert(support_hex);
            }
        }

        if (ids_support_uniques.size() < 2) {
            delete tuileTest;
            return "PLACEMENT EN HAUTEUR INVALIDE : La tuile doit être supportée par au moins deux entités différentes (tuiles posées ou hexagones de départ).";
        }

        delete tuileTest;
        return "Placement en hauteur valide";

    } else {
        return "PLACEMENT INVALIDE : Soutien incomplet (" + std::to_string(nbCasesOccupees) + "/3 cases occupées).";
    }
}

int Cite::placerTuile(TuileCite* tuile, const CoupPossible& coup) {
    // Appliquer les rotations/permutations à la vraie tuile
    // (coup.rotation / 3) donne la forme (0 ou 1)
    // (coup.rotation % 3) donne le nb de permutations
    for (int i = 0; i < (coup.rotation % 3); ++i) {
        tuile->rotationHoraire();
    }

    // Définir les 3 positions
    int forme = coup.rotation / 3;
    
    CoordHex rel1, rel2;

    if (forme == 0) {
        rel1 = CoordHex(-1, 0);   
        rel2 = CoordHex(0, -1);   
    } else {
        rel1 = CoordHex(1, 0);    
        rel2 = CoordHex(0, 1);   
    }

    CoordHex pos0 = coup.ancre;
    CoordHex pos1 = pos0 + rel1;
    CoordHex pos2 = pos0 + rel2;

    // Mettre à jour la tuile et l'inventaire
    tuile->setHauteur(coup.hauteur);
    tuiles_posees.push_back(tuile); // La Cité prend possession

    // gain de pierres par recouvrement
    int pierresGagnees = 0;

    // Vérifie si la tuile est posée en hauteur
    if (coup.recouvrement) {
        cout << "Placement en recouvrement détecté." << endl;

        array<CoordHex, 3> casesCouvertes = {pos0, pos1, pos2};

        for (const auto& caseCouvee : casesCouvertes) {
            auto it = plateau.find(caseCouvee);
            if (it != plateau.end()) {
                HexagoneConstruction* hexCouvert = it->second.first;

                if (dynamic_cast<Carriere*>(hexCouvert)) {
                    cout << " Une carrière a été recouverte ! (+1 pierre)" << endl;
                    pierresGagnees++;
                }
            }
        }
    }

    // Mettre à jour le plateau
    plateau[pos0] = { tuile->getHexagone(0), coup.hauteur };
    plateau[pos1] = { tuile->getHexagone(1), coup.hauteur };
    plateau[pos2] = { tuile->getHexagone(2), coup.hauteur };

    //Mettre à jour la frontière
    mettreAJourFrontiere(pos0);
    mettreAJourFrontiere(pos1);
    mettreAJourFrontiere(pos2);

    
    return pierresGagnees;
}

void Cite::ajouterTuileIA(TuileCite* tuile) {
    // L'IA considère les tuiles comme acquises mais ne les place pas sur le plateau
    tuiles_posees.push_back(tuile);

}





// pour générer le texte (MAR, CAR, P TEM 2*, etc.)
string Cite::getEtiquetteHexagone(const CoordHex& pos) const {
    auto it = plateau.find(pos);

    // case vide -> Coordonnées
    if (it == plateau.end()) {
        std::ostringstream ss;
        ss << pos.getQ() << "," << pos.getR();
        return ss.str();
    }

    // case occupée
    HexagoneConstruction* hex = it->second.first;
    string etiquette = "";

    if (dynamic_cast<Carriere*>(hex)) {
        return "CAR";
    }
    else if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
        // On prend les 3 premières lettres du nom en majuscule
        string nom = q->getType().getNom();
        string shortNom = nom.substr(0, 3);
        for (auto & c: shortNom) c = toupper(c);
        return shortNom;
    }
    else if (Place* p = dynamic_cast<Place*>(hex)) {
        
        string nom = p->getType().getNom();
        string shortNom = nom.substr(0, 3);
        for (auto & c: shortNom) c = toupper(c);
        return "P " + shortNom + " " + to_string(p->getNbEtoile()) + "*";
    }

    return "?";
}



void Cite::afficherGraphique(ostream& os) const {
    if (plateau.empty()) {
        os << "(Cite vide)" << endl;
        return;
    }

    // pour déterminer les bornes
    int min_col = 1000, max_col = -1000;
    int min_lig = 1000, max_lig = -1000;

    
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



    int marge = 1;

    min_col -= marge;
    max_col += marge;
    min_lig -= marge;
    max_lig += marge;

    // pour l'affichage de la grille en ASCII
    int step_x = LARGEUR_TOIT + HAUTEUR_PENTE;
    int step_y = 2 * HAUTEUR_PENTE;
    int odd_offset = HAUTEUR_PENTE;

    int nb_cols_grille = max_col - min_col + 1;
    int nb_ligs_grille = max_lig - min_lig + 1;

    int width_px = (nb_cols_grille + 1) * step_x + LARGEUR_TOIT + 2;
    int height_px = (nb_ligs_grille + 1) * step_y + HAUTEUR_PENTE + 2;

    vector<string> buffer(height_px, string(width_px, ' '));

    
    for (int lig = min_lig; lig <= max_lig; ++lig) {
        for (int col = min_col; col <= max_col; ++col) {

            CoordHex posReelle = offsetToAxial(col, lig);

            string label = getEtiquetteHexagone(posReelle);

            int draw_col = col - min_col;
            int draw_lig = lig - min_lig;

            int start_x = draw_col * step_x + 1;
            int start_y = draw_lig * step_y;
            if (col % 2 != 0) start_y += odd_offset;

            // toit
            for (int i = 0; i < LARGEUR_TOIT; ++i)
                buffer[start_y][start_x + HAUTEUR_PENTE + i] = '_';

            // cotés
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

            //pour écrire à l'intérieur de l'hexagone
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

    
    os << "\n";
    for (const auto& line : buffer) {
        size_t end = line.find_last_not_of(" ");
        if (end != string::npos) {
            os << line.substr(0, end + 1) << "\n";
        }
    }
}

}
