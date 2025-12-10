#include "Pioche_ChoixTuile.h"
#include "GameExcep_Enums.h"
#include <iostream>

namespace Akropolis {

    // --- PIOCHE ---

    Pioche::~Pioche() {
        for (auto& pile : piles) {
            for (TuileCite* t : pile) delete t;
        }
        piles.clear();
    }

    void Pioche::organiserPiles(vector<TuileCite*>& toutesLesTuiles, int nbJoueurs) {
        // Détermination de la taille des piles selon les règles (Page 2)
        size_t taillePile = 3; // Par défaut 2 joueurs 
        if (nbJoueurs == 3) taillePile = 4; // 
        if (nbJoueurs == 4) taillePile = 5; // 

        // Nombre de piles standard = 11 (Page 2 - Mise en place)
        size_t nbPiles = 11; 
        // Note: Pour une partie longue à 2 joueurs c'est 19, à 3 joueurs c'est 15.
        // Ici on reste sur le standard décrit[cite: 31].

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
        
        // Note : Les tuiles restantes (celles qui ne sont pas dans les 11 piles)
        // restent dans le vecteur 'toutesLesTuiles' (partie non copiée)
        // et seront utilisées pour l'initialisation du marché dans Partie::initialiserTuiles.
        
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


    // --- CHOIX TUILE (MARCHE) ---

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
        // Ajoute les tuiles à la FIN du marché (à droite)
        tuilesDisponibles.insert(tuilesDisponibles.end(), nouvellesTuiles.begin(), nouvellesTuiles.end());
    }
}