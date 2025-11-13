
#ifndef CITE_H
#define CITE_H


#include <iostream>
#include <vector>
#include "TuileCite_TuileDep.h"


using namespace std;
namespace Akropolis{
    
    class Cite{
    private:
        vector<const TuileCite*> tuile_cites;
        size_t nb_max;

    public:
        Cite(size_t capacite_initiale = 4): nb_max(capacite_initiale) {
            tuile_cites.reserve(capacite_initiale);
        }

        ~Cite() = default;

        void ajouter(const TuileCite& t);
        void afficher(ostream& f) const;

        //accesseurs :
        size_t getnb() const { return tuile_cites.size(); }
        size_t getnb_max() const { return nb_max; }
    };
}



#endif