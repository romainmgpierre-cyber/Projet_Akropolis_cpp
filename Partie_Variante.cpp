#include "Partie_Variante.h"

namespace Akropolis{

       Partie::~Partie() {
        for (auto* joueur : joueurs) {
            delete joueur;
        }
        delete pioche;
        for (auto* tuile : tuilesDisponibles) {
            delete tuile;
        }
    }

    void Partie::ajouterJoueur(const string& nom) {
        if (etat != EtatPartie::NON_DEMARREE) {
            throw GameException("Impossible d'ajouter un joueur en cours de partie");
        }
        if (joueurs.size() >= 4) {
            throw GameException("Maximum 4 joueurs");
        }
        joueurs.push_back(new Joueur(nom));
    }

    void Partie::retirerJoueur(const string& nom) {
        if (etat != EtatPartie::NON_DEMARREE) {
            throw GameException("Impossible de retirer un joueur en cours de partie");
        }
        auto it = find_if(joueurs.begin(), joueurs.end(),
            [&nom](const Joueur* j) { return j->getNom() == nom; });
        if (it == joueurs.end()) {
            throw GameException("Joueur '" + nom + "' introuvable");
        }
        delete *it;  // Libérer la mémoire
        joueurs.erase(it);

    }

    void Partie::activerVariante(const string& nom) {
        if (etat != EtatPartie::NON_DEMARREE) {
            throw GameException("Impossible de modifier les variantes en cours de partie");
        }
        auto it = find_if(variantes.begin(), variantes.end(),
            [&nom](const Variante& v) { return v.getNom() == nom; });
    
        if (it == variantes.end()) {
            throw GameException("Variante '" + nom + "' introuvable");
        }
        it->activer();
    }


    void Partie::desactiverVariante(const string& nom) {
        auto it = find_if(variantes.begin(), variantes.end(),
            [&nom](const Variante& v) { return v.getNom() == nom; });
        if (it == variantes.end()) {
            throw GameException("Variante '" + nom + "' introuvable");
        }
        it->desactiver();
    }

    vector<Variante> Partie::getVariantesActives() const {
        vector<Variante> actives;
        for (const auto& v : variantes) {
            if (v.estActive()) {
                actives.push_back(v);
            }
        }
        return actives;
    }

    
} // namespace Akropolis