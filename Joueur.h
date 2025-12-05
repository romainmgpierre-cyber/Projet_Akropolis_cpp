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
        bool estIA = false;
        
    public : 
        
        Joueur(const string& nom, int numeroJoueur, bool estIA = false);
        ~Joueur();
        
        Joueur(const Joueur&) = delete;
        Joueur& operator=(const Joueur&) = delete;

        const string& getNom() const { return nom; }
        int getNbPierres() const { return nbPierres; }
        Cite* getCite() const { return cite; }
        
        void setNom(const string& nouveauNom);

        bool isIA() const { return estIA; }
        
        void recupererTuileIA(TuileCite* tuile); // Méthode pour l'IA qui contourne la logique de placement standard
        

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