#include "Joueur.h"
#include "Score.h"      
#include "GameExcep_Enums.h"

namespace Akropolis {

    Joueur::Joueur(const string& nom, int numeroJoueur, bool estIA) 
        : nom(nom), nbPierres(numeroJoueur), estIA(estIA) // Le 1er joueur a 1 pierre, le 2ème 2, ...
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

    int Joueur::placerTuile(TuileCite* tuile, const Cite::CoupPossible& coup) {
        //place la tuile et nous retourne le nombre de pierres gagnées.
        int pierresGagnees = cite->placerTuile(tuile, coup);

        
        if (pierresGagnees > 0) {
            ajouterPierres(pierresGagnees);
        }
        return pierresGagnees;
    }

    void Joueur::afficher(ostream& f) const {
        f << "Joueur : " << nom << " (" << nbPierres << " pierres)" << endl;
        f << "Cité :" << endl;
        cite->afficherGraphique(f);
    }

    ostream& operator<<(ostream& f, const Joueur& joueur) {
        joueur.afficher(f);
        return f;
    }

    int Joueur::calculerScore() const {
        
        TableauScore calculateur;
        
        return calculateur.calculerScore(*this);
    }

    void Joueur::recupererTuileIA(TuileCite* tuile) {
        
        cite->ajouterTuileIA(tuile); // Pour l'IA, on ajoute la tuile à son inventaire
    }

   

}
