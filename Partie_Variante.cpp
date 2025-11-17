#include "Partie_Variante.h"
#include "HexCons_Carr_Quart_Place.h"
#include "GameExcep_Enums.h"
#include <iostream>
#include <algorithm> 
#include <limits>

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


    void Partie::initialiserTuiles() {
        // Remplir la pioche - Cette fonction peut changer par la suite lorsqu'on aura toutes nos tuiles

        for (size_t i = 0; i < 60; ++i) {
            HexagoneConstruction* h1 = new Quartier(i*3 + 1, Type::HABITATION);
            HexagoneConstruction* h2 = new Carriere(i*3 + 2);
            HexagoneConstruction* h3 = new Place(i*3 + 3, Type::MARCHE, 1);
            
        
            TuileCite* tuile = new TuileCite(i, h1, h2, h3);
            
            pioche->ajouterTuile(tuile); 
        }

        // Tuiles de départ
        for (size_t i = 0; i < joueurs.size(); ++i) {
            Place* centre = new Place(1000 + i, Type::MARCHE);
            Carriere* c1 = new Carriere(1001 + i);
            Carriere* c2 = new Carriere(1002 + i);
            Carriere* c3 = new Carriere(1003 + i);
            
            TuileDepart* td = new TuileDepart(i, centre, c1, c2, c3);
            tuilesDepart.push_back(td);
            joueurs[i]->getCite()->initialiserCite(td);
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
        cout << "\n=== Tour de " << joueur->getNom() << " ===" << endl;
        remplirChoixTuile();

        if (choixTuile->getNombreTuiles() == 0) {
            cout << "Plus de tuiles disponibles." << endl;
            return;
        }

        // Affichage rivière
        const auto& dispos = choixTuile->getTuilesDisponibles();
        for (size_t i = 0; i < dispos.size(); ++i) {
            cout << "[" << i << "] Tuile #" << dispos[i]->getId() 
                 << " (Cout: " << choixTuile->calculerCout(i) << ")" << endl;
        }

        // Choix tuile et paiement
        size_t index;
        while(true) {
            cout << "Choix (0-" << dispos.size()-1 << ") : ";
            if (cin >> index && index < dispos.size()) {
                size_t coutP = choixTuile->calculerCout(index);
                if (joueur->peutPayerPierres(coutP)) {
                    joueur->retirerPierres(coutP);
                    break;
                } else {
                    cout << "Pas assez de pierres (" << joueur->getNbPierres() << " pierres dispo)." << endl;
                }
            } else {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "Entree invalide." << endl;
            }
        }

        TuileCite* tuile = choixTuile->choisirTuile(index);
        
        // Logique de placement
        cout << "Votre Cite :" << endl;
        joueur->getCite()->afficher(cout);

        // Génération des coups
        auto coups = joueur->getCite()->genererCoupsValides(*tuile);
        
        if (coups.empty()) {
            cout << "Aucun placement possible ! La tuile est defaussee." << endl;
            delete tuile;
            return;
        }

        cout << "Coups possibles :" << endl;
        for (size_t i = 0; i < coups.size(); ++i) {
            cout << (i+1) << ". Pos: (q=" << coups[i].ancre.getQ() << ", r=" << coups[i].ancre.getR() << ") "
                 << "Rot: " << coups[i].rotation 
                 << (coups[i].recouvrement ? " [Recouvrement]" : " [Sol]") << endl;
        }

        size_t choixCoup;
        while (true) {
            cout << "Votre placement (1-" << coups.size() << ") : ";
            if (cin >> choixCoup && choixCoup >= 1 && choixCoup <= coups.size()) {
                break;
            }
            cin.clear(); cin.ignore(1000, '\n');
            cout << "Invalide." << endl;
        }

        // CORRECTION IMPORTANTE : Appel direct à la Cité
        joueur->getCite()->placerTuile(tuile, coups[choixCoup-1]);
        cout << "Tuile placee avec succes." << endl;
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