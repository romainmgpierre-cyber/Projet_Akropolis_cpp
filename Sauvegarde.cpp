#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "Cite.h"
#include "MenuConfiguration.h"
#define TAILLE 3
#define NOM_FICHIER_SAUVERGARDE "NomSauvegardes.txt"

namespace Sauvegarde {
    bool ExistanceFichier (const std::string& nomFichier) {
        std::ifstream fichier(nomFichier);
        return fichier.good();

    }
    void afficherInfo(std::string& nomFichierPartie) {
        std::ifstream fichier(nomFichierPartie);
        std::string ligne;
        while (getline(fichier, ligne)) {
            cout<<ligne<<"\n";
        }
        fichier.close();
    }

    std::vector<std::string> recupNomFichiers() {
        std::vector<std::string> listeNomPartie = {};
        std::ifstream fichier(NOM_FICHIER_SAUVERGARDE);
        if (!fichier) {
            return listeNomPartie;
        }

        int numeroLigne = 0;
        std::string NomPartie;
        while (std::getline(fichier,  NomPartie)) {
            if (NomPartie != "") {
                listeNomPartie.push_back(NomPartie);
            }
        }
        int i = 0;
        return listeNomPartie;
    }
    void afficherNomParties(std::vector<std::string>& nomsParties) {
        //affiche toute les parties disponibles
        std::cout<<"\n\n******** Partie Disponibles ********";
        std::size_t i = 1;
        for (const auto& element : nomsParties) {
            std::cout <<"\nPartie "<<"["<<i<<"]"<<" : "<<element;
            i++;
        }
    }

    void EnregistrerPartie(Akropolis::Configuration& config) {
        string nomPartie;
        while (true) {
            std::cout<<"\nQuel nom voulez vous donner à la sauvegarde ? : ";
            std::cin>>nomPartie;
            if (nomPartie == "")
                std::cout<<"le nom de sauvergarde doit contenir au moins 1 caractère";
            else
                break;
        }
        // creation du fichier de sauvegarde
        std::ofstream nouvFichier(nomPartie + ".csv");

        // verification de si la création à été un succée
        if (!nouvFichier.is_open()) {
            cout<<"\nerreur de création/ouverture du fichier sauvegarde";
        }

        //on recupere déja toutes les infos sous forme de variables

        // on recupere les nom des joeurs
        std::vector<Akropolis::Joueur*> joueurPointeurs= config.getPartieCourante()->getJoueurs();
        //initialisation du vecteur qui stocke les noms
        std::vector<std::string> nomsJoueurs(joueurPointeurs.size());
        for (int i = 0; i < joueurPointeurs.size(); i++) {
            nomsJoueurs[i] = joueurPointeurs[i]->getNom();
        }

        // nombre de joueur
        int nbJoueur = config.getPartieCourante()->getNombreJoueurs();

        // information complémentaires (mode jeu, dificulté et nombre de tuile)
        //int nbTuileTour = config.getPartieCourante()->getNbTuileTour();
        Akropolis::ModeJeu modeDuJeu = config.getPartieCourante()->getMode();
        Akropolis::NiveauDifficulte dificulte = config.getPartieCourante()->getDifficulte();


        // On stocke d'abord ces infos dans le csv sous form variable puis infos dans la case d'a coté
        // On utilise la virgule comme séparateur CSV
        const char SEPARATEUR = ',';


        // infos générale de la partie
        nouvFichier << "Variable" << SEPARATEUR << "Valeur\n";
        nouvFichier << "Mode de Jeu" << SEPARATEUR << (modeDuJeu == Akropolis::ModeJeu::SOLO ? "SOLO" : "MULTIJOUEUR") << "\n";
        nouvFichier << "Difficulté (si SOLO)" << SEPARATEUR << (modeDuJeu == Akropolis::ModeJeu::SOLO ? std::to_string((int)dificulte) : "N/A") << "\n";
        nouvFichier << "Joueur Actuel Index" << SEPARATEUR << nbJoueur<< "\n";
        nouvFichier << "Nombre de Joueurs" << SEPARATEUR << nbJoueur<< "\n";

        // On écrit quel variante sont actives
        nouvFichier << "\n--- VARIANTES ---\n";
        nouvFichier << "Nom" << SEPARATEUR << "Active\n";
        for (const auto& v : config.getPartieCourante()->getVariantesDisponibles()) {
            nouvFichier << v.getNom() << SEPARATEUR << (v.estActive() ? "1" : "0") << "\n";
        }

        //pour chaque joueur on sauvegarde les infos
        nouvFichier << "\n--- JOUEURS ---\n";
        for (int i = 0; i < joueurPointeurs.size(); i++) {
            nouvFichier << "\n--- Joueur"<<i<< "---\n";
            nouvFichier << "nbPierres" << SEPARATEUR << joueurPointeurs[i]->getNbPierres();
            nouvFichier << "\nestIA" << SEPARATEUR << joueurPointeurs[i]->isIA();
            nouvFichier << "\n--- CiteJoueur"<<i<< "---\n";

            //Enregistrement des tuiles de chaques joueur

            // Format : TYPE_TUILE, ID_TUILE, Q, R, ROTATION, HAUTEUR
            nouvFichier << "TypeTuile" << SEPARATEUR << "ID" << SEPARATEUR << "Q" << SEPARATEUR << "R" << SEPARATEUR << "Rotation" << SEPARATEUR << "Hauteur\n";

            // On récupère le plateau du joueur (std::map<CoordHex, std::pair<HexagoneConstruction*, unsigned int>>)
            const auto& plateau = joueurPointeurs[i]->getCite()->getPlateau();

            // Note : Comme votre structure actuelle stocke des Hexagones individuels,
            // nous sauvegardons l'état de chaque case pour une reconstruction fidèle.
            for (const auto& casePlateau : plateau) {
                const Akropolis::CoordHex& coord = casePlateau.first;
                Akropolis::HexagoneConstruction* hex = casePlateau.second.first;
                unsigned int hauteur = casePlateau.second.second;

                // On identifie s'il s'agit d'une Carrière, d'un Quartier ou d'une Place
                // pour pouvoir reconstruire l'hexagone exact au chargement
                std::string typeHex = "INCONNU";
                int idContenu = -1; // ID de la tuile parente si disponible

                if (dynamic_cast<Akropolis::Carriere*>(hex)) typeHex = "CARRIERE";
                else if (auto* q = dynamic_cast<Akropolis::Quartier*>(hex)) typeHex = "QUARTIER_" + std::to_string((int)q->getType().getCouleur());
                else if (auto* p = dynamic_cast<Akropolis::Place*>(hex)) typeHex = "PLACE_" + std::to_string((int)p->getType().getCouleur()) + "_" + std::to_string(p->getNbEtoile());

                nouvFichier << typeHex << SEPARATEUR
                            << hex->getId() << SEPARATEUR  // Utilisation de l'ID fixe de l'hexagone
                            << coord.getQ() << SEPARATEUR
                            << coord.getR() << SEPARATEUR
                            << "N/A" << SEPARATEUR // La rotation est implicite par la position des hexagones
                            << hauteur << "\n";
            }
        }

        nouvFichier.close();

        //on ajoute le nom de la sauvegarde dans le fichier NomSauvegardes.txt
        //ouverture du fichier NomSauvegardes.txt
        std::ofstream fichierNoms( NOM_FICHIER_SAUVERGARDE, std::ios::app);
        if (!fichierNoms.is_open()) {
            cout<<"\nerreur de création/ouverture du fichier";
        }
        fichierNoms<<"\n"<<nomPartie; //es ce que je fai + "csv ici ?"
        fichierNoms.close();
    }
    std::string demandePartie( std::vector<std::string> listeNom) {
        while (true) {
            int numPartie;
            std::cout<<"\nQuel partie voulez vous charger ? numero de partie : ";
            std::cin>>numPartie;
            if (numPartie<1 || numPartie >listeNom.size()) {
                std::cout<<"\nCe numero de sauvegarde n'existe pas veuillez ressayer";
            }else {
                std::string nomPartie = listeNom[numPartie-1]+".txt";
                if (!ExistanceFichier(nomPartie)) {
                    std::cout<<"\nPartie non trouvé veuillez ressayer";
                }else {
                    return nomPartie;
                }
            }
        }
    }

    void chargerPartie(std::string& nomSauvegarde) {
        std::ifstream fichier(nomSauvegarde + "csv");
        if (!fichier.is_open()) {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier " << nomSauvegarde << std::endl;
            return;
        }
        std::string ligne;
        std::string sectionActuelle = "";
        const char SEP = ',';

        // Les variable a initialiser
        Akropolis::ModeJeu mode;
        Akropolis::NiveauDifficulte diff;
        int nbJoueurs = 0;

        // On accède au différentes partie du csv
        while (std::getline(fichier, ligne)) {
            if (ligne.empty() || ligne == "\r") continue;

            // Détection des changements de partie
            if (ligne.find("--- VARIANTES ---") != std::string::npos) { sectionActuelle = "VARIANTES"; continue; }
            if (ligne.find("--- JOUEURS ---") != std::string::npos) { sectionActuelle = "JOUEURS"; continue; }
            if (ligne.find("--- Joueur") != std::string::npos) { sectionActuelle = "INFOS_JOUEUR"; continue; }
            if (ligne.find("--- CiteJoueur") != std::string::npos) { sectionActuelle = "CITE"; continue; }

            std::stringstream ss(ligne);
            std::string cle, valeur;

            // Lecture des infos générales (au début du fichier)
            if (sectionActuelle == "") {
                if (std::getline(ss, cle, SEP) && std::getline(ss, valeur, SEP)) {
                    if (cle == "Mode de Jeu") mode = (valeur == "SOLO" ? Akropolis::ModeJeu::SOLO : Akropolis::ModeJeu::MULTIJOUEUR);
                    if (cle == "Difficulté (si SOLO)") diff = (Akropolis::NiveauDifficulte)std::stoi(valeur);
                    if (cle == "Nombre de Joueurs") nbJoueurs = std::stoi(valeur);
                }
            }

            // Lecture des Variantes
            else if (sectionActuelle == "VARIANTES") {
                if (std::getline(ss, cle, SEP) && std::getline(ss, valeur, SEP)) {
                    if (cle != "Nom") { // On ignore l'en-tête
                        bool active = (valeur == "1");
                        // Ici, vous devrez appliquer l'état à votre futur objet Partie
                    }
                }
            }

            // Lecture de la Cité (Hexagones)
            else if (sectionActuelle == "CITE") {
                std::string type, sId, sQ, sR, sRot, sH;
                if (std::getline(ss, type, SEP) && std::getline(ss, sId, SEP) &&
                    std::getline(ss, sQ, SEP) && std::getline(ss, sR, SEP) &&
                    std::getline(ss, sRot, SEP) && std::getline(ss, sH, SEP)) {

                    if (type == "TypeTuile") continue; // Sauter l'en-tête

                    int id = std::stoi(sId);
                    int q = std::stoi(sQ);
                    int r = std::stoi(sR);
                    int h = std::stoi(sH);

                    // LOGIQUE DE RECONSTRUCTION :
                    // 1. Retrouver l'hexagone original via son ID
                    // 2. Le placer dans la map du joueur aux coordonnées (q, r) à la hauteur h
                }
            }
        }

        fichier.close();
        std::cout << "chargement de partie terminée" << std::endl;
    }
    void MenuSauvegardes() {
        // recuperation des nom des parties
        std::vector<std::string> listeNomPartie =  recupNomFichiers();

        // Affichage des partie s'il y en a
        if (listeNomPartie.empty()) {
            // Si il n'y a pas de partie déjà sauvegardées
            std::cout<<"Il n'y a pas encore de partie sauvergadées";

            std::string reponse;
            while (reponse != "oui") {
                std::cout<<"\nTappez oui pour retourner au menu principal : ";
                std::cin>>reponse;
                if (reponse == "oui") {
                    break;
                }
            }
            return;
        }else {
            // S'il y a des parties sauvegardées on les affiches
            afficherNomParties(listeNomPartie);
        }

        // On demande maintenant quel partie on veut charger
        std::string Nompartie = demandePartie(listeNomPartie);

        //On peut maintenant initialiser la partie avec toute les infos dans le fichier
        chargerPartie(Nompartie);
    }
}