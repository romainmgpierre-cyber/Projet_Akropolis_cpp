#ifndef PIOCHE_CHOIXTUILE_H
#define PIOCHE_CHOIXTUILE_H
#include <vector>
#include <list> // Utilisation de list pour gérer les piles
#include "TuileCite_TuileDep.h"
#include "Joueur.h"
#include "GameExcep_Enums.h" 

using namespace std;
namespace Akropolis{
    
    class Pioche{
        size_t id;
        // On remplace le vecteur simple par une liste de vecteurs (les piles)
        // Chaque élément de la liste est une "Pile" (vector<TuileCite*>)
        list<vector<TuileCite*>> piles; 
        
    public:
        Pioche(size_t id) : id(id) {} // Le constructeur ne fait plus de reserve simple

        ~Pioche();

        // Retourne le nombre de PILES restantes
        size_t getNbPilesRestantes() const { return piles.size(); }
        bool estVide() const { return piles.empty(); }
        
        // Nouvelle méthode pour créer les piles au début
        void organiserPiles(vector<TuileCite*>& toutesLesTuiles, int nbJoueurs);

        // Récupère une pile entière pour remplir le marché
        vector<TuileCite*> prendreUnePile();
        
        Pioche& operator=(const Pioche&) = delete;
        Pioche(const Pioche&) = delete;
    };

    class ChoixTuile {
    private:
        size_t id;
        vector<TuileCite*> tuilesDisponibles;
    
    public:
        ChoixTuile(size_t id) : id(id) {}
        
        ~ChoixTuile();
        void remettreTuile(TuileCite* tuile, size_t index);
        
        size_t calculerCout(size_t index) const {
            return index; 
        }

        TuileCite* choisirTuile(Joueur* joueur, size_t index);
        
        // Nouvelle méthode pour ajouter tout le contenu d'une pile d'un coup
        void ajouterPile(const vector<TuileCite*>& nouvellesTuiles);
        
        // Méthode simple pour l'initialisation (ajout unitaire)
        void ajouterTuile(TuileCite* t) { tuilesDisponibles.push_back(t); }

        const vector<TuileCite*>& getTuilesDisponibles() const {
            return tuilesDisponibles;
        }
        
        // Plus de MAX_TUILES statique, car la taille varie (4, 5 ou 6)
        size_t getNombreTuiles() const { return tuilesDisponibles.size(); }
        static size_t getMaxTuiles(int nbJoueurs) { return nbJoueurs + 2; } // Règle astuce page 3
    };
}

#endif