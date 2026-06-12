#ifndef TUILECITE_TUILEDEP_H
#define TUILECITE_TUILEDEP_H
#include "HexCons_Carr_Quart_Place.h"
#include "GameExcep_Enums.h"
using namespace std;
namespace Akropolis {

    class TuileCite{
        private : 
            size_t id;
            size_t id_persistant;
            array<HexagoneConstruction*, 3> hexagones;
             
            unsigned int hauteur=1;
            bool proprietaire; //true si possède des hexagones (pour la gestion de la mémoire)
            int rotation = 0; // 0, 1 ou 2, pour la sauvegarde
            CoordHex positionAncre;
            
        public :
            TuileCite(size_t id, HexagoneConstruction* h1,
              HexagoneConstruction* h2, HexagoneConstruction* h3,
              size_t id_per,
              bool possede = true);
            ~TuileCite();
            TuileCite(const TuileCite&)=delete;
            TuileCite& operator=(const TuileCite& )=delete;

            size_t getId() const { return id; }
            const array<HexagoneConstruction*, 3>& getHexagones() const { 
                return hexagones; 
            }
            HexagoneConstruction* getHexagone(size_t index) const {
                if (index >= 3) throw GameException("Index hexagone invalide");
                return hexagones[index];
            }

            void setHauteur(unsigned int h) { hauteur = h; }

            void rotationHoraire();
            void rotationAntihoraire();
            int getRotation() const { return rotation; }
            void setRotation(int r) { rotation = r % 3; }
            bool contientCarriere() const;
            bool contientPlace() const;
            int getNombreQuartiers() const;
            int getNombreCarrieres() const;
            unsigned int getHauteur() const { return hauteur; }
            size_t getIdPersistant() const { return id_persistant; }
            vector<Couleur> getCouleursQuartiers() const;
    
            void afficher(ostream& os = std::cout) const;
            friend ostream& operator<<(std::ostream& os, const TuileCite& tuile);
    
            TuileCite* clone() const;
            CoordHex getAncre() const { return positionAncre; }
            void setAncre(const CoordHex& c) { positionAncre = c; }
    };

   class TuileDepart {
    private:
        size_t id;
        std::array<HexagoneConstruction*, 4> hexagones; // 0 = centre (Place), 1-3 = Carrières
    public:
        
        TuileDepart(size_t id, Place* centre, Carriere* c1, Carriere* c2, Carriere* c3);

        ~TuileDepart() {
            
            for (auto h : hexagones) delete h;
        }

        size_t getId() const { return id; }
        const std::array<HexagoneConstruction*,4>& getHexagones() const { return hexagones; }
        HexagoneConstruction* getHexagone(size_t index) const { 
            if (index >= 4) throw GameException("Index hexagone invalide");
            return hexagones[index];
        }
   };

}

#endif