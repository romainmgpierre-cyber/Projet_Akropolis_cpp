#ifndef GameExcep_ENUMS_H
#define GameExcep_ENUMS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

using namespace std;
namespace Akropolis{
    
    class GameException {
	public:
		GameException(const string& i) :info(i) {}
		string getInfo() const { return info; }
	private:
		string info;
	};
    class PartieAnnulee : public GameException {
        public:
            PartieAnnulee(const string& msg) : GameException(msg) {}
        };
    enum class Couleur { rouge, violet, vert, bleu, jaune, gris };
    enum class Etoile { un=1, deux=2, trois=3 };
    
    string toString(Couleur c);
    string toString(Etoile e);

    ostream& operator<<(ostream& f, Couleur c);
	ostream& operator<<(ostream& f, Etoile e);

    extern initializer_list<Couleur> Couleurs;
	extern initializer_list<Etoile> Etoiles;

    void printCouleurs(ostream& f = cout);
	void printNbEtoiles(ostream& f = cout);

//pour la classe partie : 
    enum class ModeJeu {SOLO, MULTIJOUEUR};
    enum class NiveauDifficulte {FACILE, MOYEN, DIFFICILE};
    enum class EtatPartie {NON_DEMARREE, EN_COURS, TERMINEE, SUSPENDUE};

}


#endif