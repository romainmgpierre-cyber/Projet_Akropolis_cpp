#ifndef AKROPLIS_H
#define AKROPLIS_H

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

    enum class Couleur { rouge, violet, vert, bleu, gris };
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
        vector<TuileDepart*> tuilesDepart; // 4 tuiles de départ 
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
        void initialiserTuiles();
        void ajouterJoueur(const string& nom);
        void retirerJoueur(const string& nom);
        void setDifficulte(NiveauDifficulte diff) { difficulte = diff; }
        void ajouterVariante(const Variante& v) { variantes.push_back(v); }
        void activerVariante(const string& nom);
        void desactiverVariante(const string& nom);


    };

    class Joueur { 
    private : 
        string nom; 
        int nbPierres 
        Cite* cite;
        TableauScore* tableauScore; 
    public : 
        Joueur(const string& nom, size_t capaciteCite = 25)
        ~Joueur();
        Joueur(const Joueur&) = delete;
        Joueur& operator=(const Joueur&) = delete;

        const string& getNom() const { return nom; }
        int getNbPierres() const { return nbPierres; }
        Cite* getCite() const { return cite; }
        TableauScore* getTableauScore() const { return tableauScore; }
        void setNom(const string& nouveauNom);
        void setTableauScore(TableauScore* ts) { tableauScore = ts; }

        void ajouterPierre() { nbPierres++; }
        void ajouterPierres(int n);
        void retirerPierre();
        void retirerPierres(int n);
        bool peutPayerPierres(int n) const { return nbPierres >= n;}
        void ajouterTuile(const TuileCite& tuile) { cite->ajouter(tuile);}
        void afficher(ostream& f = cout) const;
        friend ostream& operator<<(ostream& f, const Joueur& joueur);
        int calculerScore() const;
    }


    class Pioche{
        size_t id;
        TuileCite ** tuiles;
        size_t nb; //nombre de tuiles dans la pioche
        public:
        Pioche(size_t id, size_t taillepioche) : id(id) {tuiles = new TuileCite*[taillepioche];}
        size_t getNbtuilesPioche() const {return nb;}
        bool estVide(){return nb==0;}
        const TuileCite& piocher();
        ~Pioche(){ delete[] tuiles;}
        Pioche& operator=(const Pioche&) = delete;
		Pioche(const Pioche&) = delete;
    };

    class ChoixTuile {
    private:
        static const size_t MAX_TUILES = 4;
        size_t id;
        vector<TuileCite*> tuilesDisponibles;
    
    public:
        ChoixTuile(size_t id) : id(id) {
            tuilesDisponibles.reserve(MAX_TUILES);
        }
        
        bool ajouterTuile(TuileCite* tuile);
        bool retirerTuile(size_t tuileId);
        const vector<TuileCite*>& getTuilesDisponibles() const {
            return tuilesDisponibles;
        }
        
        size_t getNombreTuiles() const { return tuilesDisponibles.size(); }
};




    
    class CoordHex{
        //coordonées axiales q, r,s avec s = -q -r
        int q, r; 
    }
    
    
    class TuileCite{
        private : 
            size_t id;
            array<HexagoneConstruction*, 3> hexagones;
            array<CoordHex, 3> positions; //on stocke les coordonées des 3 tuiles; 
            unsigned int hauteur=1;
            bool proprietaire; //true si possède des haxagones (pour la gestion de la mémoire)
            
        public : 
            TuileCite(size_t id, HexagoneConstruction* h1, 
              HexagoneConstruction* h2, HexagoneConstruction* h3,
              bool possede = true);
            ~TuileCite();
            TuileCite(const TuileCite&)=delete;
            TuileCite& operator=(const TuileCite& )=delete;

            size_t getId() const { return id; }
            const array<HexagoneConstruction*, 3>& getHexagones() const { 
                return hexagones; 
            }
            HexagoneConstruction* getHexagone(size_t index) const {
                if (index >= 3) throw GameException("Index hexagone invalide");
                return hexagones[index];
            }

            void setHauteur(unsigned int h) { hauteur = h; }
         
            void rotationHoraire();
            void rotationAntihoraire();

            bool contientCarriere() const;
            bool contientPlace() const;
            int getNombreQuartiers() const;
            int getNombreCarrieres() const;
            vector<Couleur> getCouleursQuartiers() const;
    
            void afficher(ostream& os = std::cout) const;
            friend ostream& operator<<(std::ostream& os, const TuileCite& tuile);
    
            TuileCite* clone() const;
    };

   class TuileDepart {
    private:
        size_t id;
        std::array<HexagoneConstruction*, 4> hexagones; // 0 = centre (Place), 1-3 = Carrières
    public:
        // Constructeur : le centre est toujours une Place, les autres sont des Carrières
        TuileDepart(size_t id, Place* centre, Carriere* c1, Carriere* c2, Carriere* c3)
            : id(id) 
        {
            hexagones[0] = centre; // centre = Place
            hexagones[1] = c1;
            hexagones[2] = c2;
            hexagones[3] = c3;
        }

        ~TuileDepart() {
            // On détruit les hexagones
            for (auto h : hexagones) delete h;
        }

        size_t getId() const { return id; }
        const std::array<HexagoneConstruction*,4>& getHexagones() const { return hexagones; }
        HexagoneConstruction* getHexagone(size_t index) const { 
            if (index >= 4) throw GameException("Index hexagone invalide");
            return hexagones[index];
        }
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
            const Type* type; // association vers Type
        public:
            Quartier(size_t id, const Type& type)
                : HexagoneConstruction(id), type(&type) {}

            const Type& getType() const { return *type; }
    };


    class Place : public HexagoneConstruction{
        private:
            const Type* type; // association vers Type
            Etoile nbetoile;
        public:
            Place(size_t id, const Type& type, Etoile nbetoile)
                : HexagoneConstruction(id), type(&type), nbetoile(nbetoile) {}

            const Type& getType() const { return *type; }
            Etoile getNbEtoile() const { return nbetoile; }
    };

    class Carriere : public HexagoneConstruction{
        private :
            Couleur coul = Couleur::gris;
        public:
            Carriere(size_t id) : HexagoneConstruction(id){}
    };

        // --- Tableau des scores ---
    class CalculScoreBase {
    public:
        virtual ~CalculScoreBase() = default;
        virtual int calculerScore(const Joueur& joueur) const = 0;
    };

    class CalculScoreRecouvrement : public virtual CalculScoreBase {
    public:
        int calculerScore(const Joueur& joueur) const ;
    };

    class CalculScorePlaces : public virtual CalculScoreBase {
    public:
        int calculerScore(const Joueur& joueur) const ;
    };

    class CalculScoreMultiplicateurs : public virtual CalculScoreBase {
    public:
        int calculerScore(const Joueur& joueur) const;
    };

    class TableauScore :
        public CalculScoreRecouvrement,
        public CalculScorePlaces,
        public CalculScoreMultiplicateurs
    {
    private:
        vector<pair<Joueur*, int>> scores;

    public:
        void ajouterJoueur(Joueur* j);
        void calculerScores();
        void afficherScores(ostream& f = cout) const;
        int calculerScore(const Joueur& joueur) const{
        return CalculScoreRecouvrement::calculerScore(joueur)
             + CalculScorePlaces::calculerScore(joueur)
             + CalculScoreMultiplicateurs::calculerScore(joueur);
    }
    };
    //les types de quartier
    class Type {
        string nom;
        Couleur couleur;
        string conditions;        
        Type(const string& nom, Couleur couleur, const string& cond)
            : nom(nom), couleur(couleur), conditions(cond) {}
    public:
        static const Type HABITATION;
        static const Type MARCHE;
        static const Type CASERNE;
        static const Type TEMPLE;
        static const Type JARDIN;
    
        Type(const Type&)=delete;
        Type& operator=(const Type&)=delete;
    
        const string& getNom() const { return nom; }
        Couleur getCouleur() const { return couleur; }
        const string& getConditions() const { return conditions; }
    };



}



#endif