#include "TuileCite_TuileDep.h"

namespace Akropolis{

    TuileCite::TuileCite(size_t id, HexagoneConstruction* h1,
            HexagoneConstruction* h2, HexagoneConstruction* h3,
            bool possede)
        : id(id), hexagones{h1, h2, h3}, hauteur(1), proprietaire(possede) {
                if (!h1 || !h2 || !h3)
                    throw GameException("HexagoneConstruction manquant dans TuileCite.");
            }

    TuileCite::~TuileCite() {
        if (proprietaire) {
            for (auto* hex : hexagones) {
                delete hex;
            }
        }
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
    TuileCite* TuileCite::clone() const {
        return new TuileCite(id,
                         hexagones[0], 
                         hexagones[1], 
                         hexagones[2],
                         true); // la tuile clonée possède ses hexagones
    }

    TuileDepart::TuileDepart(size_t id, Place* centre, Carriere* c1, Carriere* c2, Carriere* c3)
            : id(id) 
        {
            centre->SetPosition(CoordHex(0,0)); // position centrale
            c1->SetPosition(CoordHex(1,0)); // positions autour
            c2->SetPosition(CoordHex(0,-1));
            c3->SetPosition(CoordHex(-1,1));
            hexagones[0] = centre; // centre = Place
            hexagones[1] = c1;
            hexagones[2] = c2;
            hexagones[3] = c3;

        }
}