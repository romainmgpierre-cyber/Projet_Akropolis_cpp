#include "GameExcep_Enums.h"
#include <string>
using namespace std;
namespace Akropolis{
        initializer_list<Couleur> Couleurs = { Couleur::rouge, Couleur::violet, Couleur::vert, Couleur::bleu, Couleur::jaune, Couleur::gris };
	initializer_list<Etoile> Etoiles = { Etoile::un, Etoile::deux, Etoile::trois };


    string toString(Couleur c) {
		switch (c) { 
		case Couleur::rouge: return "Rouge";
		case Couleur::bleu: return "Bleu";
		case Couleur::vert: return "Vert";
        case Couleur::violet: return "Violet";
        case Couleur::jaune: return "Jaune";
        case Couleur::gris: return "Gris";
		default: throw GameException("Couleur inconnue");
		}
	}

	string toString(Etoile e){
        switch(e){
            case Etoile::un: return "*";
            case Etoile::deux: return "2*";
            case Etoile::trois: return "3*";
            default: throw GameException("Etoile inconnue");
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
}