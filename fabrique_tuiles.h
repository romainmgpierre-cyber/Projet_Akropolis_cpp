#ifndef AKROPOLIS_FABRIQUE_TUILES_H
#include "Pioche_ChoixTuile.h"
#include "GameExcep_Enums.h"
#include <algorithm>
#include <random>
#define AKROPOLIS_FABRIQUE_TUILES_H

namespace Akropolis {
    std::vector<TuileCite*> creerTuiles2j();
    std::vector<TuileCite*> creerTuiles3j();
    std::vector<TuileCite*> creerTuiles4j();
    std::vector<TuileDepart*> creer4TuileDepart();
    std::vector<TuileCite*> creerToutesTuiles(int nbJoueurs);
    void melangerTuiles(std::vector<TuileCite*>& tuiles);
}

#endif //AKROPOLIS_FABRIQUE_TUILES_H