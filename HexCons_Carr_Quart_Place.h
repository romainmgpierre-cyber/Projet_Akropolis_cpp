#ifndef HEXCONS_CARR_QUART_PLACE_H
#define HEXCONS_CARR_QUART_PLACE_H
#include "Type.h"
#include "CoordHex.h"  
using namespace std;
namespace Akropolis{
     class HexagoneConstruction{
        protected:
            size_t id; 
        public:
            HexagoneConstruction(size_t id) : id(id) {}
            virtual ~HexagoneConstruction() = default;
            size_t getId() const { return id; } 
            virtual HexagoneConstruction* clone() const = 0; 
    };

    class Quartier : public HexagoneConstruction{
        private:
            const Type* type;
        public:
            Quartier(size_t id, const Type& type)
                : HexagoneConstruction(id), type(&type) {}

            const Type& getType() const { return *type; }

            HexagoneConstruction* clone() const override {
                return new Quartier(id, *type);
            }
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

            HexagoneConstruction* clone() const override {
                return new Place(id, *type, nbetoile);
            }
    };

    class Carriere : public HexagoneConstruction{
        private :
            Couleur coul = Couleur::gris;
        public:
            public:
            Carriere(size_t id) : HexagoneConstruction(id){}

            HexagoneConstruction* clone() const override {
                return new Carriere(id);
            }
    };


}

#endif