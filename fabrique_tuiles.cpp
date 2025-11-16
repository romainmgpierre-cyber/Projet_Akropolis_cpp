#include "Pioche_ChoixTuile.h"
#include "GameExcep_Enums.h"
namespace Akropolis {
    
    // pour créer toutes les tuiles d'un coup
    std::vector<TuileCite*> creerToutesTuilesAkropolis() {
        std::vector<TuileCite*> tuiles;
        size_t idTuile = 1;
        size_t idHexagone = 1;
        
        };
        
        //1
        tuiles.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::CASERNE, 2),
            new Quartier(idHexagone++, Type::HABITATION)
        ));

        //2
        tuiles.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::TEMPLE),
            new Place(idHexagone++, Type::HABITATION, 1),
            new Quartier(idHexagone++, Type::MARCHE)
        ));

        
        //3
        tuiles.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Carriere(idHexagone++)
        ));

        //4
        tuiles.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++)
        ));
        
        //5
        tuiles.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::JARDIN),
            new Quartier(idHexagone++, Type::CASERNE),
            new Carriere(idHexagone++)
        ));
                
        //6
        tuiles.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++)
        ));
        




        
        // Vérification: doit être exactement 60 tuiles
        if (tuiles.size() != 60) {
            throw GameException("Erreur: nombre de tuiles incorrect (" + 
                              std::to_string(tuiles.size()) + " au lieu de 60)");
        }
        
        return tuiles;
    }
    
    // Fonction pour créer la tuile de départ
    TuileDepart* creerTuileDepart() {
        size_t idHexDepart = 1000; // IDs différents pour la tuile de départ
        Place* centre = new Place(idHexDepart++, TypeQuartier::getPlace(), 0);
        Carriere* c1 = new Carriere(idHexDepart++);
        Carriere* c2 = new Carriere(idHexDepart++);
        Carriere* c3 = new Carriere(idHexDepart++);
        
        return new TuileDepart(0, centre, c1, c2, c3);
    }
}