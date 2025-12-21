#ifndef PARTIE_VARIANTE_H
#define PARTIE_VARIANTE_H
#include <string>
#include "GameExcep_Enums.h"
#include "Joueur.h"
#include "TuileCite_TuileDep.h"
#include "Pioche_ChoixTuile.h"
#include "MenuConfiguration.h"
using namespace std;
namespace Akropolis{
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

    class Configuration; // Déclaration anticipée
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
        ChoixTuile* choixTuile; 
        vector<TuileCite*> tuilesDisponibles; 
        vector<TuileDepart*> tuilesDepart; 
        size_t nbTuilesParTour;

        // pour référencer la configuration
        Akropolis::Configuration* config = nullptr;

    public:
        Partie(size_t id, ModeJeu mode);
        ~Partie();
        Partie(const Partie&) = delete;
        Partie& operator=(const Partie&) = delete;
        
        void lancerPartie(); // Initialise tout
        void boucleDeJeu();  // Fait tourner les tours
        void configurerVariantes(); // Nouvelle méthode pour le menu de sélection
        void gererTourJoueur(Joueur* joueur); // Logique d'un tour
        void remplirChoixTuile();

        ModeJeu getMode() const { return mode; }
        EtatPartie getEtat() const { return etat; }
        size_t getNombreJoueurs() const { return joueurs.size(); }
        Joueur* getJoueurActuel() const { return joueurs[joueurActuelIndex]; }
        const vector<Joueur*>& getJoueurs() const { return joueurs; }
        Pioche* getPioche() const { return pioche; }
        vector<Variante> getVariantesDisponibles() const{ return variantes; }
        vector<Variante> getVariantesActives() const;
        void setVarianteEtat(const std::string& nom, bool active);
        Configuration* getConfig(){return config;}


        void initialiserTuiles();
        void ajouterJoueur(const std::string& nom, int numeroJoueur, bool estIA = false);
        void retirerJoueur(const string& nom);
        void setDifficulte(NiveauDifficulte diff) { difficulte = diff; }
        NiveauDifficulte getDifficulte() const { return difficulte; }
        void ajouterVariante(const Variante& v) { variantes.push_back(v); }
        void activerVariante(const string& nom);
        void desactiverVariante(const string& nom);
        ChoixTuile* getChoixTuile() const { return choixTuile; }
        void passerTour() {
            joueurActuelIndex = (joueurActuelIndex + 1) % joueurs.size();
        }
        void setConfig(Configuration& c){config = &c;}

        void reprendrePartie();
    
    private:
        
        void afficherEtatJeu(Joueur* joueur) const;
        void payerPierres(Joueur* joueur, size_t cout);

    };
}

#endif
