#include "GameExcep_Enums.h"
#include <string>
#ifndef Type_h
#define Type_h
using namespace std;
namespace Akropolis{
    class Type {
        string nom;
        Couleur couleur;
        string conditions;        
        Type(const string& nom, Couleur couleur, const string& cond)
            : nom(nom), couleur(couleur), conditions(cond) {}
    public:
        static const Type HABITATION;
        static const Type MARCHE;
        static const Type CASERNE;
        static const Type TEMPLE;
        static const Type JARDIN;
    
        Type(const Type&)=delete;
        Type& operator=(const Type&)=delete;
    
        const string& getNom() const { return nom; }
        Couleur getCouleur() const { return couleur; }
        const string& getConditions() const { return conditions; }
    };
}
#endif