#ifndef CITE_H
#define CITE_H


#include <iostream>
#include <vector>
#include <map>   
#include <set>    
#include <sstream> 
#include <algorithm>
#include "TuileCite_TuileDep.h"

#include "CoordHex.h" 


using namespace std;
namespace Akropolis{
    
    class Cite{
    
    private:
        // Inventaire pour le score et la gestion mémoire
        vector<TuileCite*> tuiles_posees; 
        
        // Plateau pour la logique de placement et l'affichage
        map<CoordHex, pair<HexagoneConstruction*, unsigned int>> plateau;
        
        // Cases vides adjacentes à nos tuiles
        set<CoordHex> frontiere; 
        
        

        
        void mettreAJourFrontiere(const CoordHex& pos); // Met à jour la frontière après un placement

        // Teste si les 3 hexagones d'un coup sont valides
        bool estUnCoupValide(const CoordHex& h0, const CoordHex& h1, const CoordHex& h2, 
                             bool& recouvrement, unsigned int& hauteur) const;

        
    
        static const int LARGEUR_TOIT = 5;
        static const int HAUTEUR_PENTE = 2;
        
        // Structure interne pour convertir les coordonnées axiales (q,r) en grille (col, lig)
        struct CoordGrille {
            int col;
            int lig;
        };

        // pour convertir q,r -> col,lig
        CoordGrille axialToOffset(int q, int r) const {
            int col = q;
            int lig = r + (q - (q & 1)) / 2;
            return {col, lig};
        }
        
        // pour convertir col,lig -> q,r (pour savoir quoi afficher dans une case vide)
        CoordHex offsetToAxial(int col, int lig) const {
            int q = col;
            int r = lig - (q - (q & 1)) / 2;
            return CoordHex(q, r);
        }

        // pour formater le texte à l'intérieur de l'hexagone
        string getEtiquetteHexagone(const CoordHex& pos) const;
    

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

        // Méthode spécifique pour l'IA
        void ajouterTuileIA(TuileCite* tuile);

        
        const vector<TuileCite*>& getTuiles() const { return tuiles_posees; }

        
        Cite() = default; 
        ~Cite(); 

        
        
        
        void initialiserCite(TuileDepart* tuileDepart);

        
        vector<CoupPossible> genererCoupsValides(const TuileCite& tuile) const;

        std::string validerPlacement(const TuileCite& tuile, const CoordHex& ancre, int rotation_id) const; // Appliquer le coup choisi
        
        int placerTuile(TuileCite* tuile, const CoupPossible& coup);

        

        size_t getNbTuilesPosees() const { return tuiles_posees.size(); }
        const map<CoordHex, std::pair<HexagoneConstruction*, unsigned int> >& getPlateau() const {
            return plateau;
        }

        void afficherGraphique(ostream& os) const;

        //methode pour forcer le palcement des tuiles sans les verifications
        void forcerPlacementHexagone(const CoordHex& coord, HexagoneConstruction* hex, unsigned int hauteur) {
            plateau[coord] = std::make_pair(hex, hauteur);
        }
    };

}



#endif