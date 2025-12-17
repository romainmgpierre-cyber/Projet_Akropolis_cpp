#include "TuileCite_TuileDep.h"

namespace Akropolis{

    TuileCite::TuileCite(size_t id, HexagoneConstruction* h1, 
                         HexagoneConstruction* h2, HexagoneConstruction* h3,
                         bool possede) 
        : id(id), hexagones({{h1, h2, h3}}), hauteur(1), proprietaire(possede) {}

    TuileCite::~TuileCite() {
        if (proprietaire) {
            for (auto h : hexagones) delete h;
        }
    }

    void TuileCite::rotationHoraire() {
        // permutation cyclique : [0, 1, 2] -> [2, 0, 1]
        HexagoneConstruction* temp = hexagones[2];
        hexagones[2] = hexagones[1];
        hexagones[1] = hexagones[0];
        hexagones[0] = temp;
    }

    void TuileCite::rotationAntihoraire() {
        // permutation cyclique inverse : [0, 1, 2] -> [1, 2, 0]
        HexagoneConstruction* temp = hexagones[0];
        hexagones[0] = hexagones[1];
        hexagones[1] = hexagones[2];
        hexagones[2] = temp;
    }
    
    TuileCite* TuileCite::clone() const {

        HexagoneConstruction* h1_clone = hexagones[0]->clone();
        HexagoneConstruction* h2_clone = hexagones[1]->clone();
        HexagoneConstruction* h3_clone = hexagones[2]->clone();
        
        // crée une nouvelle tuile
        TuileCite* newTuile = new TuileCite(id, h1_clone, h2_clone, h3_clone, true);
        newTuile->setHauteur(this->hauteur);
        return newTuile;
    }

    bool TuileCite::contientCarriere() const {
        for (const auto* hex : hexagones) {
            if (dynamic_cast<const Carriere*>(hex) != nullptr) {
                return true;
            }
        }
        return false;
    }

    bool TuileCite::contientPlace() const {
        for (const auto* hex : hexagones) {
            if (dynamic_cast<const Place*>(hex) != nullptr)
                return true;
        }
        return false;
    }

    int TuileCite::getNombreQuartiers() const {
        int count = 0;
        for (const auto* hex : hexagones) {
            if (dynamic_cast<const Quartier*>(hex) != nullptr)
                count++;
        }
        return count;
    }

    int TuileCite::getNombreCarrieres() const {
        int count = 0;
        for (const auto* hex : hexagones) {
            if (dynamic_cast<const Carriere*>(hex) != nullptr)
                count++;
        }
        return count;
    }

    vector<Couleur> TuileCite::getCouleursQuartiers() const {
        vector<Couleur> couleurs;
        for (auto* hex : hexagones) {
            if (auto* q = dynamic_cast<Quartier*>(hex)) {
                couleurs.push_back(q->getType().getCouleur());
            }
        }
        return couleurs;
    }
    
    void TuileCite::afficher(ostream& f) const {
        f << "Tuile #" << id << " (hauteur: " << hauteur << ")\n";
        for (size_t i = 0; i < 3; ++i) {
            f << "  Hexagone " << i << ": ";
            if (auto* q = dynamic_cast<Quartier*>(hexagones[i])) {
                f << "Quartier " << q->getType().getCouleur();
            } else if (auto* p = dynamic_cast<Place*>(hexagones[i])) {
                f << "Place " << p->getType().getCouleur() << " (" << p->getNbEtoile() << "★)";
            } else if (dynamic_cast<Carriere*>(hexagones[i])) {
                f << "Carrière";
            }
            f << "\n";
        }
    }
    ostream& operator<<(ostream& f, const TuileCite& tuile) {
        tuile.afficher(f);
        return f;
    }


    TuileDepart::TuileDepart(size_t id, Place* centre, Carriere* c1, Carriere* c2, Carriere* c3)
            : id(id) 
        {
            hexagones[0] = centre; 
            hexagones[1] = c1;
            hexagones[2] = c2;
            hexagones[3] = c3;

        }
}