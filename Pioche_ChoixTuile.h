#ifndef PIOCHE_CHOIXTUILE_H
#define PIOCHE_CHOIXTUILE_H
#include <vector>
#include "TuileCite_TuileDep.h"
#include "Joueur.h"
#include "GameExcep_Enums.h" 

using namespace std;
namespace Akropolis{
    
    class Pioche{
        size_t id;
        vector<TuileCite*> tuiles;
        
    public:
        Pioche(size_t id, size_t taillepioche) : id(id) {
            tuiles.reserve(taillepioche);
        }

        ~Pioche();

        size_t getNbtuilesPioche() const { return tuiles.size(); }
        bool estVide() const { return tuiles.empty(); }
        
        // --- CORRECTION : Ajout de cette méthode pour l'initialisation ---
        void ajouterTuile(TuileCite* t) { tuiles.push_back(t); }

        TuileCite* piocher();
        
        Pioche& operator=(const Pioche&) = delete;
        Pioche(const Pioche&) = delete;
    };

    class ChoixTuile {
    private:
        static const size_t MAX_TUILES = 4; // Ou autre valeur selon règles
        size_t id;
        vector<TuileCite*> tuilesDisponibles;
    
    public:
        ChoixTuile(size_t id) : id(id) {
            tuilesDisponibles.reserve(MAX_TUILES);
        }
        
        ~ChoixTuile();

        
        size_t calculerCout(size_t index) const {
            // Exemple : coût = index (0 pour la 1ère, 1 pour la 2ème...)
            return index; 
        }

        TuileCite* choisirTuile(Joueur* joueur,size_t index);
        bool ajouterTuile(TuileCite* tuile);  

        const vector<TuileCite*>& getTuilesDisponibles() const {
            return tuilesDisponibles;
        }
        static size_t getMaxTuiles() {return MAX_TUILES;}
        size_t getNombreTuiles() const { return tuilesDisponibles.size(); }
    };
}

#endif