#include "Joueur.h"

namespace Akropolis{
    // DÉBUT CLASSE JOUEUR 
Joueur::Joueur(const string& nom, size_t capaciteCite)
    : nom(nom), nbPierres(0), cite(new Cite(capaciteCite)){
    
    if (nom.empty()) {
        throw GameException("Le nom du joueur ne peut pas être vide");
    }
}

Joueur::~Joueur() {
    delete cite;
}

void Joueur::setNom(const string& nouveauNom) {
    if (nouveauNom.empty()) {
        throw GameException("Le nom du joueur ne peut pas être vide");
    }
    nom = nouveauNom;
}

void Joueur::ajouterPierres(int n) {
    if (n < 0) {
        throw GameException("Impossible d'ajouter un nombre négatif de pierres");
    }
    if (n > 0) {
        nbPierres += n;
    }
}

void Joueur::retirerPierre() {
    if (nbPierres <= 0) {
        throw GameException("Le joueur " + nom + " n'a pas de pierres à retirer");
    }
    nbPierres--;
}

void Joueur::retirerPierres(int n) {
    if (n < 0) {
        throw GameException("Impossible de retirer un nombre négatif de pierres");
    }
    if (n == 0) {
        return; 
    }
    
    if (nbPierres < n) {
        throw GameException("Le joueur n'as pas assez de pierre");
    }
    nbPierres -= n;
}

void Joueur::afficher(ostream& f) const {
    f << "=== Joueur: " << nom << " ===\n";
    f << "Pierres: " << nbPierres << "\n";
    f << "Nombre de tuiles dans la cité: " << cite->getnb() << "\n";
    
    //On peut prévoir d'afficher le score si le tableau de score est défini
    
    f << "Cité:\n";
    cite->afficher(f);
}

ostream& operator<<(ostream& f, const Joueur& joueur) {
    joueur.afficher(f);
    return f;
}

// A redefinir
// int Joueur::calculerScore() const {
//     if (tableauScore != nullptr) {
//         return tableauScore->calculerScore(*this);
//     }
//     return 0;
// }


//FIN DE LA CLASSE JOUEUR  

}