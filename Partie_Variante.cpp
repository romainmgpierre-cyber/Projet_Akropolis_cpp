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
#include <random> 
#include <chrono>
#include "Sauvegarde.h";

using namespace std;
namespace Akropolis {

    
    extern std::vector<TuileCite*> creerToutesTuiles(int nbJoueurs);
    extern std::vector<TuileDepart*> creer4TuileDepart();
    extern void AfficherResultats(Partie& partie); 


    string getEtiquettePourTuile(HexagoneConstruction* hex) {
        if (dynamic_cast<Carriere*>(hex)) return "CAR";
        else if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
            string nom = q->getType().getNom().substr(0, 3);
            for (auto & c: nom) c = toupper(c);
            return nom;
        } 
        else if (Place* p = dynamic_cast<Place*>(hex)) {
            string nom = p->getType().getNom().substr(0, 3);
            for (auto & c: nom) c = toupper(c);
            return "P " + nom + " " + to_string(p->getNbEtoile()) + "*";
        }
        return "???";
    }

    void afficherTuileCiteASCII(TuileCite* t, ostream& os) {
        
        const int N = 5; 
        const int H = 2; 
        
        int step_x = N + H;
        int step_y = 2 * H;
        int odd_offset = H;

        int width_px = 2 * step_x + N + 2 * H + 15; 
        int height_px = 2 * step_y + H + odd_offset + 2;

        vector<string> buffer(height_px, string(width_px, ' '));

        struct Pos { int col; int lig; int id; };

        
        vector<Pos> positions = {
            {1, 0, 0}, // Hex 0 - Haut Droite
            {0, 1, 1}, // Hex 1 - Bas Gauche
            {1, 1, 2}  // Hex 2 - Bas Droite
        };

        for (const auto& p : positions) {
            string label = getEtiquettePourTuile(t->getHexagone(p.id));
            int start_x = p.col * step_x + 1;
            int start_y = p.lig * step_y;
            
            if (p.col % 2 != 0) start_y += odd_offset;

            // Toit
            for (int i = 0; i < N; ++i) buffer[start_y][start_x + H + i] = '_';

            // Pentes
            for (int k = 0; k < H; ++k) {
                int y_top = start_y + 1 + k;
                buffer[y_top][start_x + H - 1 - k] = '/';
                buffer[y_top][start_x + H + N + k] = '\\';
                int y_bot = start_y + H + 1 + k;
                buffer[y_bot][start_x + k] = '\\';
                buffer[y_bot][start_x + (2 * H + N) - 1 - k] = '/';
                if (k == H - 1) for (int i = 0; i < N; ++i) buffer[y_bot][start_x + H + i] = '_';
            }

            // Texte intérieur
            int center_y = start_y + H;
            int center_x = start_x + H + (N / 2);
            int txt_start = center_x - (label.length() / 2);
            if (txt_start >= 0) {
                for (size_t i = 0; i < label.length(); ++i) {
                    if (center_y < height_px && txt_start + i < (size_t)width_px)
                        buffer[center_y][txt_start + i] = label[i];
                }
            }

        
            if (p.id == 0) {
                string arrow = "<-- Ref";
                int arrow_x = start_x + (2*H + N) + 2; 
                for(size_t i=0; i<arrow.length(); ++i) {
                    if(center_y < height_px && arrow_x + i < (size_t)width_px)
                        buffer[center_y][arrow_x + i] = arrow[i];
                }
            }
        }

        for (const auto& line : buffer) {
            size_t end = line.find_last_not_of(" ");
            if (end != string::npos) os << line.substr(0, end + 1) << "\n";
        }
        os << endl;
    }

void afficherTuileCiteASCII(TuileCite* t, int rotation, ostream& os) {
        
        const int N = 5; 
        const int H = 2; 
        
        int step_x = N + H;
        int step_y = 2 * H;
        int odd_offset = H;

        int width_px = 2 * step_x + N + 2 * H + 20; 
        int height_px = 2 * step_y + H + odd_offset + 2;

        vector<string> buffer(height_px, string(width_px, ' '));

        struct Pos { int col; int lig; int id; };
        vector<Pos> positions;

        
        bool formeA = (rotation >= 3); 

        if (formeA) {
            // Forme A (Pyramide)
            // L'Ancre (Hex 0) est en haut
            positions = {
                {1, 0, 0}, 
                {0, 1, 1}, 
                {1, 1, 2}  
            };
        } else {
            // Forme V (Inversée)
            // L'Ancre (Hex 0) est en bas
            positions = {
                {0, 1, 0}, 
                {0, 0, 1}, 
                {1, 0, 2}  
            };
        }

        for (const auto& p : positions) {
         
            string label = to_string(p.id) + "." + getEtiquettePourTuile(t->getHexagone(p.id));
            
            int start_x = p.col * step_x + 1;
            int start_y = p.lig * step_y;
            
            if (p.col % 2 != 0) start_y += odd_offset;

            // Toit
            for (int i = 0; i < N; ++i) buffer[start_y][start_x + H + i] = '_';

            // Pentes
            for (int k = 0; k < H; ++k) {
                int y_top = start_y + 1 + k;
                buffer[y_top][start_x + H - 1 - k] = '/';
                buffer[y_top][start_x + H + N + k] = '\\';
                int y_bot = start_y + H + 1 + k;
                buffer[y_bot][start_x + k] = '\\';
                buffer[y_bot][start_x + (2 * H + N) - 1 - k] = '/';
                if (k == H - 1) for (int i = 0; i < N; ++i) buffer[y_bot][start_x + H + i] = '_';
            }

            // Texte intérieur
            int center_y = start_y + H;
            int center_x = start_x + H + (N / 2);
            int txt_start = center_x - (label.length() / 2);
            if (txt_start >= 0) {
                for (size_t i = 0; i < label.length(); ++i) {
                    if (center_y < height_px && txt_start + i < (size_t)width_px)
                        buffer[center_y][txt_start + i] = label[i];
                }
            }

            // Marqueur ANCRE sur l'hexagone 0
            if (p.id == 0) {
                string arrow = "<-- [ANCRE]";
                int arrow_x = start_x + (2*H + N) + 2; 
                for(size_t i=0; i<arrow.length(); ++i) {
                    if(center_y < height_px && arrow_x + i < (size_t)width_px)
                        buffer[center_y][arrow_x + i] = arrow[i];
                }
            }
        }

        for (const auto& line : buffer) {
            size_t end = line.find_last_not_of(" ");
            if (end != string::npos) os << line.substr(0, end + 1) << "\n";
        }
        os << endl;
    }

    void afficherTuileCiteASCII(const ChoixTuile* choixTuile, size_t index, ostream& os) {
        const auto& dispos = choixTuile->getTuilesDisponibles();
        if (index >= dispos.size()) return;
        os << "Tuile selectionnee (Rotation 0) :" << endl;
        afficherTuileCiteASCII(dispos[index], 0, os);
    }



    Partie::Partie(size_t id, ModeJeu mode) 
        : id(id), mode(mode), etat(EtatPartie::NON_DEMARREE), 
          joueurActuelIndex(0), nbTuilesParTour(3) 
    {
        pioche = new Pioche(1); 
        choixTuile = new ChoixTuile(1); 
        variantes.push_back(Variante("Architectes", "Habitations : Si la valeur du groupe >= 10, points doubles."));
        variantes.push_back(Variante("Commercants", "Marches : Si adjacent a une Place Marche, points doubles."));
        variantes.push_back(Variante("Sentinelles", "Casernes : Si 3 ou 4 cases vides adjacentes, points doubles."));
        variantes.push_back(Variante("Pretres", "Temples : Si en hauteur (niveau 2+), points doubles."));
        variantes.push_back(Variante("Jardiniers", "Jardins : Si adjacent a un Lac (vide entoure), points doubles."));
    }

    Partie::~Partie() {
        delete pioche;
        delete choixTuile;
        for (Joueur* j : joueurs) delete j;
        for (TuileDepart* td : tuilesDepart) delete td;
    }

    void Partie::ajouterJoueur(const string& nom, int numeroJoueur, bool estIA) {
        if (etat != EtatPartie::NON_DEMARREE) {
            throw GameException("Impossible d'ajouter : la partie a deja commence.");
        }
        Joueur* nouveauJoueur = new Joueur(nom, numeroJoueur, estIA);
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

    
    void Partie::initialiserTuiles() {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rng(seed);

        //Création de toutes les tuiles
        std::vector<TuileCite*> toutesLesTuiles = creerToutesTuiles(joueurs.size());
        std::shuffle(toutesLesTuiles.begin(), toutesLesTuiles.end(), rng);
        
        delete pioche; 
        pioche = new Pioche(1);
        
       
        pioche->organiserPiles(toutesLesTuiles, joueurs.size());

        // Ajustement du marché de départ
        // Pour N joueurs, on commence avec N+1 tuiles au centre.
        size_t tailleCible = joueurs.size() + 1; 
        
        // Le calcul des piles laisse toujours un peu plus que nécessaire, on supprime le surplus
        while (toutesLesTuiles.size() > tailleCible) {
            delete toutesLesTuiles.back(); 
            toutesLesTuiles.pop_back();   
        }

        // Remplissage du marché
        for (TuileCite* t : toutesLesTuiles) {
            choixTuile->ajouterTuile(t);
        }
        toutesLesTuiles.clear(); 

        
        for (TuileDepart* td : tuilesDepart) delete td;
        tuilesDepart.clear();
        
        std::vector<TuileDepart*> departsDisponibles = creer4TuileDepart();
        std::shuffle(departsDisponibles.begin(), departsDisponibles.end(), rng);
        
        if (departsDisponibles.size() < joueurs.size()) throw GameException("Tuiles depart manquantes");

        for (size_t i = 0; i < joueurs.size(); ++i) {
            tuilesDepart.push_back(departsDisponibles[i]);
            joueurs[i]->getCite()->initialiserCite(departsDisponibles[i]);

            if (joueurs[i]->isIA()) {
                joueurs[i]->ajouterPierres(1); // L'IA commence avec ses pierres bonus
            }
        }
        
        for (size_t i = joueurs.size(); i < departsDisponibles.size(); ++i) delete departsDisponibles[i];
    }

    void Partie::lancerPartie() {
        if (joueurs.empty()) throw GameException("Pas assez de joueurs.");
        configurerVariantes();
        initialiserTuiles();
        etat = EtatPartie::EN_COURS;
        joueurActuelIndex = 0;
        cout << "--- La Partie Commence ! ---" << endl;
        boucleDeJeu();
    }

    void Partie::boucleDeJeu() {
        while (etat == EtatPartie::EN_COURS) {
            Joueur* joueur = joueurs[joueurActuelIndex];
            gererTourJoueur(joueur);
            
            joueurActuelIndex = (joueurActuelIndex + 1) % joueurs.size();

            if (pioche->estVide() && choixTuile->getNombreTuiles() <= 1) {
                etat = EtatPartie::TERMINEE;
            }
        }
        AfficherResultats(*this);
    }

    void Partie::remplirChoixTuile() {
        if (choixTuile->getNombreTuiles() == 1) {
            if (!pioche->estVide()) {
                cout << "\n>>> LE MARCHE SE REAPPROVISIONNE ! (Ajout d'une pile) <<<" << endl;
                vector<TuileCite*> nouvellePile = pioche->prendreUnePile();
                choixTuile->ajouterPile(nouvellePile);
            } else {
                cout << "\n>>> Plus de piles ! Dernier tour. <<<" << endl;
            }
        }
    }

    void Partie::configurerVariantes() {
        bool choixTermine = false;
        
        while (!choixTermine) {
            cout << "\n========================================" << endl;
            cout << "       CONFIGURATION DES VARIANTES      " << endl;
            cout << "========================================" << endl;
            cout << "Cochez les variantes que vous voulez activer pour cette partie :" << endl;
            cout << "(Les scores s'adapteront automatiquement)\n" << endl;

            // Afficher la liste avec l'état actuel [X] ou [ ]
            for (size_t i = 0; i < variantes.size(); ++i) {
                cout << (i + 1) << ". [" << (variantes[i].estActive() ? "X" : " ") << "] " 
                     << variantes[i].getNom() << " : " << variantes[i].getDescription() << endl;
            }

            cout << "\n----------------------------------------" << endl;
            cout << "Tapez le numero pour activer/desactiver." << endl;
            cout << "Tapez '0' ou 'ok' pour VALIDER et LANCER LA PARTIE." << endl;
            cout << "Votre choix : ";

            string input;
            cin >> input;

            // Validation de fin
            if (input == "0" || input == "ok" || input == "OK") {
                choixTermine = true;
                cout << "Configuration validee !\n" << endl;
            } 
            else {
                
                stringstream ss(input);
                int choix = 0;
                if (ss >> choix) {
                    if (choix >= 1 && choix <= (int)variantes.size()) {
                        
                        if (variantes[choix - 1].estActive()) {
                            variantes[choix - 1].desactiver();
                        } else {
                            variantes[choix - 1].activer();
                        }
                    } else {
                        cout << "Numero invalide." << endl;
                    }
                } else {
                    cout << "Entree invalide." << endl;
                }
            }
        }
    }

    //gestion tour des joueurs

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
        
        // pour mode solo
        if (joueur->isIA()) {
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

            cout << "-> L'IA choisit la tuile n°" << indexChoisi 
                 << " (Cout: " << choixTuile->calculerCout(indexChoisi) << ")." << endl;

            TuileCite* tuile = choixTuile->choisirTuile(joueur, indexChoisi);
            joueur->recupererTuileIA(tuile);
            TableauScore scoreHelper;
            int scoreActuel = scoreHelper.calculerScoreIA(*joueur, this->difficulte);
            cout << "   SCORE ACTUEL IA : " << scoreActuel << " points\n" << endl;
            return; 
        }

        // avec des vrais joueurs

        // Affichage Cité
        cout << "\n--- VOTRE CITE ACTUELLE ---" << endl;
        cout << "\033[?7l"; 
        joueur->getCite()->afficherGraphique(cout);
        cout << "\033[?7h"; 
        cout << "---------------------------" << endl;

        TableauScore calculateurScore;
        calculateurScore.afficherDetailsScore(*joueur, cout);
        
        bool tourValide = false; 

        while (!tourValide) {
            
            
            cout << "\n--- MARCHE DES TUILES (" << pioche->getNbPilesRestantes() << " piles en reserve) ---" << endl;
            const auto& dispos = choixTuile->getTuilesDisponibles();
            for (size_t i = 0; i < dispos.size(); ++i) {
                 size_t coutP = choixTuile->calculerCout(i);
                 cout << "[" << i << "] Cout: " << coutP << "p | ";
                 for(int k=0; k<3; ++k) cout << getEtiquettePourTuile(dispos[i]->getHexagone(k)) << " ";
                 cout << endl;
            }
            cout << "-------------------------" << endl;

            
            size_t index = 0;
            size_t coutPierrePourAnnulation = 0;

            while(true) {
                cout << "Quelle tuile acheter ? (0-" << dispos.size()-1 << ", 'q' quitter) : ";
                string input;
                cin >> input;

                if (input == "q" || input == "Q") throw PartieAnnulee("Partie annulee.");

                stringstream ss(input);
                if ((ss >> index) && index < dispos.size()) {
                    size_t coutP = choixTuile->calculerCout(index);
                    if (joueur->peutPayerPierres(coutP)) {
                        coutPierrePourAnnulation = coutP; 
                        break;
                    } else {
                        cout << "Pas assez de pierres !" << endl;
                    }
                } else {
                    cout << "Entree invalide." << endl;
                }
            }

          TuileCite* tuile = choixTuile->choisirTuile(joueur, index);

            cout << endl;
            // Affichage initial (Rotation 0 => Forme V)
            afficherTuileCiteASCII(tuile, 0, cout);

            //Placement
            auto tousCoups = joueur->getCite()->genererCoupsValides(*tuile);

            if (tousCoups.empty()) {
                cout << "Aucun placement possible ! Tuile defaussee." << endl;
                delete tuile;
                return; 
            }

            set<int> rotationsUniques;
            for (const auto& coup : tousCoups) rotationsUniques.insert(coup.rotation);
            vector<int> rotationsVec(rotationsUniques.begin(), rotationsUniques.end());

            cout << "--- Rotations possibles ---" << endl;
            for(size_t i = 0; i < rotationsVec.size(); ++i) {
                // Petit helper textuel pour dire la forme
                string forme = (rotationsVec[i] >= 3) ? "(Forme A)" : "(Forme V)";
                cout <<"["<< (i+1) << "] Rotation " << rotationsVec[i] << " " << forme << endl;
            }

            int rotationChoisie = -1;
            size_t indexRotation = 0;
            bool retourArriere = false;

            while(true) {
                cout << "Choisir orientation (1-" << rotationsVec.size() << ", 'r' retour) : ";
                string input;
                cin >> input;
                
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

            if (retourArriere) {
                cout << ">>> Annulation..." << endl;
                joueur->ajouterPierres(coutPierrePourAnnulation); 
                choixTuile->remettreTuile(tuile, index); 
                continue; 
            }

            {
                cout << "\n--- Apercu Rotation " << rotationChoisie << " ---" << endl;
                TuileCite* apercu = tuile->clone();

                for(int i=0; i < (rotationChoisie % 3); ++i) apercu->rotationHoraire();
                
                afficherTuileCiteASCII(apercu, rotationChoisie, cout);
                delete apercu;
            }

            // Choix Position
            vector<Cite::CoupPossible> coupsFiltres;
            for (const auto& coup : tousCoups) {
                if (coup.rotation == rotationChoisie) coupsFiltres.push_back(coup);
            }

            cout << "\nEmplacements possibles :" << endl;
            for (size_t i = 0; i < coupsFiltres.size(); ++i) {
                cout <<"["<<(i+1)<<"] (q=" << coupsFiltres[i].ancre.getQ()
                     << ", r=" << coupsFiltres[i].ancre.getR() << ") "
                     << (coupsFiltres[i].recouvrement ? "[ETAGE]" : "[SOL]")
                     << endl;
            }

            size_t choixCoup = 0;
            while (true) {
                cout << "Votre placement (1-" << coupsFiltres.size() << ", 'r' retour) : ";
                string input;
                cin >> input;
                
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

            if (retourArriere) {
                cout << ">>> Annulation..." << endl;
                joueur->ajouterPierres(coutPierrePourAnnulation); 
                choixTuile->remettreTuile(tuile, index); 
                continue; 
            }

            int pierresGagnees = joueur->getCite()->placerTuile(tuile, coupsFiltres[choixCoup-1]);
            if (pierresGagnees > 0) {
                joueur->ajouterPierres(pierresGagnees);
                cout << "--> " << pierresGagnees << " pierres gagnees !" << endl;
            } else {
                cout << "--> Tuile placee." << endl;
            }

            // demande si le joueur veux enrgistrer la partie ?
            std:string reponse;
            while (true) {
                cout<<"Voulez-enregistrer la partie ? (oui/non):";
                cin>>reponse;
                if (reponse != "oui" && reponse != "non") {
                    std::cout<<"Reponse invalide";
                }else {
                    break;
                }
            }
            //lancer enregistrement
            if (reponse == "oui") {
                try {
                    if (this->getConfig() != nullptr) {
                        Sauvegarde::EnregistrerPartie(*(this->getConfig()));
                    } else {
                        std::cout << "Erreur : La configuration n'est pas liée à la partie." << std::endl;
                    }
                    cout << "Sauvegarde effectuee avec succes !" << endl;
                } catch (const exception& e) {
                    cerr << "Erreur lors de l'enregistrement : " << e.what() << endl;
                }
            }
            tourValide = true; 
        }
    }
    vector<Variante> Partie::getVariantesActives() const {
        vector<Variante> actives;
        for (const auto& v : variantes) {
            if (v.estActive()) {
                actives.push_back(v);
            }
        }
        return actives;
    }

}