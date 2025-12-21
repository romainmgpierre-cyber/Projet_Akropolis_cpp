#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm> 
#include <vector>
#include "Cite.h"
#include "MenuConfiguration.h"
#include "Score.h"
#define TAILLE 3
#define NOM_FICHIER_SAUVERGARDE "NomSauvegardes.txt"
#include "fabrique_tuiles.h"
#include "Sauvegarde.h" 

using namespace std;

namespace Sauvegarde {

    bool ExistanceFichier (const std::string& nomFichier) {
        std::ifstream fichier(nomFichier);
        return fichier.good();
    }

    void afficherInfo(std::string& nomFichierPartie) {
        std::ifstream fichier(nomFichierPartie);
        std::string ligne;
        while (getline(fichier, ligne)) {
            cout << ligne << "\n";
        }
        fichier.close();
    }

    std::vector<std::string> recupNomFichiers() {
        std::vector<std::string> listeNomPartie = {};
        std::ifstream fichier(NOM_FICHIER_SAUVERGARDE);
        if (!fichier) return listeNomPartie;

        std::string NomPartie;
        while (std::getline(fichier, NomPartie)) {
            if (NomPartie != "") {
                listeNomPartie.push_back(NomPartie);
            }
        }
        return listeNomPartie;
    }

    void afficherNomParties(std::vector<std::string>& nomsParties) {
        std::cout << "\n\n******** Partie Disponibles ********";
        std::size_t i = 1;
        for (const auto& element : nomsParties) {
            std::cout << "\nPartie " << "[" << i << "]" << " : " << element;
            i++;
        }
    }

    void EnregistrerPartie(Akropolis::Configuration& config) {
        string nomPartie;
        while (true) {
            std::cout << "\nQuel nom voulez vous donner à la sauvegarde ? : ";
            std::cin >> nomPartie;
            if (nomPartie != "") break;
            std::cout << "Le nom de sauvegarde doit contenir au moins 1 caractère";
        }

        std::ofstream nouvFichier(nomPartie + ".csv");
        if (!nouvFichier.is_open()) {
            cout << "\nErreur de création/ouverture du fichier sauvegarde";
            return;
        }

        std::vector<Akropolis::Joueur*> joueurPointeurs = config.getPartieCourante()->getJoueurs();
        int nbJoueur = config.getPartieCourante()->getNombreJoueurs();
        

        size_t indexActuel = config.getPartieCourante()->getJoueurActuelIndex();

        size_t indexSauvegarde = (indexActuel + 1) % nbJoueur;
        Akropolis::ModeJeu modeDuJeu = config.getPartieCourante()->getMode();
        Akropolis::NiveauDifficulte dificulte = config.getPartieCourante()->getDifficulte();

        const char SEPARATEUR = ',';

        // Infos générales
        nouvFichier << "Variable" << SEPARATEUR << "Valeur\n";
        nouvFichier << "Mode de Jeu" << SEPARATEUR << (modeDuJeu == Akropolis::ModeJeu::SOLO ? "SOLO" : "MULTIJOUEUR") << "\n";
        nouvFichier << "Difficulté (si SOLO)" << SEPARATEUR << (modeDuJeu == Akropolis::ModeJeu::SOLO ? std::to_string((int)dificulte) : "N/A") << "\n";
        
       
        nouvFichier << "Joueur Actuel Index" << SEPARATEUR << indexSauvegarde << "\n"; 
        
        nouvFichier << "Nombre de Joueurs" << SEPARATEUR << nbJoueur << "\n";

        // Variantes
        nouvFichier << "\n--- VARIANTES ---\n";
        nouvFichier << "Nom" << SEPARATEUR << "Active\n";
        for (const auto& v : config.getPartieCourante()->getVariantesDisponibles()) {
            nouvFichier << v.getNom() << SEPARATEUR << (v.estActive() ? "1" : "0") << "\n";
        }

        // Infos pour chaque joueur
        nouvFichier << "\n--- JOUEURS ---\n";
        for (int i = 0; i < joueurPointeurs.size(); i++) {
            nouvFichier << "\n--- Joueur" << i << "---\n";
            nouvFichier << "nom" << SEPARATEUR << joueurPointeurs[i]->getNom() << "\n";
            nouvFichier << "nbPierres" << SEPARATEUR << joueurPointeurs[i]->getNbPierres() << "\n";
            nouvFichier << "estIA" << SEPARATEUR << (joueurPointeurs[i]->isIA() ? "1" : "0") << "\n";

            // Cite du joueur
            nouvFichier << "\n--- CiteJoueur" << i << "---\n";
            nouvFichier << "ID_PERSISTANT" << SEPARATEUR << "Q" << SEPARATEUR << "R" << SEPARATEUR
                        << "Hauteur" << SEPARATEUR << "Type" << SEPARATEUR << "Rotation\n";

            const auto& plateau = joueurPointeurs[i]->getCite()->getPlateau();
            const auto& tuiles = joueurPointeurs[i]->getCite()->getTuiles();

            // Trouver les hexagones de la tuile de départ
            std::set<Akropolis::HexagoneConstruction*> hexDestuiles;
            for (auto* t : tuiles) {
                for (int h = 0; h < 3; ++h) hexDestuiles.insert(t->getHexagone(h));
            }

            // Sauvegarder les hexagones du plateau qui ne font pas partie d'une TuileCite
            for (const auto& casePlateau : plateau) {
                Akropolis::HexagoneConstruction* hex = casePlateau.second.first;
                unsigned int hauteur = casePlateau.second.second;

                if (hexDestuiles.find(hex) == hexDestuiles.end() && hauteur == 1) {
                    nouvFichier << "DEPART" << SEPARATEUR
                                << casePlateau.first.getQ() << SEPARATEUR
                                << casePlateau.first.getR() << SEPARATEUR
                                << hauteur << SEPARATEUR
                                << "TUILE_DEPART" << SEPARATEUR
                                << "0\n";
                }
            }

            // On sauvegarde les tuiles citée posée
            for (auto* t : tuiles) {
                nouvFichier << t->getIdPersistant() << SEPARATEUR
                            << t->getAncre().getQ() << SEPARATEUR
                            << t->getAncre().getR() << SEPARATEUR
                            << t->getHauteur() << SEPARATEUR 
                            << "TUILE_CITE" << SEPARATEUR
                            << t->getRotation() << "\n";
            }
        }

        nouvFichier.close();

        // Ajout du nom dans le fichier référentiel
        std::ofstream fichierNoms(NOM_FICHIER_SAUVERGARDE, std::ios::app);
        if (fichierNoms.is_open()) {
            fichierNoms << nomPartie << "\n";
            fichierNoms.close();
        }

        std::cout << "Partie sauvegardée avec succès !\n";
    }

    std::string demandePartie(std::vector<std::string> listeNom) {
        while (true) {
            int numPartie;
            std::cout << "\nQuel partie voulez vous charger ? numero de partie : ";
            std::cin >> numPartie;
            if (numPartie < 1 || numPartie > listeNom.size()) {
                std::cout << "\nCe numero de sauvegarde n'existe pas veuillez ressayer";
            } else {
                std::string nomPartie = listeNom[numPartie-1] + ".csv";
                if (!ExistanceFichier(nomPartie)) {
                    std::cout << "\nPartie non trouvé veuillez ressayer";
                } else {
                    return nomPartie;
                }
            }
        }
    }

    void chargerPartie(std::string& nomSauvegarde, Akropolis::Configuration& config, Akropolis::Partie* partie) {
        std::string chemin = nomSauvegarde;
        if (chemin.find(".csv") == std::string::npos) chemin += ".csv";

        std::ifstream fichier(chemin);
        if (!fichier.is_open()) {
            std::cerr << "Erreur : Impossible d'ouvrir " << chemin << std::endl;
            return;
        }

        std::cout << ">>> Chargement de la partie...\n";

        // Initialisation de la réserve
        std::vector<Akropolis::TuileCite*> reserveTuiles = Akropolis::creerToutesTuiles(4); 
        std::map<size_t, Akropolis::TuileCite*> mapIdPersistantTuiles;
        for(auto* t : reserveTuiles) {
            mapIdPersistantTuiles[t->getIdPersistant()] = t;
        }

        std::string ligne, sectionActuelle = "";
        const char SEP = ',';

        Akropolis::ModeJeu modeCharge;
        Akropolis::NiveauDifficulte diffCharge = Akropolis::NiveauDifficulte::FACILE;
        int nbJoueursCharge = 0;
        

        int indexJoueurActuelCharge = 0;
        
        int indexJoueurCourant = -1;

        // Variables temporaires
        std::string nomJoueur = "";
        int nbPierres = 0;
        bool estIA = false;
        bool joueurPretACreer = false;

        struct TuileACharger {
            std::string idStr;
            int q, r;
            unsigned int hauteur;
            std::string type;
            int rotation;
        };
        std::vector<TuileACharger> tuilesEnAttente;

        auto creerEtAjouterJoueur = [&]() {
            if (!joueurPretACreer) return;
            partie->ajouterJoueur(nomJoueur, indexJoueurCourant + 1, estIA);
            
            Akropolis::Joueur* dernierJoueur = partie->getJoueurs().back();
            int pierresAAjouter = nbPierres - (indexJoueurCourant + 1); 
            if (pierresAAjouter > 0) dernierJoueur->ajouterPierres(pierresAAjouter);
            if (estIA) dernierJoueur->ajouterPierres(1);

            joueurPretACreer = false; 
        };

        auto chargerTuilesEnAttente = [&]() {
            if (partie->getJoueurs().empty() || tuilesEnAttente.empty()) return;

            // Tri par hauteur
            std::sort(tuilesEnAttente.begin(), tuilesEnAttente.end(), 
                [](const TuileACharger& a, const TuileACharger& b) {
                    if (a.hauteur != b.hauteur) return a.hauteur < b.hauteur;
                    return (a.type == "TUILE_DEPART") && (b.type != "TUILE_DEPART");
                }
            );

            Akropolis::Joueur* joueur = partie->getJoueurs().back();
            bool citeInitialisee = false;

            for (const auto& tuileInfo : tuilesEnAttente) {
                if (tuileInfo.type == "TUILE_DEPART" && !citeInitialisee) {
                    std::vector<Akropolis::TuileDepart*> deps = Akropolis::creer4TuileDepart();
                    if (!deps.empty()) {
                        joueur->getCite()->initialiserCite(deps[0]);
                        citeInitialisee = true;
                        for(size_t k = 1; k < deps.size(); ++k) delete deps[k];
                    }
                }
                else if (tuileInfo.type == "TUILE_CITE") {
                    if (!citeInitialisee) {
                        std::vector<Akropolis::TuileDepart*> deps = Akropolis::creer4TuileDepart();
                        if (!deps.empty()) {
                            joueur->getCite()->initialiserCite(deps[0]);
                            citeInitialisee = true;
                            for(size_t k = 1; k < deps.size(); ++k) delete deps[k];
                        }
                    }

                    size_t idPersistant = std::stoul(tuileInfo.idStr);
                    auto it = mapIdPersistantTuiles.find(idPersistant);

                    if (it != mapIdPersistantTuiles.end()) {
                        Akropolis::TuileCite* copie = it->second->clone();
                        joueur->getCite()->forcerPlacementTuile(
                            copie,
                            Akropolis::CoordHex(tuileInfo.q, tuileInfo.r),
                            tuileInfo.rotation,
                            tuileInfo.hauteur
                        );
                    }
                }
            }
            tuilesEnAttente.clear();
        };

        // --- BOUCLE DE LECTURE ---
        while (std::getline(fichier, ligne)) {
            if (ligne.empty() || ligne == "\r") continue;

            if (ligne.find("--- VARIANTES ---") != std::string::npos) { sectionActuelle = "VARIANTES"; continue; }
            if (ligne.find("--- JOUEURS ---") != std::string::npos) { sectionActuelle = "JOUEURS"; continue; }
            
            if (ligne.find("--- Joueur") != std::string::npos) {
                creerEtAjouterJoueur();     
                chargerTuilesEnAttente();   
                
                sectionActuelle = "INFOS_JOUEUR";
                indexJoueurCourant++;
                
                nomJoueur = ""; nbPierres = 0; estIA = false; joueurPretACreer = false;
                continue;
            }

            if (ligne.find("--- CiteJoueur") != std::string::npos) { sectionActuelle = "CITE"; continue; }

            std::stringstream ss(ligne);
            std::string cle, valeur;

            if (sectionActuelle == "") {
                if (std::getline(ss, cle, SEP) && std::getline(ss, valeur, SEP)) {
                    if (cle == "Mode de Jeu") modeCharge = (valeur == "SOLO" ? Akropolis::ModeJeu::SOLO : Akropolis::ModeJeu::MULTIJOUEUR);
                    if (cle == "Difficulté (si SOLO)" && valeur != "N/A") diffCharge = (Akropolis::NiveauDifficulte)std::stoi(valeur);
                    if (cle == "Nombre de Joueurs") nbJoueursCharge = std::stoi(valeur);
                    

                    if (cle == "Joueur Actuel Index") indexJoueurActuelCharge = std::stoi(valeur);
                }
            }
            else if (sectionActuelle == "VARIANTES") {
                if (std::getline(ss, cle, SEP) && std::getline(ss, valeur, SEP)) {
                    if (cle != "Nom" && cle != "Active") partie->setVarianteEtat(cle, (valeur == "1"));
                }
            }
            else if (sectionActuelle == "INFOS_JOUEUR") {
                if (std::getline(ss, cle, SEP) && std::getline(ss, valeur, SEP)) {
                    if (cle == "nom") nomJoueur = valeur;
                    if (cle == "nbPierres") nbPierres = std::stoi(valeur);
                    if (cle == "estIA") { estIA = (std::stoi(valeur) == 1); joueurPretACreer = true; }
                }
            }
            else if (sectionActuelle == "CITE") {
                std::vector<std::string> cols;
                std::string temp;
                while(std::getline(ss, temp, SEP)) cols.push_back(temp);

                if (cols.size() >= 6 && cols[0] != "ID_PERSISTANT" && cols[0] != "ID") {
                    TuileACharger tuileInfo;
                    tuileInfo.idStr = cols[0];
                    tuileInfo.q = std::stoi(cols[1]);
                    tuileInfo.r = std::stoi(cols[2]);
                    tuileInfo.hauteur = std::stoul(cols[3]);
                    tuileInfo.type = cols[4];
                    tuileInfo.rotation = std::stoi(cols[5]);
                    tuilesEnAttente.push_back(tuileInfo);
                }
            }
        }

        creerEtAjouterJoueur();     
        chargerTuilesEnAttente();   

        for (auto* t : reserveTuiles) delete t;

        config.setNbJoueurHumain(nbJoueursCharge);
        partie->setDifficulte(diffCharge);
        

        partie->setJoueurActuelIndex(indexJoueurActuelCharge);

        fichier.close();
        std::cout << ">>> Chargement terminé avec succès !\n";
    }

    void MenuSauvegardes() {
        std::vector<std::string> listeNomPartie = recupNomFichiers();

        if (listeNomPartie.empty()) {
            std::cout << "Il n'y a pas encore de partie sauvergadées";
            std::string reponse;
            while (reponse != "oui") {
                std::cout << "\nTappez oui pour retourner au menu principal : ";
                std::cin >> reponse;
                if (reponse == "oui") break;
            }
            return;
        } else {
            afficherNomParties(listeNomPartie);
        }

        std::string Nompartie = demandePartie(listeNomPartie);
        Akropolis::Configuration config;
        Akropolis::Partie* partie = new Akropolis::Partie(1, config.getMode());
        partie->setConfig(config);
        config.setPartie(partie);

        chargerPartie(Nompartie, config, partie);

        partie->reprendrePartie();
        AfficherResultats(*partie);
    }
}