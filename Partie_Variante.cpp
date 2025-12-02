#include "Partie_Variante.h"
#include "HexCons_Carr_Quart_Place.h"
#include "GameExcep_Enums.h"
#include "Score.h"  
#include <iostream>
#include <algorithm> 
#include <limits>
#include <sstream>
#include <set>
#include <vector> 
#include <random> // AJOUTÉ : Pour std::shuffle et default_random_engine
#include <chrono> // AJOUTÉ : Pour la graine (seed) basée sur le temps

using namespace std;
namespace Akropolis {

    // --- DÉCLARATIONS EXTERNES POUR LA FABRIQUE ---
    extern std::vector<TuileCite*> creerToutesTuiles(int nbJoueurs);
    extern std::vector<TuileDepart*> creer4TuileDepart();
    // ----------------------------------------------

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

    void Partie::initialiserTuiles() {
        // --- PRÉPARATION DU GÉNÉRATEUR ALÉATOIRE (C++17) ---
        // On utilise l'horloge système pour générer une graine unique
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rng(seed);

        // 1. Création des tuiles via la fabrique
        std::vector<TuileCite*> toutesLesTuiles = creerToutesTuiles(joueurs.size());
        
        // 2. Mélange aléatoire avec std::shuffle
        std::shuffle(toutesLesTuiles.begin(), toutesLesTuiles.end(), rng);
        
        // 3. Remplissage de la Pioche
        delete pioche; 
        pioche = new Pioche(1, toutesLesTuiles.size());
        
        for (TuileCite* t : toutesLesTuiles) {
            pioche->ajouterTuile(t);
        }
        toutesLesTuiles.clear(); // La pioche possède maintenant les pointeurs

        // 4. Gestion des Tuiles de Départ
        for (TuileDepart* td : tuilesDepart) delete td;
        tuilesDepart.clear();
        
        std::vector<TuileDepart*> departsDisponibles = creer4TuileDepart();
        
        // Mélange aléatoire des départs
        std::shuffle(departsDisponibles.begin(), departsDisponibles.end(), rng);
        
        if (departsDisponibles.size() < joueurs.size()) {
             throw GameException("Nombre de tuiles de depart insuffisant pour le nombre de joueurs.");
        }
        
        for (size_t i = 0; i < joueurs.size(); ++i) {
            tuilesDepart.push_back(departsDisponibles[i]);
            joueurs[i]->getCite()->initialiserCite(departsDisponibles[i]);
        }
        
        // Nettoyage des tuiles de départ inutilisées
        for (size_t i = joueurs.size(); i < departsDisponibles.size(); ++i) {
            delete departsDisponibles[i];
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
        if (Place* p = dynamic_cast<Place*>(hex)) {
            string nomType = p->getType().getNom().substr(0, 3);
            string upperNom = "";
            for(char c : nomType) upperNom += toupper(c);

            os << "[P " << upperNom << " ";
            for(size_t i = 0; i < p->getNbEtoile(); ++i) {
                os << "*";
            }
            if ( p->getNbEtoile() == 1) {
                os << "  ";
            }else if (p->getNbEtoile() == 2) {
                os << " ";
            }
            os << "]";

        } else if (dynamic_cast<Carriere*>(hex)) {
            os << "[   CAR   ]";

        } else if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
            string nomType = q->getType().getNom().substr(0, 3);
            string upperNom = "";
            for(char c : nomType) upperNom += toupper(c);

            os << "[   " << upperNom << "   ]";
        }
    }


    void afficherTuileDansLaRiviere(const ChoixTuile* choixTuile, size_t index, ostream& os) {
        const auto& dispos = choixTuile->getTuilesDisponibles();
        TuileCite* t = dispos[index];
        size_t coutPierre = choixTuile->calculerCout(index);

        os << "[" << index << "]. Tuile #" << t->getId()
        << " (Cout: " << coutPierre << "p) : ";
        
        afficherHexagoneVisuel(t->getHexagone(0), os);
        os << "-";
        afficherHexagoneVisuel(t->getHexagone(1), os);
        os << "-";
        afficherHexagoneVisuel(t->getHexagone(2), os);
        os << endl;
    }

    void afficherTuileCiteASCII(const ChoixTuile* choixTuile, size_t index, ostream& os) {
        const auto& dispos = choixTuile->getTuilesDisponibles();
        TuileCite* t = dispos[index];

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
            
            joueurActuelIndex = (joueurActuelIndex + 1) % joueurs.size();

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

        // --- Choix tuile ---
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
                
                if (joueur->peutPayerPierres(coutP)) {
                    // Vérification uniquement, pas de paiement ici !
                    afficherTuileCiteASCII(choixTuile, index, cout);
                    break; 
                } else {
                    cout << "Pas assez de pierres ! (Coût: " << coutP 
                         << ", Vous avez: " << joueur->getNbPierres() << ")" << endl;
                }
            } else {
                cout << "Entree invalide." << endl;
            }
        }

        TuileCite* tuile = choixTuile->choisirTuile(joueur, index);

        // --- Logique de Placement ---

        auto tousCoups = joueur->getCite()->genererCoupsValides(*tuile);

        if (tousCoups.empty()) {
            cout << "Aucun placement possible ! La tuile est defaussee." << endl;
            delete tuile;
            return;
        }

        set<int> rotationsUniques;
        for (const auto& coup : tousCoups) {
            rotationsUniques.insert(coup.rotation);
        }

        cout << "\n--- Orientations disponibles (Rotations de contenu/forme) ---" << endl;
        vector<int> rotationsVec(rotationsUniques.begin(), rotationsUniques.end());
        for(size_t i = 0; i < rotationsVec.size(); ++i) {
            cout <<"["<< (i+1) << "]"<<". Rotation: " << rotationsVec[i] << endl;
        }
        cout << "---------------------------------------------------------" << endl;
        
        cout << "Votre Cite :" << endl;
        joueur->getCite()->afficher(cout);
        
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

        vector<Cite::CoupPossible> coupsFiltres;
        for (const auto& coup : tousCoups) {
            if (coup.rotation == rotationChoisie) {
                coupsFiltres.push_back(coup);
            }
        }

        cout << "\nCoups possibles pour la Rotation " << rotationChoisie << " :" << endl;
        for (size_t i = 0; i < coupsFiltres.size(); ++i) {
            cout << (i+1) << ". Position (q=" << coupsFiltres[i].ancre.getQ()
                 << ", r=" << coupsFiltres[i].ancre.getR() << ") "
                 << (coupsFiltres[i].recouvrement ? " [Recouvrement H:" : " [Sol H:")
                 << coupsFiltres[i].hauteur << "]" << endl;
        }

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