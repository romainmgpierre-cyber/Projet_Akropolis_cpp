#include "Akropolis.h"
namespace Akropolis{ 

    std::initializer_list<Couleur> Couleurs = { Couleur::rouge, Couleur::violet, Couleur::vert, Couleur::bleu, Couleur::gris };
	std::initializer_list<Etoile> Etoiles = { Etoile::un, Etoile::deux, Etoile::trois };


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

    std::ostream& operator<<(std::ostream& f, Couleur c) { f << toString(c); return f; }
	std::ostream& operator<<(std::ostream& f, Etoile e) {	f << toString(e); return f; }
    
    void printCouleurs(std::ostream& f) {
        for (auto c : Couleurs) f << c << " ";
        f << "\n";
    }
    void printNbEtoiles(std::ostream& f) {
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
	
}