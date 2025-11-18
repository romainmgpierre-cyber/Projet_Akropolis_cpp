#include "Pioche_ChoixTuile.h"
#include "GameExcep_Enums.h"
namespace Akropolis {

/*Je créer 4 fonctions pour créer :
- les tuiles 2+
- 3+
- 4
- les tuiles de départ
pour une partie à 2 joueurs on prendra uniquement les cartes de la première fct plus celles de départ
pour une à trois joueurs, on prend les cartes 2+ et 3+ et celles de départ
pour une à quatre on prend toutes les cartes*/
    
    std::vector<TuileCite*> creerTuiles2j() {
        std::vector<TuileCite*> tuiles2j; //uniquement les tuiles pour deux jouers 
        size_t idTuile = 1;
        size_t idHexagone = 1;
        
        //1
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::MARCHE, 2),
            new Carriere(idHexagone++)
        ));

        //2
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::MARCHE, 2),
            new Quartier(idHexagone++, Type::JARDIN)
        ));

        
        //3
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::TEMPLE)
        ));

        //4
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::MARCHE),
            new Carriere(idHexagone++)
        ));
        
        //5
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::CASERNE, 2)
        ));
                
        //6
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2)
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++)
        ));
        
        //7
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2)
            new Carriere(idHexagone++)
            new Carriere(idHexagone++)
        ));
        //8
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2)
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::TEMPLE),
        ));
        //9
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::TEMPLE, 2)
            new Carriere(idHexagone++)
            new Carriere(idHexagone++)
        ));
        //10
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::JARDIN),
            new Quartier(idHexagone++, Type::MARCHE)
        ));
        //11
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::TEMPLE)
        ));
        //12
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //13
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::HABITATION, 1),
            new Carriere(idHexagone++)
        ));
        //14
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::JARDIN)
        ));
        //15
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE)
        ));


        
        // Vérification: doit être exactement 60 tuiles
        if (tuiles.size() != 60) {
            throw GameException("Erreur: nombre de tuiles incorrect (" + 
                              std::to_string(tuiles.size()) + " au lieu de 60)");
        }
        
        return tuiles2j;
    }
    
    // Fonction pour créer les 4 tuiles de départ
    std::vector<TuileDepart*> creer4TuileDepart() {
        std::vector<TuileCite*> TuilesDepart; 
        size_t idTuile = 1;

        tuiles.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::HABITATION, 1);
            new Carriere(idHexDepart++),
            new Carriere(idHexDepart++),
            new Carriere(idHexDepart++)));

        tuiles.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::HABITATION, 1);
            new Carriere(idHexDepart++),
            new Carriere(idHexDepart++),
            new Carriere(idHexDepart++))):

        tuiles.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::HABITATION, 1);
            new Carriere(idHexDepart++),
            new Carriere(idHexDepart++),
            new Carriere(idHexDepart++)));

        tuiles.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::HABITATION, 1);
            new Carriere(idHexDepart++),
            new Carriere(idHexDepart++),
            new Carriere(idHexDepart++)));
        
        return TuilesDepart;
    }
}