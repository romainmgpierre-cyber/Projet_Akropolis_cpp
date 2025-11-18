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