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

    const TuileCite& Pioche::piocher(){
        if (estVide()) throw GameException("Pioche Vide");
        int i = rand() % nb; //tire un numéro de carte au hasard
        const TuileCite* tmp = tuiles[i];
        tuiles[i]=tuiles[--nb];
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
        joueurActuelIndex(0),pioche(nullptr), nbTuilesParTour(3) {
    
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

// DÉBUT CLASSE JOUEUR 
Joueur::Joueur(const string& nom, size_t capaciteCite)
    : nom(nom), nbPierres(0), cite(new Cite(capaciteCite)), tableauScore(nullptr) {
    
    if (nom.empty()) {
        throw GameException("Le nom du joueur ne peut pas être vide");
    }
}

Joueur::~Joueur() {
    delete cite;
}

void Joueur::setNom(const string& nouveauNom) {
    if (nouveauNom.empty()) {
        throw GameException("Le nom du joueur ne peut pas être vide");
    }
    nom = nouveauNom;
}



//FIN DE LA CLASSE JOUEUR  


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


    const Type Type::HABITATION("Habitation", Couleur::bleu, "Seules celles du plus grand groupe rapportent des points.");
    const Type Type::MARCHE("Marché", Couleur::jaune, "Rapporte des points s'il n'est pas adjacent à un autre marché.");
    const Type Type::CASERNE("Caserne", Couleur::rouge, "Rapporte des points si elle est placée en périphérie de la cité.");
    const Type Type::TEMPLE("Temple", Couleur::violet, "Rapporte des points si il est entièrement entouré.");
    const Type Type::JARDIN("Jardin", Couleur::vert, "Rapporte toujours des points, sans contrainte.");

    TuileCite::TuileCite(size_t id, HexagoneConstruction* h1,
            HexagoneConstruction* h2, HexagoneConstruction* h3,
            bool possede)
        : id(id), hexagones{h1, h2, h3}, hauteur(1), proprietaire(possede) {
                if (!h1 || !h2 || !h3)
                    throw GameException("HexagoneConstruction manquant dans TuileCite.");
            }

    TuileCite::~TuileCite() {
        if (proprietaire) {
            for (auto* hex : hexagones) {
                delete hex;
            }
        }
    }

    bool TuileCite::contientCarriere() const {
        for (const auto* hex : hexagones) {
            if (dynamic_cast<const Carriere*>(hex) != nullptr) {
                return true;
            }
        }
        return false;
    }

    bool TuileCite::contientPlace() const {
        for (const auto* hex : hexagones) {
            if (dynamic_cast<const Place*>(hex) != nullptr)
                return true;
        }
        return false;
    }

    int TuileCite::getNombreQuartiers() const {
        int count = 0;
        for (const auto* hex : hexagones) {
            if (dynamic_cast<const Quartier*>(hex) != nullptr)
                count++;
        }
        return count;
    }

    int TuileCite::getNombreCarrieres() const {
        int count = 0;
        for (const auto* hex : hexagones) {
            if (dynamic_cast<const Carriere*>(hex) != nullptr)
                count++;
        }
        return count;
    }

    vector<Couleur> TuileCite::getCouleursQuartiers() const {
        vector<Couleur> couleurs;
        for (auto* hex : hexagones) {
            if (auto* q = dynamic_cast<Quartier*>(hex)) {
                couleurs.push_back(q->getType().getCouleur());
            }
        }
        return couleurs;
    }
    
    void TuileCite::afficher(ostream& f) const {
        f << "Tuile #" << id << " (hauteur: " << hauteur << ")\n";
        for (size_t i = 0; i < 3; ++i) {
            f << "  Hexagone " << i << ": ";
            // Affichage selon le type
            if (auto* q = dynamic_cast<Quartier*>(hexagones[i])) {
                f << "Quartier " << q->getCouleur();
            } else if (auto* p = dynamic_cast<Place*>(hexagones[i])) {
                f << "Place " << p->getCouleur() << " (" << p->getNbEtoiles() << "★)";
            } else if (dynamic_cast<Carriere*>(hexagones[i])) {
                f << "Carrière";
            }
            f << "\n";
        }
    }
    ostream& operator<<(ostream& f, const TuileCite& tuile) {
        tuile.afficher(f);
        return f;
    }
    TuileCite* TuileCite::clone() const {
        return new TuileCite(id,
                         hexagones[0], 
                         hexagones[1], 
                         hexagones[2],
                         true); // la tuile clonée possède ses hexagones
    }



}