#ifndef JOUEUR_H
#define JOUEUR_H

#include <string>
#include <iostream>
#include "Cite.h" 

using namespace std;

namespace Akropolis {
    
    // pour éviter les dépendances circulaires si nécessaire
    class TableauScore; 

    class Joueur { 
    private : 
        string nom; 
        int nbPierres; 
        Cite* cite;
        
    public : 
        
        Joueur(const string& nom, int numeroJoueur);
        ~Joueur();
        
        Joueur(const Joueur&) = delete;
        Joueur& operator=(const Joueur&) = delete;

        const string& getNom() const { return nom; }
        int getNbPierres() const { return nbPierres; }
        Cite* getCite() const { return cite; }
        
        void setNom(const string& nouveauNom);

        void ajouterPierre() { nbPierres++; }
        void ajouterPierres(int n);
        void retirerPierre();
        void retirerPierres(int n);
        bool peutPayerPierres(int n) const { return nbPierres >= n; }

        
        void placerTuile(TuileCite* tuile, const Cite::CoupPossible& coup);

        void afficher(ostream& f = cout) const;
        friend ostream& operator<<(ostream& f, const Joueur& joueur);
        
    
        int calculerScore() const;
    };
}

#endif