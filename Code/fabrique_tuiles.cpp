#include "Pioche_ChoixTuile.h"
#include "GameExcep_Enums.h"
#include <algorithm>
#include <random>
namespace Akropolis {

    void melangerTuiles(std::vector<TuileCite*>& tuiles);
    
    std::vector<TuileCite*> creerTuiles2j() {
        std::vector<TuileCite*> tuiles2j; //uniquement les tuiles pour deux joueurs
        size_t idTuile = 1;
        size_t idHexagone = 1;
        
        //1
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::MARCHE, 2),
            new Carriere(idHexagone++),
            1
        ));

        //2
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::MARCHE, 2),
            new Quartier(idHexagone++, Type::JARDIN),
            2
        ));

        
        //3
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::TEMPLE),
            3
        ));

        //4
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::MARCHE),
            new Carriere(idHexagone++),
            4
        ));
        
        //5
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
            new Place(idHexagone++, Type::CASERNE, 2),
            5
        ));
                
        //6
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2),
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            6
        ));
        
        //7
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            7
        ));
        //8
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Quartier(idHexagone++, Type::HABITATION),
            8
        ));
        //9
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            9
        ));
        //10
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::JARDIN),
            new Quartier(idHexagone++, Type::MARCHE),
            10
        ));
        //11
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::TEMPLE),
            11
        ));
        //12
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::HABITATION),
            12
        ));
        //13
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::HABITATION, 1),
            new Carriere(idHexagone++),
            13
        ));
        //14
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::JARDIN),
            14
        ));
        //15
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
            15
        ));
        //16
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::HABITATION, 1),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            16
        ));
        //17
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::JARDIN, 3),
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++),
            17
        ));
        //18
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++),
            18
        ));
        //19
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::CASERNE),
            new Carriere(idHexagone++),
            19
        ));
        //20
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::JARDIN),
            new Carriere(idHexagone++),
            20
        ));
        //21
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++),
            21
        ));
        //22
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::JARDIN, 3),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            22
        ));
        //23
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::JARDIN),
            23
        ));
        //24
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Carriere(idHexagone++),
            24
        ));
        //25
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Place(idHexagone++, Type::HABITATION, 1),
            new Quartier(idHexagone++, Type::CASERNE),
            25
        ));
        //26
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Carriere(idHexagone++),
            26
        ));
        //27
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Place(idHexagone++, Type::MARCHE, 2),
            new Carriere(idHexagone++),
            27
        ));
        //28
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Quartier(idHexagone++, Type::CASERNE),
            28
        ));
        //29
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Quartier(idHexagone++, Type::MARCHE),
            29
        ));
        //30
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::JARDIN, 3),
            new Carriere(idHexagone++),
            30
        ));
        //31
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::TEMPLE),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            31
        ));
        //32
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Place(idHexagone++, Type::MARCHE, 2),
            32
        ));
        //33
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Quartier(idHexagone++, Type::CASERNE),
            33
        ));
        //34
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::MARCHE),
            34
        ));
        //35
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::HABITATION, 1),
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::JARDIN),
            35
        ));
        //36
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::CASERNE),
            36
        ));
        //37
        tuiles2j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::HABITATION, 1),
            37
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
            new Quartier(idHexagone++, Type::HABITATION),
            38
        ));
        //39
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION),
            39
        ));
        //40
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::MARCHE, 2),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),40
        ));
        //41
       tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::JARDIN),
            new Quartier(idHexagone++, Type::CASERNE),
            41
        ));
        //42
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Place(idHexagone++, Type::HABITATION, 1),
            42
        ));
        //43
       tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
            43
        ));
        //44
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::HABITATION),
            44
        ));
        //45
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::JARDIN, 3),
            45
        ));
        //46
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            46
        ));
        //47
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::CASERNE, 2),
            new Quartier(idHexagone++, Type::HABITATION),
            47
        ));
        //48
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::TEMPLE, 2),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            48
        ));
        //49
        tuiles3j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::TEMPLE),
            49
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
            new Quartier(idHexagone++, Type::TEMPLE),
            50
        ));
        //51
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Place(idHexagone++, Type::HABITATION, 1),
            new Quartier(idHexagone++, Type::MARCHE),
            51
        ));
        //52
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION),
            52
        ));
        //53
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::JARDIN),
            53
        ));
        //54
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::MARCHE),
            new Quartier(idHexagone++, Type::HABITATION),
            new Place(idHexagone++, Type::TEMPLE, 2),
            54
        ));
        //55
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Place(idHexagone++, Type::CASERNE, 2),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            55
        ));
        //56
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::CASERNE),
            new Carriere(idHexagone++),
            new Carriere(idHexagone++),
            56
        ));
        //57
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::TEMPLE),
            new Quartier(idHexagone++, Type::HABITATION),
            57
        ));
        //58
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::CASERNE),
            58
        ));
        //59
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::CASERNE),
            new Quartier(idHexagone++, Type::HABITATION),
            59
        ));
        //60
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Carriere(idHexagone++),
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::HABITATION),
            60
        ));
        //61
        tuiles4j.push_back(new TuileCite(
            idTuile++,
            new Quartier(idHexagone++, Type::HABITATION),
            new Quartier(idHexagone++, Type::MARCHE),
            new Place(idHexagone++, Type::JARDIN, 3),
            61
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