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
            //ça rentre dans cette boucle pourtant j'ai un fichier texte nomé NomSauvegardes.txt
            // On retourne le vecteur vide
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

    void EnregistrerPartie() { ///*Akropolis::Configuration& config*/
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
        std::ofstream nouvFichier(nomPartie + ".txt");
        // verification de si la création à été un succée
        if (!nouvFichier.is_open()) {
            cout<<"\nerreur de création/ouverture du fichier sauvegarde";
        }
        //on écrit toute les infos de sauvegardes
        nouvFichier<<"information diverse et variables sur la sauvegarde de la partie";
        nouvFichier<<"tellement d'infos";
        nouvFichier<<"c'est trop biennnnnn!!!";
        nouvFichier.close();

        //on ajoute le nom de la sauvegarde dans le fichier NomSauvegardes.txt
        //ouverture du fichier NomSauvegardes.txt
        std::ofstream fichierNoms( NOM_FICHIER_SAUVERGARDE, std::ios::app);
        if (!fichierNoms.is_open()) {
            cout<<"\nerreur de création/ouverture du fichier";
        }
        fichierNoms<<"\n"<<nomPartie;
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
        afficherInfo(Nompartie);
    }
}