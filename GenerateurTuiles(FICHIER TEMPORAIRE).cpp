//################################################
//Ce fichier est temporaire et sert à générer des tuiles pour que l'on puisse tester
//le placement en attendant d'avoir les vrais tuiles
//################################################

#include "Akropolis.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>       // Pour std::mt19937
#include <ctime>        // Pour time()
#include <algorithm>    // Pour std::shuffle
#include <stdexcept>    // Pour std::runtime_error
#include <set>          // Pour garantir l'unicité
#include <map>          // Pour la logique de génération
#include <functional>   // Pour std::function

// Utilisation des namespaces comme dans vos fichiers
using namespace std;
using namespace Akropolis;

// --- Variables globales pour la génération ---

// Générateur de nombres aléatoires Mersenne Twister
std::mt19937 gen;

// Compteur global pour assurer un ID unique à chaque hexagone
size_t global_hexagon_id = 1;

// --- Helpers de génération ---

// Pour représenter un "type" atomique d'hexagone
// et savoir comment le construire.
struct HexagoneStub {
    string signature; // Pour l'unicité (ex: "Q_BLEU", "P_ROUGE_2", "C")
    function<HexagoneConstruction*()> create; // Lambda pour créer l'hexagone
    bool estPlace;

    HexagoneStub(string sig, function<HexagoneConstruction*()> fn, bool p)
        : signature(sig), create(fn), estPlace(p) {}
};

// Pour représenter une composition de tuile unique avant sa création
struct TuileComposition {
    // On stocke les signatures triées pour l'unicité
    // (Bleu, Rouge, Carriere) == (Rouge, Bleu, Carriere)
    std::multiset<string> signatureSet; 
    
    // On stocke les stubs pour la construction
    HexagoneStub* h1_stub;
    HexagoneStub* h2_stub;
    HexagoneStub* h3_stub;

    // Surcharge de l'opérateur < pour pouvoir mettre les compos dans un set
    bool operator<(const TuileComposition& other) const {
        return signatureSet < other.signatureSet;
    }
};

/**
 * @brief Génère la liste complète des 61 TuileCite uniques pour la pioche.
 * Respecte les contraintes : 20 Place, 41 non-Place, toutes uniques.
 *
 * @return Un vecteur de pointeurs vers les TuileCite générées.
 */
vector<TuileCite*> genererTuilesPourPioche() {
    
    // --- 1. Définir les 21 "briques" atomiques d'hexagones ---
    vector<HexagoneStub> stubsAtomiques;
    stubsAtomiques.reserve(21);

    // 1.1 La Carrière (1 type)
    stubsAtomiques.emplace_back("C", []{ 
        return new Carriere(global_hexagon_id++); 
    }, false);

    // 1.2 Les Quartiers (5 types)
    const vector<const Type*> typesQuartier = {
        &Type::HABITATION, &Type::MARCHE, &Type::CASERNE, 
        &Type::TEMPLE, &Type::JARDIN
    };
    for (const Type* t : typesQuartier) {
        stubsAtomiques.emplace_back(
            "Q_" + t->getNom(), // Utilise le nom pour unicité
            [t]{ return new Quartier(global_hexagon_id++, *t); },
            false
        );
    }

    // 1.3 Les Places (5 types * 3 étoiles = 15 types)
    for (const Type* t : typesQuartier) {
        for (size_t etoile : {1, 2, 3}) {
            stubsAtomiques.emplace_back(
                "P_" + t->getNom() + "_" + to_string(etoile),
                [t, etoile]{ return new Place(global_hexagon_id++, *t, etoile); },
                true
            );
        }
    }
    // Total stubsAtomiques.size() == 21

    // --- 2. Générer TOUTES les combinaisons uniques possibles ---
    // (h1, h2, h3) sans ordre. On utilise 3 boucles imbriquées
    // i, j=i, k=j pour générer toutes les combinaisons de 3 parmi 21.
    
    // Utilise un set pour garantir l'unicité des compositions
    set<TuileComposition> poolPlaceUnique;
    set<TuileComposition> poolNonPlaceUnique;

    for (size_t i = 0; i < stubsAtomiques.size(); ++i) {
        for (size_t j = i; j < stubsAtomiques.size(); ++j) {
            for (size_t k = j; k < stubsAtomiques.size(); ++k) {
                
                HexagoneStub* h1s = &stubsAtomiques[i];
                HexagoneStub* h2s = &stubsAtomiques[j];
                HexagoneStub* h3s = &stubsAtomiques[k];

                // Compter les places
                int nbPlaces = h1s->estPlace + h2s->estPlace + h3s->estPlace;

                // Règle: Max 1 place par tuile
                if (nbPlaces > 1) continue;

                // Créer la composition
                TuileComposition comp;
                comp.signatureSet = {h1s->signature, h2s->signature, h3s->signature};
                comp.h1_stub = h1s;
                comp.h2_stub = h2s;
                comp.h3_stub = h3s;

                // Trier dans le bon pool
                if (nbPlaces == 1) {
                    poolPlaceUnique.insert(comp);
                } else {
                    poolNonPlaceUnique.insert(comp);
                }
            }
        }
    }
    
    // --- 3. Vérifier les pools et sélectionner les tuiles ---
    // Pool "Place" (PCC, PQC, PQQ) : 315 combinaisons uniques
    // Pool "Non-Place" (CCC, QCC, QQC, QQQ) : 56 combinaisons uniques
    
    // Convertir les sets en vecteurs pour les mélanger
    vector<TuileComposition> vecPoolPlace(poolPlaceUnique.begin(), poolPlaceUnique.end());
    vector<TuileComposition> vecPoolNonPlace(poolNonPlaceUnique.begin(), poolNonPlaceUnique.end());

    // Mélanger les pools
    std::shuffle(vecPoolPlace.begin(), vecPoolPlace.end(), gen);
    std::shuffle(vecPoolNonPlace.begin(), vecPoolNonPlace.end(), gen);
    
    // Contraintes de la demande
    const int PLACES_VOULUES = 20;
    const int NON_PLACES_VOULUES = 41;

    // Vérifier si c'est possible
    if (vecPoolPlace.size() < PLACES_VOULUES) {
        throw runtime_error("Erreur : Pas assez de compos Place uniques !");
    }
    if (vecPoolNonPlace.size() < NON_PLACES_VOULUES) {
        // Cette erreur se produira si on n'inclut pas QQQ et CCC
        throw runtime_error("Erreur : Pas assez de compos Non-Place uniques !");
    }

    // --- 4. Construire la pioche finale ---
    vector<TuileComposition> compositionsFinales;
    
    // Prendre 20 tuiles Place
    for(int i=0; i<PLACES_VOULUES; ++i) {
        compositionsFinales.push_back(vecPoolPlace[i]);
    }
    // Prendre 41 tuiles non-Place
    for(int i=0; i<NON_PLACES_VOULUES; ++i) {
        compositionsFinales.push_back(vecPoolNonPlace[i]);
    }

    // Mélanger la pioche finale de 61 compositions
    std::shuffle(compositionsFinales.begin(), compositionsFinales.end(), gen);

    // --- 5. Instancier les objets TuileCite ---
    vector<TuileCite*> pioche;
    pioche.reserve(compositionsFinales.size());
    
    for (size_t id = 0; id < compositionsFinales.size(); ++id) {
        auto& comp = compositionsFinales[id];
        
        // Appelle les lambdas create() pour allouer les hexagones
        pioche.push_back(new TuileCite(
            id + 1, // Tuile ID
            comp.h1_stub->create(), // h1
            comp.h2_stub->create(), // h2
            comp.h3_stub->create()  // h3
        ));
    }

    return pioche;
}


// --- Fonction principale (main) pour tester la génération ---

int main() {
    // Initialiser le générateur de nombres aléatoires une seule fois
    gen.seed(time(nullptr));

    cout << "Génération des 61 Tuiles Cité uniques..." << endl;
    cout << "Objectif: 20 Tuiles 'Place' et 41 Tuiles 'Non-Place'." << endl;
    cout << "-------------------------------------------------" << endl;

    vector<TuileCite*> maPioche;

    try {
        maPioche = genererTuilesPourPioche();

        cout << "Génération terminée. Total des tuiles : " << maPioche.size() << endl;
        
        // Vérifier les stats
        int countPlaces = 0;
        int countNonPlaces = 0;
        for (const auto* tuile : maPioche) {
            if (tuile->contientPlace()) {
                countPlaces++;
            } else {
                countNonPlaces++;
            }
        }
        cout << "Stats: " << countPlaces << " Tuiles Place, " 
             << countNonPlaces << " Tuiles Non-Place." << endl;
        
        cout << "-------------------------------------------------" << endl;
        cout << "Affichage des 10 premières tuiles de la pioche mélangée :" << endl;

        for (size_t i = 0; i < 10 && i < maPioche.size(); ++i) {
            cout << *maPioche[i] << endl; 
        }

        // --- Nettoyage de la mémoire ---
        cout << "Nettoyage de la mémoire..." << endl;
        for (auto* tuile : maPioche) {
            delete tuile;
        }
        maPioche.clear();
        cout << "Nettoyage terminé." << endl;

    } catch (const std::exception& e) {
        cerr << "Une erreur critique est survenue : " << e.what() << endl;
        return 1;
    }

    return 0;
}