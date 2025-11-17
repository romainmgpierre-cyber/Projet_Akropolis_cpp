#include "Pioche_ChoixTuile.h"
#include "GameExcep_Enums.h"

namespace Akropolis {



    Pioche::~Pioche() {
        for (TuileCite* t : tuiles) delete t;
        tuiles.clear();
    }

    TuileCite* Pioche::piocher() {
        if (estVide()) {
            throw GameException("La pioche est vide !");
        }
        TuileCite* tuile = tuiles.back();
        tuiles.pop_back();
        return tuile; 
    }
    


    ChoixTuile::~ChoixTuile() {
        for (TuileCite* t : tuilesDisponibles) delete t;
        tuilesDisponibles.clear();
    }

    TuileCite* ChoixTuile::choisirTuile(size_t index) {
        if (index >= tuilesDisponibles.size()) {
             throw GameException("Index de tuile invalide dans ChoixTuile.");
        }
        
        TuileCite* tuile = tuilesDisponibles[index];
        
        // Retirer la tuile du vecteur (décalage des éléments suivants)
        tuilesDisponibles.erase(tuilesDisponibles.begin() + index);
        
        return tuile;
    }

    bool ChoixTuile::ajouterTuile(TuileCite* tuile) {
        if (tuilesDisponibles.size() < MAX_TUILES) {
            tuilesDisponibles.push_back(tuile);
            return true;
        }
        return false;
    }
}