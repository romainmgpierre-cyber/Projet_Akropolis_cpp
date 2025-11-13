#ifndef HEXCONS_CARR_QUART_PLACE_H
#define HEXCONS_CARR_QUART_PLACE_H
#include "Type.h"
#include "CoordHex.h"  
using namespace std;
namespace Akropolis{
     class HexagoneConstruction{
        protected:
            size_t id; //compris entre 1 et 61
            CoordHex* position = nullptr; //pointeur  de coordonées qui sera intialisé lors du placement dans la cité
        public:
            HexagoneConstruction(size_t id) : id(id) {}
            virtual ~HexagoneConstruction() = default;
            void SetPosition(const CoordHex& pos) { *position = pos; }
            size_t getId() const { return id; } 

    };

    class Quartier : public HexagoneConstruction{
        private:
            const Type* type; // association vers Type
        public:
            Quartier(size_t id, const Type& type)
                : HexagoneConstruction(id), type(&type) {}

            const Type& getType() const { return *type; }
    };


    class Place : public HexagoneConstruction{
        private:
            const Type* type; // association vers Type
            size_t nbetoile=0;
        public:
            Place(size_t id, const Type& type, size_t nbetoile=0)
                : HexagoneConstruction(id), type(&type), nbetoile(nbetoile) {}

            const Type& getType() const { return *type; }
            size_t getNbEtoile() const { return nbetoile; }
    };

    class Carriere : public HexagoneConstruction{
        private :
            Couleur coul = Couleur::gris;
        public:
            Carriere(size_t id) : HexagoneConstruction(id){}
    };


}

#endif