#ifndef PIOCHE_CHOIXTUILE_H
#define PIOCHE_CHOIXTUILE_H
#include <vector>
#include "TuileCite_TuileDep.h"
#include "Joueur.h"
using namespace std;
namespace Akropolis{
    class Pioche{
        size_t id;
        vector<TuileCite*> tuiles;
        
    public:
        Pioche(size_t id, size_t taillepioche) : id(id) {
            tuiles.reserve(taillepioche);
        }
        
        size_t getNbtuilesPioche() const { return tuiles.size(); }
        bool estVide() const { return tuiles.empty(); }
        const TuileCite& piocher();
        
        ~Pioche() = default;
        Pioche& operator=(const Pioche&) = delete;
        Pioche(const Pioche&) = delete;
    };

    class ChoixTuile {
    private:
        static const size_t MAX_TUILES = 4;
        size_t id;
        vector<TuileCite*> tuilesDisponibles;
    
    public:
        ChoixTuile(size_t id) : id(id) {
            tuilesDisponibles.reserve(MAX_TUILES);
        }
        
        // --- AJOUTS ICI ---
        size_t calculerCout(size_t index) const {
            return index;
        }

        TuileCite* choisirTuile(Joueur* joueur, size_t index);
        // ------------------
        
        bool ajouterTuile(TuileCite* tuile);
        bool retirerTuile(size_t tuileId);
        const vector<TuileCite*>& getTuilesDisponibles() const {
            return tuilesDisponibles;
        }
        
        size_t getNombreTuiles() const { return tuilesDisponibles.size(); }
};
}

#endif