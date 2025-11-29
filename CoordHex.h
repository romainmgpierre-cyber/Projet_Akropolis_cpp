#ifndef CoordHex_H
#define CoordHex_H
#include <array>
#include <cmath>
using namespace std;
namespace Akropolis{
    class CoordHex {
    private:
        int q = 0;
        int r = 0;
    public:
        CoordHex() : q(0), r(0) {}
        CoordHex(int q_, int r_) : q(q_), r(r_) {}
        ~CoordHex()=default;
        int getQ() const { return q; }
        int getR() const { return r; }
        // Coordonées axiales donc <s, q, r>
        // s est implicite : s = -q - r
        int s() const { return -q - r; }

        // Surcharges opérateurs
        CoordHex operator+(const CoordHex& autre) const {
            return CoordHex(q + autre.q, r + autre.r);
        }
        
        CoordHex operator-(const CoordHex& autre) const {
            return CoordHex(q - autre.q, r - autre.r);
        }
        
        bool operator==(const CoordHex& autre) const {
            return q == autre.q && r == autre.r;
        }
        bool operator!=(const CoordHex& autre) const {
            return !(*this == autre);
        }
        // cet opérateur sera utilisé dans le tri
        bool operator<(const CoordHex& autre) const {
            if (q != autre.q) return q < autre.q;
            return r < autre.r;
        }
        

        // Donne le voisin à une direction
        CoordHex voisin(int direction) const {
            static const array<CoordHex, 6> directions = {{
                CoordHex(1, 0), CoordHex(0, 1), CoordHex(-1, 1),
                CoordHex(-1, 0), CoordHex(0, -1), CoordHex(1, -1)
            }};
            return *this + directions[direction % 6];
        }

        // Distance entre deux hexagones
        int distance(const CoordHex& autre) const {
            CoordHex diff = *this - autre;
            // Formule de RedBlobGames (convertie de cubique)
            return (abs(diff.q) + abs(diff.r) + abs(diff.s())) / 2;
        }
    };
}

#endif
