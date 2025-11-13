#include "Cite.h"

using namespace std;
namespace Akropolis{
    
    void Cite::ajouter(const TuileCite &t) {
        tuile_cites.push_back(&t);
    }


    void Cite::afficher(ostream& f) const {
        f << "Cité (" << tuile_cites.size() << " tuiles):\n";
        for (size_t i = 0; i < tuile_cites.size(); ++i) {
            const TuileCite* t = tuile_cites[i];
            if (t) {
                f << *t;
            } else {
                f << "  (tuile nulle)\n";
            }
        }
    }
}