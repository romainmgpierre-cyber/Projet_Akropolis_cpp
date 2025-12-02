#include "Joueur.h"
#include "Score.h"      
#include "GameExcep_Enums.h"

namespace Akropolis {

    Joueur::Joueur(const string& nom, int numeroJoueur) 
        : nom(nom), nbPierres(numeroJoueur) // Le 1er joueur a 1 pierre, le 2ème 2, etc.
    {
        cite = new Cite();
    }

    Joueur::~Joueur() {
        delete cite;
    }

    void Joueur::setNom(const string& nouveauNom) {
        nom = nouveauNom;
    }

    void Joueur::ajouterPierres(int n) {
        nbPierres += n;
    }

    void Joueur::retirerPierre() {
        if (nbPierres > 0) nbPierres--;
        else throw GameException("Pas assez de pierres !");
    }

    void Joueur::retirerPierres(int n) {
        if (nbPierres >= n) nbPierres -= n;
        else throw GameException("Pas assez de pierres !");
    }

    void Joueur::placerTuile(TuileCite* tuile, const Cite::CoupPossible& coup) {
        // La Cité place la tuile et nous retourne le nombre de pierres gagnées.
        int pierresGagnees = cite->placerTuile(tuile, coup);

        // Le joueur ajoute les pierres à son inventaire
        if (pierresGagnees > 0) {
            ajouterPierres(pierresGagnees);
        }
    }

    void Joueur::afficher(ostream& f) const {
        f << "Joueur : " << nom << " (" << nbPierres << " pierres)" << endl;
        f << "Cité :" << endl;
        cite->afficher(f);
    }

    ostream& operator<<(ostream& f, const Joueur& joueur) {
        joueur.afficher(f);
        return f;
    }

    int Joueur::calculerScore() const {
        
        TableauScore calculateur;
        // On délègue le calcul
        return calculateur.calculerScore(*this);
    }

}