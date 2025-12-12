#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#define TAILLE 3
#define NOM_FICHIER_SAUVERGARDE "NomSauvegardes.txt"


bool ExistanceFichier (const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);
    return fichier.good();
}
void afficherInfo() {
    std::cout<<"\n[infos]";
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
        listeNomPartie.push_back(NomPartie);
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

void EnregistrerPartie(std::string& NomFichierSauvegarde) {
    // creation du fichier de sauvegarde
    std::ofstream nouvFichier(NomFichierSauvegarde + ".txt");
    //if (nouvFichier.isopen())


    std::ifstream fichier(NomFichierSauvegarde);
    //il faut une verif avant d'appeler cette fonction pour verifier si ce nom de partie existe déjà

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
        }
        return;
    }else {
        // S'il y a des parties sauvegardées on les affiches
        afficherNomParties(listeNomPartie);
    }

    // On demande maintenant quel partie on veut charger
    while (true) {
        std::string nomPartie;
        std::cout<<"\nQuel partie voulez vous charger : ";
        std::cin>>nomPartie;

        if (ExistanceFichier(nomPartie)) {
            break;
        }else {
            std::cout<<"\nCe nom de sauvegarde n'existe pas veuillez ressayer";
        }
    }
    afficherInfo();
}