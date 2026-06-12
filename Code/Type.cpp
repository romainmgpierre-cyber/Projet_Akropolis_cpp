#include "Type.h"
#include <string> 

using namespace std;
namespace Akropolis{
    const Type Type::HABITATION("Habitation", Couleur::bleu, "Seules celles du plus grand groupe rapportent des points.");
    const Type Type::MARCHE("Marché", Couleur::jaune, "Rapporte des points s'il n'est pas adjacent à un autre marché.");
    const Type Type::CASERNE("Caserne", Couleur::rouge, "Rapporte des points si elle est placée en périphérie de la cité.");
    const Type Type::TEMPLE("Temple", Couleur::violet, "Rapporte des points si il est entièrement entouré.");
    const Type Type::JARDIN("Jardin", Couleur::vert, "Rapporte toujours des points, sans contrainte.");
}