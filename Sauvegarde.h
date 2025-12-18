#ifndef AKROPOLIS_SAUVEGARDE_H
#define AKROPOLIS_SAUVEGARDE_H

#ifndef UNTITLED_SAUVEGARDE_H
#define UNTITLED_SAUVEGARDE_H
namespace Sauvegarde {
    using namespace std;
    //lancement du menu partie
    void MenuSauvegardes();

    // verifie si le fichier existe dans le répertoire
    bool ExistanceFichier (const string& nomFichier);

    // Affiche toute les infos d'une sauvegarde
    void afficherInfo(std::string& nomFichierPartie);

    // Renvois un vecteur contenant tout les nom des fichier de sauvegarde
    vector<string> recupNomFichiers();

    // Sert a afficher les différentes sauvergardes disponibles
    void afficherNomParties(vector<string>& nomsParties);

    // Enregistrer une partie
    void EnregistrerPartie(Akropolis::Configuration& config);
#endif //UNTITLED_SAUVEGARDE_H

#endif //AKROPOLIS_SAUVEGARDE_H
}
