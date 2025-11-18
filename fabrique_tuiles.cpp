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
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
            new Place(idHexagone++, Type::CASERNE, 2)
        ));
                
        //6
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2),
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        
        //7
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++)
        ));
        //8
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //9
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Carriere(idHexagone++),
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
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
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
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::JARDIN)
        ));
        //15
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE)
        ));
        //16
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::HABITATION, 1),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++)
        ));
        //17
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::JARDIN, 3),
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++)
        ));
        //18
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++)
        ));
        //19
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::CASERNE),
            new Carriere(idHexagone++)
        ));
        //20
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::JARDIN),
            new Carriere(idHexagone++)
        ));
        //21
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++)
        ));
        //22
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::JARDIN, 3),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++)
        ));
        //23
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::JARDIN)
        ));
        //24
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Carriere(idHexagone++)
        ));
        //25
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Place(idHexagone++, Type::HABITATION, 1),
            new Quartier(idHexagone++, Type::CASERNE)
        ));
        //26
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Carriere(idHexagone++)
        ));
        //27
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Place(idHexagone++, Type::MARCHE, 2),
            new Carriere(idHexagone++)
        ));
        //28
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Quartier(idHexagone++, Type::CASERNE)
        ));
        //29
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Quartier(idHexagone++, Type::MARCHE)
        ));
        //30
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::JARDIN, 3),
            new Carriere(idHexagone++)
        ));
        //31
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::TEMPLE),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++)
        ));
        //32
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Place(idHexagone++, Type::MARCHE, 2),
        ));
        //33
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Quartier(idHexagone++, Type::CASERNE)
        ));
        //34
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::MARCHE)
        ));
        //35
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::HABITATION, 1),
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::JARDIN)
        ));
        //36
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++)
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::CASERNE)
        ));
        //37
        tuiles2j.push_back(new(TuileCite
            idTuile++
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::HABITATION, 1)
        ));

        if (tuiles2j.size() != 37) {
            throw GameException("Erreur: nombre de tuiles incorrect (" + 
                              std::to_string(tuiles2j.size()) + " au lieu de 37)");
        }
        
        return tuiles2j;
    }
    
    std::vector<TuileCite*> creerTuiles3j() {
        std::vector<TuileCite*> tuiles3j; //uniquement les tuiles pour 3 joueurs (penser a rajouter les 2j)
        size_t idTuile = 38; // dans les 2j l'id se termine à 37, donc on prend la suite
        size_t idHexagone = 112; // de même avec 3x37=111 donc on commence à 112



        if (tuiles3j.size() != 37) {
            throw GameException("Erreur: nombre de tuiles incorrect (" + 
                              std::to_string(tuiles3j.size()) + " au lieu de 37)");
        }
        
        return tuiles3j;
    }

    std::vector<TuileCite*> creerTuiles4j() {
        std::vector<TuileCite*> tuiles4j; //uniquement les tuiles pour 4 joueurs (penser à rajouter les 2j et 3j)
        size_t idTuile = 1;
        size_t idHexagone = 1;



        if (tuiles4j.size() != 37) {
            throw GameException("Erreur: nombre de tuiles incorrect (" + 
                              std::to_string(tuiles4j.size()) + " au lieu de 37)");
        }
        
        return tuiles4j;
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