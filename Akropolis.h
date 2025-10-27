#ifndef AKROPLIS_H
#define AKROPLIS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace Akropolis{

    class GameException {
	public:
		GameException(const string& i) :info(i) {}
		string getInfo() const { return info; }
	private:
		string info;
	};

    enum class Couleur { rouge, violet, vert, bleu, gris };
    enum class Etoile { un=1, deux=2, trois=3 };
    
    string toString(Couleur c);
    string toString(Etoile e);

    ostream& operator<<(ostream& f, Couleur c);
	ostream& operator<<(ostream& f, Etoile e);

    extern std::initializer_list<Couleur> Couleurs;
	extern std::initializer_list<Etoile> Etoiles;

    void printCouleurs(std::ostream& f = cout);
	void printNbEtoiles(std::ostream& f = cout);

    class Partie{

    };

    class Joueur{

    };

    class TableauScore{


    };

    class Pioche{
        size_t id;
        TuileCite ** tuiles;
        size_t nb; //nombre de tuiles dans la pioche
        public:
        Pioche(size_t id, size_t taillepioche) : id(id) {tuiles = new TuileCite*[taillepioche];}
        size_t getNbtuilesPioche() const {return nb;}
        bool estVide(){return nb==0;}
        ~Pioche(){ delete[] tuiles;}
    };

    


    class Cite{
        // S'inspirer de la classe Plateau qu'on avait pour le jeu SET mais il faut aller voir sur un site internet spécial pour voir comment gérer le placement de pièce hexagonal
        // Lien du site : 

    };

    class TuileCite{
        unsigned int hauteur = 1;

    };

    class HexagoneConstruction{
        protected:
            size_t id;
        public:
            HexagoneConstruction(size_t id) : id(id) {}

    };

    class Quartier : public HexagoneConstruction{
        private:
            Couleur coul;
        public:
            Quartier(size_t id, const Couleur& coul ) : HexagoneConstruction(id), coul(coul){ }
    };


    class Place : public HexagoneConstruction{
        private:
            Couleur coul;
            Etoile nbetoile;
        public:
            Place(size_t id, const Couleur& coul, const Etoile& nbetoile) : HexagoneConstruction(id), coul(coul), nbetoile(nbetoile) {}
    };

    class Carriere : public HexagoneConstruction{
        private :
            Couleur coul = Couleur::gris;
        public:
            Carriere(size_t id) : HexagoneConstruction(id){}
    };

}

#endif