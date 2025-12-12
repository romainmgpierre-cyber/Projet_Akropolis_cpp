#ifndef AKROPOLIS_SAUVEGARDE_H
#define AKROPOLIS_SAUVEGARDE_H

#ifndef UNTITLED_SAUVEGARDE_H
#define UNTITLED_SAUVEGARDE_H
namespace Sauvegarde {
    //lancement du menu partie
    void MenuSauvegardes();

    // verifie si le fichier existe dans le répertoire
    bool ExistanceFichier (const std::string& nomFichier);

    // Affiche toute les infos d'une sauvegarde
    void afficherInfo();

    // Renvois un vecteur contenant tout les nom des fichier de sauvegarde
    std::vector<std::string> recupNomFichiers();

    // Sert a afficher les différentes sauvergardes disponibles
    void afficherNomParties(std::vector<std::string>& nomsParties);

    // Enregistrer une partie
    void EnregistrerPartie(std::string&);
#endif //UNTITLED_SAUVEGARDE_H

#endif //AKROPOLIS_SAUVEGARDE_H
}
