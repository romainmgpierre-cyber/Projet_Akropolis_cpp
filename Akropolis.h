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

//pour la classe partie : 
    enum class ModeJeu {SOLO, MULTIJOUEUR};
    enum class NiveauDifficulte {FACILE, MOYEN, DIFFICILE};
    enum class EtatPartie {NON_DEMARREE, EN_COURS, TERMINEE, SUSPENDUE};

/* "Le jeu propose quelques variantes qui permettent de corser le jeu.
Dans ce cas, la mise en place et le déroulé de la partie ne changent pas. 
Chaque variante propose une condition de placement supplémentaire
pour rapporter des points. Il est possible de jouer avec plusieurs variantes dans la même partie."
*/
    class Variante {
    private:
        string nom;
        string description;
        bool active;
    
    public:
        Variante(const string& n, const string& desc) 
            : nom(n), description(desc), active(false) {}
    
        void activer() { active = true; }
        void desactiver() { active = false; }
        bool estActive() const { return active; }
        string getNom() const { return nom; }
        string getDescription() const { return description; }
    };

    class Partie{
        size_t id;
        ModeJeu mode;
        EtatPartie etat;
        NiveauDifficulte difficulte;
        vector<Variante> variantes;

        //joueurs
        vector<Joueur*> joueurs;
        size_t joueurActuelIndex;

        //pioche
        Pioche* pioche;
        vector<TuileCite*> tuilesDisponibles; 
        size_t nbTuilesParTour;

    public:
        Partie(size_t id, ModeJeu mode);
        ~Partie();
        Partie(const Partie&) = delete;
        Partie& operator=(const Partie&) = delete;

        ModeJeu getMode() const { return mode; }
        EtatPartie getEtat() const { return etat; }
        size_t getNombreJoueurs() const { return joueurs.size(); }
        Joueur* getJoueurActuel() const { return joueurs[joueurActuelIndex]; }
        const vector<Joueur*>& getJoueurs() const { return joueurs; }
        Pioche* getPioche() const { return pioche; }
        vector<Variante> getVariantesDisponibles() const{ return variantes; }
        vector<Variante> getVariantesActives() const;

        void ajouterJoueur(const string& nom);
        void retirerJoueur(const string& nom);
        void setModeEtendu(bool etendu) { modeEtendu = etendu; }
        void setDifficulte(NiveauDifficulte diff) { difficulte = diff; }
        void ajouterVariante(const Variante& v) { variantes.push_back(v); }
        void activerVariante(const string& nom);
        void desactiverVariante(const std::string& nom);


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

    class TuileCite{
        unsigned int hauteur = 1;

    };

    class Cite{
    private:
        const TuileCite** tuile_cites = nullptr; //on stocke le pointeur de chaque tuile
        size_t nb = 0; //nombre de tuile stockées
        size_t nb_max = 0; //taille du tableau

    public:
        Cite(size_t capacite_initiale = 4): nb(0), nb_max(capacite_initiale),
          tuile_cites(new const TuileCite*[capacite_initiale]) {}

        ~Cite(){delete [] tuile_cites;}

        void ajouter(const TuileCite& t);
        void afficher(ostream& f) const; //afficher l'ensemble des carte contenu dans Cite

        //accesseurs :
        size_t getnb() const {return nb;}
        size_t getnb_max() const {return nb_max;}

        //je pense qu'il faudrais stocker les coordonée des différente tuiles dans tuileCite
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