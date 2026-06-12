#ifndef PIOCHE_CHOIXTUILE_H
#define PIOCHE_CHOIXTUILE_H
#include <vector>
#include <list> 
#include "TuileCite_TuileDep.h"
#include "Joueur.h"
#include "GameExcep_Enums.h" 

using namespace std;
namespace Akropolis{
    
    class Pioche{
        size_t id;
        list<vector<TuileCite*>> piles; 
        
    public:
        Pioche(size_t id) : id(id) {}

        ~Pioche();

        
        size_t getNbPilesRestantes() const { return piles.size(); }
        bool estVide() const { return piles.empty(); }
        
        
        void organiserPiles(vector<TuileCite*>& toutesLesTuiles, int nbJoueurs);

        
        vector<TuileCite*> prendreUnePile();
        
        Pioche& operator=(const Pioche&) = delete;
        Pioche(const Pioche&) = delete;
    };

    class ChoixTuile {
    private:
        size_t id;
        vector<TuileCite*> tuilesDisponibles;
    
    public:
        ChoixTuile(size_t id) : id(id) {}
        
        ~ChoixTuile();
        void remettreTuile(TuileCite* tuile, size_t index);
        
        size_t calculerCout(size_t index) const {
            return index; 
        }

        TuileCite* choisirTuile(Joueur* joueur, size_t index);
        
        
        void ajouterPile(const vector<TuileCite*>& nouvellesTuiles);
        
        
        void ajouterTuile(TuileCite* t) { tuilesDisponibles.push_back(t); }

        const vector<TuileCite*>& getTuilesDisponibles() const {
            return tuilesDisponibles;
        }
        
        // Plus de MAX_TUILES, car la taille varie (4, 5 ou 6)
        size_t getNombreTuiles() const { return tuilesDisponibles.size(); }
        static size_t getMaxTuiles(int nbJoueurs) { return nbJoueurs + 2; } 
    };
}

#endif