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

    void ChoixTuile::remettreTuile(TuileCite* tuile, size_t index) {
        // On réinsère la tuile à son emplacement d'origine
        if (index <= tuilesDisponibles.size()) {
            tuilesDisponibles.insert(tuilesDisponibles.begin() + index, tuile);
        } else {
            // Sécurité (ne devrait pas arriver si la logique est bonne)
            tuilesDisponibles.push_back(tuile);
        }
    }

    TuileCite* ChoixTuile::choisirTuile(Joueur* joueur, size_t index) {
    if (index >= tuilesDisponibles.size()) {
         throw GameException("Index de tuile invalide dans ChoixTuile.");
    }

    // Le coût est égal à l'index (0 pour la première, 1 pour la deuxième, etc.)
    size_t coutPierres = index;
    TuileCite* tuile = tuilesDisponibles[index];

    // 1. VÉRIFICATION DU PAIEMENT
    if (!joueur->peutPayerPierres(coutPierres)) {
        // Retour à l'opérateur '+' comme demandé
        throw GameException("Le joueur " + joueur->getNom() + " n'a pas assez de pierres pour cette tuile (cout: " + std::to_string(coutPierres) + ").");
    }

    // 2. PAIEMENT DES PIERRES
    joueur->retirerPierres(coutPierres);
    
    // 3. RETRAIT DE LA TUILE
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