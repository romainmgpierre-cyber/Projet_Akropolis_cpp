#ifndef JOUEUR_H
#define JOUEUR_H
#include <string>
#include "Cite.h"
using namespace std;
namespace Akropolis{
    class Joueur { 
    private : 
        string nom; 
        int nbPierres; 
        Cite* cite;
        
    public : 
        Joueur(const string& nom, size_t capaciteCite = 25);
        ~Joueur();
        Joueur(const Joueur&) = delete;
        Joueur& operator=(const Joueur&) = delete;

        const string& getNom() const { return nom; }
        int getNbPierres() const { return nbPierres; }
        Cite* getCite() const { return cite; }
        // TableauScore* getTableauScore() const { return tableauScore; } a modifié dans tableau score
        void setNom(const string& nouveauNom);
        // void setTableauScore(TableauScore* ts) { tableauScore = ts; } a modifié dans tableau score

        void ajouterPierre() { nbPierres++; }
        void ajouterPierres(int n);
        void retirerPierre();
        void retirerPierres(int n);
        bool peutPayerPierres(int n) const { return nbPierres >= n;}
        void ajouterTuile(const TuileCite& tuile) { cite->ajouter(tuile);}
        void afficher(ostream& f = cout) const;
        friend ostream& operator<<(ostream& f, const Joueur& joueur);
        int calculerScore() const;
    };

}


#endif