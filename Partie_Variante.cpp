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

    void Partie::ajouterJoueur(const string& nom, int numeroJoueur, bool estIA) {
        if (etat != EtatPartie::NON_DEMARREE) {
            throw GameException("Impossible d'ajouter : la partie a deja commence.");
        }
        Joueur* nouveauJoueur = new Joueur(nom, numeroJoueur, estIA); //Initialise le nb de pierres du joueur en fonction de son tour 
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
        
        // On utilise l'horloge système pour générer une graine unique
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rng(seed);

        // Création des tuiles via la fabrique
        std::vector<TuileCite*> toutesLesTuiles = creerToutesTuiles(joueurs.size());
        
        // Mélange aléatoire avec std::shuffle
        std::shuffle(toutesLesTuiles.begin(), toutesLesTuiles.end(), rng);
        
        // Remplissage de la Pioche
        delete pioche; 
        pioche = new Pioche(1, toutesLesTuiles.size());
        
        for (TuileCite* t : toutesLesTuiles) {
            pioche->ajouterTuile(t);
        }
        toutesLesTuiles.clear(); // La pioche possède maintenant les pointeurs

        // Gestion des Tuiles de Départ
        for (TuileDepart* td : tuilesDepart) delete td;
        tuilesDepart.clear();
        
        std::vector<TuileDepart*> departsDisponibles = creer4TuileDepart();
        
        // Mélange aléatoire des départs
        std::shuffle(departsDisponibles.begin(), departsDisponibles.end(), rng);
        
        if (departsDisponibles.size() < joueurs.size()) {
             throw GameException("Nombre de tuiles de depart insuffisant pour le nombre de joueurs.");
        }
        
        for (size_t i = 0; i < joueurs.size(); ++i) {
            
            
            // Configuration IA
            if (joueurs[i]->isIA()) {
                
                joueurs[i]->ajouterPierres(1); // L'IA commence avec 2 pierres, elle en a deja 1 par defaut
                
                // On ne donne pas la tuile de départ à l'IA car son score est compté virtuellement.
                // On la supprime simplement pour éviter les fuites de mémoire.
                delete departsDisponibles[i];
                 
            } else { // L'IA n'utilise pas initialiserCite() car elle ne construit pas.
                tuilesDepart.push_back(departsDisponibles[i]);
                joueurs[i]->getCite()->initialiserCite(departsDisponibles[i]);
            }
        }
        
        // Nettoyage des tuiles de départ inutilisées
        for (size_t i = joueurs.size(); i < departsDisponibles.size(); ++i) {
            delete departsDisponibles[i];
        }

        
    }

    void Partie::lancerPartie() {
        if (joueurs.empty()) throw GameException("Pas assez de joueurs.");
        initialiserTuiles();
        remplirChoixTuile();
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

        os << "[" << index + 1 << "] Tuile #" << t->getId()
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
    // La boucle continue tant que le marché n'est pas rempli à son maximum 
    while (choixTuile->getNombreTuiles() < ChoixTuile::getMaxTuiles()) {
        if (pioche->estVide()) {
            // Le jeu continue si la pioche est vide, mais le marché ne sera pas complet.
            cout << "Attention : Pioche épuisée. Le marché de tuiles n'est pas complet." << endl;
            break; 
        }
        // Pioche la tuile du dessus de la Pioche
        TuileCite* nouvelleTuile = pioche->piocher();
        // L'ajoute au ChoixTuile (elle sera la tuile de coût 0, 1, 2 ou 3 selon l'ordre)
        choixTuile->ajouterTuile(nouvelleTuile);
    }
}


    // Tour du Joueur

void Partie::gererTourJoueur(Joueur* joueur) {
        cout << "\n========================================" << endl;
        cout << "   TOUR DE : " << joueur->getNom();
        if (joueur->isIA()) cout << " (Illustre Architecte)";
        cout << " (" << joueur->getNbPierres() << " pierres)" << endl;
        cout << "========================================" << endl;

        remplirChoixTuile();

        if (choixTuile->getNombreTuiles() == 0) {
           cout << "Plus de tuiles disponibles." << endl;
           return;
        }
        
        // --- LOGIQUE IA (Inchangée) ---
        if (joueur->isIA()) {
            // ... (Gardez votre code IA existant ici, je ne le répète pas pour alléger la réponse)
            // ... 
            // Si vous avez besoin que je remette le code IA complet, dites-le moi.
            // Pour l'instant, je me concentre sur la partie Humain.
             const auto& dispos = choixTuile->getTuilesDisponibles();
            int indexChoisi = -1;
            int coutMin = 999;

            for (size_t i = 0; i < dispos.size(); ++i) {
                size_t coutTuile = choixTuile->calculerCout(i);
                if (dispos[i]->contientPlace() && joueur->peutPayerPierres(coutTuile)) {
                    if ((int)coutTuile < coutMin) {
                        coutMin = coutTuile;
                        indexChoisi = i;
                    }
                }
            }
            if (indexChoisi == -1) indexChoisi = 0; 

            cout << "-> L'Illustre Architecte choisit la tuile n°" << indexChoisi 
                 << " (Cout: " << choixTuile->calculerCout(indexChoisi) << " pierres)." << endl;

            TuileCite* tuile = choixTuile->choisirTuile(joueur, indexChoisi);
            joueur->recupererTuileIA(tuile);
            TableauScore scoreHelper;
            int scoreActuel = scoreHelper.calculerScoreIA(*joueur, this->difficulte);
            cout << "   SCORE ACTUEL IA : " << scoreActuel << " points\n" << endl;
            return; 
        }

        // --- LOGIQUE JOUEUR HUMAIN (MODIFIÉE POUR UNDO) ---
        if (!joueur->isIA()) {
            cout << "\n--- VOTRE CITE ACTUELLE ---" << endl;
            // Désactive l'auto-wrap pour éviter que le terminal casse le dessin
            cout << "\033[?7l"; 
            joueur->getCite()->afficherGraphique(cout);
            cout << "\033[?7h"; // Réactive l'auto-wrap
            cout << "---------------------------" << endl;
        }
        // Affichage du score
        TableauScore calculateurScore;
        calculateurScore.afficherDetailsScore(*joueur, cout);
        cout << "========================================\n" << endl;

        bool tourValide = false; // Tant que ce n'est pas true, le joueur recommence son choix

        while (!tourValide) {
            
            // 1. Affichage de la rivière
            cout << "\n--- Tuiles Disponibles ---" << endl;
            const auto& dispos = choixTuile->getTuilesDisponibles();
            for (size_t i = 0; i < dispos.size(); ++i) {
                afficherTuileDansLaRiviere(choixTuile, i, cout);
            }
            cout << " Vous avez actuellement " << joueur->getNbPierres() << " pierres." << endl;
            cout << "--------------------------" << endl;

            // 2. Choix de la tuile
            size_t index = 0;
            size_t coutPierrePourAnnulation = 0; // On garde le coût pour rembourser si besoin

            while(true) {
                cout << "Choix (1-" << dispos.size() << ", ou 'q' pour quitter) : ";
                string input;
                cin >> input;

                if (input == "q" || input == "Q") throw PartieAnnulee("Le joueur a quitte la partie.");

                stringstream ss(input);
                if ((ss >> index) && index < dispos.size()) {
                    size_t coutP = choixTuile->calculerCout(index);
                    if (joueur->peutPayerPierres(coutP)) {
                        afficherTuileCiteASCII(choixTuile, index, cout);
                        coutPierrePourAnnulation = coutP; // On sauvegarde le coût
                        break;
                    } else {
                        cout << "Pas assez de pierres ! (Cout: " << coutP 
                             << ", Vous avez: " << joueur->getNbPierres() << ")" << endl;
                    }
                } else {
                    cout << "Entree invalide." << endl;
                }
            }

            // 3. Achat temporaire (Retrait pierres + Tuile)
            // La fonction choisirTuile retire les pierres du joueur.
            TuileCite* tuile = choixTuile->choisirTuile(joueur, index);


            // 4. Phase de Placement (avec option Retour)
            auto tousCoups = joueur->getCite()->genererCoupsValides(*tuile);

            if (tousCoups.empty()) {
                cout << "Aucun placement possible ! La tuile est defaussee." << endl;
                delete tuile;
                return; // Cas rare, on ne peut pas annuler ici car la tuile est perdue
            }

            // --- Choix Rotation ---
            set<int> rotationsUniques;
            for (const auto& coup : tousCoups) rotationsUniques.insert(coup.rotation);
            vector<int> rotationsVec(rotationsUniques.begin(), rotationsUniques.end());

            cout << "\n--- Orientations disponibles ---" << endl;
            for(size_t i = 0; i < rotationsVec.size(); ++i) {
                cout <<"["<< (i+1) << "] Rotation " << rotationsVec[i] << endl;
            }
            

            cout << "\n(Tapez 'r' pour retourner au choix des tuiles)" << endl;

            int rotationChoisie = -1;
            size_t indexRotation = 0;
            bool retourArriere = false;

            while(true) {
                cout << "Choix d'Orientation (1-" << rotationsVec.size() << ", 'r' retour, 'q' quitter) : ";
                string input;
                cin >> input;
                
                if (input == "q" || input == "Q") throw PartieAnnulee("Le joueur a quitte.");
                
                // GESTION DU RETOUR
                if (input == "r" || input == "R") {
                    retourArriere = true;
                    break;
                }

                stringstream ss(input);
                if ((ss >> indexRotation) && indexRotation >= 1 && indexRotation <= rotationsVec.size()) {
                    rotationChoisie = rotationsVec[indexRotation - 1];
                    break;
                }
                cout << "Invalide." << endl;
            }

            // Si retour demandé à l'étape rotation
            if (retourArriere) {
                cout << ">>> Annulation du choix de tuile..." << endl;
                joueur->ajouterPierres(coutPierrePourAnnulation); // Remboursement
                choixTuile->remettreTuile(tuile, index); // Remise en place
                continue; // Retour au début du while(!tourValide)
            }


            // --- Choix Position ---
            vector<Cite::CoupPossible> coupsFiltres;
            for (const auto& coup : tousCoups) {
                if (coup.rotation == rotationChoisie) coupsFiltres.push_back(coup);
            }

            cout << "\nEmplacements possibles pour la Rotation " << rotationChoisie << " :" << endl;
            for (size_t i = 0; i < coupsFiltres.size(); ++i) {
                cout <<"["<<(i+1)<<"] Position (q=" << coupsFiltres[i].ancre.getQ()
                     << ", r=" << coupsFiltres[i].ancre.getR() << ") "
                     << (coupsFiltres[i].recouvrement ? "[HAUTEUR]" : "[SOL]")
                     << endl;
            }

            size_t choixCoup = 0;
            while (true) {
                cout << "Votre placement (1-" << coupsFiltres.size() << ", 'r' retour, 'q' quitter) : ";
                string input;
                cin >> input;
                
                if (input == "q" || input == "Q") throw PartieAnnulee("Le joueur a quitte.");
                
                // GESTION DU RETOUR
                if (input == "r" || input == "R") {
                    retourArriere = true;
                    break;
                }

                stringstream ss(input);
                if ((ss >> choixCoup) && choixCoup >= 1 && choixCoup <= coupsFiltres.size()) {
                    break;
                }
                cout << "Invalide." << endl;
            }

            // Si retour demandé à l'étape placement
            if (retourArriere) {
                cout << ">>> Annulation du choix de tuile..." << endl;
                joueur->ajouterPierres(coutPierrePourAnnulation); // Remboursement
                choixTuile->remettreTuile(tuile, index); // Remise en place
                continue; // Retour au début du while(!tourValide)
            }

            // 5. Validation finale
            int pierresGagnees = joueur->getCite()->placerTuile(tuile, coupsFiltres[choixCoup-1]);
            if (pierresGagnees > 0) {
                joueur->ajouterPierres(pierresGagnees);
                cout << "Tuile placee en hauteur ! " << pierresGagnees << " pierres gagnees !" << endl;
            } else {
                cout << "Tuile placee avec succes." << endl;
            }
            
            tourValide = true; // Sortie de la boucle
        }
    }
}