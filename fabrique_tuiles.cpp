#include "Pioche_ChoixTuile.h"
#include "GameExcep_Enums.h"
#include <algorithm>
#include <random>
namespace Akropolis {


    void melangerTuiles(std::vector<TuileCite*>& tuiles);
    
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
            new Place(idHexagone++, Type::MARCHE, 2)
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
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::CASERNE)
        ));
        //37
        tuiles2j.push_back(new TuileCite(
            idTuile++,
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

        //38
       tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //39
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //40
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::MARCHE, 2),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++)
        ));
        //41
       tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::JARDIN),
            new Quartier(idHexagone++, Type::CASERNE)
        ));
        //42
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Place(idHexagone++, Type::HABITATION, 1)
        ));
        //43
       tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE)
        ));
        //44
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //45
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::JARDIN, 3)
        ));
        //46
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++)
        ));
        //47
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::CASERNE, 2),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //48
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++)
        ));
        //49
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::TEMPLE)
        ));



        if (tuiles3j.size() != 12) {
            throw GameException("Erreur: nombre de tuiles incorrect (" + 
                              std::to_string(tuiles3j.size()) + " au lieu de 12)");
        }
        
        return tuiles3j;
    }

    std::vector<TuileCite*> creerTuiles4j() {
        std::vector<TuileCite*> tuiles4j; //uniquement les tuiles pour 4 joueurs (penser à rajouter les 2j et 3j)
        size_t idTuile = 50;
        size_t idHexagone = 148;

        //50
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::MARCHE, 2),
            new Quartier(idHexagone++, Type::TEMPLE)
        ));
        //51
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::HABITATION, 1),
            new Quartier(idHexagone++, Type::MARCHE)
        ));
        //52
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //53
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::JARDIN)
        ));
        //54
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::TEMPLE, 2)
        ));
        //55
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++)
        ));
        //56
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++)
        ));
        //57
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //58
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::CASERNE)
        ));
        //59
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //60
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::HABITATION)
        ));
        //61
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
            new Place(idHexagone++, Type::JARDIN, 3)
        ));


        if (tuiles4j.size() != 12) {
            throw GameException("Erreur: nombre de tuiles incorrect (" + 
                              std::to_string(tuiles4j.size()) + " au lieu de 12)");
        }
        
        return tuiles4j;
    }


    std::vector<TuileDepart*> creer4TuileDepart() {
        std::vector<TuileDepart*> tuilesDepart; // Type de retour corrigé
        size_t idTuile = 62;
        size_t idHexagone = 184; // On réutilise idHexagone

        for (int i = 0; i < 4; ++i) {
            // Toutes les tuiles de départ ont la même structure
            tuilesDepart.push_back(new TuileDepart(
                idTuile++,
                new Place(idHexagone++, Type::HABITATION, 1),
                new Carriere(idHexagone++),
                new Carriere(idHexagone++),
                new Carriere(idHexagone++))
            );
        }
        
        return tuilesDepart;
    }

    std::vector<TuileCite*> creerToutesTuiles(int nbJoueurs) {
        std::vector<TuileCite*> toutesLesTuiles;
        
        std::vector<TuileCite*> tuiles2 = creerTuiles2j();
        toutesLesTuiles.insert(toutesLesTuiles.end(), tuiles2.begin(), tuiles2.end());
        
        if (nbJoueurs >= 3) {
            std::vector<TuileCite*> tuiles3 = creerTuiles3j();
            toutesLesTuiles.insert(toutesLesTuiles.end(), tuiles3.begin(), tuiles3.end());
        }
        
        if (nbJoueurs >= 4) {
            std::vector<TuileCite*> tuiles4 = creerTuiles4j();
            toutesLesTuiles.insert(toutesLesTuiles.end(), tuiles4.begin(), tuiles4.end());
        }
        melangerTuiles(toutesLesTuiles);
        return toutesLesTuiles;
    }

    void melangerTuiles(std::vector<TuileCite*>& tuiles) {
        // mélange des tuiles après leurs insertion dans le vecteur
        std::random_device rd;
        auto rng = std::default_random_engine(rd());
        std::shuffle(std::begin(tuiles), std::end(tuiles), rng);
    }
}