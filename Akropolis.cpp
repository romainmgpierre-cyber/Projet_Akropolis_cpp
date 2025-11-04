#include "Akropolis.h"
using namespace std;
namespace Akropolis{ 

    initializer_list<Couleur> Couleurs = { Couleur::rouge, Couleur::violet, Couleur::vert, Couleur::bleu, Couleur::gris };
	initializer_list<Etoile> Etoiles = { Etoile::un, Etoile::deux, Etoile::trois };


    string toString(Couleur c) {
		switch (c) { 
		case Couleur::rouge: return "Rouge";
		case Couleur::bleu: return "BLeu";
		case Couleur::vert: return "Vert";
        case Couleur::violet: return "Violet";
        case Couleur::gris: return "Gris"; //couleur par défaut pour les carrières
		default: throw GameException("Couleur inconnue");
		}
	}

	string toString(Etoile e){
        switch(e){
            case Etoile::un: return "*";
            case Etoile::deux: return "2*";
            case Etoile::trois: return "3*";
        }
    }

    ostream& operator<<(ostream& f, Couleur c) { f << toString(c); return f; }
	ostream& operator<<(ostream& f, Etoile e) {	f << toString(e); return f; }
    
    void printCouleurs(ostream& f) {
        for (auto c : Couleurs) f << c << " ";
        f << "\n";
    }
    void printNbEtoiles(ostream& f) {
        for (auto e : Etoiles) f << e << " ";
        f << "\n";
    }

    void Cite::ajouter(const TuileCite &t) {
        if (nb == nb_max) {
            size_t new_max = (nb_max + 1) * 2; // appels logarithmique

            // création d'un tableau plus grand si le tabelau est déjà remplis
            auto newtab = new const TuileCite * [new_max];

            // recopie des adresses des Tuiles citées dans le nouveau tableau
            for (size_t i = 0; i < nb; i++)
                newtab[i] = tuile_cites[i];

            // on met a jour la taille max
            nb_max = new_max;
            
            // desalocation de l'ancien tableau
            auto old = tuile_cites;
            tuile_cites = newtab;
            delete[] old;
        }
        tuile_cites[nb++] = &t;
    }
	
    Partie::Partie(size_t id, ModeJeu mode)
        : id(id), mode(mode), etat(EtatPartie::NON_DEMARREE),
        joueurActuelIndex(0),pioche(nullptr), nbTuilesParTour(3), {
    
    }

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
    // --- CalculScoreRecouvrement ---
int CalculScoreRecouvrement::calculerScore(const Joueur& joueur) const {
    // TODO : Calculer les points liés aux tuiles recouvertes pour ce joueur
    return 0;
}

// --- CalculScorePlaces ---
int CalculScorePlaces::calculerScore(const Joueur& joueur) const {
    // TODO : Calculer les points des places selon les règles du jeu
    return 0;
}

// --- CalculScoreMultiplicateurs ---
int CalculScoreMultiplicateurs::calculerScore(const Joueur& joueur) const {
    // TODO : Calculer les multiplicateurs selon les quartiers ou autres critères
    return 0;
}

// --- TableauScore ---
void TableauScore::ajouterJoueur(Joueur* j) {
    scores.push_back({j, 0});
}

void TableauScore::calculerScores() {
    // On parcourt tous les joueurs et on calcule le score total
    for(auto& pair : scores) {
        Joueur* j = pair.first;

        int scoreRecouvrement    = CalculScoreRecouvrement::calculerScore(*j);
        int scorePlaces          = CalculScorePlaces::calculerScore(*j);
        int scoreMultiplicateur  = CalculScoreMultiplicateurs::calculerScore(*j);

        pair.second = scoreRecouvrement + scorePlaces + scoreMultiplicateur;
    }
}

void TableauScore::afficherScores(ostream& f) const {
    f << "=== Scores ===\n";
    for(const auto& pair : scores) {
        // TODO : remplacer getNom() par la méthode réelle pour récupérer le nom du joueur
        f << "Joueur " /* << pair.first->getNom() */ << ": " << pair.second << " pts\n";
    }
}
}