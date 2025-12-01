#include "Partie_Variante.h"
#include "HexCons_Carr_Quart_Place.h"
#include "GameExcep_Enums.h"
#include "Score.h"  // Ajout pour l'affichage du score
#include <iostream>
#include <algorithm> 
#include <limits>
#include <sstream>
#include <set>

using namespace std;
namespace Akropolis {


    // Constructeur & Destructeur


    Partie::Partie(size_t id, ModeJeu mode) 
        : id(id), mode(mode), etat(EtatPartie::NON_DEMARREE), 
          joueurActuelIndex(0), nbTuilesParTour(3) 
    {
        pioche = new Pioche(1, 60);
        choixTuile = new ChoixTuile(1); 
    }

    Partie::~Partie() {
        delete pioche;
        delete choixTuile;
        for (Joueur* j : joueurs) delete j;
        for (TuileDepart* td : tuilesDepart) delete td;
    }


    // Gestion Joueurs


    void Partie::ajouterJoueur(const string& nom) {
        if (etat != EtatPartie::NON_DEMARREE) {
            throw GameException("Impossible d'ajouter : la partie a deja commence.");
        }
        Joueur* nouveauJoueur = new Joueur(nom);
        joueurs.push_back(nouveauJoueur);
    }

    void Partie::retirerJoueur(const string& nom) {
        for (auto it = joueurs.begin(); it != joueurs.end(); ++it) {
            if ((*it)->getNom() == nom) {
                delete *it;
                joueurs.erase(it);
                return;
            }
        }
        throw GameException("Joueur non trouve.");
    }


    // Initialisation

    // Ces fonctions viennent de fabrique_tuiles.cpp
    extern std::vector<TuileCite*> creerToutesTuiles(int nbJoueurs);
    extern std::vector<TuileDepart*> creer4TuileDepart();


    
    void Partie::initialiserTuiles() {
        // --- PROBLÈME RÉSOLU ICI : UTILISATION DE LA FABRIQUE ---

        // 1. Créer la liste de Tuiles Cité en fonction du nombre de joueurs
        std::vector<TuileCite*> toutesLesTuiles = creerToutesTuiles(joueurs.size());
        
        // 2. Remplacer l'ancien objet Pioche par un nouveau rempli avec les vraies tuiles
        // Suppression de l'ancienne pioche (pour la sécurité, même si le destructeur de Partie le fait)
        delete pioche; 
        
        // Crée une nouvelle Pioche et lui transfère la propriété des Tuiles Cité
        pioche = new Pioche(1, toutesLesTuiles.size());
        
        // Remplir la Pioche avec les tuiles de la fabrique
        for (TuileCite* t : toutesLesTuiles) {
            pioche->ajouterTuile(t);
        }

        // Videz le vecteur temporaire pour ne pas appeler delete deux fois
        toutesLesTuiles.clear();


        // --- GESTION DES TUILES DE DÉPART (Début du plateau) ---

        // 3. Créer et affecter les 4 tuiles de départ
        // Suppression de l'ancien contenu de tuilesDepart (si existant)
        for (TuileDepart* td : tuilesDepart) delete td;
        tuilesDepart.clear();
        
        tuilesDepart = creer4TuileDepart();
        
        // 4. Initialiser la Cité de chaque joueur avec sa tuile de départ
        if (tuilesDepart.size() < joueurs.size()) {
             throw GameException("Nombre de tuiles de depart insuffisant pour le nombre de joueurs.");
        }
        
        for (size_t i = 0; i < joueurs.size(); ++i) {
            // Note: Joueur[i] prend la tuile Depart[i]
            // Le joueur a déjà sa cité (initialisée dans son constructeur)
            joueurs[i]->getCite()->initialiserCite(tuilesDepart[i]);
        }
    }

    void Partie::lancerPartie() {
        if (joueurs.empty()) throw GameException("Pas assez de joueurs.");
        initialiserTuiles();
        etat = EtatPartie::EN_COURS;
        joueurActuelIndex = 0;
        cout << "--- La Partie Commence ! ---" << endl;
        boucleDeJeu();
    }


    void afficherHexagoneVisuel(HexagoneConstruction* hex, ostream& os) {
    // La couleur est remplacée par le nom du type ou du quartier
        if (Place* p = dynamic_cast<Place*>(hex)) {
            // Ex: [P MAR *]
            string nomType = p->getType().getNom().substr(0, 3);
            string upperNom = "";
            for(char c : nomType) upperNom += toupper(c);

            os << "[P " << upperNom << " ";
            // Ajout d'une chaîne d'étoiles
            for(size_t i = 0; i < p->getNbEtoile(); ++i) {
                os << "*";
            }
            //pour que la taille du str soit la même dans tout les cas
            if ( p->getNbEtoile() == 1) {
                os << "  ";
            }else if (p->getNbEtoile() == 2) {
                os << " ";
            }
            os << "]";

        } else if (dynamic_cast<Carriere*>(hex)) {
            // Ex: [CAR]
            os << "[   CAR   ]";

        } else if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
            // Ex: [HAB]
            string nomType = q->getType().getNom().substr(0, 3);
            string upperNom = "";
            for(char c : nomType) upperNom += toupper(c);

            os << "[   " << upperNom << "   ]";
        }
    }


    // Helper pour afficher une tuile complète dans la rivière
    void afficherTuileDansLaRiviere(const ChoixTuile* choixTuile, size_t index, ostream& os) {
        // 1. Récupérer la tuile et les données
        const auto& dispos = choixTuile->getTuilesDisponibles();
        TuileCite* t = dispos[index];
        size_t coutPierre = choixTuile->calculerCout(index);

        os << "[" << index << "]. Tuile #" << t->getId()
        << " (Cout: " << coutPierre << "p) : ";
        
        // 2. Afficher les 3 hexagones de la tuile triangulaire côte à côte
        // Nous utilisons la fonction d'affichage visuel existante.
        afficherHexagoneVisuel(t->getHexagone(0), os);
        os << "-";
        afficherHexagoneVisuel(t->getHexagone(1), os);
        os << "-";
        afficherHexagoneVisuel(t->getHexagone(2), os);
        os << endl;
    }

    void afficherTuileCiteASCII(const ChoixTuile* choixTuile, size_t index, ostream& os) {
        // 1. Récupérer la tuile et les données
        const auto& dispos = choixTuile->getTuilesDisponibles();
        TuileCite* t = dispos[index];

        // afichage de la tuile hexagonese
        os<<"La tuile choisie en rotation 0 : \n";
        os<<"            ________ \n";
        os<<"           /        \\  \n";
        os<<"          /          \\ \n";
        os<<"  ,------(";
        afficherHexagoneVisuel(t->getHexagone(0), os);
        os<<")\n";
        os<<" /        \\         / \n";
        os<<"/";
        afficherHexagoneVisuel(t->getHexagone(2), os);
        os<<"\\______/ \n";
        os<<"\\          /        \\ \n";
        os<<" \\        /          \\ \n";
        os<<"  \\______(";
        afficherHexagoneVisuel(t->getHexagone(1), os);
        os<<") \n";
        os<<"          \\         / \n";
        os<<"           \\_______/ \n";
        os << endl;
    }
    
    // Boucle Principale

    void Partie::boucleDeJeu() {
        while (etat == EtatPartie::EN_COURS) {
            Joueur* joueur = joueurs[joueurActuelIndex];
            gererTourJoueur(joueur);
            
            // Joueur suivant
            joueurActuelIndex = (joueurActuelIndex + 1) % joueurs.size();

            // Fin de partie ?
            if (pioche->estVide() && choixTuile->getNombreTuiles() == 0) {
                etat = EtatPartie::TERMINEE;
            }
        }
        cout << "\n--- FIN DE LA PARTIE ---" << endl;
    }

    void Partie::remplirChoixTuile() {
        while (choixTuile->getNombreTuiles() <= 1 && !pioche->estVide()) {
            TuileCite* t = pioche->piocher();
            choixTuile->ajouterTuile(t);
        }
    }


    // Tour du Joueur

    void Partie::gererTourJoueur(Joueur* joueur) {
        cout << "\n========================================" << endl;
        cout << "   TOUR DE : " << joueur->getNom() ;
        cout << " (" << joueur->getNbPierres() << " pierres)" << endl;
        cout << "========================================" << endl;
        
        // --- AFFICHAGE DU SCORE ---
        TableauScore calculateurScore;
        calculateurScore.afficherDetailsScore(*joueur, cout);
        cout << "========================================\n" << endl;

        remplirChoixTuile();

        if (choixTuile->getNombreTuiles() == 0) {
           cout << "Plus de tuiles disponibles." << endl;
           return;
        }
        cout << "\n--- Tuiles Disponibles ---" << endl;
        const auto& dispos = choixTuile->getTuilesDisponibles();

        for (size_t i = 0; i < dispos.size(); ++i) {
            afficherTuileDansLaRiviere(choixTuile, i, cout);
        }
        cout << "--------------------------" << endl;

        // --- Choix tuile avec option Quitter ---
        size_t index = 0;

        while(true) {
            cout << "Choix (0-" << dispos.size()-1 << ", ou 'q' pour quitter) : ";
            string input;
            cin >> input;

            if (input == "q" || input == "Q") {
                throw PartieAnnulee("Le joueur a quitté la partie.");
            }

            stringstream ss(input);
        if ((ss >> index) && index < dispos.size()) {
            size_t coutP = choixTuile->calculerCout(index);
            
            // --- CORRECTION ICI ---
            if (joueur->peutPayerPierres(coutP)) {
                // NE PAS FAIRE CA : joueur->retirerPierres(coutP); 
                // On vérifie juste qu'il PEUT payer pour valider la saisie.
                // Le paiement réel se fera dans choixTuile->choisirTuile() juste après.
                
                afficherTuileCiteASCII(choixTuile, index, cout);
                break; 
            } else {
                // Ce message s'affiche correctement sans planter
                cout << "Pas assez de pierres ! (Coût: " << coutP 
                     << ", Vous avez: " << joueur->getNbPierres() << ")" << endl;
            }
            // ----------------------
        } else {
            cout << "Entree invalide." << endl;
        }
        }

        TuileCite* tuile = choixTuile->choisirTuile(joueur, index);

        // --- DÉBUT DE LA NOUVELLE LOGIQUE DE PLACEMENT ---

        // 1. Génération de TOUS les coups possibles (toutes rotations confondues)
        auto tousCoups = joueur->getCite()->genererCoupsValides(*tuile);

        if (tousCoups.empty()) {
            cout << "Aucun placement possible ! La tuile est defaussee." << endl;
            delete tuile;
            return;
        }

        // 2. Isoler les rotations uniques valides
        set<int> rotationsUniques;
        for (const auto& coup : tousCoups) {
            rotationsUniques.insert(coup.rotation);
        }

        // Afficher les rotations disponibles
        cout << "\n--- Orientations disponibles (Rotations de contenu/forme) ---" << endl;
        vector<int> rotationsVec(rotationsUniques.begin(), rotationsUniques.end());
        for(size_t i = 0; i < rotationsVec.size(); ++i) {
            cout <<"["<< (i+1) << "]"<<". Rotation: " << rotationsVec[i] << endl;
        }
        cout << "---------------------------------------------------------" << endl;
        
        // Afichage de la citée du joueur
        cout << "Votre Cite :" << endl;
        joueur->getCite()->afficher(cout);
        
        // 3. Choix de la rotation
        int rotationChoisie = -1;
        size_t indexRotation = 0;
        while(true) {
            cout << "Choix d'Orientation (1-" << rotationsVec.size() << ", ou 'q' pour quitter) : ";
            string input;
            cin >> input;

            if (input == "q" || input == "Q") {
                throw PartieAnnulee("Le joueur a quitté la partie.");
            }

            stringstream ss(input);
            if ((ss >> indexRotation) && indexRotation >= 1 && indexRotation <= rotationsVec.size()) {
                rotationChoisie = rotationsVec[indexRotation - 1];
                break;
            }
            cout << "Choix d'orientation invalide." << endl;
        }

        // 4. Filtrer les coups pour ne garder que la rotation choisie
        vector<Cite::CoupPossible> coupsFiltres;
        for (const auto& coup : tousCoups) {
            if (coup.rotation == rotationChoisie) {
                coupsFiltres.push_back(coup);
            }
        }

        // 5. Afficher les positions valides pour cette rotation
        cout << "\nCoups possibles pour la Rotation " << rotationChoisie << " :" << endl;
        for (size_t i = 0; i < coupsFiltres.size(); ++i) {
            // Affichage plus clair de la position (Ancre) et du type de placement
            cout << (i+1) << ". Position (q=" << coupsFiltres[i].ancre.getQ()
                 << ", r=" << coupsFiltres[i].ancre.getR() << ") "
                 << (coupsFiltres[i].recouvrement ? " [Recouvrement H:" : " [Sol H:")
                 << coupsFiltres[i].hauteur << "]" << endl;
        }

        // 6. Choix du placement final
        size_t choixCoup = 0;
        while (true) {
            cout << "Votre placement (1-" << coupsFiltres.size() << ", ou 'q' pour quitter) : ";
            string input;
            cin >> input;

            if (input == "q" || input == "Q") {
                throw PartieAnnulee("Le joueur a quitté la partie.");
            }

            stringstream ss(input);
            if ((ss >> choixCoup) && choixCoup >= 1 && choixCoup <= coupsFiltres.size()) {
                break;
            }
            cout << "Invalide." << endl;
        }

        // Le placement se fait sur la liste filtrée
        // Note: joueur->getCite()->placerTuile retourne les pierres gagnées
        int pierresGagnees = joueur->getCite()->placerTuile(tuile, coupsFiltres[choixCoup-1]);
        
        if (pierresGagnees > 0) {
            joueur->ajouterPierres(pierresGagnees);
            cout << "Tuile placee en hauteur ! " << pierresGagnees << " pierres gagnees !" << endl;
        } else {
            cout << "Tuile placee avec succes." << endl;
        }
    }

    void Partie::activerVariante(const string& nom) {
        for (auto& v : variantes) if (v.getNom() == nom) v.activer();
    }
    void Partie::desactiverVariante(const string& nom) {
        for (auto& v : variantes) if (v.getNom() == nom) v.desactiver();
    }
    vector<Variante> Partie::getVariantesActives() const {
        vector<Variante> actives;
        for (const auto& v : variantes) if (v.estActive()) actives.push_back(v);
        return actives;
    }
}