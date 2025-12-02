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

    void Partie::ajouterJoueur(const string& nom, bool estIA) {
        if (etat != EtatPartie::NON_DEMARREE) {
            throw GameException("Impossible d'ajouter : la partie a deja commence.");
        }
        Joueur* nouveauJoueur = new Joueur(nom, estIA);
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
        cout << "   TOUR DE : " << joueur->getNom();
        if (joueur->isIA()) cout << " (Illustre Architecte)";
        cout << " (" << joueur->getNbPierres() << " pierres)" << endl;
        cout << "========================================" << endl;

        // On s'assure que le chantier est plein au début du tour
        remplirChoixTuile();

        if (choixTuile->getNombreTuiles() == 0) {
           cout << "Plus de tuiles disponibles." << endl;
           return;
        }

        
        //                 LOGIQUE INTELLIGENCE ARTIFICIELLE
        
        if (joueur->isIA()) {
            const auto& dispos = choixTuile->getTuilesDisponibles();
            int indexChoisi = -1;
            int coutMin = 999;

            // cherche la tuile la moins chère contenant une Place
            for (size_t i = 0; i < dispos.size(); ++i) {
                size_t coutTuile = choixTuile->calculerCout(i);
                
                // Si la tuile a une place et que l'IA peut la payer et qu'elle est moins chère que la précédente trouvée
                if (dispos[i]->contientPlace() && joueur->peutPayerPierres(coutTuile)) {
                    if ((int)coutTuile < coutMin) {
                        coutMin = coutTuile;
                        indexChoisi = i;
                    }
                }
            }

            // Si aucune tuile avec Place n'est accessible, l'IA prend la 1ère du chantier
            if (indexChoisi == -1) {
                cout << "L'IA ne trouve pas de Place abordable, elle prend la premiere tuile." << endl;
                indexChoisi = 0; 
                // On suppose que l'IA peut toujours payer la 1ère tuile (coût 0 ou faible).
                
            }

            cout << "-> L'Illustre Architecte choisit la tuile n°" << indexChoisi 
                 << " (Cout: " << choixTuile->calculerCout(indexChoisi) << " pierres)." << endl;

            //Les pierres dépensées par l'IA retournent à la réserve (comportement par défaut)
            TuileCite* tuile = choixTuile->choisirTuile(joueur, indexChoisi);

            // L'IA ne place pas la tuile, elle la stocke simplement
            joueur->recupererTuileIA(tuile);
            return; 
        }

        
        //LOGIQUE JOUEUR HUMAIN
        

        // Affichage du score actuel
        TableauScore calculateurScore;
        calculateurScore.afficherDetailsScore(*joueur, cout);
        cout << "========================================\n" << endl;

        // Affichage de la Rivière (Chantier)
        cout << "\n--- Tuiles Disponibles ---" << endl;
        const auto& dispos = choixTuile->getTuilesDisponibles();
        for (size_t i = 0; i < dispos.size(); ++i) {
            afficherTuileDansLaRiviere(choixTuile, i, cout);
        }
        cout << " Vous avez actuellement " << joueur->getNbPierres() << " pierres." << endl;
        cout << "--------------------------" << endl;

        // Choix de la tuile
        size_t index = 0;

        while(true) {
            cout << "Choix (0-" << dispos.size()-1 << ", ou 'q' pour quitter) : ";
            string input;
            cin >> input;

            if (input == "q" || input == "Q") throw PartieAnnulee("Le joueur a quitte la partie.");

            stringstream ss(input);
            if ((ss >> index) && index < dispos.size()) {
                size_t coutP = choixTuile->calculerCout(index);
                
                if (joueur->peutPayerPierres(coutP)) {
                    // Confirmation visuelle (ASCII Art)
                    afficherTuileCiteASCII(choixTuile, index, cout);
                    
                    // Gestion du paiement en mode solo
                    // En solo les pierres payées vont à l'IA
                    if (mode == ModeJeu::SOLO && coutP > 0) {
                        for (Joueur* j : joueurs) {
                            if (j->isIA()) {
                                j->ajouterPierres(coutP);
                                cout << "--> (Mode Solo) Vos " << coutP << " pierre(s) sont donnees a l'Illustre Architecte." << endl;
                                break; 
                            }
                        }
                    }
                    
                    
                    break;
                } else {
                    cout << "Pas assez de pierres ! (Cout: " << coutP 
                         << ", Vous avez: " << joueur->getNbPierres() << ")" << endl;
                }
            } else {
                cout << "Entree invalide." << endl;
            }
        }

        // Achat de la tuile (retrait des pierres du joueur et récupération de l'objet)
        TuileCite* tuile = choixTuile->choisirTuile(joueur, index);

        //Placement de la tuile

        auto tousCoups = joueur->getCite()->genererCoupsValides(*tuile);

        if (tousCoups.empty()) {
            cout << "Aucun placement possible ! La tuile est defaussee." << endl;
            delete tuile;
            return;
        }

        // Tri et affichage des rotations disponibles
        set<int> rotationsUniques;
        for (const auto& coup : tousCoups) {
            rotationsUniques.insert(coup.rotation);
        }

        cout << "\n--- Orientations disponibles ---" << endl;
        vector<int> rotationsVec(rotationsUniques.begin(), rotationsUniques.end());
        for(size_t i = 0; i < rotationsVec.size(); ++i) {
            cout <<"["<< (i+1) << "]"<<". Rotation: " << rotationsVec[i] << endl;
        }
        
        cout << "Votre Cite :" << endl;
        joueur->getCite()->afficher(cout);
        
        // Sélection de l'orientation
        int rotationChoisie = -1;
        size_t indexRotation = 0;
        while(true) {
            cout << "Choix d'Orientation (1-" << rotationsVec.size() << ", ou 'q') : ";
            string input;
            cin >> input;
            if (input == "q" || input == "Q") throw PartieAnnulee("Le joueur a quitte.");

            stringstream ss(input);
            if ((ss >> indexRotation) && indexRotation >= 1 && indexRotation <= rotationsVec.size()) {
                rotationChoisie = rotationsVec[indexRotation - 1];
                break;
            }
            cout << "Invalide." << endl;
        }

        // Filtrage des coups selon la rotation choisie
        vector<Cite::CoupPossible> coupsFiltres;
        for (const auto& coup : tousCoups) {
            if (coup.rotation == rotationChoisie) {
                coupsFiltres.push_back(coup);
            }
        }

        cout << "\nEmplacements possibles pour la Rotation " << rotationChoisie << " :" << endl;
        for (size_t i = 0; i < coupsFiltres.size(); ++i) {
            cout << (i+1) << ". Position (q=" << coupsFiltres[i].ancre.getQ()
                 << ", r=" << coupsFiltres[i].ancre.getR() << ") "
                 << (coupsFiltres[i].recouvrement ? " [Recouvrement H:" : " [Sol H:")
                 << coupsFiltres[i].hauteur << "]" << endl;
        }

        // Sélection de la position finale
        size_t choixCoup = 0;
        while (true) {
            cout << "Votre placement (1-" << coupsFiltres.size() << ", ou 'q') : ";
            string input;
            cin >> input;
            if (input == "q" || input == "Q") throw PartieAnnulee("Le joueur a quitte.");

            stringstream ss(input);
            if ((ss >> choixCoup) && choixCoup >= 1 && choixCoup <= coupsFiltres.size()) {
                break;
            }
            cout << "Invalide." << endl;
        }

        // Application du coup et gain de pierres éventuel
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