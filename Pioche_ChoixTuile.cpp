#include "Pioche_ChoixTuile.h"
#include "GameExcep_Enums.h"
#include <iostream>

namespace Akropolis {

    

    Pioche::~Pioche() {
        for (auto& pile : piles) {
            for (TuileCite* t : pile) delete t;
        }
        piles.clear();
    }

    void Pioche::organiserPiles(vector<TuileCite*>& toutesLesTuiles, int nbJoueurs) {
        // Détermination de la taille des piles selon les règles
        size_t taillePile = 3; // Par défaut 2 joueurs 
        if (nbJoueurs == 3) taillePile = 4; // 
        if (nbJoueurs == 4) taillePile = 5; // 

        
        size_t nbPiles = 11; 
       

        size_t indexCourant = 0;

        // Création des piles
        for (size_t i = 0; i < nbPiles; ++i) {
            vector<TuileCite*> nouvellePile;
            for (size_t j = 0; j < taillePile; ++j) {
                if (indexCourant < toutesLesTuiles.size()) {
                    nouvellePile.push_back(toutesLesTuiles[indexCourant]);
                    indexCourant++;
                }
            }
            if (!nouvellePile.empty()) {
                piles.push_back(nouvellePile);
            }
        }
        
        
        
        // On supprime du vecteur d'origine ce qu'on a mis dans les piles
        // pour ne garder que le "surplus" initial
        toutesLesTuiles.erase(toutesLesTuiles.begin(), toutesLesTuiles.begin() + indexCourant);
    }

    vector<TuileCite*> Pioche::prendreUnePile() {
        if (piles.empty()) {
            throw GameException("Plus de piles disponibles !");
        }
        // Prend la première pile disponible
        vector<TuileCite*> pile = piles.front();
        piles.pop_front();
        return pile;
    }


    // gestion du choix des tuiles

    ChoixTuile::~ChoixTuile() {
        for (TuileCite* t : tuilesDisponibles) delete t;
        tuilesDisponibles.clear();
    }

    void ChoixTuile::remettreTuile(TuileCite* tuile, size_t index) {
        if (index <= tuilesDisponibles.size()) {
            tuilesDisponibles.insert(tuilesDisponibles.begin() + index, tuile);
        } else {
            tuilesDisponibles.push_back(tuile);
        }
    }

    TuileCite* ChoixTuile::choisirTuile(Joueur* joueur, size_t index) {
        if (index >= tuilesDisponibles.size()) {
             throw GameException("Index invalide.");
        }

        size_t coutPierres = index;
        
        if (!joueur->peutPayerPierres(coutPierres)) {
            throw GameException("Pas assez de pierres (cout: " + std::to_string(coutPierres) + ").");
        }

        joueur->retirerPierres(coutPierres);
        TuileCite* tuile = tuilesDisponibles[index];
        tuilesDisponibles.erase(tuilesDisponibles.begin() + index);
        
        return tuile;
    }

    void ChoixTuile::ajouterPile(const vector<TuileCite*>& nouvellesTuiles) {
        // Ajoute les tuiles à la fin du marché
        tuilesDisponibles.insert(tuilesDisponibles.end(), nouvellesTuiles.begin(), nouvellesTuiles.end());
    }
}