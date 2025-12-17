#include "CoordHex.h"

namespace Akropolis{

    CoordHex CoordHex::rotate(int rotation) const {
        // gestion de la rotation de 0 à 5, 6 rotations possibles
        int normalizedRot = (rotation % 6 + 6) % 6;
        
        int q = this->q;
        int r = this->r;
        
        // Logique de rotation
        switch (normalizedRot) {
            case 0: return *this;             
            case 1: return CoordHex(-r, q + r); // 60 degrés
            case 2: return CoordHex(-q - r, q); // 120 degrés
            case 3: return CoordHex(-q, -r);    // 180 degrés
            case 4: return CoordHex(r, -q - r); // 240 degrés
            case 5: return CoordHex(q + r, -q); // 300 degrés
            default: return *this; 
        }

    }   
}