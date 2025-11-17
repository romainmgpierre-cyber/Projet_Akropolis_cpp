#include "Joueur.h"
#include "Score.h"      
#include "GameExcep_Enums.h"

namespace Akropolis {

    Joueur::Joueur(const string& nom) 
        : nom(nom), nbPierres(1) // Règle officielle : on commence souvent avec 1 pierre (ou ajuster selon règle)
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
        // Délègue simplement à la méthode de la Cité
        // C'est ici qu'on fait le lien entre l'ancienne méthode "ajouterTuile"
        // et la nouvelle logique "placerTuile" avec coordonnées.
        cite->placerTuile(tuile, coup);
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