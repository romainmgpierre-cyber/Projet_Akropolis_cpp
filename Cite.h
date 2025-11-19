#ifndef CITE_H
#define CITE_H


#include <iostream>
#include <vector>
#include <map>   
#include <set>    
#include "TuileCite_TuileDep.h"
#include "CoordHex.h" 


using namespace std;
namespace Akropolis{
    
    class Cite{
    public:

        class CoupPossible {
        public:
            CoordHex ancre;         // Coordonnée de l'hexagone "ancre"
            int rotation;           // L'index de rotation (0-5)
            bool recouvrement;      // Est-ce un coup en hauteur ?
            unsigned int hauteur;   // Hauteur finale de la tuile

            CoupPossible() = default;
            CoupPossible(const CoordHex& a, int r, bool rec, unsigned int h)
                : ancre(a), rotation(r), recouvrement(rec), hauteur(h) {}
        };

    private:
        // "Inventaire" pour le score et la gestion mémoire
        vector<TuileCite*> tuiles_posees; 
        
        // "Plateau" pour la logique de placement et l'affichage
        map<CoordHex, pair<HexagoneConstruction*, unsigned int>> plateau;
        
        // Cases vides adjacentes à nos tuiles (pour coups au sol)
        set<CoordHex> frontiere; 

    public:
        
        Cite() = default; 
        ~Cite(); 

        
        
        // Place la tuile de départ au centre
        void initialiserCite(TuileDepart* tuileDepart);

        // trouver tous les coups légaux
        vector<CoupPossible> genererCoupsValides(const TuileCite& tuile) const;

        // Appliquer le coup choisi
        void placerTuile(TuileCite* tuile, const CoupPossible& coup);


        void afficher(ostream& f) const; // Modifié

        size_t getNbTuilesPosees() const { return tuiles_posees.size(); }
        const map<CoordHex, std::pair<HexagoneConstruction*, unsigned int> >& getPlateau() const {
            return plateau;
        }

    private:

        
        // Met à jour la frontière après un placement
        void mettreAJourFrontiere(const CoordHex& pos);

        // Teste si les 3 hexagones d'un coup sont valides
        bool estUnCoupValide(const CoordHex& h0, const CoordHex& h1, const CoordHex& h2, 
                             bool& recouvrement, unsigned int& hauteur) const;
    };

}



#endif