#include "Pioche_ChoixTuile.h"

namespace Akropolis{
        const TuileCite& Pioche::piocher(){
        if (estVide()) throw GameException("Pioche Vide");
        int i = rand() % tuiles.size(); // tire un numéro de carte au hasard
        const TuileCite* tmp = tuiles[i];
        tuiles[i] = tuiles.back();
        tuiles.pop_back();
        return *tmp;
    }

    bool ChoixTuile::ajouterTuile(TuileCite* tuile) {
            if (tuilesDisponibles.size() < MAX_TUILES) {
                tuilesDisponibles.push_back(tuile);
                return true;
            }
            return false; // Déjà plein
        }
    
    bool ChoixTuile::retirerTuile(size_t tuileId) {
            auto it = std::find_if(tuilesDisponibles.begin(), tuilesDisponibles.end(),
                [tuileId](TuileCite* t) { return t->getId() == tuileId; });
            if (it != tuilesDisponibles.end()) {
                tuilesDisponibles.erase(it);
                return true;
            }
            return false; // Tuile non trouvée
        }

        // IMPLÉMENTATION DE LA LOGIQUE DE PAIEMENT DES PIERRES
    TuileCite* ChoixTuile::choisirTuile(Joueur* joueur, size_t index) {
        if (index >= tuilesDisponibles.size()) {
            throw GameException("Index de tuile invalide pour le choix.");
        }
    
        size_t coutPierres = calculerCout(index);
        TuileCite* tuileChoisie = tuilesDisponibles[index];

        if (!joueur->peutPayerPierres(coutPierres)) {
            // Utilisation de l'opérateur + et de std::to_string
            throw GameException("Le joueur " + joueur->getNom() + " n'a pas assez de pierres pour cette tuile (cout: " + std::to_string(coutPierres) + ").");
        }

        // 1. Paiement des pierres
        joueur->retirerPierres(coutPierres);

        // 2. Retrait de la tuile de la liste
        auto it = tuilesDisponibles.begin() + index;
        tuilesDisponibles.erase(it);

        // 3. Retourner la tuile choisie
        return tuileChoisie;
}


}